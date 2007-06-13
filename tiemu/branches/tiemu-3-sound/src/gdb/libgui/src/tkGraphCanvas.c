#include "default.h"
#include "tkInt.h"
#include "tkPort.h"
#include "tkCanvas.h"
#include "tkCanvLayout.h"

extern Tk_ItemType tkEdgeType;

/*
 * To support Tcl/Tk8.3 correctly we must support the new type of
 * TagSearch.
 */

#ifndef USE_OLD_TAG_SEARCH
  #if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION < 3)
    #define USE_OLD_TAG_SEARCH 1
  #endif
#endif

#undef USE_OLD_TAG_SEARCH

#ifndef USE_OLD_TAG_SEARCH
/*
 * Uids for operands in compiled advanced tag search expressions
 * Initialization is done by InitCanvas()
 */
static Tk_Uid allUid = NULL;
static Tk_Uid currentUid = NULL;
static Tk_Uid andUid = NULL;
static Tk_Uid orUid = NULL;
static Tk_Uid xorUid = NULL;
static Tk_Uid parenUid = NULL;
static Tk_Uid negparenUid = NULL;
static Tk_Uid endparenUid = NULL;
static Tk_Uid tagvalUid = NULL;
static Tk_Uid negtagvalUid = NULL;
#else  /* USE_OLD_TAG_SEARCH */
static Tk_Uid allUid = NULL;
#endif /* USE_OLD_TAG_SEARCH */

typedef struct Layout_Graph Layout_Graph;

static
char* layableitems[] = {
    "bitmap",
    "edge",
    "oval",
    "polygon",
    "rectangle",
    "text",
    "window",
    (char*)0
};

/* Define a config set for graph command */
static Tk_ConfigSpec graphspecs[] = {
  {TK_CONFIG_BOOLEAN, "-computenodesize", (char*)NULL, (char*)NULL, 
     "0", Tk_Offset(LayoutConfig,computenodesize), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_INT, "-gridlock", (char*)NULL, (char*)NULL, 
     "0", Tk_Offset(LayoutConfig,gridlock), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_INT, "-elementsperline", (char*)NULL, (char*)NULL, 
     "0", Tk_Offset(LayoutConfig,elementsperline), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_BOOLEAN, "-hideedges", (char*)NULL, (char*)NULL, 
     "0", Tk_Offset(LayoutConfig,hideedges), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_BOOLEAN, "-keeprandompositions", (char*)NULL, (char*)NULL, 
     "0", Tk_Offset(LayoutConfig,keeprandompositions), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_PIXELS, "-nodespaceh", (char*)NULL, (char*)NULL, 
     "5", Tk_Offset(LayoutConfig,nodespaceH), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_PIXELS, "-nodespacev", (char*)NULL, (char*)NULL, 
     "5", Tk_Offset(LayoutConfig,nodespaceV), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_PIXELS, "-maxx", (char*)NULL, (char*)NULL, 
     (char*)NULL, Tk_Offset(LayoutConfig,maxx),
     TK_CONFIG_DONT_SET_DEFAULT, (Tk_CustomOption*)NULL},
  {TK_CONFIG_PIXELS, "-maxy", (char*)NULL, (char*)NULL, 
     (char*)NULL, Tk_Offset(LayoutConfig,maxy),
     TK_CONFIG_DONT_SET_DEFAULT, (Tk_CustomOption*)NULL},
  {TK_CONFIG_INT, "-order", (char*)NULL, (char*)NULL, 
     "0", Tk_Offset(LayoutConfig,graphorder), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_PIXELS, "-xoffset", (char*)NULL, (char*)NULL, 
     "4", Tk_Offset(LayoutConfig,xoffset), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_PIXELS, "-yoffset", (char*)NULL, (char*)NULL, 
     "4", Tk_Offset(LayoutConfig,yoffset), 0, (Tk_CustomOption*)NULL},
  {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL,
     (char *) NULL, 0, 0}
};


/*
 * See tkCanvas.h for key data structures used to implement canvases.
 */

#ifdef USE_OLD_TAG_SEARCH
/*
 * The structure defined below is used to keep track of a tag search
 * in progress.  Only the "prevPtr" field should be accessed by anyone
 * other than StartTagSearch and NextItem.
 */

typedef struct TagSearch {
    TkCanvas *canvasPtr;        /* Canvas widget being searched. */
    Tk_Uid tag;                 /* Tag to search for.   0 means return
				 * all items. */
    Tk_Item *prevPtr;           /* Item just before last one found (or NULL
				 * if last one found was first in the item
				 * list of canvasPtr). */
    Tk_Item *currentPtr;        /* Pointer to last item returned. */
    int searchOver;             /* Non-zero means NextItem should always
				 * return NULL. */
} TagSearch;

#else /* USE_OLD_TAG_SEARCH */
/*
 * The structure defined below is used to keep track of a tag search
 * in progress.  No field should be accessed by anyone other than
 * TagSearchScan, TagSearchFirst, TagSearchNext,
 * TagSearchScanExpr, TagSearchEvalExpr, 
 * TagSearchExprInit, TagSearchExprDestroy,
 * TagSearchDestroy.
 * (
 *   Not quite accurate: the TagSearch structure is also accessed from:
 *    CanvasWidgetCmd, FindItems, RelinkItems
 *   The only instances of the structure are owned by:
 *    CanvasWidgetCmd
 *   CanvasWidgetCmd is the only function that calls:
 *    FindItems, RelinkItems
 *   CanvasWidgetCmd, FindItems, RelinkItems, are the only functions that call
 *    TagSearch*
 * )
 */

typedef struct TagSearch {
    TkCanvas *canvasPtr;	/* Canvas widget being searched. */
    Tk_Item *currentPtr;	/* Pointer to last item returned. */
    Tk_Item *lastPtr;		/* The item right before the currentPtr
				 * is tracked so if the currentPtr is
				 * deleted we don't have to start from the
				 * beginning. */
    int searchOver;		/* Non-zero means NextItem should always
				 * return NULL. */
    int type;			/* search type */
    int id;			/* item id for searches by id */

    char *string;		/* tag expression string */
    int stringIndex;		/* current position in string scan */
    int stringLength;		/* length of tag expression string */

    char *rewritebuffer;	/* tag string (after removing escapes) */
    unsigned int rewritebufferAllocated;	/* available space for rewrites */

    TagSearchExpr *expr;	/* compiled tag expression */
} TagSearch;
#endif /* USE_OLD_TAG_SEARCH */

#ifdef USE_OLD_TAG_SEARCH
static Tk_Item *        NextItem _ANSI_ARGS_((TagSearch *searchPtr));
static Tk_Item *        StartTagSearch _ANSI_ARGS_((TkCanvas *canvasPtr,
			    char *tag, TagSearch *searchPtr));
#else /* USE_OLD_TAG_SEARCH */
static void 		TagSearchExprInit _ANSI_ARGS_ ((
			    TagSearchExpr **exprPtrPtr));
static void		TagSearchExprDestroy _ANSI_ARGS_((TagSearchExpr *expr));
static void		TagSearchDestroy _ANSI_ARGS_((TagSearch *searchPtr));
static int		TagSearchScan _ANSI_ARGS_((TkCanvas *canvasPtr,
			    Tcl_Obj *tagObj, TagSearch **searchPtrPtr));
static int		TagSearchScanExpr _ANSI_ARGS_((Tcl_Interp *interp,
			    TagSearch *searchPtr, TagSearchExpr *expr));
static int		TagSearchEvalExpr _ANSI_ARGS_((TagSearchExpr *expr,
			    Tk_Item *itemPtr));
static Tk_Item *	TagSearchFirst _ANSI_ARGS_((TagSearch *searchPtr));
static Tk_Item *	TagSearchNext _ANSI_ARGS_((TagSearch *searchPtr));
#endif /* USE_OLD_TAG_SEARCH */


static Tcl_HashTable *  graph_table _ANSI_ARGS_((Tcl_Interp *interp));

