/* 
 * tkCanvEdge.c --
 *
 *	This file implements edge items for canvas widgets.
 *
 * Copyright (c) 1993 by Sven Delmas
 * All rights reserved.
 * See the file COPYRIGHT for the copyright notes.
 *
 *
 * This source is based upon the file tkCanvLine.c from:
 *
 * John Ousterhout
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

/* 05may96 wmt: converted to tk4.1 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdio.h>
#include <math.h>
#include "tkInt.h"
#include "tkCanvas.h"
/* #include "tkConfig.h" 05nov95 wmt */
#include "tkPort.h"

#ifdef _MSC_VER
#define F_OK 0
#endif

/*
 * The structure below defines the record for each edge item.
 */
typedef struct EdgeItem  {
  Tk_Item header;		/* Generic stuff that's the same for all
				 * types.  MUST BE FIRST IN STRUCTURE. */
  Tk_Canvas canvas;	        /* Canvas containing item.  Needed for
			         * parsing arrow shapes. a register variable */
  int numPoints;		/* Number of points in edge (always >= 2). */
  double *coordPtr;		/* Pointer to malloc-ed array containing
				 * x- and y-coords of all points in edge.
				 * X-coords are even-valued indices, y-coords
				 * are corresponding odd-valued indices. If
				 * the edge has arrowheads then the first
				 * and last points have been adjusted to refer
				 * to the necks of the arrowheads rather than
				 * their tips.  The actual endpoints are
				 * stored in the *firstArrowPtr and
				 * *lastArrowPtr, if they exist. */
  
  char *label;                  /* Label to display. */
  char *menu1;	                /* Standard menu for item, usually
                                 * activated with button-3. */
  char *menu2;		        /* Alternative menu for item, usually
                                 * activated with meta-button-3. */
  char *menu3;		        /* Alternative menu for item, usually
                                 * activated with control-button-3. */
  char *name;		        /* Name for item. */
  char *state;		        /* State of item, this value is used
                                 * to represent the selection status
                                 * (normal, selected). */
  char *graphName;              /* Name of the Graph. */
  char *from;                   /* From icon id. */
  char *to;                     /* To icon id. */
  Tk_Font tkfont;		/* Font for drawing text. */
  Tk_TextLayout textLayout;	/* Cached text layout information. */
  Tk_Justify justify;		/* Justification to use for text within
				 * window. */

  int width;			/* Width of edge. */
  int textHeight;		/* Height of text label in points. */
  int textWidth;		/* Width of text label in points. */
  XColor *fgColor;		/* Foreground color for edge. */
  XColor *bgColor;		/* Background color to use for icon. */
  Pixmap fillStipple;		/* Stipple bitmap for filling edge. */
  int capStyle;		        /* Cap style for edge. */
  int joinStyle;		/* Join style for edge. */

  GC invertedGc;		/* Graphics context to use for drawing
				 * the edge label on screen. */
  GC gc;			/* Graphics context for filling edge. */
  Tk_Uid arrow;		        /* Indicates whether or not to draw arrowheads:
			         * "none", "first", "last", or "both". */
  float arrowShapeA;		/* Distance from tip of arrowhead to center. */
  float arrowShapeB;		/* Distance from tip of arrowhead to trailing
				 * point, measured along shaft. */
  float arrowShapeC;		/* Distance of trailing points from outside
				 * edge of shaft. */
  double *firstArrowPtr;	/* Points to array of PTS_IN_ARROW points
				 * describing polygon for arrowhead at first
				 * point in edge.  First point of arrowhead
				 * is tip.  Malloc'ed.  NULL means no arrowhead
				 * at first point. */
  double *lastArrowPtr;     	/* Points to polygon for arrowhead at last
			         * point in edge (PTS_IN_ARROW points, first
			         * of which is tip).  Malloc'ed.  NULL means
			         * no arrowhead at last point. */
  int smooth;			/* Non-zero means draw edge smoothed (i.e.
				 * with Bezier splines). */
  int splineSteps;		/* Number of steps in each spline segment. */
} EdgeItem;

/*
 * Number of points in an arrowHead:
 */
#define PTS_IN_ARROW 6

/*
 * Prototypes for procedures defined in this file:
 */
static int		ArrowheadPostscript _ANSI_ARGS_((Tcl_Interp *interp,
                            Tk_Canvas canvas, EdgeItem *edgePtr,
                            double *arrowPtr));
static void		ComputeEdgeBbox _ANSI_ARGS_((Tk_Canvas canvas,
			    EdgeItem *edgePtr));
static int		ConfigureEdge _ANSI_ARGS_((Tcl_Interp *interp,
			    Tk_Canvas canvas, Tk_Item *itemPtr, int argc,
			    Tcl_Obj **ObjArgv, int flags));
static int		ConfigureArrows _ANSI_ARGS_((Tk_Canvas canvas,
			    EdgeItem *edgePtr));
static int		CreateEdge _ANSI_ARGS_((Tcl_Interp *interp,
                            Tk_Canvas canvas, struct Tk_Item *itemPtr, 
			    int argc, Tcl_Obj **ObjArgv));
static void		DeleteEdge _ANSI_ARGS_((Tk_Canvas canvas,
			    Tk_Item *itemPtr, Display *display));
static void		DisplayEdge _ANSI_ARGS_((Tk_Canvas canvas,
			    Tk_Item *itemPtr, Display *display, Drawable dst,
                             int x, int y, int width, int height));
static int		EdgeCoords _ANSI_ARGS_((Tcl_Interp *interp,
                            Tk_Canvas canvas, Tk_Item *itemPtr,
			    int argc, Tcl_Obj **ObjArgv));
static int		EdgeToArea _ANSI_ARGS_((Tk_Canvas canvas,
			    Tk_Item *itemPtr, double *rectPtr));
static double		EdgeToPoint _ANSI_ARGS_((Tk_Canvas canvas,
			    Tk_Item *itemPtr, double *coordPtr));
static int		EdgeToPostscript _ANSI_ARGS_((Tcl_Interp *interp,
                            Tk_Canvas canvas, Tk_Item *itemPtr, int prepass));
static int		ParseArrowShape _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tk_Window tkwin, char *value,
			    char *recordPtr, int offset));
static char *		PrintArrowShape _ANSI_ARGS_((ClientData clientData,
			    Tk_Window tkwin, char *recordPtr, int offset,
			    Tcl_FreeProc **freeProcPtr));
static void		ScaleEdge _ANSI_ARGS_((Tk_Canvas canvas,
			    Tk_Item *itemPtr, double originX, double originY,
			    double scaleX, double scaleY));
static void		TranslateEdge _ANSI_ARGS_((Tk_Canvas canvas,
			    Tk_Item *itemPtr, double deltaX, double deltaY));

/*
 * Information used for parsing configuration specs.  If you change any
 * of the default strings, be sure to change the corresponding default
 * values in CreateEdge.
 */
static Tk_CustomOption arrowShapeOption =
{ ParseArrowShape, PrintArrowShape, (ClientData) NULL};

/*
 * The callbacks for tagsOption are initialized in ConfigureEdge()
 */

static Tk_CustomOption tagsOption =
{ (Tk_OptionParseProc *) NULL,
  (Tk_OptionPrintProc *) NULL,
  (ClientData) NULL};

static Tk_ConfigSpec configSpecs[] = {
  {TK_CONFIG_UID, "-arrow", (char *) NULL, (char *) NULL,
     "none", Tk_Offset(EdgeItem, arrow), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_CUSTOM, "-arrowshape", (char *) NULL, (char *) NULL,
     "8 10 3", Tk_Offset(EdgeItem, arrowShapeA),
     TK_CONFIG_DONT_SET_DEFAULT, &arrowShapeOption},
  {TK_CONFIG_COLOR, "-background", (char *) NULL, (char *) NULL,
     (char *) NULL, Tk_Offset(EdgeItem, bgColor), TK_CONFIG_NULL_OK},
  {TK_CONFIG_CAP_STYLE, "-capstyle", (char *) NULL, (char *) NULL,
     "butt", Tk_Offset(EdgeItem, capStyle), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_COLOR, "-fill", (char *) NULL, (char *) NULL,
     "black", Tk_Offset(EdgeItem, fgColor), TK_CONFIG_NULL_OK},
  {TK_CONFIG_FONT, "-font", (char *) NULL, (char *) NULL,
     "Helvetica 12 bold", Tk_Offset(EdgeItem, tkfont), 0},
  {TK_CONFIG_STRING, "-from", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, from), 0},
  {TK_CONFIG_STRING, "-graphname", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, graphName), 0},
  {TK_CONFIG_JOIN_STYLE, "-joinstyle", (char *) NULL, (char *) NULL,
     "round", Tk_Offset(EdgeItem, joinStyle), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_STRING, "-label", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, label), 0},
  {TK_CONFIG_STRING, "-menu1", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, menu1), 0},
  {TK_CONFIG_STRING, "-menu2", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, menu2), 0},
  {TK_CONFIG_STRING, "-menu3", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, menu3), 0},
  {TK_CONFIG_STRING, "-name", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, name), 0},
  {TK_CONFIG_BOOLEAN, "-smooth", (char *) NULL, (char *) NULL,
     "0", Tk_Offset(EdgeItem, smooth), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_INT, "-splinesteps", (char *) NULL, (char *) NULL,
     "12", Tk_Offset(EdgeItem, splineSteps), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_STRING, "-state", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, state), 0},
  {TK_CONFIG_BITMAP, "-stipple", (char *) NULL, (char *) NULL,
     (char *) NULL, Tk_Offset(EdgeItem, fillStipple), TK_CONFIG_NULL_OK},
  {TK_CONFIG_CUSTOM, "-tags", (char *) NULL, (char *) NULL,
     (char *) NULL, 0, TK_CONFIG_NULL_OK, &tagsOption},
  {TK_CONFIG_PIXELS, "-textheight", (char *) NULL, (char *) NULL,
     "0", Tk_Offset(EdgeItem, textHeight), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_PIXELS, "-textwidth", (char *) NULL, (char *) NULL,
     "0", Tk_Offset(EdgeItem, textWidth), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_STRING, "-to", (char *) NULL, (char *) NULL,
     "", Tk_Offset(EdgeItem, to), 0},
  {TK_CONFIG_PIXELS, "-width", (char *) NULL, (char *) NULL,
     "1", Tk_Offset(EdgeItem, width), TK_CONFIG_DONT_SET_DEFAULT},
  {TK_CONFIG_JUSTIFY, "-justify", "justify", "Justify",
	"left", Tk_Offset(EdgeItem, justify), 0},
  {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL,
     (char *) NULL, 0, 0}
};

/*
 * The structures below defines the edge item type by means
 * of procedures that can be invoked by generic item code.
 */
Tk_ItemType tkEdgeType = {
    "edge",				/* name */
    sizeof(EdgeItem),			/* itemSize */
    CreateEdge,				/* createProc */
    configSpecs,			/* configSpecs */
    ConfigureEdge,			/* configureProc */
    EdgeCoords,				/* coordProc */
    DeleteEdge,				/* deleteProc */
    DisplayEdge,			/* displayProc */
    TK_CONFIG_OBJS,			/* flags */
    EdgeToPoint,			/* pointProc */
    EdgeToArea,				/* areaProc */
    EdgeToPostscript,			/* postscriptProc */
    ScaleEdge,				/* scaleProc */
    TranslateEdge,			/* translateProc */
    (Tk_ItemIndexProc *) NULL,		/* indexProc */
    (Tk_ItemCursorProc *) NULL,		/* icursorProc */
    (Tk_ItemSelectionProc *) NULL,	/* selectionProc */
    (Tk_ItemInsertProc *) NULL,		/* insertProc */
    (Tk_ItemDCharsProc *) NULL,		/* dTextProc */
    (Tk_ItemType *) NULL		/* nextPtr */
};

/*
 * The Tk_Uid's below refer to uids for the various arrow types:
 */
static Tk_Uid noneUid = NULL;
static Tk_Uid firstUid = NULL;
static Tk_Uid lastUid = NULL;
static Tk_Uid bothUid = NULL;

/*
 * The definition below determines how large are static arrays
 * used to hold spline points (splines larger than this have to
 * have their arrays malloc-ed).
 */
#define MAX_STATIC_POINTS 200

/*
 *--------------------------------------------------------------
 *
 * CreateEdge --
 *
 *	This procedure is invoked to create a new edge item in
 *	a canvas.
 *
 * Results:
 *	A standard Tcl return value.  If an error occurred in
 *	creating the item, then an error message is left in
 *	interp->result;  in this case itemPtr is
 *	left uninitialized, so it can be safely freed by the
 *	caller.
 *
 * Side effects:
 *	A new edge item is created.
 *
 *--------------------------------------------------------------
 */

static int
CreateEdge(interp, canvas, itemPtr, argc, ObjArgv)
     Tcl_Interp *interp;		/* Interpreter for error reporting. */
     Tk_Canvas canvas;	                /* Canvas to hold new item. */
     Tk_Item *itemPtr;			/* Record to hold new item;  header
					 * has been initialized by caller. */
     int argc;				/* Number of arguments in argv. */
     Tcl_Obj **ObjArgv;			/* Arguments describing edge. */
{
  char **argv;
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  int i;
  
  if (argc < 4) {
    Tcl_AppendResult(interp, "wrong # args:  should be \"",
		     Tk_PathName(Tk_CanvasTkwin(canvas)), "\" create ",
		     itemPtr->typePtr->name,
		     " x1 y1 x2 y2 ?x3 y3 ...? ?options?",
		     (char *) NULL);
    return TCL_ERROR;
  }
  
  /*
   * Carry out initialization that is needed to set defaults and to
   * allow proper cleanup after errors during the the remainder of
   * this procedure.
   */
  edgePtr->bgColor = None;
  edgePtr->canvas = canvas;
  edgePtr->capStyle = CapButt;
  edgePtr->coordPtr = NULL;
  edgePtr->fgColor = None;
  edgePtr->fillStipple = None;
  edgePtr->tkfont = NULL;
  edgePtr->from = NULL;
  edgePtr->graphName = NULL;
  edgePtr->joinStyle = JoinRound;
  edgePtr->label = NULL;
  edgePtr->menu1 = NULL;
  edgePtr->menu2 = NULL;
  edgePtr->menu3 = NULL;
  edgePtr->name = NULL;
  edgePtr->numPoints = 0;
  edgePtr->smooth = 0;
  edgePtr->splineSteps = 12;
  edgePtr->state = NULL;
  edgePtr->textWidth = 0;
  edgePtr->to = NULL;
  edgePtr->width = 1;
  edgePtr->textLayout = NULL;
  edgePtr->justify = TK_JUSTIFY_LEFT;

  edgePtr->invertedGc = None;
  edgePtr->gc = None;
  if (noneUid == NULL) {
    noneUid = Tk_GetUid("none");
    firstUid = Tk_GetUid("first");
    lastUid = Tk_GetUid("last");
    bothUid = Tk_GetUid("both");
  }
  edgePtr->arrow = noneUid;
  edgePtr->arrowShapeA = 8.0;
  edgePtr->arrowShapeB = 10.0;
  edgePtr->arrowShapeC = 3.0;
  edgePtr->firstArrowPtr = NULL;
  edgePtr->lastArrowPtr = NULL;

  /*
   * Count the number of points and then parse them into a point
   * array.  Leading arguments are assumed to be points if they
   * start with a digit or a minus sign followed by a digit.
   */

  /* TODO: tidy up for loop, we shouldn't need to do
   * ckalloc and Tcl_GetString, should we?
   */

  /*
   * FIXME: memory leak here. 
   */  
  argv = (char**) ckalloc(argc * sizeof(char**));
  for (i = 4; i < (argc-1); i+=2) {
    argv[i]=Tcl_GetString(ObjArgv[i]);
    if ((!isdigit(UCHAR(argv[i][0]))) &&
	((argv[i][0] != '-') || (!isdigit(UCHAR(argv[i][1]))))) {
      break;
    }
  }

  if (EdgeCoords(interp, canvas, itemPtr, i, ObjArgv) != TCL_OK) {
    goto error;
  }
  if (ConfigureEdge(interp, canvas, itemPtr, argc-i, ObjArgv+i, 0) == TCL_OK) {
    return TCL_OK;
  } 
  
 error:
  DeleteEdge(canvas, itemPtr, Tk_Display(Tk_CanvasTkwin(canvas)));
  return TCL_ERROR;
}

/*
 *--------------------------------------------------------------
 *
 * EdgeCoords --
 *
 *	This procedure is invoked to process the "coords" widget
 *	command on edges.  See the user documentation for details
 *	on what it does.
 *
 * Results:
 *	Returns TCL_OK or TCL_ERROR, and sets interp->result.
 *
 * Side effects:
 *	The coordinates for the given item may be changed.
 *
 *--------------------------------------------------------------
 */

static int
EdgeCoords(interp, canvas, itemPtr, argc, ObjArgv)
     Tcl_Interp *interp;		/* Used for error reporting. */
     Tk_Canvas canvas;	                /* Canvas containing item. */
     Tk_Item *itemPtr;			/* Item whose coordinates are to be
					 * read or modified. */
     int argc;				/* Number of coordinates supplied in
					 * argv. */
     Tcl_Obj **ObjArgv;			/* Array of coordinates: x1, y1,
					 * x2, y2, ... */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  char buffer[TCL_DOUBLE_SPACE];
  int i, numPoints;
  
  if (argc == 0) {
    double *coordPtr;
    int numCoords;
    
    numCoords = 2*edgePtr->numPoints;
    if (edgePtr->firstArrowPtr != NULL) {
      coordPtr = edgePtr->firstArrowPtr;
    } else {
      coordPtr = edgePtr->coordPtr;
    }
    for (i = 0; i < numCoords; i++, coordPtr++) {
      if (i == 2) {
	coordPtr = edgePtr->coordPtr+2;
      }
      if ((edgePtr->lastArrowPtr != NULL) && (i == (numCoords-2))) {
	coordPtr = edgePtr->lastArrowPtr;
      }
      Tcl_PrintDouble(interp, *coordPtr, buffer);
      Tcl_AppendElement(interp, buffer);
    }
  } else if (argc < 4) {
    Tcl_AppendResult(interp,
		     "too few coordinates for edge:  must have at least 4",
		     (char *) NULL);
    return TCL_ERROR;
  } else if (argc & 1) {
    Tcl_AppendResult(interp,
		     "odd number of coordinates specified for edge",
		     (char *) NULL);
    return TCL_ERROR;
  } else {
    numPoints = argc/2;
    if (edgePtr->numPoints != numPoints) {
      if (edgePtr->coordPtr != NULL) {
	ckfree((char *) edgePtr->coordPtr);
      }
      edgePtr->coordPtr = (double *) ckalloc((unsigned)
					     (sizeof(double) * argc));
      edgePtr->numPoints = numPoints;
    }
    for (i = argc-1; i >= 0; i--) {
      if (Tk_CanvasGetCoord(interp, canvas, Tcl_GetString(ObjArgv[i]), &edgePtr->coordPtr[i])
	  != TCL_OK) {
	return TCL_ERROR;
      }
    }
    
    /*
     * Update arrowheads by throwing away any existing arrow-head
     * information and calling ConfigureArrows to recompute it.
     */
    
    if (edgePtr->firstArrowPtr != NULL) {
      ckfree((char *) edgePtr->firstArrowPtr);
      edgePtr->firstArrowPtr = NULL;
    }
    if (edgePtr->lastArrowPtr != NULL) {
      ckfree((char *) edgePtr->lastArrowPtr);
      edgePtr->lastArrowPtr = NULL;
    }
    if (edgePtr->arrow != noneUid) {
      ConfigureArrows(canvas, edgePtr);
    }
    ComputeEdgeBbox(canvas, edgePtr);
  }
  return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * ConfigureEdge --
 *
 *	This procedure is invoked to configure various aspects
 *	of a edge item such as its background color.
 *
 * Results:
 *	A standard Tcl result code.  If an error occurs, then
 *	an error message is left in interp->result.
 *
 * Side effects:
 *	Configuration information, such as colors and stipple
 *	patterns, may be set for itemPtr.
 *
 *--------------------------------------------------------------
 */