int    MY_graphOrder   (struct Layout_Graph* This);
void * MY_EdgeParent   (struct Layout_Graph* This, int i, int num);
void * MY_EdgeFromNode (struct Layout_Graph* This, int i);


static
int
getnodebbox(interp,canvasPtr, iPtr, bbox)
    Tcl_Interp* interp;
    TkCanvas* canvasPtr;
    Tk_Item* iPtr;
    ItemGeom* bbox;
{
    bbox->x1 = iPtr->x1;
    bbox->y1 = iPtr->y1;
    bbox->x2 = iPtr->x2;
    bbox->y2 = iPtr->y2;
    return TCL_OK;
}

static
int
getedgedim(canvasPtr, e, dim)
    TkCanvas* canvasPtr;
    Tk_Item* e;
    ItemGeom* dim;
{
    int argc2; 
    char **argv2;

    /* Read the text height of this edge. */
    Tk_ConfigureInfo(canvasPtr->interp, canvasPtr->tkwin,
			 e->typePtr->configSpecs,
			 (char *) e, "-textheight", 0);
    if(Tcl_SplitList(canvasPtr->interp, canvasPtr->interp->result,
			  &argc2, &argv2) != TCL_OK) {
	return TCL_ERROR;
    }
    dim->height = atol(argv2[4]);
    ckfree((char *) argv2);
    /* Read the text width of this edge. */
    Tk_ConfigureInfo(canvasPtr->interp, canvasPtr->tkwin,
			 e->typePtr->configSpecs,
			 (char *) e, "-textwidth", 0);
    if(Tcl_SplitList(canvasPtr->interp, canvasPtr->interp->result,
			  &argc2, &argv2) != TCL_OK) {
	return TCL_ERROR;
    }
    dim->width = atol(argv2[4]);
    ckfree((char *) argv2);
    Tcl_ResetResult(canvasPtr->interp);
    return TCL_OK;
}

static
int
setnodegeom(interp,canvasPtr,iPtr,geom)
    Tcl_Interp* interp;
    TkCanvas* canvasPtr;
    Tk_Item* iPtr;
    ItemGeom geom;
{
    double deltax, deltay;

    if(iPtr->typePtr->translateProc == NULL) {
	Tcl_AppendResult(interp,"item has no translation proc",(char*)0);
	return TCL_ERROR;
    }

    /* get the delta x,y of the item */
    deltax = geom.x1 - iPtr->x1;
    deltay = geom.y1 - iPtr->y1;

    Tk_CanvasEventuallyRedraw((Tk_Canvas) canvasPtr, iPtr->x1, iPtr->y1, iPtr->x2, iPtr->y2);
    (void)(*iPtr->typePtr->translateProc)((Tk_Canvas) canvasPtr, iPtr, deltax, deltay);
    Tk_CanvasEventuallyRedraw((Tk_Canvas) canvasPtr, iPtr->x1, iPtr->y1, iPtr->x2, iPtr->y2);   
    return TCL_OK;
}

static Layout_Graph *
GetGraphLayoutII(TkCanvas *canvasPtr, Tcl_Interp *interp);
static
int
setedgegeom(interp,canvasPtr,iPtr,geom,i)
    Tcl_Interp* interp;
    TkCanvas* canvasPtr;
    Tk_Item* iPtr;
    ItemGeom geom;
    int i;
{
    /* register char* nm;
       register int c; */
    int argc = 4;
    char x1[TCL_DOUBLE_SPACE];
    char y1[TCL_DOUBLE_SPACE];
    char x2[TCL_DOUBLE_SPACE];
    char y2[TCL_DOUBLE_SPACE];
    char x3[TCL_DOUBLE_SPACE];
    char y3[TCL_DOUBLE_SPACE];
    char x4[TCL_DOUBLE_SPACE];
    char y4[TCL_DOUBLE_SPACE];
    char* argv[8];
    Tcl_Obj* argvObj[8]; int loopcount;
    Layout_Graph *graph=GetGraphLayoutII(canvasPtr, interp);

    LayoutConfig cnf = GetLayoutConfig (/*canvasPtr->*/graph);
    double xd = geom.x1 - geom.x2 /*- 10.0*/ , xdiff;
    
    if (xd < 0.0) xd = geom.x2 - geom.x1 /*- 10.0*/;

    if(iPtr->typePtr->coordProc == NULL) {
	Tcl_AppendResult(interp,"could not set edge item coordinates",(char*)0);
	return TCL_ERROR; 
    }
    argv[0] = x1;
    argv[1] = y1;
    argv[2] = x2;
    argv[3] = y2;
    argv[4] = x3;
    argv[5] = y3;
    argv[6] = x4;
    argv[7] = y4;

    sprintf(x1,"%g",geom.x1);
    sprintf(y1,"%g",geom.y1);
    sprintf(x2,"%g",geom.x2);
    sprintf(y2,"%g",geom.y2);

	if (cnf.gridlock!=0)
	{
		/* changing lines, only when right to left */
		if (! MY_graphOrder (/*canvasPtr->*/graph))
		  {
	        xdiff = (double) cnf.nodespaceH - xd;
		    if (xdiff < 0.0) xdiff = xd - (double) cnf.nodespaceH;

			if (xdiff < 2.0 &&
		       MY_EdgeParent(/*canvasPtr->*/graph, i, 0) ==
		       MY_EdgeFromNode (/*canvasPtr->*/graph, i))
			{
				sprintf (x4, "%g", geom.x2);
				sprintf (y4, "%g", geom.y2);

				sprintf (x2, "%g", geom.x1 + (geom.x2 - geom.x1) / 2);
				sprintf (y2, "%g", geom.y1);
				sprintf (x3, "%g", geom.x1 + (geom.x2 - geom.x1) / 2);
				sprintf (y3, "%g", geom.y2);
				argc = 8;
			}
		  }
	}

    Tk_CanvasEventuallyRedraw((Tk_Canvas) canvasPtr, iPtr->x1, iPtr->y1, iPtr->x2, iPtr->y2);
    for (loopcount = 0 ; loopcount < 8 ; loopcount++) {
       argvObj[loopcount] = Tcl_NewStringObj(argv[loopcount], -1);
    }
    (void)(*iPtr->typePtr->coordProc)(interp, (Tk_Canvas) canvasPtr, iPtr,
				      /* argc-3, argv+3); 08nov95 wmt */
				      argc, argvObj);
    Tk_CanvasEventuallyRedraw((Tk_Canvas) canvasPtr, iPtr->x1, iPtr->y1, iPtr->x2, iPtr->y2);   
    return TCL_OK;
}
 
static
int
GetEdgeNodes(interp,canvasPtr,i,fp,tp)
	Tcl_Interp* interp;
	TkCanvas* canvasPtr;
	Tk_Item* i;
	char** fp;
	char** tp;
{
    int argc;
    char** argv;
    char * buf;

    /* Read the from node id of this edge. */
    Tk_ConfigureInfo(interp, canvasPtr->tkwin,
			 i->typePtr->configSpecs,
			 (char *) i, "-from", 0);
    if(Tcl_SplitList(interp, interp->result,
			  &argc, &argv) != TCL_OK) {
	return TCL_ERROR;
    }

    *fp = ckalloc (strlen (argv[4]) + 1);
    strcpy(*fp, argv[4]);

    ckfree((char*)argv);
    /* Read the to node id of this edge. */
    Tk_ConfigureInfo(interp, canvasPtr->tkwin,
			 i->typePtr->configSpecs,
			 (char *) i, "-to", 0);
    if(Tcl_SplitList(interp, interp->result,
			  &argc, &argv) != TCL_OK) {
	return TCL_ERROR;
    }

    *tp = ckalloc(strlen (argv[4]) + 1);
    strcpy(*tp, argv[4]);

    ckfree((char*)argv);
    Tcl_ResetResult(interp);
    return TCL_OK;
}