static int
ConfigureEdge(interp, canvas, itemPtr, argc, ObjArgv, flags)
     Tcl_Interp *interp;	/* Used for error reporting. */
     Tk_Canvas canvas;	        /* Canvas containing itemPtr. */
     Tk_Item *itemPtr;		/* Edge item to reconfigure. */
     int argc;			/* Number of elements in argv.  */
     Tcl_Obj **ObjArgv;		/* Arguments describing things to configure. */
     int flags;			/* Flags to pass to Tk_ConfigureWidget. */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  XGCValues gcValues;
  GC newGC;
  unsigned long mask;
  char *value, *fullName, **list;
  int counter, listCounter = 0;
  Tcl_DString varName, fileName, buffer;
  Tk_Window tkwin;
  Tk_3DBorder bgBorder;
  char **argv;
  int loopcount;

  tkwin = Tk_CanvasTkwin(canvas);
  bgBorder = ((TkCanvas *) canvas)->bgBorder;

  argv = (char**) ckalloc(argc * sizeof(char**));
  for (loopcount = 0 ; loopcount < argc ; loopcount++) {
      argv[loopcount] = Tcl_GetString( ObjArgv[loopcount] );
  }
  /*
   * Init callbacks in tagsOption before accessing configSpecs.
   * This init can't be done statically when using Windows gcc
   * since these symbols are imported from the Tk dll.
   */

  if (tagsOption.parseProc == NULL) {
    tagsOption.parseProc = Tk_CanvasTagsParseProc;
    tagsOption.printProc = Tk_CanvasTagsPrintProc;
  }

  if (Tk_ConfigureWidget(interp, tkwin,
			 configSpecs, argc, argv,
			 (char *) edgePtr, flags) != TCL_OK) {
    return TCL_ERROR;
  }

  /*
   * A few of the options require additional processing, such as
   * graphics contexts.
   */

  /* the normal gc */
  if (edgePtr->fgColor == NULL) {
    newGC = None;
  } else {
    mask = GCBackground|GCForeground|GCJoinStyle|GCLineWidth;
    if (edgePtr->bgColor != NULL) {
      gcValues.background = edgePtr->bgColor->pixel;
    } else {
      gcValues.background = Tk_3DBorderColor(bgBorder)->pixel;
    }
    gcValues.foreground = edgePtr->fgColor->pixel;
    gcValues.join_style = edgePtr->joinStyle;
    if (edgePtr->width < 0) {
      edgePtr->width = 1;
    }
    gcValues.line_width = edgePtr->width;
    if (edgePtr->fillStipple != None) {
      gcValues.stipple = edgePtr->fillStipple;
      gcValues.fill_style = FillStippled;
      mask |= GCStipple|GCFillStyle;
    }
    if (edgePtr->arrow == noneUid) {
      gcValues.cap_style = edgePtr->capStyle;
      mask |= GCCapStyle;
    }
    if (edgePtr->tkfont != NULL) {
      gcValues.font = Tk_FontId(edgePtr->tkfont);
      mask |= GCFont;
    }
    newGC = Tk_GetGC(tkwin, mask, &gcValues);
  }
  if (edgePtr->gc != None) {
    Tk_FreeGC(((TkCanvas *) canvas)->display, edgePtr->gc);
  }
  edgePtr->gc = newGC;
  
  /* the inverted gc */
  if (edgePtr->fgColor == NULL) {
    newGC = None;
  } else {
    mask = GCForeground | GCBackground;
    gcValues.background = edgePtr->fgColor->pixel;
    if (edgePtr->bgColor != NULL) {
      gcValues.foreground = edgePtr->bgColor->pixel;
    } else {
      gcValues.foreground = Tk_3DBorderColor(bgBorder)->pixel;
    }
    if (edgePtr->tkfont != NULL) {
      gcValues.font = Tk_FontId(edgePtr->tkfont);
      mask |= GCFont;
    }
    newGC = Tk_GetGC(tkwin, mask, &gcValues);
  }
  if (edgePtr->invertedGc != None) {
    Tk_FreeGC(((TkCanvas *) canvas)->display, edgePtr->invertedGc);
  }
  edgePtr->invertedGc = newGC;
  
  /*
   * Keep spline parameters within reasonable limits.
   */
  if (edgePtr->splineSteps < 1) {
    edgePtr->splineSteps = 1;
  } else if (edgePtr->splineSteps > 100) {
    edgePtr->splineSteps = 100;
  }
  
  /*
   * Setup arrowheads, if needed.  If arrowheads are turned off,
   * restore the edge's endpoints (they were shortened when the
   * arrowheads were added).
   */
  if ((edgePtr->firstArrowPtr != NULL) && (edgePtr->arrow != firstUid)
      && (edgePtr->arrow != bothUid)) {
    edgePtr->coordPtr[0] = edgePtr->firstArrowPtr[0];
    edgePtr->coordPtr[1] = edgePtr->firstArrowPtr[1];
    ckfree((char *) edgePtr->firstArrowPtr);
    edgePtr->firstArrowPtr = NULL;
  }
  if ((edgePtr->lastArrowPtr != NULL) && (edgePtr->arrow != lastUid)
      && (edgePtr->arrow != bothUid)) {
    int index;
    
    index = 2*(edgePtr->numPoints-1);
    edgePtr->coordPtr[index] = edgePtr->lastArrowPtr[0];
    edgePtr->coordPtr[index+1] = edgePtr->lastArrowPtr[1];
    ckfree((char *) edgePtr->lastArrowPtr);
    edgePtr->lastArrowPtr = NULL;
  }
  if (edgePtr->arrow != noneUid) {
    if ((edgePtr->arrow != firstUid) && (edgePtr->arrow != lastUid)
	&& (edgePtr->arrow != bothUid)) {
      Tcl_AppendResult(interp, "bad arrow spec \"",
		       edgePtr->arrow,
		       "\": must be none, first, last, or both",
		       (char *) NULL);
      edgePtr->arrow = noneUid;
      return TCL_ERROR;
    }
    ConfigureArrows(canvas, edgePtr);
  }

  /* Calculate the text width & height in points. */
  Tk_FreeTextLayout(edgePtr->textLayout);
  edgePtr->textLayout = Tk_ComputeTextLayout(edgePtr->tkfont,
	    edgePtr->label,
	    strlen (edgePtr->label),
	    edgePtr->width,
	    edgePtr->justify,
	    0, &edgePtr->textWidth, &edgePtr->textHeight);
  
  /* do we have a menu ? */
  if (edgePtr->menu1 != NULL && strlen(edgePtr->menu1) > (size_t) 0 &&
      edgePtr->menu1[0] != '.') {
    /* do we have to load the new menu definition ? */
    (void) Tcl_VarEval(interp, "info commands .emenu-",
		       edgePtr->menu1, (char *) NULL);
    if (strlen(interp->result) == 0) {
      /* the following code retrieves the path list for the menus. This */
      /* is done because I don't want to attatch the pathname list to */
      /* each icon. */
      Tcl_DStringInit(&varName);
      Tcl_DStringAppend(&varName, "ip_priv(", -1);
      Tcl_DStringAppend(&varName, Tk_PathName(tkwin), -1);
      Tcl_DStringAppend(&varName, ",edgemenupath)", -1);
      if ((value = Tcl_GetVar(interp, varName.string,
			      TCL_GLOBAL_ONLY)) != NULL) {
	if (Tcl_SplitList(interp, value, &listCounter,
			  &list) == TCL_OK) {
	  /* walk through list of pathnames. */
	  for (counter = 0; counter < listCounter; counter++) {
	    /* create the filename to load. */
	    Tcl_DStringInit(&fileName);
	    Tcl_DStringAppend(&fileName, list[counter], -1);
	    Tcl_DStringAppend(&fileName, "/", -1);
	    Tcl_DStringAppend(&fileName, edgePtr->menu1, -1);
	    Tcl_DStringAppend(&fileName, ".emenu", -1);
	    Tcl_DStringInit(&buffer);
	    fullName = Tcl_TildeSubst(interp,
				      fileName.string, &buffer);
	    if (access(fullName, F_OK) != -1) {
	      /* load new menu. */
	      Tcl_VarEval(interp, "source ", fullName,
			  (char *) NULL);
	    }
	    Tcl_DStringFree(&fileName);
	    Tcl_DStringFree(&buffer);
	  }
	  ckfree((char *) list);
	}
      }
      Tcl_DStringFree(&varName);
    }
  }
    
  /* do we have a menu ? */
  if (edgePtr->menu2 != NULL && strlen(edgePtr->menu2) > (size_t) 0 &&
      edgePtr->menu2[0] != '.') {
    /* do we have to load the new menu definition ? */
    (void) Tcl_VarEval(interp, "info commands .emenu-",
		       edgePtr->menu2, (char *) NULL);
    if (strlen(interp->result) == 0) {
      /* the following code retrieves the path list for the menus. This */
      /* is done because I don't want to attatch the pathname list to */
      /* each icon. */
      Tcl_DStringInit(&varName);
      Tcl_DStringAppend(&varName, "ip_priv(", -1);
      Tcl_DStringAppend(&varName, Tk_PathName(tkwin), -1);
      Tcl_DStringAppend(&varName, ",edgemenupath)", -1);
      if ((value = Tcl_GetVar(interp, varName.string,
			      TCL_GLOBAL_ONLY)) != NULL) {
	if (Tcl_SplitList(interp, value, &listCounter,
			  &list) == TCL_OK) {
	  /* walk through list of pathnames. */
	  for (counter = 0; counter < listCounter; counter++) {
	    /* create the filename to load. */
	    Tcl_DStringInit(&fileName);
	    Tcl_DStringAppend(&fileName, list[counter], -1);
	    Tcl_DStringAppend(&fileName, "/", -1);
	    Tcl_DStringAppend(&fileName, edgePtr->menu2, -1);
	    Tcl_DStringAppend(&fileName, ".emenu", -1);
	    Tcl_DStringInit(&buffer);
	    fullName = Tcl_TildeSubst(interp,
				      fileName.string, &buffer);
	    if (access(fullName, F_OK) != -1) {
	      /* load new menu. */
	      Tcl_VarEval(interp, "source ", fullName,
			  (char *) NULL);
	    }
	    Tcl_DStringFree(&fileName);
	    Tcl_DStringFree(&buffer);
	  }
	  ckfree((char *) list);
	}
      }
      Tcl_DStringFree(&varName);
    }
  }
  
  /* do we have a menu ? */
  if (edgePtr->menu3 != NULL && strlen(edgePtr->menu3) > (size_t) 0 &&
      edgePtr->menu3[0] != '.') {
    /* do we have to load the new menu definition ? */
    (void) Tcl_VarEval(interp, "info commands .emenu-",
		       edgePtr->menu3, (char *) NULL);
    if (strlen(interp->result) == 0) {
      /* the following code retrieves the path list for the menus. This */
      /* is done because I don't want to attatch the pathname list to */
      /* each icon. */
      Tcl_DStringInit(&varName);
      Tcl_DStringAppend(&varName, "ip_priv(", -1);
      Tcl_DStringAppend(&varName, Tk_PathName(tkwin), -1);
      Tcl_DStringAppend(&varName, ",edgemenupath)", -1);
      if ((value = Tcl_GetVar(interp, varName.string,
			      TCL_GLOBAL_ONLY)) != NULL) {
	if (Tcl_SplitList(interp, value, &listCounter,
			  &list) == TCL_OK) {
	  /* walk through list of pathnames. */
	  for (counter = 0; counter < listCounter; counter++) {
	    /* create the filename to load. */
	    Tcl_DStringInit(&fileName);
	    Tcl_DStringAppend(&fileName, list[counter], -1);
	    Tcl_DStringAppend(&fileName, "/", -1);
	    Tcl_DStringAppend(&fileName, edgePtr->menu3, -1);
	    Tcl_DStringAppend(&fileName, ".emenu", -1);
	    Tcl_DStringInit(&buffer);
	    fullName = Tcl_TildeSubst(interp,
				      fileName.string, &buffer);
	    if (access(fullName, F_OK) != -1) {
	      /* load new menu. */
	      Tcl_VarEval(interp, "source ", fullName,
			  (char *) NULL);
	    }
	    Tcl_DStringFree(&fileName);
	    Tcl_DStringFree(&buffer);
	  }
	  ckfree((char *) list);
	}
      }
      Tcl_DStringFree(&varName);
    }
  }
  
  /*
   * Recompute bounding box for edge.
   */
  
  ComputeEdgeBbox(canvas, edgePtr);
  
  return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * DeleteEdge --
 *
 *	This procedure is called to clean up the data structure
 *	associated with a edge item.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Resources associated with itemPtr are released.
 *
 *--------------------------------------------------------------
 */

static void
DeleteEdge(canvas, itemPtr, display)
     Tk_Canvas canvas;		        /* Info about overall canvas widget. */
     Tk_Item *itemPtr;			/* Item that is being deleted. */
     Display *display;			/* Display containing window for
					 * canvas. */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  
  if (edgePtr->bgColor != NULL) {
    Tk_FreeColor(edgePtr->bgColor);
  }
  if (edgePtr->coordPtr != NULL) {
    ckfree((char *) edgePtr->coordPtr);
  }
  if (edgePtr->fgColor != NULL) {
    Tk_FreeColor(edgePtr->fgColor);
  }
  if (edgePtr->fillStipple != None) {
    Tk_FreeBitmap(display, edgePtr->fillStipple);
  }
  if (edgePtr->tkfont != NULL) {
    Tk_FreeFont(edgePtr->tkfont);
  }
  if (edgePtr->from != NULL) {
    ckfree(edgePtr->from);
  }
  if (edgePtr->graphName != NULL) {
    ckfree(edgePtr->graphName);
  }
  if (edgePtr->label != NULL) {
    ckfree(edgePtr->label);
  }
  if (edgePtr->menu1 != NULL) {
    ckfree(edgePtr->menu1);
  }
  if (edgePtr->menu2 != NULL) {
    ckfree(edgePtr->menu2);
  }
  if (edgePtr->menu3 != NULL) {
    ckfree(edgePtr->menu3);
  }
  if (edgePtr->name != NULL) {
    ckfree(edgePtr->name);
  }
  if (edgePtr->state != NULL) {
    ckfree(edgePtr->state);
  }
  if (edgePtr->to != NULL) {
    ckfree(edgePtr->to);
  }
  
  if (edgePtr->invertedGc != None) {
    Tk_FreeGC(display, edgePtr->invertedGc);
  }
  if (edgePtr->gc != None) {
    Tk_FreeGC(display, edgePtr->gc);
  }
  if (edgePtr->firstArrowPtr != NULL) {
    ckfree((char *) edgePtr->firstArrowPtr);
  }
  if (edgePtr->lastArrowPtr != NULL) {
    ckfree((char *) edgePtr->lastArrowPtr);
  }
  if (edgePtr->textLayout != NULL) {
    ckfree((char *) edgePtr->textLayout);
  }
}

/*
 *--------------------------------------------------------------
 *
 * ComputeEdgeBbox --
 *
 *	This procedure is invoked to compute the bounding box of
 *	all the pixels that may be drawn as part of a edge.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The fields x1, y1, x2, and y2 are updated in the header
 *	for itemPtr.
 *
 *--------------------------------------------------------------
 */

static void
ComputeEdgeBbox(canvas, edgePtr)
     Tk_Canvas canvas;	                /* Canvas that contains item. */
     EdgeItem *edgePtr;			/* Item whose bbos is to be
					 * recomputed. */
{
  double *coordPtr;
  int i, lineWidth, lineHeight;
  
  coordPtr = edgePtr->coordPtr;
  edgePtr->header.x1 = edgePtr->header.x2 = *coordPtr;
  edgePtr->header.y1 = edgePtr->header.y2 = coordPtr[1];
  
  /*
   * Compute the bounding box of all the points in the edge,
   * then expand in all directions by the edge's width to take
   * care of butting or rounded corners and projecting or
   * rounded caps.  This expansion is an overestimate (worst-case
   * is square root of two over two) but it's simple.  Don't do
   * anything special for curves.  This causes an additional
   * overestimate in the bounding box, but is faster.
   */
  
  for (i = 1, coordPtr = edgePtr->coordPtr+2; i < edgePtr->numPoints;
       i++, coordPtr += 2) {
    TkIncludePoint((Tk_Item *) edgePtr, coordPtr);
  }
  edgePtr->header.x1 -= edgePtr->width;
  edgePtr->header.x2 += edgePtr->width;
  edgePtr->header.y1 -= edgePtr->width;
  edgePtr->header.y2 += edgePtr->width;
  
  /*
   * For mitered edges, make a second pass through all the points.
   * Compute the locations of the two miter vertex points and add
   * those into the bounding box.
   */
  
  if (edgePtr->joinStyle == JoinMiter) {
    for (i = edgePtr->numPoints, coordPtr = edgePtr->coordPtr; i >= 3;
	 i--, coordPtr += 2) {
      double miter[4];
      int j;
      
      if (TkGetMiterPoints(coordPtr, coordPtr+2, coordPtr+4,
			   (double) edgePtr->width, miter, miter+2)) {
	for (j = 0; j < 4; j += 2) {
	  TkIncludePoint((Tk_Item *) edgePtr, miter+j);
	}
      }
    }
  }
  
  /*
   * Add in the sizes of arrowheads, if any.
   */
  
  if (edgePtr->arrow != noneUid) {
    if (edgePtr->arrow != lastUid) {
      for (i = 0, coordPtr = edgePtr->firstArrowPtr; i < PTS_IN_ARROW;
	   i++, coordPtr += 2) {
	TkIncludePoint((Tk_Item *) edgePtr, coordPtr);
      }
    }
    if (edgePtr->arrow != firstUid) {
      for (i = 0, coordPtr = edgePtr->lastArrowPtr; i < PTS_IN_ARROW;
	   i++, coordPtr += 2) {
	TkIncludePoint((Tk_Item *) edgePtr, coordPtr);
      }
    }
  }
  
  /*
   * Add one more pixel of fudge factor just to be safe (e.g.
   * X may round differently than we do).
   */
  
  edgePtr->header.x1 -= 1;
  edgePtr->header.x2 += 1;
  edgePtr->header.y1 -= 1;
  edgePtr->header.y2 += 1;

  /* maybe we have a label that is wider than the line */
  if (edgePtr->tkfont != NULL && edgePtr->label != NULL) {
  Tk_FreeTextLayout(edgePtr->textLayout);
  edgePtr->textLayout = Tk_ComputeTextLayout(edgePtr->tkfont,
	    edgePtr->label,
	    strlen (edgePtr->label),
	    edgePtr->width,
	    edgePtr->justify,
	    0, &lineWidth, &lineHeight);
   lineWidth = strlen(edgePtr->label);
    if (lineWidth > (edgePtr->header.x2 - edgePtr->header.x2)) {
      edgePtr->header.x1 -=
	(lineWidth - (edgePtr->header.x2 - edgePtr->header.x2)) / 2;
      edgePtr->header.x2 +=
	(lineWidth - (edgePtr->header.x2 - edgePtr->header.x2)) / 2;
    }
    if (lineHeight >
	(edgePtr->header.y2 - edgePtr->header.y2)) {
      edgePtr->header.y1 -=
	(lineHeight - (edgePtr->header.y2 - edgePtr->header.y2))
	/ 2;
      edgePtr->header.y2 +=
	(lineHeight - (edgePtr->header.y2 - edgePtr->header.y2)) / 2;
    }
  }
}