int
createcanvasgraph(interp,canvCmd,graph)
    Tcl_Interp* interp;
    Tcl_CmdInfo *canvCmd;
    Layout_Graph **graph;
{
    LayoutConfig cfg;
    int argc1; char* argv1[3];

    *graph = LayoutCreateGraph();
    if(!*graph) {
	Tcl_AppendResult(interp,"cannot create graph for canvas",(char*)0);
	return TCL_ERROR;
    }
    cfg = GetLayoutConfig(*graph);
    /* Establish max x and max y based on canvas height/width */
    argv1[0] = "<graph-canvas>";
    argv1[1] = "cget";
    argv1[2] = "-width";
    argc1 = 3;
    if ((canvCmd->proc)(canvCmd->clientData, interp, argc1, argv1) 
	!= TCL_OK) {
	return TCL_ERROR;
    }
    cfg.maxx = atol(Tcl_GetStringResult(interp));

    argv1[2] = "-height";
    if ((canvCmd->proc) (canvCmd->clientData, interp, argc1, argv1) 
	!= TCL_OK) {
	return TCL_ERROR;
    }
    cfg.maxy = atol(Tcl_GetStringResult(interp));
    Tcl_ResetResult(interp);
    SetLayoutConfig(*graph,cfg);
    return TCL_OK;
}

static Tcl_HashTable *
graph_table (Tcl_Interp *interp)
{
    return (Tcl_HashTable *) Tcl_GetAssocData (interp, "canvasgraph", NULL);
}

/* 
 *-------------------------------------------------------------
 *
 * GetGraphLayout --
 *      Gets graph info for canvas.  Adds a new entry if needed.
 * 
 * Results:
 *      Standard Tcl Error
 *-------------------------------------------------------------
 */

static Layout_Graph *
GetGraphLayout(canvCmd, interp)
     Tcl_CmdInfo *canvCmd;
     Tcl_Interp *interp;
{
    Tcl_HashEntry *entry;
    
    entry = Tcl_FindHashEntry(graph_table(interp), (char *)canvCmd->objClientData);
    if (entry)
	return (Layout_Graph *)Tcl_GetHashValue(entry);
    
    return NULL;
}

static Layout_Graph *
GetGraphLayoutII(canvasPtr, interp)
     TkCanvas *canvasPtr;
     Tcl_Interp *interp;
{
    Tcl_HashEntry *entry;
    entry = Tcl_FindHashEntry(graph_table(interp), (char *)canvasPtr);
    if (entry)
	return (Layout_Graph *)Tcl_GetHashValue(entry);
    
    return NULL;
}