/*
 *--------------------------------------------------------------
 *
 * DisplayEdge --
 *
 *	This procedure is invoked to draw a edge item in a given
 *	drawable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	ItemPtr is drawn in drawable using the transformation
 *	information in canvas.
 *
 *--------------------------------------------------------------
 */

static void
DisplayEdge(canvas, itemPtr, display, drawable, x, y, width, height)
     Tk_Canvas canvas;  	        /* Canvas that contains item. */
     Tk_Item *itemPtr;			/* Item to be displayed. */
     Display *display;			/* Display on which to draw item. */
     Drawable drawable;			/* Pixmap or window in which to draw
					 * item. */
    int x, y, width, height;		/* Describes region of canvas that
					 * must be redisplayed (not used). */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  XPoint staticPoints[MAX_STATIC_POINTS];
  XPoint *pointPtr;
  XPoint *pPtr;
  register double *coordPtr;
  int i, numPoints, lineHeight;
  int centerX, centerY, lineWidth;
  short drawableX, drawableY;
  
  if (edgePtr->gc == None) {
    return;
  }
  
  /*
   * Build up an array of points in screen coordinates.  Use a
   * static array unless the edge has an enormous number of points;
   * in this case, dynamically allocate an array.  For smoothed edges,
   * generate the curve points on each redisplay.
   */
  
  if ((edgePtr->smooth) && (edgePtr->numPoints > 2)) {
    numPoints = 1 + edgePtr->numPoints*edgePtr->splineSteps;
  } else {
    numPoints = edgePtr->numPoints;
  }
  
  if (numPoints <= MAX_STATIC_POINTS) {
    pointPtr = staticPoints;
  } else {
    pointPtr = (XPoint *) ckalloc((unsigned) (numPoints * sizeof(XPoint)));
  }
  
  if (edgePtr->smooth) {
    numPoints = TkMakeBezierCurve(canvas, edgePtr->coordPtr,
				  edgePtr->numPoints,
				  edgePtr->splineSteps, pointPtr,
				  (double *) NULL);
  } else {
    for (i = 0, coordPtr = edgePtr->coordPtr, pPtr = pointPtr;
	 i < edgePtr->numPoints;  i += 1, coordPtr += 2, pPtr++) {
      Tk_CanvasDrawableCoords(canvas, coordPtr[0], coordPtr[1],
                              &pPtr->x, &pPtr->y);
    }
  }
  
  /*
   * Display edge, the free up edge storage if it was dynamically
   * allocated.  If we're stippling, then modify the stipple offset
   * in the GC.  Be sure to reset the offset when done, since the
   * GC is supposed to be read-only.
   */
  
  if (edgePtr->fillStipple != None) {
    XSetTSOrigin(display, edgePtr->gc,
		 -((TkCanvas *) canvas)->drawableXOrigin,
                 -((TkCanvas *) canvas)->drawableYOrigin);
  }
  XDrawLines(display, drawable, edgePtr->gc,
	     pointPtr, numPoints, CoordModeOrigin);
  if (pointPtr[numPoints-1].x > pointPtr[numPoints-2].x) {
    centerX = ((pointPtr[numPoints-1].x - pointPtr[numPoints-2].x) / 2) +
      pointPtr[numPoints-2].x;
  } else {
    centerX = ((pointPtr[numPoints-2].x - pointPtr[numPoints-1].x) / 2) +
      pointPtr[numPoints-1].x;
  }
  if (pointPtr[numPoints-1].y > pointPtr[numPoints-2].y) {
    centerY = ((pointPtr[numPoints-1].y - pointPtr[numPoints-2].y) / 2) +
      pointPtr[numPoints-2].y;
  } else {
    centerY = ((pointPtr[numPoints-2].y - pointPtr[numPoints-1].y) / 2) +
      pointPtr[numPoints-1].y;
  }
  if (pointPtr != staticPoints) {
    ckfree((char *) pointPtr);
  }
  
  /*
   * Display arrowheads, if they are wanted.
   */
  
  if (edgePtr->arrow != noneUid) {
    if (edgePtr->arrow != lastUid) {
      TkFillPolygon(canvas, edgePtr->firstArrowPtr, PTS_IN_ARROW,
		    display, drawable, edgePtr->gc, NULL);
    }
    if (edgePtr->arrow != firstUid) {
      TkFillPolygon(canvas, edgePtr->lastArrowPtr, PTS_IN_ARROW,
		    display, drawable, edgePtr->gc, NULL);
    }
  }
  if (edgePtr->fillStipple != None) {
    XSetTSOrigin(display, edgePtr->gc, 0, 0);
  }

  /* display the label */
  if (edgePtr->label != NULL && (size_t) strlen(edgePtr->label) > 0) {
    Tk_FreeTextLayout(edgePtr->textLayout);
    edgePtr->textLayout = Tk_ComputeTextLayout(edgePtr->tkfont,
	    edgePtr->label,
	    strlen (edgePtr->label),
	    edgePtr->width,
	    edgePtr->justify,
	    0, &lineWidth, &lineHeight);
    lineWidth = strlen(edgePtr->label);
    if (strcmp(edgePtr->state, "selected") == 0) {
      XFillRectangle(display, drawable,
		     edgePtr->gc,
		     centerX - (lineWidth / 2) - 1,
		     centerY - (lineHeight / 2) - 1,
		     lineWidth + 2, lineHeight + 2);
      Tk_CanvasDrawableCoords(canvas,
			(double) (edgePtr->header.x1 + x),
			(double) (edgePtr->header.y1 + y),
			&drawableX, &drawableY);
      Tk_DrawTextLayout(display, drawable,  edgePtr->gc,
	    edgePtr->textLayout,
	    drawableX, drawableY,
	    0, -1);
    } else {
      XFillRectangle(display, drawable,
		     edgePtr->invertedGc,
		     centerX - (lineWidth / 2) - 1,
		     centerY - (lineHeight / 2) - 1,
		     lineWidth + 2, lineHeight + 2);
      Tk_CanvasDrawableCoords(canvas,
			(double) (edgePtr->header.x1 + x),
			(double) (edgePtr->header.y1 + y),
			&drawableX, &drawableY);
      Tk_DrawTextLayout(display, drawable,  edgePtr->gc,
	    edgePtr->textLayout,
	    drawableX, drawableY,
	    0, -1);
    }
  }
}

/*
 *--------------------------------------------------------------
 *
 * EdgeToPoint --
 *
 *	Computes the distance from a given point to a given
 *	edge, in canvas units.
 *
 * Results:
 *	The return value is 0 if the point whose x and y coordinates
 *	are pointPtr[0] and pointPtr[1] is inside the edge.  If the
 *	point isn't inside the edge then the return value is the
 *	distance from the point to the edge.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

	/* ARGSUSED */