static int
GetCreatedGraphLayout(interp, canvCmd, graph)
     Tcl_Interp *interp; 
     Tcl_CmdInfo *canvCmd;
     Layout_Graph **graph;
{
    *graph = GetGraphLayout(canvCmd, interp);
    if (*graph == NULL) {
	Tcl_HashEntry *newitem;
	int new;

	/* No item, let's make one and add it to the table. */
	if (createcanvasgraph(interp, canvCmd, graph) != TCL_OK)
	    return TCL_ERROR;
	/*	newitem = Tcl_CreateHashEntry(graph_table(interp), 
		(char *)(canvCmd->objClientData), &new);*/
	newitem = Tcl_CreateHashEntry(graph_table(interp), 
				      (char *)(canvCmd->objClientData), &new);
	Tcl_SetHashValue(newitem, (ClientData) *graph);
    }
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 * 
 * GraphCanvasCmd -- 
 *      This procedure is invoked to process the new "graph"
 *      command.  This command takes a canvas and uses it to layout
 *      the canvas items with a pretty graph structure.
 *
 * Results:
 *      Standard Tcl result.
 * 
 *--------------------------------------------------------------
 */

int
GraphCanvasCmd(clientData, interp, argc, argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     char **argv;
{
    Tcl_CmdInfo canvCmd;
    size_t length;
    int c, i, result;
    Layout_Graph *graph;
    TkCanvas *canvasPtr;
    
    if (argc < 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " canvas option ?arg arg ...?\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    /* The second arg is the canvas widget */
    if (!Tcl_GetCommandInfo(interp, argv[1], &canvCmd)) {
	Tcl_AppendResult(interp, "couldn't get canvas information for \"",
			 argv[1], "\"", (char *) NULL);
	return TCL_ERROR;
    }
    canvasPtr = (TkCanvas *) (canvCmd.objClientData);
    canvasPtr->hotPtr = NULL;
    c = argv[2][0];
    length = strlen(argv[2]);
    if ((c == 'a') && (strncmp(argv[2], "add", length) == 0)) {
	char* newargv[4];
	if (argc < 4) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
			     " ", argv[1], " add tagOrId ?tagOrId ...?\"",
			     (char *) NULL);
	    goto error;
	}

	if (GetCreatedGraphLayout(interp, &canvCmd, &graph) != TCL_OK)
	    goto error;

	for (i = 3; i < argc; i++) {
	    Tk_Item *itemPtr;
#ifdef USE_OLD_TAG_SEARCH
	    TagSearch search;
#else /* USE_OLD_TAG_SEARCH */
            TagSearch *searchPtr = NULL;
            Tcl_Obj *tagObj = NULL;
            TagSearch *searchPtrTmp = NULL; 
            /* Allocated by first TagSearchScan
	     * Freed by TagSearchDestroy */
#endif /* USE_OLD_TAG_SEARCH */
	    /* Loop through all the items */
#ifdef USE_OLD_TAG_SEARCH
	    for (itemPtr = StartTagSearch(canvasPtr, argv[i], &search);
		 itemPtr != NULL; itemPtr = NextItem(&search)) {
#else /* USE_OLD_TAG_SEARCH */
	    tagObj = Tcl_NewStringObj(argv[i],-1);
	    if ((result = TagSearchScan(canvasPtr, tagObj, &searchPtr)) != TCL_OK) {
		goto done;
	    }

	    for (itemPtr = TagSearchFirst(searchPtr);
		    itemPtr != NULL; itemPtr = TagSearchNext(searchPtr)) {
#endif /* USE_OLD_TAG_SEARCH */
		char* nm = itemPtr->typePtr->name;
		/* create a new edge or node */
		if(strcmp(nm,"edge") == 0) {
		    char* fname;
		    char* tname;
                    Tcl_Obj* fnametagObj;
                    Tcl_Obj* tnametagObj;
		    Tk_Item* f;
		    Tk_Item* t;
		    /* find the from and to node pItems */
		    if(GetEdgeNodes(interp,canvasPtr,itemPtr,&fname,&tname) != TCL_OK)
			goto error;
		    /* find the from and to node pItems */
#ifdef USE_OLD_TAG_SEARCH
		    f = StartTagSearch(canvasPtr, fname, &search);
		    t = StartTagSearch(canvasPtr, tname, &search);
#else /* USE_OLD_TAG_SEARCH */
                    fnametagObj = Tcl_NewStringObj(fname,-1);
 	            if (TagSearchScan(canvasPtr, fnametagObj, &searchPtrTmp) != TCL_OK) {
		         goto done;
	            }
                    f = TagSearchFirst(searchPtrTmp);
                    tnametagObj = Tcl_NewStringObj(tname,-1);
 	            if (TagSearchScan(canvasPtr, tnametagObj, &searchPtrTmp) != TCL_OK) {
		         goto done;
	            }
                    t = TagSearchFirst(searchPtrTmp);

#endif /* USE_OLD_TAG_SEARCH */
                    ckfree(fname); ckfree(tname);
		    if(LayoutCreateEdge(graph,
					(pItem)itemPtr,
					(pItem)f, (pItem)t) != TCL_OK) {
			char* msg = LayoutGetError(graph);
			if(!msg)
			    msg = "could not record edge in graph";
			Tcl_AppendResult(interp,msg,(char*)0);
			goto error;
		    }
		} else { /* not an edge; assume a node */
		    /* verify that we can handle this */
		    char** p;
		    for(p=layableitems;*p;p++) {
			if(strcmp(*p,nm)==0) break;
		    }
		    if(!*p) {
			Tcl_AppendResult(interp,"cannot yet handle ",nm,(char*)0);
			goto error;
		    }
		    if(LayoutCreateNode(graph,
					(pItem)itemPtr,NULL,NULL) !=TCL_OK) {
			char* msg = LayoutGetError(graph);
			if(!msg)
			    msg = "could not record node in graph";
			Tcl_AppendResult(interp,msg,(char*)0);
			goto error;
		    }
		}
	    }
	}

    } else if ((c == 'c') && (strncmp(argv[2], "configure", length) == 0)) {
	    register int ok;
	    LayoutConfig cfg;
	    if (GetCreatedGraphLayout(interp, &canvCmd, &graph) != TCL_OK)
		goto error;
	    cfg = GetLayoutConfig(graph);

	    if(argc == 3) {
		/* get all options */
		ok = Tk_ConfigureInfo(interp, 
				      Tk_CanvasTkwin(*(Tk_Canvas *)canvasPtr), 
				      graphspecs,(char*)&cfg, (char*)NULL, 0);
	    } else if(argc == 4) {
		/* get one option */
		ok = Tk_ConfigureInfo(interp, 
				      Tk_CanvasTkwin(*(Tk_Canvas *)canvasPtr), 
				      graphspecs,(char*)&cfg, argv[3], 0);
	    } else { /* setting one or more options */
		ok = Tk_ConfigureWidget(interp, 
					Tk_CanvasTkwin(*(Tk_Canvas *)canvasPtr),
					graphspecs, argc-3, argv+3, 
					(char*)&cfg, TK_CONFIG_ARGV_ONLY);
		if(ok == TCL_OK) {
		    SetLayoutConfig(graph,cfg);
		}
	    }
	    if(ok != TCL_OK) goto error;
	} else if ((c == 'c') && (strncmp(argv[2], "clear", length) == 0)) {
	    /* clear graph; ignore if no graph */
	    Layout_Graph *graph = GetGraphLayout(&canvCmd, interp);
	    if (graph)
		LayoutClearGraph(graph);
	} else if ((c == 'd') && (strncmp(argv[2], "destroy", length) == 0)) {
	    /* destroy any graph info connected to the canvas,
	       but without destroying the canvas
	    */
	    Layout_Graph *graph = GetGraphLayout(&canvCmd, interp);
	    if (graph) {
		Tcl_HashEntry *entry;
		entry = Tcl_FindHashEntry(graph_table(interp),
					  (char *)(canvCmd.objClientData));
		
		LayoutFreeGraph(graph);
		/* Remove hash table entry */
		Tcl_DeleteHashEntry(entry);
	    }
	} else if ((c == 'e') && (strncmp(argv[2], "edges", length) == 0)) {
	    Tk_Item* ip;
	    Layout_Graph *graph = GetGraphLayout(&canvCmd, interp);
	    /* return list of edges associated with graph, if any */
	    if(!graph) goto done;
	    for(i=0;LayoutGetIthEdge(graph,i,(pItem*)&ip)==TCL_OK;i++) {
		char convertbuffer[20];
		sprintf(convertbuffer, "%d", ip->id);
		Tcl_AppendElement(interp,convertbuffer);
	    }
	} else if ((c == 'l') && (strncmp(argv[2], "layout", length) == 0)) {
	    char* which;
	    Tk_Item* ip;
	    Layout_Graph *graph = GetGraphLayout(&canvCmd, interp);

	    if(!graph) goto done;

	    /* get the geometries of the items attached to the graph */
	    for(i=0;LayoutGetIthNode(graph,i,(pItem*)&ip)==TCL_OK;i++) {
		ItemGeom geom;
		if(getnodebbox(interp,canvasPtr,ip,&geom) != TCL_OK
		   || LayoutSetNodeBBox(graph,ip,geom) != TCL_OK) {
		    Tcl_AppendResult(interp, "could not get node location", (char *) NULL);
		    goto error;
		}
	    }
	    for(i=0;LayoutGetIthEdge(graph,i,(pItem*)&ip)==TCL_OK;i++) {
		ItemGeom geom;
		if(getedgedim(canvasPtr,ip,&geom) != TCL_OK
		   || LayoutSetEdgeDim(graph,ip,geom) != TCL_OK) {
		    Tcl_AppendResult(interp, "could not get edge location", (char *) NULL);
		    goto error;
		}
	    }

	    if(argc > 3) which = argv[3]; else which = "isi";
	    if(strcmp(which,"tree")==0) {
		if(LayoutTree(graph) == TCL_ERROR) {
		    Tcl_AppendResult(interp, "layout failed",(char *) NULL);
		    goto error;
		}
	    } else if(strcmp(which,"isi")==0) {
		if(LayoutISI(graph) == TCL_ERROR) {
		    Tcl_AppendResult(interp, "layout failed",(char *) NULL);
		    goto error;
		}
	    } else if(strcmp(which,"matrix")==0) {
		if(LayoutMatrix(graph) == TCL_ERROR) {
		    Tcl_AppendResult(interp, "layout failed",(char *) NULL);
		    goto error;
		}
	    } else if(strcmp(which,"random")==0) {
		if(LayoutRandom(graph) == TCL_ERROR) {
		    Tcl_AppendResult(interp, "layout failed",(char *) NULL);
		    goto error;
		}
	    } else {
		Tcl_AppendResult(interp, "unknown layout algorithm", which, (char *) NULL);
		goto error;
	    }
	    /* move the various items into place after layout */
	    for(i=0;LayoutGetIthNode(graph,i,(pItem*)&ip)==TCL_OK;i++) {
		ItemGeom geom;
		if(LayoutGetNodeBBox(graph,ip,&geom) != TCL_OK
		   || setnodegeom(interp,canvasPtr,ip,geom) != TCL_OK) {
		    Tcl_AppendResult(interp, "could not set node location", (char *) NULL);
		    goto error;
		}
	    }
	    for(i=0;LayoutGetIthEdge(graph,i,(pItem*)&ip)==TCL_OK;i++) {
		ItemGeom geom;
		if(LayoutGetEdgeEndPoints(graph,ip,&geom) != TCL_OK
		   || setedgegeom(interp,canvasPtr,ip,geom,i) != TCL_OK) {
		    Tcl_AppendResult(interp, "could not set edge location", (char *) NULL);
		    goto error;
		}
	    }
	} else if ((c == 'n') && (strncmp(argv[2], "nodes", length) == 0)) {
	    Tk_Item* ip;
	    Layout_Graph *graph = GetGraphLayout(&canvCmd, interp);

	    /* return list of nodes associated with graph */
	    if(!graph) goto done;
	    for(i=0;LayoutGetIthNode(graph,i,(pItem*)&ip)==TCL_OK;i++) {
		char convertbuffer[20];
		sprintf(convertbuffer, "%d", ip->id);
		Tcl_AppendElement(interp,convertbuffer);
	    }
	} else if ((c == 'r') && (strncmp(argv[2], "remove", length) == 0)) {
	    char* nm;
	    Tk_Item *itemPtr;
#ifdef USE_OLD_TAG_SEARCH
	    TagSearch search;
#else /* USE_OLD_TAG_SEARCH */
	    Tcl_Obj* tagObj;
            TagSearch *searchPtr = NULL;
#endif /* USE_OLD_TAG_SEARCH */
	    Layout_Graph *graph = GetGraphLayout(&canvCmd, interp);

	    if(!graph) goto done;
	    for (i = 3; i < argc; i++) {
#ifdef USE_OLD_TAG_SEARCH
		for (itemPtr = StartTagSearch(canvasPtr, argv[i], &search);
		     itemPtr != NULL; itemPtr = NextItem(&search)) {
#else /* USE_OLD_TAG_SEARCH */
		tagObj = Tcl_NewStringObj(argv[i],-1);
		TagSearchScan(canvasPtr, tagObj, &searchPtr);
		for (itemPtr = TagSearchFirst(searchPtr); itemPtr != NULL;
                     itemPtr = TagSearchNext(searchPtr)) {
#endif /* USE_OLD_TAG_SEARCH */ 
		    nm = itemPtr->typePtr->name;
		    /* delete a new edge or node */
		    if(strcmp(nm,"edge") == 0) {
			(void)LayoutDeleteEdge(graph,itemPtr);
		    } else { /* not an edge; assume a node */
			(void)LayoutDeleteNode(graph,itemPtr);
		    }
		}
	    }
	} else {
	    Tcl_AppendResult(interp, "bad option \"", argv[2],
		"\":  must be add, configure, clear, ",
		"destroy, edges, layout, nodes, remove",
		(char *) NULL);  
	    goto error;
	}
 done:
    return TCL_OK;
 error:
    return TCL_ERROR;
}

/* If graph is deleted, make it go away */
static void 
delete_graph_command(ClientData clientData, Tcl_Interp *interp)
{
    Tcl_DeleteHashTable((Tcl_HashTable *) clientData);
    
    ckfree ((char*) clientData);
}

/*
 *-------------------------------------------------------------
 * GraphLayoutInit()
 *      Adds appropriate commands to Tcl interpreter, and 
 *      inits necessary tables.
 *-------------------------------------------------------------
 */
int
create_graph_command(Tcl_Interp *interp)
{
    Tcl_HashTable *graphTable= (Tcl_HashTable*)ckalloc (sizeof (Tcl_HashTable));

    Tcl_InitHashTable(graphTable, TCL_ONE_WORD_KEYS);
    
    /*
     * Create an associated data that stores the
     * hash table
     */
    Tcl_SetAssocData (interp, "canvasgraph",
                      delete_graph_command,
		      (void*) graphTable);
    
    allUid = Tk_GetUid("all");

    if (Tcl_CreateCommand(interp, "graph", GraphCanvasCmd, 
			  NULL, NULL /*delete_graph_command*/ ) == NULL)
	return TCL_ERROR;

    Tk_CreateItemType(&tkEdgeType);
   
    return TCL_OK;
}


#ifdef USE_OLD_TAG_SEARCH

/*
 *--------------------------------------------------------------
 *
 * StartTagSearch --
 *
 *      This procedure is called to initiate an enumeration of
 *      all items in a given canvas that contain a given tag.
 *
 * Results:
 *      The return value is a pointer to the first item in
 *      canvasPtr that matches tag, or NULL if there is no
 *      such item.  The information at *searchPtr is initialized
 *      such that successive calls to NextItem will return
 *      successive items that match tag.
 *
 * Side effects:
 *      SearchPtr is linked into a list of searches in progress
 *      on canvasPtr, so that elements can safely be deleted
 *      while the search is in progress.  EndTagSearch must be
 *      called at the end of the search to unlink searchPtr from
 *      this list.
 *
 *--------------------------------------------------------------
 */

static Tk_Item *
StartTagSearch(canvasPtr, tag, searchPtr)
     TkCanvas *canvasPtr;                /* Canvas whose items are to be
					  * searched. */
     char *tag;                          /* String giving tag value. */
     TagSearch *searchPtr;               /* Record describing tag search;
					  * will be initialized here. */
{
    int id;
    Tk_Item *itemPtr, *prevPtr;
    Tk_Uid *tagPtr;
    Tk_Uid uid;
    int count;
    
    /*
     * Initialize the search.
     */
    
    searchPtr->canvasPtr = canvasPtr;
    searchPtr->searchOver = 0;

    /*
     * Find the first matching item in one of several ways. If the tag
     * is a number then it selects the single item with the matching
     * identifier.  In this case see if the item being requested is the
     * hot item, in which case the search can be skipped.
     */
    
    if (isdigit(UCHAR(*tag))) {
	char *end;
	
	id = strtoul(tag, &end, 0);
	if (*end == 0) {
	    itemPtr = canvasPtr->hotPtr;
	    prevPtr = canvasPtr->hotPrevPtr;
	    if ((itemPtr == NULL) || (itemPtr->id != id) || (prevPtr == NULL)
		|| (prevPtr->nextPtr != itemPtr)) {
		for (prevPtr = NULL, itemPtr = canvasPtr->firstItemPtr;
		     itemPtr != NULL;
		     prevPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
		    if (itemPtr->id == id) {
			break;
		    }
		}
	    }
	    searchPtr->prevPtr = prevPtr;
	    searchPtr->searchOver = 1;
	    canvasPtr->hotPtr = itemPtr;
	    canvasPtr->hotPrevPtr = prevPtr;
	    return itemPtr;
	}
    }
    
    searchPtr->tag = uid = Tk_GetUid(tag);
    if (uid == allUid) {
	
	/*
	 * All items match.
	 */
	
	searchPtr->tag = NULL;
	searchPtr->prevPtr = NULL;
	searchPtr->currentPtr = canvasPtr->firstItemPtr;
	return canvasPtr->firstItemPtr;
    }
    
    /*
     * None of the above.  Search for an item with a matching tag.
     */
    
    for (prevPtr = NULL, itemPtr = canvasPtr->firstItemPtr; itemPtr != NULL;
	 prevPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
	for (tagPtr = itemPtr->tagPtr, count = itemPtr->numTags;
	     count > 0; tagPtr++, count--) {
	    if (*tagPtr == uid) {
		searchPtr->prevPtr = prevPtr;
		searchPtr->currentPtr = itemPtr;
		return itemPtr;
	    }
	}
    }
    searchPtr->prevPtr = prevPtr;
    searchPtr->searchOver = 1;
    return NULL;
}

/*
 *--------------------------------------------------------------
 *
 * NextItem --
 *
 *      This procedure returns successive items that match a given
 *      tag;  it should be called only after StartTagSearch has been
 *      used to begin a search.
 *
 * Results:
 *      The return value is a pointer to the next item that matches
 *      the tag specified to StartTagSearch, or NULL if no such
 *      item exists.  *SearchPtr is updated so that the next call
 *      to this procedure will return the next item.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

static Tk_Item *
NextItem(searchPtr)
     TagSearch *searchPtr;               /* Record describing search in
					  * progress. */
{
    Tk_Item *itemPtr, *prevPtr;
    int count;
    Tk_Uid uid;
    Tk_Uid *tagPtr;
    
    /*
     * Find next item in list (this may not actually be a suitable
     * one to return), and return if there are no items left.
     */
    
    prevPtr = searchPtr->prevPtr;
    if (prevPtr == NULL) {
	itemPtr = searchPtr->canvasPtr->firstItemPtr;
    } else {
	itemPtr = prevPtr->nextPtr;
    }
    if ((itemPtr == NULL) || (searchPtr->searchOver)) {
	searchPtr->searchOver = 1;
	return NULL;
    }
    if (itemPtr != searchPtr->currentPtr) {
	/*
	 * The structure of the list has changed.  Probably the
	 * previously-returned item was removed from the list.
	 * In this case, don't advance prevPtr;  just return
	 * its new successor (i.e. do nothing here).
	 */
    } else {
	prevPtr = itemPtr;
	itemPtr = prevPtr->nextPtr;
    }
    
    /*
     * Handle special case of "all" search by returning next item.
     */
    
    uid = searchPtr->tag;
    if (uid == NULL) {
	searchPtr->prevPtr = prevPtr;
	searchPtr->currentPtr = itemPtr;
	return itemPtr;
    }
    
    /*
     * Look for an item with a particular tag.
     */
    
    for ( ; itemPtr != NULL; prevPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
	for (tagPtr = itemPtr->tagPtr, count = itemPtr->numTags;
	     count > 0; tagPtr++, count--) {
	    if (*tagPtr == uid) {
		searchPtr->prevPtr = prevPtr;
		searchPtr->currentPtr = itemPtr;
		return itemPtr;
	    }
	}
    }
    searchPtr->prevPtr = prevPtr;
    searchPtr->searchOver = 1;
    return NULL;
}

#else /* USE_OLD_TAG_SEARCH */
/*
 *--------------------------------------------------------------
 *
 * TagSearchExprInit --
 *
 *      This procedure allocates and initializes one TagSearchExpr struct.
 *
 * Results:
 *
 * Side effects:
 *
 *--------------------------------------------------------------
 */

static void
TagSearchExprInit(exprPtrPtr)
TagSearchExpr **exprPtrPtr;
{
    TagSearchExpr* expr = *exprPtrPtr;

    if (! expr) {
	expr = (TagSearchExpr *) ckalloc(sizeof(TagSearchExpr));
	expr->allocated = 0;
	expr->uids = NULL;
	expr->next = NULL;
    }
    expr->uid = NULL;
    expr->index = 0;
    expr->length = 0;
    *exprPtrPtr = expr;
}
 
/*
 *--------------------------------------------------------------
 *
 * TagSearchExprDestroy --
 *
 *      This procedure destroys one TagSearchExpr structure.
 *
 * Results:
 *
 * Side effects:
 *
 *--------------------------------------------------------------
     */

static void
TagSearchExprDestroy(expr)
    TagSearchExpr *expr;
{
    if (expr) {
    	if (expr->uids) {
        	ckfree((char *)expr->uids);
	}
        ckfree((char *)expr);
    }
}

/*
 *--------------------------------------------------------------
 *
 * TagSearchScan --
 *
 *      This procedure is called to initiate an enumeration of
 *      all items in a given canvas that contain a tag that matches
 *      the tagOrId expression.
 *
 * Results:
 *      The return value indicates if the tagOrId expression
 *      was successfully scanned (syntax).
 *      The information at *searchPtr is initialized
 *      such that a call to TagSearchFirst, followed by
 *      successive calls to TagSearchNext will return items
 *      that match tag.
 *
 * Side effects:
 *      SearchPtr is linked into a list of searches in progress
 *      on canvasPtr, so that elements can safely be deleted
 *      while the search is in progress.
 *
 *--------------------------------------------------------------
 */

static int
TagSearchScan(canvasPtr, tagObj, searchPtrPtr)
    TkCanvas *canvasPtr;                /* Canvas whose items are to be
                                         * searched. */
    Tcl_Obj *tagObj;                    /*  string giving tag value. */
    TagSearch **searchPtrPtr;           /* Record describing tag search;
                                         * will be initialized here. */
{
    char *tag = Tcl_GetStringFromObj(tagObj,NULL);
    int i;
    TagSearch *searchPtr;

    /*
     * Initialize the search.
     */

    if (*searchPtrPtr) {
        searchPtr = *searchPtrPtr;
    } else {
        /* Allocate primary search struct on first call */
        *searchPtrPtr = searchPtr = (TagSearch *) ckalloc(sizeof(TagSearch));
	searchPtr->expr = NULL;

        /* Allocate buffer for rewritten tags (after de-escaping) */
        searchPtr->rewritebufferAllocated = 100;
        searchPtr->rewritebuffer =
            ckalloc(searchPtr->rewritebufferAllocated);
    }
    TagSearchExprInit(&(searchPtr->expr));

    /* How long is the tagOrId ? */
    searchPtr->stringLength = strlen(tag);

    /* Make sure there is enough buffer to hold rewritten tags */
    if ((unsigned int)searchPtr->stringLength >=
	    searchPtr->rewritebufferAllocated) {
        searchPtr->rewritebufferAllocated = searchPtr->stringLength + 100;
        searchPtr->rewritebuffer =
            ckrealloc(searchPtr->rewritebuffer,
		    searchPtr->rewritebufferAllocated);
    }

    /* Initialize search */
    searchPtr->canvasPtr = canvasPtr;
    searchPtr->searchOver = 0;
    searchPtr->type = 0;

    /*
     * Find the first matching item in one of several ways. If the tag
     * is a number then it selects the single item with the matching
     * identifier.  In this case see if the item being requested is the
     * hot item, in which case the search can be skipped.
     */

    if (searchPtr->stringLength && isdigit(UCHAR(*tag))) {
        char *end;

        searchPtr->id = strtoul(tag, &end, 0);
        if (*end == 0) {
            searchPtr->type = 1;
            return TCL_OK;
	}
    }

    /*
     * For all other tags and tag expressions convert to a UID.
     * This UID is kept forever, but this should be thought of
     * as a cache rather than as a memory leak.
     */
    searchPtr->expr->uid = Tk_GetUid(tag);

    /* short circuit impossible searches for null tags */
    if (searchPtr->stringLength == 0) {
	return TCL_OK;
    }

    /*
     * Pre-scan tag for at least one unquoted "&&" "||" "^" "!"
     *   if not found then use string as simple tag
     */
    for (i = 0; i < searchPtr->stringLength ; i++) {
        if (tag[i] == '"') {
            i++;
            for ( ; i < searchPtr->stringLength; i++) {
                if (tag[i] == '\\') {
                    i++;
                    continue;
                }
                if (tag[i] == '"') {
                    break;
                }
            }
        } else {
            if ((tag[i] == '&' && tag[i+1] == '&')
             || (tag[i] == '|' && tag[i+1] == '|')
             || (tag[i] == '^')
             || (tag[i] == '!')) {
                searchPtr->type = 4;
                break;
            }
        }
    }

    searchPtr->string = tag;
    searchPtr->stringIndex = 0;
    if (searchPtr->type == 4) {
        /*
         * an operator was found in the prescan, so
         * now compile the tag expression into array of Tk_Uid
         * flagging any syntax errors found
         */
	if (TagSearchScanExpr(canvasPtr->interp, searchPtr, searchPtr->expr) != TCL_OK) {
            /* Syntax error in tag expression */
	    /* Result message set by TagSearchScanExpr */
	    return TCL_ERROR;
	}
	searchPtr->expr->length = searchPtr->expr->index;
    } else {
        if (searchPtr->expr->uid == allUid) {
            /*
             * All items match.
             */
            searchPtr->type = 2;
        } else {
            /*
             * Optimized single-tag search
             */
            searchPtr->type = 3;
        }
    }
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * TagSearchDestroy --
 *
 *      This procedure destroys any dynamic structures that
 *      may have been allocated by TagSearchScan.
 *
 * Results:
 *
 * Side effects:
 *
 *--------------------------------------------------------------
 */

static void
TagSearchDestroy(searchPtr)
    TagSearch *searchPtr;               /* Record describing tag search */
{
    if (searchPtr) {
        TagSearchExprDestroy(searchPtr->expr);
        ckfree((char *)searchPtr->rewritebuffer);
        ckfree((char *)searchPtr);
    }
}

/*
 *--------------------------------------------------------------
 *
 * TagSearchScanExpr --
 *
 *      This recursive procedure is called to scan a tag expression
 *      and compile it into an array of Tk_Uids.
 *
 * Results:
 *      The return value indicates if the tagOrId expression
 *      was successfully scanned (syntax).
 *      The information at *searchPtr is initialized
 *      such that a call to TagSearchFirst, followed by
 *      successive calls to TagSearchNext will return items
 *      that match tag.
 *
 * Side effects:
 *
 *--------------------------------------------------------------
 */

static int
TagSearchScanExpr(interp, searchPtr, expr)
    Tcl_Interp *interp;         /* Current interpreter. */
    TagSearch *searchPtr;       /* Search data */
    TagSearchExpr *expr;	/* compiled expression result */
{
    int looking_for_tag;        /* When true, scanner expects
                                 * next char(s) to be a tag,
                                 * else operand expected */
    int found_tag;              /* One or more tags found */
    int found_endquote;         /* For quoted tag string parsing */
    int negate_result;          /* Pending negation of next tag value */
    char *tag;                  /* tag from tag expression string */
    char c;

    negate_result = 0;
    found_tag = 0;
    looking_for_tag = 1;
    while (searchPtr->stringIndex < searchPtr->stringLength) {
        c = searchPtr->string[searchPtr->stringIndex++];

        if (expr->allocated == expr->index) {
            expr->allocated += 15;
	    if (expr->uids) {
		expr->uids =
                    (Tk_Uid *) ckrealloc((char *)(expr->uids),
                    (expr->allocated)*sizeof(Tk_Uid));
	    } else {
		expr->uids =
		(Tk_Uid *) ckalloc((expr->allocated)*sizeof(Tk_Uid));
	    }
        }

        if (looking_for_tag) {

            switch (c) {
                case ' '  :	/* ignore unquoted whitespace */
                case '\t' :
                case '\n' :
                case '\r' :
                    break;

                case '!'  :	/* negate next tag or subexpr */
                    if (looking_for_tag > 1) {
                        Tcl_AppendResult(interp,
                            "Too many '!' in tag search expression",
                            (char *) NULL);
                        return TCL_ERROR;
                    }
                    looking_for_tag++;
                    negate_result = 1;
                    break;

                case '('  :	/* scan (negated) subexpr recursively */
                    if (negate_result) {
                        expr->uids[expr->index++] = negparenUid;
                        negate_result = 0;
		    } else {
                        expr->uids[expr->index++] = parenUid;
		    }
                    if (TagSearchScanExpr(interp, searchPtr, expr) != TCL_OK) {
                        /* Result string should be already set
                         * by nested call to tag_expr_scan() */
			return TCL_ERROR;
		    }
                    looking_for_tag = 0;
                    found_tag = 1;
                    break;

                case '"'  :	/* quoted tag string */
                    if (negate_result) {
                        expr->uids[expr->index++] = negtagvalUid;
                        negate_result = 0;
                    } else {
                        expr->uids[expr->index++] = tagvalUid;
		    }
                    tag = searchPtr->rewritebuffer;
                    found_endquote = 0;
                    while (searchPtr->stringIndex < searchPtr->stringLength) {
                        c = searchPtr->string[searchPtr->stringIndex++];
                        if (c == '\\') {
                            c = searchPtr->string[searchPtr->stringIndex++];
			}
                        if (c == '"') {
                            found_endquote = 1;
			    break;
			}
                        *tag++ = c;
                    }
                    if (! found_endquote) {
                        Tcl_AppendResult(interp,
				"Missing endquote in tag search expression",
				(char *) NULL);
                        return TCL_ERROR;
                    }
                    if (! (tag - searchPtr->rewritebuffer)) {
                        Tcl_AppendResult(interp,
                            "Null quoted tag string in tag search expression",
                            (char *) NULL);
                        return TCL_ERROR;
                    }
                    *tag++ = '\0';
                    expr->uids[expr->index++] =
                        Tk_GetUid(searchPtr->rewritebuffer);
                    looking_for_tag = 0;
                    found_tag = 1;
                    break;

                case '&'  :	/* illegal chars when looking for tag */
                case '|'  :
                case '^'  :
                case ')'  :
                    Tcl_AppendResult(interp,
			    "Unexpected operator in tag search expression",
			    (char *) NULL);
                    return TCL_ERROR;

                default :	/* unquoted tag string */
                    if (negate_result) {
                        expr->uids[expr->index++] = negtagvalUid;
                        negate_result = 0;
                    } else {
                        expr->uids[expr->index++] = tagvalUid;
                    }
                    tag = searchPtr->rewritebuffer;
                    *tag++ = c;
                    /* copy rest of tag, including any embedded whitespace */
                    while (searchPtr->stringIndex < searchPtr->stringLength) {
                        c = searchPtr->string[searchPtr->stringIndex];
                        if (c == '!' || c == '&' || c == '|' || c == '^'
				|| c == '(' || c == ')' || c == '"') {
			    break;
                        }
                        *tag++ = c;
                        searchPtr->stringIndex++;
                    }
                    /* remove trailing whitespace */
                    while (1) {
                        c = *--tag;
                        /* there must have been one non-whitespace char,
                         *  so this will terminate */
                        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                            break;
			}
                    }
                    *++tag = '\0';
                    expr->uids[expr->index++] =
                        Tk_GetUid(searchPtr->rewritebuffer);
                    looking_for_tag = 0;
                    found_tag = 1;
            }

        } else {    /* ! looking_for_tag */

            switch (c) {
                case ' '  :	/* ignore whitespace */
                case '\t' :
                case '\n' :
                case '\r' :
                    break;

                case '&'  :	/* AND operator */
                    c = searchPtr->string[searchPtr->stringIndex++];
                    if (c != '&') {
                        Tcl_AppendResult(interp,
                                "Singleton '&' in tag search expression",
                                (char *) NULL);
                        return TCL_ERROR;
                    }
                    expr->uids[expr->index++] = andUid;
                    looking_for_tag = 1;
                    break;

                case '|'  :	/* OR operator */
                    c = searchPtr->string[searchPtr->stringIndex++];
                    if (c != '|') {
                        Tcl_AppendResult(interp,
                                "Singleton '|' in tag search expression",
                                (char *) NULL);
                        return TCL_ERROR;
                    }
                    expr->uids[expr->index++] = orUid;
                    looking_for_tag = 1;
                    break;

                case '^'  :	/* XOR operator */
                    expr->uids[expr->index++] = xorUid;
                    looking_for_tag = 1;
                    break;

                case ')'  :	/* end subexpression */
                    expr->uids[expr->index++] = endparenUid;
                    goto breakwhile;

                default   :	/* syntax error */
                    Tcl_AppendResult(interp,
			    "Invalid boolean operator in tag search expression",
			    (char *) NULL);
                    return TCL_ERROR;
            }
        }
    }
    breakwhile:
    if (found_tag && ! looking_for_tag) {
        return TCL_OK;
    }
    Tcl_AppendResult(interp, "Missing tag in tag search expression",
	    (char *) NULL);
    return TCL_ERROR;
}

/*
 *--------------------------------------------------------------
 *
 * TagSearchEvalExpr --
 *
 *      This recursive procedure is called to eval a tag expression.
 *
 * Results:
 *      The return value indicates if the tagOrId expression
 *      successfully matched the tags of the current item.
 *
 * Side effects:
 *
 *--------------------------------------------------------------
 */

static int
TagSearchEvalExpr(expr, itemPtr)
    TagSearchExpr *expr;        /* Search expression */
    Tk_Item *itemPtr;           /* Item being test for match */
{
    int looking_for_tag;        /* When true, scanner expects
                                 * next char(s) to be a tag,
                                 * else operand expected */
    int negate_result;          /* Pending negation of next tag value */
    Tk_Uid uid;
    Tk_Uid *tagPtr;
    int count;
    int result;                 /* Value of expr so far */
    int parendepth;

    result = 0;  /* just to keep the compiler quiet */

    negate_result = 0;
    looking_for_tag = 1;
    while (expr->index < expr->length) {
        uid = expr->uids[expr->index++];
        if (looking_for_tag) {
            if (uid == tagvalUid) {
/*
 *              assert(expr->index < expr->length);
 */
                uid = expr->uids[expr->index++];
                result = 0;
                /*
                 * set result 1 if tag is found in item's tags
                 */
                for (tagPtr = itemPtr->tagPtr, count = itemPtr->numTags;
                    count > 0; tagPtr++, count--) {
                    if (*tagPtr == uid) {
                        result = 1;
                        break;
                    }
                }

            } else if (uid == negtagvalUid) {
                negate_result = ! negate_result;
/*
 *              assert(expr->index < expr->length);
 */
                uid = expr->uids[expr->index++];
                result = 0;
                /*
                 * set result 1 if tag is found in item's tags
                 */
                for (tagPtr = itemPtr->tagPtr, count = itemPtr->numTags;
                    count > 0; tagPtr++, count--) {
                    if (*tagPtr == uid) {
                        result = 1;
                        break;
                    }
                }

            } else if (uid == parenUid) {
                /*
                 * evaluate subexpressions with recursion
                 */
                result = TagSearchEvalExpr(expr, itemPtr);

            } else if (uid == negparenUid) {
                negate_result = ! negate_result;
                /*
                 * evaluate subexpressions with recursion
                 */
                result = TagSearchEvalExpr(expr, itemPtr);
/*
 *          } else {
 *              assert(0);
 */
            }
            if (negate_result) {
                result = ! result;
                negate_result = 0;
            }
            looking_for_tag = 0;
        } else {    /* ! looking_for_tag */
            if (((uid == andUid) && (!result)) || ((uid == orUid) && result)) {
                /*
                 * short circuit expression evaluation
                 *
                 * if result before && is 0, or result before || is 1,
                 *   then the expression is decided and no further
                 *   evaluation is needed.
                 */

                    parendepth = 0;
		while (expr->index < expr->length) {
		    uid = expr->uids[expr->index++];
		    if (uid == tagvalUid || uid == negtagvalUid) {
			expr->index++;
			continue;
		    }
                        if (uid == parenUid || uid == negparenUid) {
                            parendepth++;
			continue;
		    } 
		    if (uid == endparenUid) {
                            parendepth--;
                            if (parendepth < 0) {
                                break;
                            }
                        }
                    }
                return result;

            } else if (uid == xorUid) {
                /*
                 * if the previous result was 1
                 *   then negate the next result
                 */
                negate_result = result;

            } else if (uid == endparenUid) {
                return result;
/*
 *          } else {
 *               assert(0);
 */
            }
            looking_for_tag = 1;
        }
    }
/*
 *  assert(! looking_for_tag);
 */
    return result;
}

/*
 *--------------------------------------------------------------
 *
 * TagSearchFirst --
 *
 *      This procedure is called to get the first item
 *      item that matches a preestablished search predicate
 *      that was set by TagSearchScan.
 *
 * Results:
 *      The return value is a pointer to the first item, or NULL
 *      if there is no such item.  The information at *searchPtr
 *      is updated such that successive calls to TagSearchNext
 *      will return successive items.
 *
 * Side effects:
 *      SearchPtr is linked into a list of searches in progress
 *      on canvasPtr, so that elements can safely be deleted
 *      while the search is in progress.
 *
 *--------------------------------------------------------------
 */

static Tk_Item *
TagSearchFirst(searchPtr)
    TagSearch *searchPtr;               /* Record describing tag search */
{
    Tk_Item *itemPtr, *lastPtr;
    Tk_Uid uid, *tagPtr;
    int count;

    /* short circuit impossible searches for null tags */
    if (searchPtr->stringLength == 0) {
        return NULL;
    }

    /*
     * Find the first matching item in one of several ways. If the tag
     * is a number then it selects the single item with the matching
     * identifier.  In this case see if the item being requested is the
     * hot item, in which case the search can be skipped.
     */

    if (searchPtr->type == 1) {
        Tcl_HashEntry *entryPtr;

        itemPtr = searchPtr->canvasPtr->hotPtr;
        lastPtr = searchPtr->canvasPtr->hotPrevPtr;
        if ((itemPtr == NULL) || (itemPtr->id != searchPtr->id) || (lastPtr == NULL)
	     || (lastPtr->nextPtr != itemPtr)) {
	       entryPtr = NULL;
	      entryPtr = Tcl_FindHashEntry(&searchPtr->canvasPtr->idTable,
                  (char *) searchPtr->id);
            
            if (entryPtr != NULL) {
                itemPtr = (Tk_Item *)Tcl_GetHashValue(entryPtr);
                lastPtr = itemPtr->prevPtr;
            } else {
                lastPtr = itemPtr = NULL;
            }
        }
        searchPtr->lastPtr = lastPtr;
        searchPtr->searchOver = 1;
        searchPtr->canvasPtr->hotPtr = itemPtr;
        searchPtr->canvasPtr->hotPrevPtr = lastPtr;
        return itemPtr;
    }

    if (searchPtr->type == 2) {

        /*
         * All items match.
         */

        searchPtr->lastPtr = NULL;
        searchPtr->currentPtr = searchPtr->canvasPtr->firstItemPtr;
        return searchPtr->canvasPtr->firstItemPtr;
    }

    if (searchPtr->type == 3) {

        /*
         * Optimized single-tag search
         */

        uid = searchPtr->expr->uid;
        for (lastPtr = NULL, itemPtr = searchPtr->canvasPtr->firstItemPtr;
                itemPtr != NULL; lastPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
            for (tagPtr = itemPtr->tagPtr, count = itemPtr->numTags;
                    count > 0; tagPtr++, count--) {
                if (*tagPtr == uid) {
                    searchPtr->lastPtr = lastPtr;
                    searchPtr->currentPtr = itemPtr;
                    return itemPtr;
                }
            }
        }
    } else {

    /*
         * None of the above.  Search for an item matching the tag expression.
     */

    for (lastPtr = NULL, itemPtr = searchPtr->canvasPtr->firstItemPtr;
                itemPtr != NULL; lastPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
	    searchPtr->expr->index = 0;
	    if (TagSearchEvalExpr(searchPtr->expr, itemPtr)) {
            searchPtr->lastPtr = lastPtr;
            searchPtr->currentPtr = itemPtr;
            return itemPtr;
        }
        }
    }
    searchPtr->lastPtr = lastPtr;
    searchPtr->searchOver = 1;
    return NULL;
}

/*
 *--------------------------------------------------------------
 *
 * TagSearchNext --
 *
 *      This procedure returns successive items that match a given
 *      tag;  it should be called only after TagSearchFirst has been
 *      used to begin a search.
 *
 * Results:
 *      The return value is a pointer to the next item that matches
 *      the tag expr specified to TagSearchScan, or NULL if no such
 *      item exists.  *SearchPtr is updated so that the next call
 *      to this procedure will return the next item.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

static Tk_Item *
TagSearchNext(searchPtr)
    TagSearch *searchPtr;               /* Record describing search in
                                         * progress. */
{
    Tk_Item *itemPtr, *lastPtr;
    Tk_Uid uid, *tagPtr;
    int count;

    /*
     * Find next item in list (this may not actually be a suitable
     * one to return), and return if there are no items left.
     */

    lastPtr = searchPtr->lastPtr;
    if (lastPtr == NULL) {
        itemPtr = searchPtr->canvasPtr->firstItemPtr;
    } else {
        itemPtr = lastPtr->nextPtr;
    }
    if ((itemPtr == NULL) || (searchPtr->searchOver)) {
        searchPtr->searchOver = 1;
        return NULL;
    }
    if (itemPtr != searchPtr->currentPtr) {
        /*
         * The structure of the list has changed.  Probably the
         * previously-returned item was removed from the list.
         * In this case, don't advance lastPtr;  just return
         * its new successor (i.e. do nothing here).
         */
    } else {
        lastPtr = itemPtr;
        itemPtr = lastPtr->nextPtr;
    }

    if (searchPtr->type == 2) {

        /*
         * All items match.
         */

        searchPtr->lastPtr = lastPtr;
        searchPtr->currentPtr = itemPtr;
        return itemPtr;
    }

    if (searchPtr->type == 3) {

        /*
         * Optimized single-tag search
         */

        uid = searchPtr->expr->uid;
        for ( ; itemPtr != NULL; lastPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
            for (tagPtr = itemPtr->tagPtr, count = itemPtr->numTags;
                    count > 0; tagPtr++, count--) {
                if (*tagPtr == uid) {
                    searchPtr->lastPtr = lastPtr;
                    searchPtr->currentPtr = itemPtr;
                    return itemPtr;
                }
            }
        }
        searchPtr->lastPtr = lastPtr;
        searchPtr->searchOver = 1;
        return NULL;
    }

    /*
     * Else.... evaluate tag expression
     */

    for ( ; itemPtr != NULL; lastPtr = itemPtr, itemPtr = itemPtr->nextPtr) {
        searchPtr->expr->index = 0;
        if (TagSearchEvalExpr(searchPtr->expr, itemPtr)) {
            searchPtr->lastPtr = lastPtr;
            searchPtr->currentPtr = itemPtr;
            return itemPtr;
        }
    }
    searchPtr->lastPtr = lastPtr;
    searchPtr->searchOver = 1;
    return NULL;
}
#endif /* USE_OLD_TAG_SEARCH */