static double
EdgeToPoint(canvas, itemPtr, pointPtr)
     Tk_Canvas canvas;	        /* Canvas containing item. */
     Tk_Item *itemPtr;		/* Item to check against point. */
     double *pointPtr;		/* Pointer to x and y coordinates. */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  register double *coordPtr, *edgePoints;
  double staticSpace[2*MAX_STATIC_POINTS];
  double poly[10];
  double bestDist, dist;
  int numPoints, count;
  int changedMiterToBevel;	/* Non-zero means that a mitered corner
				 * had to be treated as beveled after all
				 * because the angle was < 11 degrees. */

  bestDist = 1.0e40;
  
  /*
   * Handle smoothed edges by generating an expanded set of points
   * against which to do the check.
   */
  
  if ((edgePtr->smooth) && (edgePtr->numPoints > 2)) {
    numPoints = 1 + edgePtr->numPoints*edgePtr->splineSteps;
    if (numPoints <= MAX_STATIC_POINTS) {
      edgePoints = staticSpace;
    } else {
      edgePoints = (double *) ckalloc((unsigned)
				      (2*numPoints*sizeof(double)));
    }
    numPoints = TkMakeBezierCurve(canvas, edgePtr->coordPtr,
				  edgePtr->numPoints,
				  edgePtr->splineSteps, (XPoint *) NULL,
				  edgePoints);
  } else {
    numPoints = edgePtr->numPoints;
    edgePoints = edgePtr->coordPtr;
  }
  
  /*
   * The overall idea is to iterate through all of the edges of
   * the edge, computing a polygon for each edge and testing the
   * point against that polygon.  In addition, there are additional
   * tests to deal with rounded joints and caps.
   */
  
  changedMiterToBevel = 0;
  for (count = numPoints, coordPtr = edgePoints; count >= 2;
       count--, coordPtr += 2) {
    
    /*
     * If rounding is done around the first point then compute
     * the distance between the point and the point.
     */
    
    if (((edgePtr->capStyle == CapRound) && (count == numPoints))
	|| ((edgePtr->joinStyle == JoinRound)
	    && (count != numPoints))) {
      dist = hypot(coordPtr[0] - pointPtr[0], coordPtr[1] - pointPtr[1])
	- edgePtr->width/2.0;
      if (dist <= 0.0) {
	bestDist = 0.0;
	goto done;
      } else if (dist < bestDist) {
	bestDist = dist;
      }
    }
    
    /*
     * Compute the polygonal shape corresponding to this edge,
     * consisting of two points for the first point of the edge
     * and two points for the last point of the edge.
     */
    
    if (count == numPoints) {
      TkGetButtPoints(coordPtr+2, coordPtr, (double) edgePtr->width,
		      edgePtr->capStyle == CapProjecting, poly, poly+2);
    } else if ((edgePtr->joinStyle == JoinMiter) && !changedMiterToBevel) {
      poly[0] = poly[6];
      poly[1] = poly[7];
      poly[2] = poly[4];
      poly[3] = poly[5];
    } else {
      TkGetButtPoints(coordPtr+2, coordPtr, (double) edgePtr->width, 0,
		      poly, poly+2);
      
      /*
       * If this edge uses beveled joints, then check the distance
       * to a polygon comprising the last two points of the previous
       * polygon and the first two from this polygon;  this checks
       * the wedges that fill the mitered joint.
       */
      
      if ((edgePtr->joinStyle == JoinBevel) || changedMiterToBevel) {
	poly[8] = poly[0];
	poly[9] = poly[1];
	dist = TkPolygonToPoint(poly, 5, pointPtr);
	if (dist <= 0.0) {
	  bestDist = 0.0;
	  goto done;
	} else if (dist < bestDist) {
	  bestDist = dist;
	}
	changedMiterToBevel = 0;
      }
    }
    if (count == 2) {
      TkGetButtPoints(coordPtr, coordPtr+2, (double) edgePtr->width,
		      edgePtr->capStyle == CapProjecting, poly+4, poly+6);
    } else if (edgePtr->joinStyle == JoinMiter) {
      if (TkGetMiterPoints(coordPtr, coordPtr+2, coordPtr+4,
			   (double) edgePtr->width, poly+4, poly+6) == 0) {
	changedMiterToBevel = 1;
	TkGetButtPoints(coordPtr, coordPtr+2, (double) edgePtr->width,
			0, poly+4, poly+6);
      }
    } else {
      TkGetButtPoints(coordPtr, coordPtr+2, (double) edgePtr->width, 0,
		      poly+4, poly+6);
    }
    poly[8] = poly[0];
    poly[9] = poly[1];
    dist = TkPolygonToPoint(poly, 5, pointPtr);
    if (dist <= 0.0) {
      bestDist = 0.0;
      goto done;
    } else if (dist < bestDist) {
      bestDist = dist;
    }
  }
  
  /*
   * If caps are rounded, check the distance to the cap around the
   * final end point of the edge.
   */
  
  if (edgePtr->capStyle == CapRound) {
    dist = hypot(coordPtr[0] - pointPtr[0], coordPtr[1] - pointPtr[1])
      - edgePtr->width/2.0;
    if (dist <= 0.0) {
      bestDist = 0.0;
      goto done;
    } else if (dist < bestDist) {
      bestDist = dist;
    }
  }

  /*
   * If there are arrowheads, check the distance to the arrowheads.
   */

  if (edgePtr->arrow != noneUid) {
    if (edgePtr->arrow != lastUid) {
      dist = TkPolygonToPoint(edgePtr->firstArrowPtr, PTS_IN_ARROW,
			      pointPtr);
      if (dist <= 0.0) {
	bestDist = 0.0;
	goto done;
      } else if (dist < bestDist) {
	bestDist = dist;
      }
    }
    if (edgePtr->arrow != firstUid) {
      dist = TkPolygonToPoint(edgePtr->lastArrowPtr, PTS_IN_ARROW,
			      pointPtr);
      if (dist <= 0.0) {
	bestDist = 0.0;
	goto done;
      } else if (dist < bestDist) {
	bestDist = dist;
      }
    }
  }
  
 done:
  if ((edgePoints != staticSpace) && (edgePoints != edgePtr->coordPtr)) {
    ckfree((char *) edgePoints);
  }
  return bestDist;
}

/*
 *--------------------------------------------------------------
 *
 * EdgeToArea --
 *
 *	This procedure is called to determine whether an item
 *	lies entirely inside, entirely outside, or overlapping
 *	a given rectangular area.
 *
 * Results:
 *	-1 is returned if the item is entirely outside the
 *	area, 0 if it overlaps, and 1 if it is entirely
 *	inside the given area.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

	/* ARGSUSED */
static int
EdgeToArea(canvas, itemPtr, rectPtr)
     Tk_Canvas canvas;	        /* Canvas containing item. */
     Tk_Item *itemPtr;		/* Item to check against edge. */
     double *rectPtr;
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  register double *coordPtr;
  double staticSpace[2*MAX_STATIC_POINTS];
  double *edgePoints, poly[10];
  double radius;
  int numPoints, count;
  int changedMiterToBevel;	/* Non-zero means that a mitered corner
				 * had to be treated as beveled after all
				 * because the angle was < 11 degrees. */
  int inside;			/* Tentative guess about what to return,
				 * based on all points seen so far:  one
				 * means everything seen so far was
				 * inside the area;  -1 means everything
				 * was outside the area.  0 means overlap
				 * has been found. */ 

  radius = edgePtr->width/2.0;
  inside = -1;
  
  /*
   * Handle smoothed edges by generating an expanded set of points
   * against which to do the check.
   */
  
  if ((edgePtr->smooth) && (edgePtr->numPoints > 2)) {
    numPoints = 1 + edgePtr->numPoints*edgePtr->splineSteps;
    if (numPoints <= MAX_STATIC_POINTS) {
      edgePoints = staticSpace;
    } else {
      edgePoints = (double *) ckalloc((unsigned)
				      (2*numPoints*sizeof(double)));
    }
    numPoints = TkMakeBezierCurve(canvas, edgePtr->coordPtr,
				  edgePtr->numPoints,
				  edgePtr->splineSteps, (XPoint *) NULL,
				  edgePoints);
  } else {
    numPoints = edgePtr->numPoints;
    edgePoints = edgePtr->coordPtr;
  }
  
  coordPtr = edgePoints;
  if ((coordPtr[0] >= rectPtr[0]) && (coordPtr[0] <= rectPtr[2])
      && (coordPtr[1] >= rectPtr[1]) && (coordPtr[1] <= rectPtr[3])) {
    inside = 1;
  }
  
  /*
   * Iterate through all of the edges of the edge, computing a polygon
   * for each edge and testing the area against that polygon.  In
   * addition, there are additional tests to deal with rounded joints
   * and caps.
   */
  
  changedMiterToBevel = 0;
  for (count = numPoints; count >= 2; count--, coordPtr += 2) {
    
    /*
     * If rounding is done around the first point of the edge
     * then test a circular region around the point with the
     * area.
     */
    
    if (((edgePtr->capStyle == CapRound) && (count == numPoints))
	|| ((edgePtr->joinStyle == JoinRound)
	    && (count != numPoints))) {
      poly[0] = coordPtr[0] - radius;
      poly[1] = coordPtr[1] - radius;
      poly[2] = coordPtr[0] + radius;
      poly[3] = coordPtr[1] + radius;
      if (TkOvalToArea(poly, rectPtr) != inside) {
	inside = 0;
	goto done;
      }
    }
    
    /*
     * Compute the polygonal shape corresponding to this edge,
     * consisting of two points for the first point of the edge
     * and two points for the last point of the edge.
     */
    
    if (count == numPoints) {
      TkGetButtPoints(coordPtr+2, coordPtr, (double) edgePtr->width,
		      edgePtr->capStyle == CapProjecting, poly, poly+2);
    } else if ((edgePtr->joinStyle == JoinMiter) && !changedMiterToBevel) {
      poly[0] = poly[6];
      poly[1] = poly[7];
      poly[2] = poly[4];
      poly[3] = poly[5];
    } else {
      TkGetButtPoints(coordPtr+2, coordPtr, (double) edgePtr->width, 0,
		      poly, poly+2);
      
      /*
       * If the last joint was beveled, then also check a
       * polygon comprising the last two points of the previous
       * polygon and the first two from this polygon;  this checks
       * the wedges that fill the beveled joint.
       */
      
      if ((edgePtr->joinStyle == JoinBevel) || changedMiterToBevel) {
	poly[8] = poly[0];
	poly[9] = poly[1];
	if (TkPolygonToArea(poly, 5, rectPtr) != inside) {
	  inside = 0;
	  goto done;
	}
	changedMiterToBevel = 0;
      }
    }
    if (count == 2) {
      TkGetButtPoints(coordPtr, coordPtr+2, (double) edgePtr->width,
		      edgePtr->capStyle == CapProjecting, poly+4, poly+6);
    } else if (edgePtr->joinStyle == JoinMiter) {
      if (TkGetMiterPoints(coordPtr, coordPtr+2, coordPtr+4,
			   (double) edgePtr->width, poly+4, poly+6) == 0) {
	changedMiterToBevel = 1;
	TkGetButtPoints(coordPtr, coordPtr+2, (double) edgePtr->width,
			0, poly+4, poly+6);
      }
    } else {
      TkGetButtPoints(coordPtr, coordPtr+2, (double) edgePtr->width, 0,
		      poly+4, poly+6);
    }
    poly[8] = poly[0];
    poly[9] = poly[1];
    if (TkPolygonToArea(poly, 5, rectPtr) != inside) {
      inside = 0;
      goto done;
    }
  }
  
  /*
   * If caps are rounded, check the cap around the final point
   * of the edge.
   */

  if (edgePtr->capStyle == CapRound) {
    poly[0] = coordPtr[0] - radius;
    poly[1] = coordPtr[1] - radius;
    poly[2] = coordPtr[0] + radius;
    poly[3] = coordPtr[1] + radius;
    if (TkOvalToArea(poly, rectPtr) != inside) {
      inside = 0;
      goto done;
    }
  }
  
  /*
   * Check arrowheads, if any.
   */
  
  if (edgePtr->arrow != noneUid) {
    if (edgePtr->arrow != lastUid) {
      if (TkPolygonToArea(edgePtr->firstArrowPtr, PTS_IN_ARROW,
			  rectPtr) != inside) {
	inside = 0;
	goto done;
      }
    }
    if (edgePtr->arrow != firstUid) {
      if (TkPolygonToArea(edgePtr->lastArrowPtr, PTS_IN_ARROW,
			  rectPtr) != inside) {
	inside = 0;
	goto done;
      }
    }
  }
  
 done:
  if ((edgePoints != staticSpace) && (edgePoints != edgePtr->coordPtr)) {
    ckfree((char *) edgePoints);
  }
  return inside;
}

/*
 *--------------------------------------------------------------
 *
 * ScaleEdge --
 *
 *	This procedure is invoked to rescale a edge item.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The edge referred to by itemPtr is rescaled so that the
 *	following transformation is applied to all point
 *	coordinates:
 *		x' = originX + scaleX*(x-originX)
 *		y' = originY + scaleY*(y-originY)
 *
 *--------------------------------------------------------------
 */

static void
ScaleEdge(canvas, itemPtr, originX, originY, scaleX, scaleY)
     Tk_Canvas canvas;	        	/* Canvas containing edge. */
     Tk_Item *itemPtr;			/* Edge to be scaled. */
     double originX, originY;		/* Origin about which to scale rect. */
     double scaleX;			/* Amount to scale in X direction. */
     double scaleY;			/* Amount to scale in Y direction. */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  double *coordPtr;
  int i;
  
  for (i = 0, coordPtr = edgePtr->coordPtr; i < edgePtr->numPoints;
       i++, coordPtr += 2) {
    coordPtr[0] = originX + scaleX*(*coordPtr - originX);
    coordPtr[1] = originY + scaleY*(coordPtr[1] - originY);
  }
  if (edgePtr->firstArrowPtr != NULL) {
    for (i = 0, coordPtr = edgePtr->firstArrowPtr; i < PTS_IN_ARROW;
	 i++, coordPtr += 2) {
      coordPtr[0] = originX + scaleX*(coordPtr[0] - originX);
      coordPtr[1] = originY + scaleY*(coordPtr[1] - originY);
    }
  }
  if (edgePtr->lastArrowPtr != NULL) {
    for (i = 0, coordPtr = edgePtr->lastArrowPtr; i < PTS_IN_ARROW;
	 i++, coordPtr += 2) {
      coordPtr[0] = originX + scaleX*(coordPtr[0] - originX);
      coordPtr[1] = originY + scaleY*(coordPtr[1] - originY);
    }
  }
  ComputeEdgeBbox(canvas, edgePtr);
}

/*
 *--------------------------------------------------------------
 *
 * TranslateEdge --
 *
 *	This procedure is called to move a edge by a given amount.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The position of the edge is offset by (xDelta, yDelta), and
 *	the bounding box is updated in the generic part of the item
 *	structure.
 *
 *--------------------------------------------------------------
 */

static void
TranslateEdge(canvas, itemPtr, deltaX, deltaY)
     Tk_Canvas canvas;	        	/* Canvas containing item. */
     Tk_Item *itemPtr;			/* Item that is being moved. */
     double deltaX, deltaY;		/* Amount by which item is to be
					 * moved. */
{
  EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  double *coordPtr;
  int i;
  
  for (i = 0, coordPtr = edgePtr->coordPtr; i < edgePtr->numPoints;
       i++, coordPtr += 2) {
    coordPtr[0] += deltaX;
    coordPtr[1] += deltaY;
  }
  if (edgePtr->firstArrowPtr != NULL) {
    for (i = 0, coordPtr = edgePtr->firstArrowPtr; i < PTS_IN_ARROW;
	 i++, coordPtr += 2) {
      coordPtr[0] += deltaX;
      coordPtr[1] += deltaY;
    }
  }
  if (edgePtr->lastArrowPtr != NULL) {
    for (i = 0, coordPtr = edgePtr->lastArrowPtr; i < PTS_IN_ARROW;
	 i++, coordPtr += 2) {
      coordPtr[0] += deltaX;
      coordPtr[1] += deltaY;
    }
  }
  ComputeEdgeBbox(canvas, edgePtr);
}

/*
 *--------------------------------------------------------------
 *
 * ParseArrowShape --
 *
 *	This procedure is called back during option parsing to
 *	parse arrow shape information.
 *
 * Results:
 *	The return value is a standard Tcl result:  TCL_OK means
 *	that the arrow shape information was parsed ok, and
 *	TCL_ERROR means it couldn't be parsed.
 *
 * Side effects:
 *	Arrow information in recordPtr is updated.
 *
 *--------------------------------------------------------------
 */

	/* ARGSUSED */
static int
ParseArrowShape(clientData, interp, tkwin, value, recordPtr, offset)
     ClientData clientData;	/* Not used. */
     Tcl_Interp *interp;	/* Used for error reporting. */
     Tk_Window tkwin;		/* Not used. */
     char *value;		/* Textual specification of arrow shape. */
     char *recordPtr;		/* Pointer to item record in which to
				 * store arrow information. */
     int offset;		/* Offset of shape information in widget
				 * record. */
{
  EdgeItem *edgePtr = (EdgeItem *) recordPtr;
  double a, b, c;
  int argc;
  char **argv = NULL;
  
  if (offset != Tk_Offset(EdgeItem, arrowShapeA)) {
    panic("ParseArrowShape received bogus offset");
  }
  
  if (Tcl_SplitList(interp, value, &argc, &argv) != TCL_OK) {
  syntaxError:
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, "bad arrow shape \"", value,
		     "\": must be list with three numbers", (char *) NULL);
    if (argv != NULL) {
      ckfree((char *) argv);
    }
    return TCL_ERROR;
  }
  if (argc != 3) {
    goto syntaxError;
  }
  if ((Tk_CanvasGetCoord(interp, edgePtr->canvas, argv[0], &a) != TCL_OK)
      || (Tk_CanvasGetCoord(interp, edgePtr->canvas, argv[1], &b) != TCL_OK)
      || (Tk_CanvasGetCoord(interp, edgePtr->canvas, argv[2], &c) != TCL_OK)) {
    goto syntaxError;
  }
  edgePtr->arrowShapeA = a;
  edgePtr->arrowShapeB = b;
  edgePtr->arrowShapeC = c;
  ckfree((char *) argv);
  return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * PrintArrowShape --
 *
 *	This procedure is a callback invoked by the configuration
 *	code to return a printable value describing an arrow shape.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

    /* ARGSUSED */
static char *
PrintArrowShape(clientData, tkwin, recordPtr, offset, freeProcPtr)
     ClientData clientData;	/* Not used. */
     Tk_Window tkwin;		/* Window associated with edgePtr's widget. */
     char *recordPtr;		/* Pointer to item record containing current
				 * shape information. */
     int offset;		/* Offset of arrow information in record. */
     Tcl_FreeProc **freeProcPtr;/* Store address of procedure to call to
				 * free string here. */
{
  EdgeItem *edgePtr = (EdgeItem *) recordPtr;
  char *buffer;
  
  buffer = ckalloc(120);
  sprintf(buffer, "%.5g %.5g %.5g", edgePtr->arrowShapeA,
	  edgePtr->arrowShapeB, edgePtr->arrowShapeC);
  *freeProcPtr = (Tcl_FreeProc *) free;
  return buffer;
}

/*
 *--------------------------------------------------------------
 *
 * ConfigureArrows --
 *
 *	If arrowheads have been requested for a edge, this
 *	procedure makes arrangements for the arrowheads.
 *
 * Results:
 *	A standard Tcl return value.  If an error occurs, then
 *	an error message is left in interp->result.
 *
 * Side effects:
 *	Information in edgePtr is set up for one or two arrowheads.
 *	the firstArrowPtr and lastArrowPtr polygons are allocated
 *	and initialized, if need be, and the end points of the edge
 *	are adjusted so that a thick edge doesn't stick out past
 *	the arrowheads.
 *
 *--------------------------------------------------------------
 */

	/* ARGSUSED */
static int
ConfigureArrows(canvas, edgePtr)
     Tk_Canvas canvas;		        /* Canvas in which arrows will be
					 * displayed (interp and tkwin
					 * fields are needed). */
     EdgeItem *edgePtr;	                /* Item to configure for arrows. */
{
  double *poly, *coordPtr;
  double dx, dy, length, sinTheta, cosTheta, temp, shapeC;
  double fracHeight;			/* Edge width as fraction of
					 * arrowhead width. */
  double backup;			/* Distance to backup end points
					 * so the edge ends in the middle
					 * of the arrowhead. */
  double vertX, vertY;		/* Position of arrowhead vertex. */
  
  /*
   * If there's an arrowhead on the first point of the edge, compute
   * its polygon and adjust the first point of the edge so that the
   * edge doesn't stick out past the leading edge of the arrowhead.
   */
  
  shapeC = edgePtr->arrowShapeC + edgePtr->width/2.0;
  fracHeight = (edgePtr->width/2.0)/shapeC;
  backup = fracHeight*edgePtr->arrowShapeB
    + edgePtr->arrowShapeA*(1.0 - fracHeight)/2.0;
  if (edgePtr->arrow != lastUid) {
    poly = edgePtr->firstArrowPtr;
    if (poly == NULL) {
      poly = (double *) ckalloc((unsigned)
				(2*PTS_IN_ARROW*sizeof(double)));
      poly[0] = poly[10] = edgePtr->coordPtr[0];
      poly[1] = poly[11] = edgePtr->coordPtr[1];
      edgePtr->firstArrowPtr = poly;
    }
    dx = poly[0] - edgePtr->coordPtr[2];
    dy = poly[1] - edgePtr->coordPtr[3];
    length = hypot(dx, dy);
    if (length == 0) {
      sinTheta = cosTheta = 0.0;
    } else {
      sinTheta = dy/length;
      cosTheta = dx/length;
    }
    vertX = poly[0] - edgePtr->arrowShapeA*cosTheta;
    vertY = poly[1] - edgePtr->arrowShapeA*sinTheta;
    temp = shapeC*sinTheta;
    poly[2] = poly[0] - edgePtr->arrowShapeB*cosTheta + temp;
    poly[8] = poly[2] - 2*temp;
    temp = shapeC*cosTheta;
    poly[3] = poly[1] - edgePtr->arrowShapeB*sinTheta - temp;
    poly[9] = poly[3] + 2*temp;
    poly[4] = poly[2]*fracHeight + vertX*(1.0-fracHeight);
    poly[5] = poly[3]*fracHeight + vertY*(1.0-fracHeight);
    poly[6] = poly[8]*fracHeight + vertX*(1.0-fracHeight);
    poly[7] = poly[9]*fracHeight + vertY*(1.0-fracHeight);
    
    /*
     * Polygon done.  Now move the first point towards the second so
     * that the corners at the end of the edge are inside the
     * arrowhead.
     */
    
    edgePtr->coordPtr[0] = poly[0] - backup*cosTheta;
    edgePtr->coordPtr[1] = poly[1] - backup*sinTheta;
  }
  
  /*
   * Similar arrowhead calculation for the last point of the edge.
   */
  
  if (edgePtr->arrow != firstUid) {
    coordPtr = edgePtr->coordPtr + 2*(edgePtr->numPoints-2);
    poly = edgePtr->lastArrowPtr;
    if (poly == NULL) {
      poly = (double *) ckalloc((unsigned)
				(2*PTS_IN_ARROW*sizeof(double)));
      poly[0] = poly[10] = coordPtr[2];
      poly[1] = poly[11] = coordPtr[3];
      edgePtr->lastArrowPtr = poly;
    }
    dx = poly[0] - coordPtr[0];
    dy = poly[1] - coordPtr[1];
    length = hypot(dx, dy);
    if (length == 0) {
      sinTheta = cosTheta = 0.0;
    } else {
      sinTheta = dy/length;
      cosTheta = dx/length;
    }
    vertX = poly[0] - edgePtr->arrowShapeA*cosTheta;
    vertY = poly[1] - edgePtr->arrowShapeA*sinTheta;
    temp = shapeC*sinTheta;
    poly[2] = poly[0] - edgePtr->arrowShapeB*cosTheta + temp;
    poly[8] = poly[2] - 2*temp;
    temp = shapeC*cosTheta;
    poly[3] = poly[1] - edgePtr->arrowShapeB*sinTheta - temp;
    poly[9] = poly[3] + 2*temp;
    poly[4] = poly[2]*fracHeight + vertX*(1.0-fracHeight);
    poly[5] = poly[3]*fracHeight + vertY*(1.0-fracHeight);
    poly[6] = poly[8]*fracHeight + vertX*(1.0-fracHeight);
    poly[7] = poly[9]*fracHeight + vertY*(1.0-fracHeight);
    coordPtr[2] = poly[0] - backup*cosTheta;
    coordPtr[3] = poly[1] - backup*sinTheta;
  }
  
  return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * EdgeToPostscript --
 *
 *	This procedure is called to generate Postscript for
 *	edge items.
 *
 * Results:
 *	The return value is a standard Tcl result.  If an error
 *	occurs in generating Postscript then an error message is
 *	left in interp->result, replacing whatever used
 *	to be there.  If no error occurs, then Postscript for the
 *	item is appended to the result.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
EdgeToPostscript(interp, canvas, itemPtr, prepass)
     Tcl_Interp *interp;		/* Leave Postscript or error message
					 * here. */
     Tk_Canvas canvas;		        /* Information about overall canvas. */
     Tk_Item *itemPtr;			/* Item for which Postscript is
					 * wanted. */
    int prepass;			/* 1 means this is a prepass to
					 * collect font information;  0 means
					 * final Postscript is being created. */
{
  register EdgeItem *edgePtr = (EdgeItem *) itemPtr;
  char buffer[200];
  char *style;
  
  if (edgePtr->fgColor == NULL) {
    return TCL_OK;
  }
  
  /*
   * Generate a path for the edge's center-edge (do this differently
   * for straight edges and smoothed edges).
   */
  
  if (!edgePtr->smooth) {
    Tk_CanvasPsPath(interp, canvas, edgePtr->coordPtr, edgePtr->numPoints);
  } else {
    if (edgePtr->fillStipple == None) {
      TkMakeBezierPostscript(interp, canvas, edgePtr->coordPtr, edgePtr->numPoints);
    } else {
      /*
       * Special hack: Postscript printers don't appear to be able
       * to turn a path drawn with "curveto"s into a clipping path
       * without exceeding resource limits, so TkMakeBezierPostscript
       * won't work for stippled curves.  Instead, generate all of
       * the intermediate points here and output them into the
       * Postscript file with "edgeto"s instead.
       */
      
      double staticPoints[2*MAX_STATIC_POINTS];
      double *pointPtr;
      int numPoints;
      
      numPoints = 1 + edgePtr->numPoints*edgePtr->splineSteps;
      pointPtr = staticPoints;
      if (numPoints > MAX_STATIC_POINTS) {
	pointPtr = (double *) ckalloc((unsigned)
				      (numPoints * 2 * sizeof(double)));
      }
      numPoints = TkMakeBezierCurve(canvas, edgePtr->coordPtr,
				    edgePtr->numPoints,
				    edgePtr->splineSteps, (XPoint *) NULL,
				    pointPtr);
      Tk_CanvasPsPath(interp, canvas, pointPtr, numPoints);
      if (pointPtr != staticPoints) {
	ckfree((char *) pointPtr);
      }
    }
  }

  /*
   * Set other edge-drawing parameters and stroke out the edge.
   */
  
  sprintf(buffer, "%d setlinewidth\n", edgePtr->width);
  Tcl_AppendResult(interp, buffer, (char *) NULL);
  style = "0 setlinecap\n";
  if (edgePtr->capStyle == CapRound) {
    style = "1 setlinecap\n";
  } else if (edgePtr->capStyle == CapProjecting) {
    style = "2 setlinecap\n";
  }
  Tcl_AppendResult(interp, style, (char *) NULL);
  style = "0 setlinejoin\n";
  if (edgePtr->joinStyle == JoinRound) {
    style = "1 setlinejoin\n";
  } else if (edgePtr->joinStyle == JoinBevel) {
    style = "2 setlinejoin\n";
  }
  Tcl_AppendResult(interp, style, (char *) NULL);
  if (Tk_CanvasPsColor(interp, canvas, edgePtr->fgColor) != TCL_OK) {
    return TCL_ERROR;
  };
  if (edgePtr->fillStipple != None) {
    if (Tk_CanvasPsStipple(interp, canvas, edgePtr->fillStipple)
	!= TCL_OK) {
      return TCL_ERROR;
    }
  } else {
    Tcl_AppendResult(interp, "stroke\n", (char *) NULL);
  }
  
  /*
   * Output polygons for the arrowheads, if there are any.
   */
  
  if (edgePtr->firstArrowPtr != NULL) {
    if (ArrowheadPostscript(interp, canvas, edgePtr, edgePtr->firstArrowPtr) !=
        TCL_OK) {
      return TCL_ERROR;
    }
  }
  if (edgePtr->lastArrowPtr != NULL) {
    if (ArrowheadPostscript(interp, canvas, edgePtr, edgePtr->lastArrowPtr) !=
        TCL_OK) {
      return TCL_ERROR;
    }
  }
  return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * ArrowheadPostscript --
 *
 *	This procedure is called to generate Postscript for
 *	an arrowhead for a edge item.
 *
 * Results:
 *	The return value is a standard Tcl result.  If an error
 *	occurs in generating Postscript then an error message is
 *	left in interp->result, replacing whatever used
 *	to be there.  If no error occurs, then Postscript for the
 *	arrowhead is appended to the result.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
ArrowheadPostscript(interp, canvas, edgePtr, arrowPtr)
     Tcl_Interp *interp;		/* Leave Postscript or error message
					 * here. */
     Tk_Canvas canvas;		        /* Information about overall canvas. */
     EdgeItem *edgePtr;			/* Edge item for which Postscript is
					 * being generated. */
     double *arrowPtr;			/* Pointer to first of five points
					 * describing arrowhead polygon. */
{
  Tk_CanvasPsPath(interp, canvas, arrowPtr, PTS_IN_ARROW);
  if (edgePtr->fillStipple != None) {
    if (Tk_CanvasPsStipple(interp, canvas, edgePtr->fillStipple)
	!= TCL_OK) {
      return TCL_ERROR;
    }
  } else {
    Tcl_AppendResult(interp, "fill\n", (char *) NULL);
  }
  return TCL_OK;
}
