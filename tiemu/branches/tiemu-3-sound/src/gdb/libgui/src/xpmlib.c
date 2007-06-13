/**
 * tixImgXpm.c --
 *
 *	This file implements images of type "pixmap" for Tix.
 * ______________________________________________________________________
 *
 * Copyright statement for tixImgXpm.c
 *	Copyright 1996, Expert Interface Technologies
 *
 * The following terms apply only to this file and no other parts of the
 * Tix library.
 *
 *	Permission is hereby granted, without written agreement and
 *	without license or royalty fees, to use, copy, modify, and
 *	distribute this file, for any purpose, provided that existing
 *	copyright notices are retained in all copies and that this
 *	notice is included verbatim in any distributions.
 *
 *	DISCLAIMER OF ALL WARRANTIES
 *
 *	IN NO EVENT SHALL THE AUTHOR OF THIS SOFTWARE BE LIABLE TO ANY
 *	PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 *	CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 *	AND ITS DOCUMENTATION, EVEN IF THE AUTHOR OF THIS SOFTWARE HAS
 *	BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	THE AUTHOR OF THIS SOFTWARE SPECIFICALLY DISCLAIMS ANY
 *	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *	PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 *	BASIS, AND THE AUTHOR OF THIS SOFTWARE HAS NO OBLIGATION TO
 *	PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 *	MODIFICATIONS.
 * ___________________________________________________________________
 *
 * This file is adapted from the Tk 4.0 source file tkImgBmap.c
 * Original tkImgBmap.c copyright information:
 *
 *	Copyright (c) 1994 The Regents of the University of California.
 *	Copyright (c) 1994-1995 Sun Microsystems, Inc.
 *
 *	See the file "license.terms.tcltk" for information on usage
 *	and redistribution of the original tkImgBmap.c file, and for a
 *	DISCLAIMER OF ALL WARRANTIES from the authors of tkImgBmap.c.
 */
#include "tkInt.h"
#include "tkPort.h"

#include "guitcl.h"

/* constants used only in this file */

#define XPM_MONO		1
#define XPM_GRAY_4		2
#define XPM_GRAY		3
#define XPM_COLOR		4
#define XPM_SYMBOLIC		5
#define XPM_UNKNOWN		6

/*
 * The following data structure represents the master for a pixmap
 * image:
 */

typedef struct PixmapMaster {
    Tk_ImageMaster tkMaster;	/* Tk's token for image master.  NULL means
				 * the image is being deleted. */
    Tcl_Interp *interp;		/* Interpreter for application that is
				 * using image. */
    Tcl_Command imageCmd;	/* Token for image command (used to delete
				 * it when the image goes away).  NULL means
				 * the image command has already been
				 * deleted. */
    char *fileString;		/* Value of -file option (malloc'ed).
				 * valid only if the -file option is specified
				 */
    char *dataString;		/* Value of -data option (malloc'ed).
				 * valid only if the -data option is specified
				 */
				/* First in list of all instances associated
				 * with this master. */
    Tk_Uid id;			/* ID's for XPM data already compiled
				 * into the tixwish binary */
    int size[2];		/* width and height */
    int ncolors;		/* number of colors */
    int cpp;			/* characters per pixel */
    char ** data;		/* The data that defines this pixmap 
				 * image (array of strings). It is
				 * converted into an X Pixmap when this
				 * image is instanciated
				 */
    int isDataAlloced;		/* False iff the data is got from
				 * the -id switch */
    struct PixmapInstance *instancePtr;
} PixmapMaster;

/* Make this more portable */

typedef struct ColorStruct {
    char c;			/* This is used if CPP is one */
    char * cstring;		/* This is used if CPP is bigger than one */
    XColor * colorPtr;
} ColorStruct;

/*
 * The following data structure represents all of the instances of an
 * image that lie within a particular window:
 *
 * %% ToDo
 * Currently one instance is created for each window that uses this pixmap.
 * This is usually OK because pixmaps are usually not shared or only shared by
 * a small number of windows. To improve resource allocation, we can
 * create an instance for each (Display x Visual x Depth) combo. This will
 * usually reduce the number of instances to one.
 */
typedef struct PixmapInstance {
    int refCount;		/* Number of instances that share this
				 * data structure. */
    PixmapMaster *masterPtr;	/* Pointer to master for image. */
    Tk_Window tkwin;		/* Window in which the instances will be
				 * displayed. */
    Pixmap pixmap;		/* The pixmap to display. */
    Pixmap mask;		/* Mask: only display pixmap pixels where
				 * there are 1's here. */
    GC gc;			/* Graphics context for displaying pixmap.
				 * None means there was an error while
				 * setting up the instance, so it cannot
				 * be displayed. */
    struct PixmapInstance *nextPtr;
				/* Next in list of all instance structures
				 * associated with masterPtr (NULL means
				 * end of list).
				 */
    ColorStruct * colors;
} PixmapInstance;

/*
 * The type record for pixmap images:
 */

static int		ImgXpmCreate _ANSI_ARGS_((Tcl_Interp *interp,
			    char *name, int argc, Tcl_Obj *CONST objv[],
			    Tk_ImageType *typePtr, Tk_ImageMaster master,
			    ClientData *clientDataPtr));
static ClientData	ImgXpmGet _ANSI_ARGS_((Tk_Window tkwin,
			    ClientData clientData));
static void		ImgXpmDisplay _ANSI_ARGS_((ClientData clientData,
			    Display *display, Drawable drawable, 
			    int imageX, int imageY, int width, int height,
			    int drawableX, int drawableY));
static void		ImgXpmFree _ANSI_ARGS_((ClientData clientData,
			    Display *display));
static void		ImgXpmDelete _ANSI_ARGS_((ClientData clientData));

static Tk_ImageType tixPixmapImageType = {
    "pixmap",			/* name */
    ImgXpmCreate,		/* createProc */
    ImgXpmGet,			/* getProc */
    ImgXpmDisplay,		/* displayProc */
    ImgXpmFree,			/* freeProc */
    ImgXpmDelete,		/* deleteProc */
    (Tk_ImageType *) NULL	/* nextPtr */
};

/*
 * Information used for parsing configuration specs:
 */

static Tk_ConfigSpec configSpecs[] = {
    {TK_CONFIG_STRING, "-data", (char *) NULL, (char *) NULL,
	(char *) NULL, Tk_Offset(PixmapMaster, dataString), TK_CONFIG_NULL_OK},
    {TK_CONFIG_STRING, "-file", (char *) NULL, (char *) NULL,
	(char *) NULL, Tk_Offset(PixmapMaster, fileString), TK_CONFIG_NULL_OK},
    {TK_CONFIG_UID, "-id", (char *) NULL, (char *) NULL,
	(char *) NULL, Tk_Offset(PixmapMaster, id), TK_CONFIG_NULL_OK},
    {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL,
	(char *) NULL, 0, 0}
};

/*
 * Prototypes for procedures used only locally in this file:
 */
static int		ImgXpmCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static void		ImgXpmCmdDeletedProc _ANSI_ARGS_((
			    ClientData clientData));
static void		ImgXpmConfigureInstance _ANSI_ARGS_((
			    PixmapInstance *instancePtr));
static int		ImgXpmConfigureMaster _ANSI_ARGS_((
			    PixmapMaster *masterPtr, int argc, char **argv,
			    int flags));
static int		ImgXpmGetData _ANSI_ARGS_((Tcl_Interp *interp,
			    PixmapMaster *masterPtr));
static char ** 		ImgXpmGetDataFromFile _ANSI_ARGS_((Tcl_Interp * interp,
			    char * string, int * numLines_return));
static char ** 		ImgXpmGetDataFromId _ANSI_ARGS_((Tcl_Interp * interp,
			    char * id));
static char ** 		ImgXpmGetDataFromString _ANSI_ARGS_((Tcl_Interp*interp,
			    char * string, int * numLines_return));
static void 		ImgXpmGetPixmapFromData _ANSI_ARGS_((
			    Tcl_Interp * interp,
			    PixmapMaster *masterPtr,
			    PixmapInstance *instancePtr));

/* Local data, used only in this file */
static Tcl_HashTable xpmTable;
static int xpmTableInited = 0;


/*
 *----------------------------------------------------------------------
 *
 * ImgXpmCreate --
 *
 *	This procedure is called by the Tk image code to create "pixmap"
 *	images.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	The data structure for a new image is allocated.
 *
 *----------------------------------------------------------------------
 */
static int
ImgXpmCreate(interp, name, argc, objv, typePtr, master, clientDataPtr)
    Tcl_Interp *interp;		/* Interpreter for application containing
				 * image. */
    char *name;			/* Name to use for image. */
    int argc;			/* Number of arguments. */
    Tcl_Obj *CONST objv[];	/* Argument strings for options (doesn't
				 * include image name or type). */
    Tk_ImageType *typePtr;	/* Pointer to our type record (not used). */
    Tk_ImageMaster master;	/* Token for image, to be used by us in
				 * later callbacks. */
    ClientData *clientDataPtr;	/* Store manager's token for image here;
				 * it will be returned in later callbacks. */
{
    PixmapMaster *masterPtr;
    char **argv;
    int i;

    masterPtr = (PixmapMaster *) ckalloc(sizeof(PixmapMaster));
    masterPtr->tkMaster = master;
    masterPtr->interp = interp;
    masterPtr->imageCmd = Tcl_CreateCommand(interp, name, ImgXpmCmd,
	    (ClientData) masterPtr, ImgXpmCmdDeletedProc);

    masterPtr->fileString = NULL;
    masterPtr->dataString = NULL;
    masterPtr->id = NULL;
    masterPtr->data = NULL;
    masterPtr->isDataAlloced = 0;
    masterPtr->instancePtr = NULL;

    argv = (char **) ckalloc (argc * sizeof (char *));
    for (i = 0; i < argc; i++) {
	argv[i] = Tcl_GetStringFromObj(objv[i], (int *) NULL);
    }

    if (ImgXpmConfigureMaster(masterPtr, argc, argv, 0) != TCL_OK) {
	ImgXpmDelete((ClientData) masterPtr);
	ckfree ((char *) argv);
	return TCL_ERROR;
    }
    ckfree ((char *) argv);
    *clientDataPtr = (ClientData) masterPtr;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmConfigureMaster --
 *
 *	This procedure is called when a pixmap image is created or
 *	reconfigured.  It process configuration options and resets
 *	any instances of the image.
 *
 * Results:
 *	A standard Tcl return value.  If TCL_ERROR is returned then
 *	an error message is left in masterPtr->interp->result.
 *
 * Side effects:
 *	Existing instances of the image will be redisplayed to match
 *	the new configuration options.
 *
 *	If any error occurs, the state of *masterPtr is restored to
 *	previous state.
 *
 *----------------------------------------------------------------------
 */
static int
ImgXpmConfigureMaster(masterPtr, argc, argv, flags)
    PixmapMaster *masterPtr;	/* Pointer to data structure describing
				 * overall pixmap image to (reconfigure). */
    int argc;			/* Number of entries in argv. */
    char **argv;		/* Pairs of configuration options for image. */
    int flags;			/* Flags to pass to Tk_ConfigureWidget,
				 * such as TK_CONFIG_ARGV_ONLY. */
{
    PixmapInstance *instancePtr;
    char * oldData, * oldFile;
    Tk_Uid oldId;

    oldData = masterPtr->dataString;
    oldFile = masterPtr->fileString;
    oldId   = masterPtr->id;

    if (Tk_ConfigureWidget(masterPtr->interp, Tk_MainWindow(masterPtr->interp),
	    configSpecs, argc, argv, (char *) masterPtr, flags)
	    != TCL_OK) {
	return TCL_ERROR;
    }

    if (masterPtr->id != NULL ||
	masterPtr->dataString != NULL ||
	masterPtr->fileString != NULL) {
	if (ImgXpmGetData(masterPtr->interp, masterPtr) != TCL_OK) {
	    goto error;
	}
    } else {
	Tcl_AppendResult(masterPtr->interp,
	    "must specify one of -data, -file or -id", NULL);
	goto error;
    }

    /*
     * Cycle through all of the instances of this image, regenerating
     * the information for each instance.  Then force the image to be
     * redisplayed everywhere that it is used.
     */
    for (instancePtr = masterPtr->instancePtr; instancePtr != NULL;
	instancePtr = instancePtr->nextPtr) {
	ImgXpmConfigureInstance(instancePtr);
    }

    if (masterPtr->data) {
	Tk_ImageChanged(masterPtr->tkMaster, 0, 0,
	    masterPtr->size[0], masterPtr->size[1],
	    masterPtr->size[0], masterPtr->size[1]);
    } else {
	Tk_ImageChanged(masterPtr->tkMaster, 0, 0, 0, 0, 0, 0);
    }

  done:
    return TCL_OK;

  error:
    /* Restore it to the original (possible valid) mode */
    masterPtr->dataString = oldData;
    masterPtr->fileString = oldFile;
    masterPtr->id = oldId;
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmGetData --
 *
 *	Given a file name or ASCII string, this procedure parses the
 *	file or string contents to produce binary data for a pixmap.
 *
 * Results:
 *	If the pixmap description was parsed successfully then the data
 *	is read into an array of strings. This array will later be used
 *	to create X Pixmaps for each instance.
 *
 * Side effects:
 *	The masterPtr->data array is allocated when successful. Contents of
 *	*masterPtr is changed only when successful.
 *----------------------------------------------------------------------
 */
static int
ImgXpmGetData(interp, masterPtr)
    Tcl_Interp *interp;			/* For reporting errors. */
    PixmapMaster *masterPtr;
{
    char ** data = NULL;
    int  isAllocated = 0;		/* do we need to free "data"? */
    int listArgc;
    char ** listArgv = NULL;
    int numLines;
    int size[2];
    int cpp;
    int ncolors;

    if (masterPtr->id != NULL) {
	data = ImgXpmGetDataFromId(interp, masterPtr->id);
	isAllocated = 0;
    }
    else if (masterPtr->fileString != NULL) {
	data = ImgXpmGetDataFromFile(interp, masterPtr->fileString, &numLines);
	isAllocated = 1;
    }
    else if (masterPtr->dataString != NULL) {
	data = ImgXpmGetDataFromString(interp,masterPtr->dataString,&numLines);
	isAllocated = 1;
    }
    else {
	/* Should have been enforced by ImgXpmConfigureMaster() */
	panic("ImgXpmGetData(): -data, -file and -id are all NULL");
    }

    if (data == NULL) {
	return TCL_ERROR;
    }

    /* Parse the first line of the data and get info about this pixmap */
    if (Tcl_SplitList(interp, data[0], &listArgc, &listArgv) != TCL_OK) {
	goto error;
    }

    if (listArgc < 4) {
	Tcl_AppendResult(interp, "File format error", NULL);
	goto error;
    }

    if (Tcl_GetInt(interp, listArgv[0], &size[0]) != TCL_OK) {
	goto error;
    }
    if (Tcl_GetInt(interp, listArgv[1], &size[1]) != TCL_OK) {
	goto error;
    }
    if (Tcl_GetInt(interp, listArgv[2], &ncolors) != TCL_OK) {
	goto error;
    }
    if (Tcl_GetInt(interp, listArgv[3], &cpp) != TCL_OK) {
	goto error;
    }

    if (isAllocated) {
	if (numLines != size[1] + ncolors + 1) {
	    /* the number of lines read from the file/data
	     * is not the same as specified in the data
	     */
	    goto error;
	}
    }

  done:
    if (masterPtr->isDataAlloced && masterPtr->data) {
	ckfree((char*)masterPtr->data);
    }
    masterPtr->isDataAlloced = isAllocated;
    masterPtr->data = data;
    masterPtr->size[0] = size[0];
    masterPtr->size[1] = size[1];
    masterPtr->cpp = cpp;
    masterPtr->ncolors = ncolors;
    
#if 1 /* Zsolt Koppany 17-sep-96 */
    if (listArgv) {
	ckfree((char*)listArgv);
    }
#endif /* 1 */
    return TCL_OK;

  error:
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, "File format error", NULL);

    if (isAllocated && data) {
	ckfree((char*)data);
    }
    if (listArgv) {
	ckfree((char*)listArgv);
    }
		   
    return TCL_ERROR;
}

static char ** ImgXpmGetDataFromId(interp, id)
    Tcl_Interp * interp;
    char * id;
{
    Tcl_HashEntry * hashPtr;

    if (xpmTableInited == 0) {
	hashPtr = NULL;
    } else {
	hashPtr = Tcl_FindHashEntry(&xpmTable, id);
    }

    if (hashPtr == NULL) {
	Tcl_AppendResult(interp, "unknown pixmap ID \"", id,
	    "\"", NULL);
	return (char**)NULL;
    } else {
	return (char**)Tcl_GetHashValue(hashPtr);
    }
}

static char ** ImgXpmGetDataFromString(interp, string, numLines_return)
    Tcl_Interp * interp;
    char * string;
    int * numLines_return;
{
    int quoted;
    char * p, * list;
    int numLines;
    char ** data = NULL;

    /* skip the leading blanks (leading blanks are not defined in the
     * the XPM definition, but skipping them shouldn't hurt. Also, the ability
     * to skip the leading blanks is good for using in-line XPM data in TCL
     * scripts
     */
    while (isspace(*string)) {
	++ string;
    }

    /* parse the header */
    if (strncmp("/* XPM", string, 6) != 0) {
	goto error;
    }

    /* strip the comments */
    for (quoted = 0, p=string; *p;) {
	if (!quoted) {
	    if (*p == '"') {
		quoted = 1;
		++ p;
		continue;
	    }

	    if (*p == '/' && *(p+1) == '*') {
		*p++ = ' ';
		*p++ = ' ';
		while (1) {
		    if (*p == 0) {
			break;
		    }
		    if (*p == '*' && *(p+1) == '/') {
			*p++ = ' ';
			*p++ = ' ';
			break;
		    }
		    *p++ = ' ';
		}
		continue;
	    }
	    ++ p;
	} else {
	    if (*p == '"') {
		quoted = 0;
	    }
	    ++ p;
	}
    }

    /* Search for the opening brace */
    for (p=string; *p;) {
	if (*p != '{') {
	    ++ p;
	} else {
	    ++p;
	    break;
	}
    }

    /* Change the buffer in to a proper TCL list */
    quoted = 0;
    list = p;

    while (*p) {
	if (!quoted) {
	    if (*p == '"') {
		quoted = 1;
		++ p;
		continue;
	    }

	    if (isspace(*p)) {
		*p = ' ';
	    }
	    else if (*p == ',') {
		*p = ' ';
	    }
	    else if (*p == '}') {
		*p = 0;
		break;
	    }
	    ++p;
	}
	else {
	    if (*p == '"') {
		quoted = 0;
	    }
	    ++ p;
	}
    }

    /* The following code depends on the fact that Tcl_SplitList
     * strips away double quoates inside a list: ie:
     * if string == "\"1\" \"2\"" then
     *		list[0] = "1"
     *		list[1] = "2"
     * and NOT
     *
     *		list[0] = "\"1\""
     *		list[1] = "\"2\""
     */
    if (Tcl_SplitList(interp, list, &numLines, &data) != TCL_OK) {
	goto error;
    } else {
	if (numLines == 0) {
	    /* error: empty data? */
	    if (data != NULL) {
		ckfree((char*)data);
		goto error;
	    }
	}
	* numLines_return = numLines;
	return data;
    }

  error:
    Tcl_AppendResult(interp, "File format error", NULL);
    return (char**) NULL;
}

static char ** ImgXpmGetDataFromFile(interp, fileName, numLines_return)
    Tcl_Interp * interp;
    char * fileName;
    int * numLines_return;
{
    int fileId, size;
    char ** data;
    struct stat statBuf;
    char *cmdBuffer = NULL;
    Tcl_DString buffer;			/* initialized by Tcl_TildeSubst */

#if 1
    fileId = -1;        /* Zsolt Koppany 23-mar-96 */
#endif

    fileName = Tcl_TildeSubst(interp, fileName, &buffer);
    if (fileName == NULL) {
	goto error;
    }

    fileId = open(fileName, O_RDONLY, 0);
    if (fileId < 0) {
	Tcl_AppendResult(interp, "couldn't read file \"", fileName,
		"\": ", Tcl_PosixError(interp), (char *) NULL);
	goto error;
    }
    if (fstat(fileId, &statBuf) == -1) {
	Tcl_AppendResult(interp, "couldn't stat file \"", fileName,
		"\": ", Tcl_PosixError(interp), (char *) NULL);
	close(fileId);
	goto error;
    }
    cmdBuffer = (char *) ckalloc((unsigned) statBuf.st_size+1);
    size = read(fileId, cmdBuffer, (size_t) statBuf.st_size);
    if (size < 0) {
	Tcl_AppendResult(interp, "error in reading file \"", fileName,
		"\": ", Tcl_PosixError(interp), (char *) NULL);
	close(fileId);
	goto error;
    }
    if (close(fileId) != 0) {
	Tcl_AppendResult(interp, "error closing file \"", fileName,
		"\": ", Tcl_PosixError(interp), (char *) NULL);
	goto error;
    }
    cmdBuffer[size] = 0;

  done:
    data = ImgXpmGetDataFromString(interp, cmdBuffer, numLines_return);
#if 1   /* Zsolt Koppany 23-mar-96 */
    if (fileId != -1)
        close(fileId);
#endif

    ckfree(cmdBuffer);
    Tcl_DStringFree(&buffer);
    return data;

  error:
#if 1   /* Zsolt Koppany 23-mar-96 */
    if (fileId != -1)
        close(fileId);
#endif
    if (cmdBuffer != NULL) {
	ckfree(cmdBuffer);
    }
    Tcl_DStringFree(&buffer);
    return (char**)NULL;
}


static char * GetType(colorDefn, type_ret)
    char * colorDefn;
    int  * type_ret;
{
    char * p = colorDefn;

    /* skip white spaces */
    while (*p && isspace(*p)) {
	p ++;
    }

    /* parse the type */
    if (p[0] != '\0' && p[0] == 'm' &&
	p[1] != '\0' && isspace(p[1])) {
	*type_ret = XPM_MONO;
	p += 2;
    }
    else if (p[0] != '\0' && p[0] == 'g' &&
	     p[1] != '\0' && p[1] == '4' &&
	     p[2] != '\0' && isspace(p[2])) {
	*type_ret = XPM_GRAY_4;
	p += 3;
    }
    else if (p[0] != '\0' && p[0] == 'g' &&
	     p[1] != '\0' && isspace(p[1])) {
	*type_ret = XPM_GRAY;
	p += 2;
    }
    else if (p[0] != '\0' && p[0] == 'c' &&
	     p[1] != '\0' && isspace(p[1])) {
	*type_ret = XPM_COLOR;
	p += 2;
    }
    else if (p[0] != '\0' && p[0] == 's' &&
	     p[1] != '\0' && isspace(p[1])) {
	*type_ret = XPM_SYMBOLIC;
	p += 2;
    }
    else {
	*type_ret = XPM_UNKNOWN;
	return NULL;
    }

    return p;
}

/* colorName is guaranteed to be big enough */
static char * GetColor(colorDefn, colorName, type_ret)
    char * colorDefn;
    char * colorName;		/* if found, name is copied to this array */
    int  * type_ret;
{
    int type;
    char * p;

    if (!colorDefn) {
	return NULL;
    }

    if ((colorDefn = GetType(colorDefn, &type)) == NULL) {
	/* unknown type */
	return NULL;
    }
    else {
	*type_ret = type;
    }

    /* skip white spaces */
    while (*colorDefn && isspace(*colorDefn)) {
	colorDefn ++;
    }

    p = colorName;

    while (1) {
	int dummy;

	while (*colorDefn && !isspace(*colorDefn)) {
	    *p++ = *colorDefn++;
	}

	if (!*colorDefn) {
	    break;
	}

	if (GetType(colorDefn, &dummy) == NULL) {
	    /* the next string should also be considered as a part of a color
	     * name */
	    
	    while (*colorDefn && isspace(*colorDefn)) {
		*p++ = *colorDefn++;
	    }
	} else {
	    break;
	}
	if (!*colorDefn) {
	    break;
	}
    }

    /* Mark the end of the colorName */
    *p = '\0';

    return colorDefn;
}

/*----------------------------------------------------------------------
 * ImgXpmGetPixmapFromData --
 *
 *	Creates a pixmap for an image instance.
 *----------------------------------------------------------------------
 */
static void ImgXpmGetPixmapFromData(interp, masterPtr, instancePtr)
    Tcl_Interp * interp;
    PixmapMaster *masterPtr;
    PixmapInstance *instancePtr;
{
    XImage * image = NULL, * mask = NULL;
    int pad, bitmap_pad, depth, i, j, k, lOffset, isTransp = 0, isMono, bpl;
    ColorStruct * colors;
    GC gc;
    Display *display = Tk_Display(instancePtr->tkwin);

    depth = Tk_Depth(instancePtr->tkwin);
    if (depth > 16) {
	pad = 32;
    }
    else if (depth > 8) {
	pad = 16;
    }
    else {
	pad = 8;
    }

    switch ((Tk_Visual(instancePtr->tkwin))->class) {
      case StaticGray:
      case GrayScale:
	isMono = 1;
	break;
      default:
	isMono = 0;
    }

    /*
     * Create the XImage structures to store the temporary image
     */
#ifdef _WIN32
    /* On Windows, we always create the bitmap using 24 bits, because
       that lets us just store the RGB value, and not worry about
       building a color palette. */
    image = XCreateImage(display,
	Tk_Visual(instancePtr->tkwin),
	24, ZPixmap, 0, 0,
	masterPtr->size[0], masterPtr->size[1], pad, 0);
#else
    image = XCreateImage(display,
	Tk_Visual(instancePtr->tkwin),
	depth, ZPixmap, 0, 0,
	masterPtr->size[0], masterPtr->size[1], pad, 0);
#endif
    image->data =
      (char *)ckalloc(image->bytes_per_line * masterPtr->size[1]);
#ifdef _WIN32
    /* On Windows, we don't use a clip pixmap, so it's important to
       clear the data. */
    memset (image->data, 0, image->bytes_per_line * masterPtr->size[1]);
#endif

/* If the width of the mask is half or less than the size of
   the padding used, then the pixmap mask might be drawn twice as
   high as it should.  Adding one to the width seems to fix this problem.
    [irox: 10/14/98 ] */
#define LONGBITS    (sizeof(long) * 8)
    bitmap_pad = (pad + LONGBITS - 1) / LONGBITS * LONGBITS;

    if (masterPtr->size[0]<=(bitmap_pad/2)) {
        mask  = XCreateImage(display,
	Tk_Visual(instancePtr->tkwin),
	    1, ZPixmap, 0, 0,
	    masterPtr->size[0]+1, masterPtr->size[1], pad, 0);
    } else {
	mask  = XCreateImage(display,
	    Tk_Visual(instancePtr->tkwin),
	    1, ZPixmap, 0, 0,
	    masterPtr->size[0], masterPtr->size[1], pad, 0);
    }

    mask->data =
      (char *)ckalloc(mask->bytes_per_line  * masterPtr->size[1]);
#ifdef _WIN32
    /* On Windows, we don't use a clip pixmap, so it's important to
       clear the data. */
    memset (mask->data, 0, mask->bytes_per_line * masterPtr->size[1]);
#endif

    /*
     * Parse the colors
     */
    lOffset = 1;
    colors = (ColorStruct*)ckalloc(sizeof(ColorStruct)*masterPtr->ncolors);

    /* Initialize the color structures */
    for (i=0; i<masterPtr->ncolors; i++) {
	colors[i].colorPtr = NULL;
	if (masterPtr->cpp == 1) {
	    colors[i].c = 0;
	} else {
	    colors[i].cstring = (char*)ckalloc(masterPtr->cpp);
	    colors[i].cstring[0] = 0;
	}
    }

    for (i=0; i<masterPtr->ncolors; i++) {
	char * colorDefn;		/* the color definition line */
	char * colorName;		/* temp place to hold the color name
					 * defined for one type of visual */
	char * useName;			/* the color name used for this
					 * color. If there are many names
					 * defined, choose the name that is
					 * "best" for the target visual
					 */
	int found;

	colorDefn = masterPtr->data[i+lOffset]+masterPtr->cpp;
	colorName = (char*)ckalloc(strlen(colorDefn));
	useName   = (char*)ckalloc(strlen(colorDefn));
	found     = 0;

	while (colorDefn && *colorDefn) {
	    int type;

	    if ((colorDefn=GetColor(colorDefn, colorName, &type)) == NULL) {
		break;
	    }
	    if (colorName[0] == '\0') {
		continue;
	    }

	    switch (type) {
	      case XPM_MONO:
		if (isMono && depth == 1) {
		    strcpy(useName, colorName);
		    found = 1; goto gotcolor;
		}
		break;
	      case XPM_GRAY_4:
		if (isMono && depth == 4) {
		    strcpy(useName, colorName);
		    found = 1; goto gotcolor;
		}
		break;
	      case XPM_GRAY:
		if (isMono && depth > 4) {
		    strcpy(useName, colorName);
		    found = 1; goto gotcolor;
		}
		break;
	      case XPM_COLOR:
		if (!isMono) {
		    strcpy(useName, colorName);
		    found = 1; goto gotcolor;
		}
		break;
	    }
	    if (type != XPM_SYMBOLIC && type != XPM_UNKNOWN) {
		if (!found) {			/* use this color as default */
		    strcpy(useName, colorName);
		    found = 1;
		}
	    }
	}

      gotcolor:
	if (masterPtr->cpp == 1) {
	    colors[i].c = masterPtr->data[i+lOffset][0];
	} else {
	    strncpy(colors[i].cstring, masterPtr->data[i+lOffset],
		(size_t)masterPtr->cpp);
	} 

	if (found) {
	    if (strncasecmp(useName, "none",4) != 0) {
		colors[i].colorPtr = Tk_GetColor(interp,
		    instancePtr->tkwin, Tk_GetUid(useName));
		if (colors[i].colorPtr == NULL) {
		    colors[i].colorPtr = Tk_GetColor(interp,
			instancePtr->tkwin, Tk_GetUid("black"));
		}
	    }
	} else {
	    colors[i].colorPtr = Tk_GetColor(interp,
		instancePtr->tkwin, Tk_GetUid("black"));
	}

	ckfree(colorName);
	ckfree(useName);
    }

    lOffset += masterPtr->ncolors;

    /*
     * Parse the main body of the image
     */
    for (i=0; i<masterPtr->size[1]; i++) {
	char * p = masterPtr->data[i+lOffset];

	for (j=0; j<masterPtr->size[0]; j++) {
	    if (masterPtr->cpp == 1) {
		for (k=0; k<masterPtr->ncolors; k++) {
		    if (*p == colors[k].c) {
			if (colors[k].colorPtr != NULL) {
			    XPutPixel(image, j, i, colors[k].colorPtr->pixel);
			    XPutPixel(mask,  j, i, 1);
			} else {
			    XPutPixel(mask,  j, i, 0);
			    isTransp = 1;
			}
			break;
		    }
		}
		if (*p) {
		    p++;
		}
	    } else {
		for (k=0; k<masterPtr->ncolors; k++) {
		    if (strncmp(p, colors[k].cstring, 
			    (size_t)masterPtr->cpp) == 0) {
			if (colors[k].colorPtr != NULL) {
			    XPutPixel(image, j, i, colors[k].colorPtr->pixel);
			    XPutPixel(mask,  j, i, 1);
			} else {
			    XPutPixel(mask,  j, i, 0);
			    isTransp = 1;
			}
			break;
		    }
		}
		for (k=0; *p && k<masterPtr->cpp; k++) {
		    p++;
		}
	    }
	}
    }

    /*
     * Create the pixmap(s) from the XImage structure. The mask is created
     * only if needed (i.e., there is at least one transparent pixel)
     */
    instancePtr->colors = colors;

    /* main image */
    instancePtr->pixmap = Tk_GetPixmap(display,
	Tk_WindowId(instancePtr->tkwin),
	masterPtr->size[0], masterPtr->size[1], depth);

    gc = Tk_GetGC(instancePtr->tkwin, 0, NULL);

    TkPutImage(NULL, 0, display, instancePtr->pixmap,
	gc, image, 0, 0, 0, 0, masterPtr->size[0], masterPtr->size[1]);

    Tk_FreeGC(display, gc);

    /* mask, if necessary */
    if (isTransp) {
	instancePtr->mask = Tk_GetPixmap(display,
	    Tk_WindowId(instancePtr->tkwin),
	    masterPtr->size[0], masterPtr->size[1], 1);
	gc = XCreateGC(display, instancePtr->mask, 0, NULL);

	TkPutImage(NULL, 0, display, instancePtr->mask,
	    gc, mask,  0, 0, 0, 0, masterPtr->size[0], masterPtr->size[1]);
	XFreeGC(display, gc);
    } else {
	instancePtr->mask = None;
    }

    /* Done */
    if (image) {
	ckfree((char*)image->data);
	image->data = NULL;
#ifndef _WIN32
	XDestroyImage(image);
#else
	ckfree((char *)image);
#endif
    }
    if (mask) {
	ckfree((char*)mask->data);
	mask->data = NULL;
#ifndef _WIN32
	XDestroyImage(mask);
#else
	ckfree((char *)mask);
#endif
    }
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmConfigureInstance --
 *
 *	This procedure is called to create displaying information for
 *	a pixmap image instance based on the configuration information
 *	in the master.  It is invoked both when new instances are
 *	created and when the master is reconfigured.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Generates errors via Tk_BackgroundError if there are problems
 *	in setting up the instance.
 *
 *----------------------------------------------------------------------
 */
static void
ImgXpmConfigureInstance(instancePtr)
    PixmapInstance *instancePtr;	/* Instance to reconfigure. */
{
    PixmapMaster *masterPtr = instancePtr->masterPtr;
    XGCValues gcValues;
    GC gc;
    unsigned int gcMask;

    if (instancePtr->pixmap != None) {
	Tk_FreePixmap(Tk_Display(instancePtr->tkwin), instancePtr->pixmap);
    }
    if (instancePtr->mask != None) {
	Tk_FreePixmap(Tk_Display(instancePtr->tkwin), instancePtr->mask);
    }

    if (instancePtr->colors != NULL) {
	int i;
	for (i=0; i<masterPtr->ncolors; i++) {
	    if (instancePtr->colors[i].colorPtr != NULL) {
		Tk_FreeColor(instancePtr->colors[i].colorPtr);
	    }
	    if (masterPtr->cpp != 1) {
		ckfree(instancePtr->colors[i].cstring);
	    }
	}
	ckfree((char*)instancePtr->colors);
    }

    if (Tk_WindowId(instancePtr->tkwin) == None) {
	Tk_MakeWindowExist(instancePtr->tkwin);
    }

    /* Assumption: masterPtr->data is always non NULL (enfored by
     * ImgXpmConfigureMaster()). Also, the data must be in a valid
     * format (partially enforced by ImgXpmConfigureMaster(), see comments
     * inside that function).
     */
    ImgXpmGetPixmapFromData(masterPtr->interp, masterPtr, instancePtr);

    /* Allocate a GC for drawing this instance (mask is not used if there
     * is no transparent pixels inside the image).*/
    if (instancePtr->mask != None) {
	gcMask = GCGraphicsExposures|GCClipMask;
    } else {
	gcMask = GCGraphicsExposures;
    }

#ifdef _WIN32
    if (instancePtr->mask != None) {
	/* See ImgXpmDisplay.  If we have a mask, we set the GC
	   function to merge the source onto the destination.  In
	   ImgXpmDisplay we use the mask to clear the destination
	   first. */
	gcMask |= GCFunction;
	gcValues.function = GXor;
    }
#endif

    gcValues.graphics_exposures = False;
    gcValues.clip_mask = instancePtr->mask;
    
    gc = Tk_GetGC(instancePtr->tkwin, gcMask, &gcValues);

    if (instancePtr->gc != None) {
	Tk_FreeGC(Tk_Display(instancePtr->tkwin), instancePtr->gc);
    }
    instancePtr->gc = gc;
    return;
}

/*
 *--------------------------------------------------------------
 *
 * ImgXpmCmd --
 *
 *	This procedure is invoked to process the Tcl command
 *	that corresponds to an image managed by this module.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static int
ImgXpmCmd(clientData, interp, argc, argv)
    ClientData clientData;	/* Information about button widget. */
    Tcl_Interp *interp;		/* Current interpreter. */
    int argc;			/* Number of arguments. */
    char **argv;		/* Argument strings. */
{
    PixmapMaster *masterPtr = (PixmapMaster *) clientData;
    int c, code;
    size_t length;

    if (argc < 2) {
	sprintf(interp->result,
	    "wrong # args: should be \"%.50s option ?arg arg ...?\"",
	    argv[0]);
	return TCL_ERROR;
    }
    c = argv[1][0];
    length = strlen(argv[1]);
    if ((c == 'c') && (strncmp(argv[1], "cget", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " cget option\"",
		    (char *) NULL);
	    return TCL_ERROR;
	}
	return Tk_ConfigureValue(interp, Tk_MainWindow(interp), configSpecs,
		(char *) masterPtr, argv[2], 0);
    } else if ((c == 'c') && (strncmp(argv[1], "configure", length) == 0)
	    && (length >= 2)) {
	if (argc == 2) {
	    code = Tk_ConfigureInfo(interp, Tk_MainWindow(interp),
		    configSpecs, (char *) masterPtr, (char *) NULL, 0);
	} else if (argc == 3) {
	    code = Tk_ConfigureInfo(interp, Tk_MainWindow(interp),
		    configSpecs, (char *) masterPtr, argv[2], 0);
	} else {
	    code = ImgXpmConfigureMaster(masterPtr, argc-2, argv+2,
		    TK_CONFIG_ARGV_ONLY);
	}
	return code;
    }

  error:

    Tcl_AppendResult(interp, "bad option \"", argv[1],
	"\": must be cget or configure", (char *) NULL);
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmGet --
 *
 *	This procedure is called for each use of a pixmap image in a
 *	widget.
 *
 * Results:
 *	The return value is a token for the instance, which is passed
 *	back to us in calls to ImgXpmDisplay and ImgXpmFre.
 *
 * Side effects:
 *	A data structure is set up for the instance (or, an existing
 *	instance is re-used for the new one).
 *
 *----------------------------------------------------------------------
 */

static ClientData
ImgXpmGet(tkwin, masterData)
    Tk_Window tkwin;		/* Window in which the instance will be
				 * used. */
    ClientData masterData;	/* Pointer to our master structure for the
				 * image. */
{
    PixmapMaster *masterPtr = (PixmapMaster *) masterData;
    PixmapInstance *instancePtr;

    /*
     * See if there is already an instance for this window.  If so
     * then just re-use it.
     */

    for (instancePtr = masterPtr->instancePtr; instancePtr != NULL;
	    instancePtr = instancePtr->nextPtr) {
	if (instancePtr->tkwin == tkwin) {
	    instancePtr->refCount++;
	    return (ClientData) instancePtr;
	}
    }

    /*
     * The image isn't already in use in this window.  Make a new
     * instance of the image.
     */
    instancePtr = (PixmapInstance *) ckalloc(sizeof(PixmapInstance));
    instancePtr->refCount = 1;
    instancePtr->masterPtr = masterPtr;
    instancePtr->tkwin = tkwin;
    instancePtr->pixmap = None;
    instancePtr->mask = None;
    instancePtr->gc = None;
    instancePtr->nextPtr = masterPtr->instancePtr;
    instancePtr->colors = NULL;
    masterPtr->instancePtr = instancePtr;
    ImgXpmConfigureInstance(instancePtr);

    /*
     * If this is the first instance, must set the size of the image.
     */
    if (instancePtr->nextPtr == NULL) {
	if (masterPtr->data) {
	    Tk_ImageChanged(masterPtr->tkMaster, 0, 0,
	        masterPtr->size[0], masterPtr->size[1],
	        masterPtr->size[0], masterPtr->size[1]);
	} else {
	    Tk_ImageChanged(masterPtr->tkMaster, 0, 0, 0, 0, 0, 0);
	}
    }

    return (ClientData) instancePtr;
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmDisplay --
 *
 *	This procedure is invoked to draw a pixmap image.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	A portion of the image gets rendered in a pixmap or window.
 *
 *----------------------------------------------------------------------
 */

static void
ImgXpmDisplay(clientData, display, drawable, imageX, imageY, width,
	height, drawableX, drawableY)
    ClientData clientData;	/* Pointer to PixmapInstance structure for
				 * for instance to be displayed. */
    Display *display;		/* Display on which to draw image. */
    Drawable drawable;		/* Pixmap or window in which to draw image. */
    int imageX, imageY;		/* Upper-left corner of region within image
				 * to draw. */
    int width, height;		/* Dimensions of region within image to draw.*/
    int drawableX, drawableY;	/* Coordinates within drawable that
				 * correspond to imageX and imageY. */
{
    PixmapInstance *instancePtr = (PixmapInstance *) clientData;

    /*
     * If there's no graphics context, it means that an error occurred
     * while creating the image instance so it can't be displayed.
     */

    if (instancePtr->gc == None) {
	return;
    }

    /*
     * We always use masking: modify the mask origin within
     * the graphics context to line up with the image's origin.
     * Then draw the image and reset the clip origin, if there's
     * a mask.
     */

#ifdef _WIN32
    /* The Tk 7.6 XCopyArea implementation on Windows does not support
       a pixmap as a clip region, so we use the mask to first clear
       out everything in the destination that we want to paint.  */
    if (instancePtr->mask != None) {
	XGCValues gcValues;
	GC gc;

	gcValues.function = GXandInverted;
	gcValues.graphics_exposures = False;
	gc = Tk_GetGC(instancePtr->tkwin, GCFunction|GCGraphicsExposures,
		      &gcValues);
	XCopyArea(display, instancePtr->mask, drawable, gc, imageX,
	    imageY, (unsigned) width, (unsigned) height, drawableX,
	    drawableY);
	Tk_FreeGC(display, gc);
    }
#endif

    XSetClipOrigin(display, instancePtr->gc, drawableX - imageX,
	drawableY - imageY);
    XCopyArea(display, instancePtr->pixmap, drawable, instancePtr->gc,
	imageX, imageY, (unsigned) width, (unsigned) height,
	drawableX, drawableY);
    XSetClipOrigin(display, instancePtr->gc, 0, 0);
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmFree --
 *
 *	This procedure is called when a widget ceases to use a
 *	particular instance of an image.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Internal data structures get cleaned up.
 *
 *----------------------------------------------------------------------
 */

static void
ImgXpmFree(clientData, display)
    ClientData clientData;	/* Pointer to PixmapInstance structure for
				 * for instance to be displayed. */
    Display *display;		/* Display containing window that used image.*/
{
    PixmapInstance *instancePtr = (PixmapInstance *) clientData;
    PixmapInstance *prevPtr;

    instancePtr->refCount--;
    if (instancePtr->refCount > 0) {
	return;
    }

    /*
     * There are no more uses of the image within this widget.  Free
     * the instance structure.
     */
    if (instancePtr->pixmap != None) {
	Tk_FreePixmap(display, instancePtr->pixmap);
    }
    if (instancePtr->mask != None) {
	Tk_FreePixmap(display, instancePtr->mask);
    }
    if (instancePtr->gc != None) {
	Tk_FreeGC(display, instancePtr->gc);
    }
    if (instancePtr->colors != NULL) {
	int i;
	for (i=0; i<instancePtr->masterPtr->ncolors; i++) {
	    if (instancePtr->colors[i].colorPtr != NULL) {
		Tk_FreeColor(instancePtr->colors[i].colorPtr);
	    }
	    if (instancePtr->masterPtr->cpp != 1) {
		ckfree(instancePtr->colors[i].cstring);
	    }
	}
	ckfree((char*)instancePtr->colors);
    }

    if (instancePtr->masterPtr->instancePtr == instancePtr) {
	instancePtr->masterPtr->instancePtr = instancePtr->nextPtr;
    } else {
	for (prevPtr = instancePtr->masterPtr->instancePtr;
		prevPtr->nextPtr != instancePtr; prevPtr = prevPtr->nextPtr) {
	    /* Empty loop body */
	}
	prevPtr->nextPtr = instancePtr->nextPtr;
    }
    ckfree((char *) instancePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmDelete --
 *
 *	This procedure is called by the image code to delete the
 *	master structure for an image.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Resources associated with the image get freed.
 *
 *----------------------------------------------------------------------
 */

static void
ImgXpmDelete(masterData)
    ClientData masterData;	/* Pointer to PixmapMaster structure for
				 * image.  Must not have any more instances. */
{
    PixmapMaster *masterPtr = (PixmapMaster *) masterData;

    if (masterPtr->instancePtr != NULL) {
	panic("tried to delete pixmap image when instances still exist");
    }
    masterPtr->tkMaster = NULL;
    if (masterPtr->imageCmd != NULL) {
	Tcl_DeleteCommand(masterPtr->interp,
		Tcl_GetCommandName(masterPtr->interp, masterPtr->imageCmd));
    }
    if (masterPtr->isDataAlloced && masterPtr->data != NULL) {
	ckfree((char*)masterPtr->data);
    }
    Tk_FreeOptions(configSpecs, (char *) masterPtr, (Display *) NULL, 0);
    ckfree((char *) masterPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * ImgXpmCmdDeletedProc --
 *
 *	This procedure is invoked when the image command for an image
 *	is deleted.  It deletes the image.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The image is deleted.
 *
 *----------------------------------------------------------------------
 */
static void
ImgXpmCmdDeletedProc(clientData)
    ClientData clientData;	/* Pointer to PixmapMaster structure for
				 * image. */
{
    PixmapMaster *masterPtr = (PixmapMaster *) clientData;

    masterPtr->imageCmd = NULL;
    if (masterPtr->tkMaster != NULL) {
	Tk_DeleteImage(masterPtr->interp, Tk_NameOfImage(masterPtr->tkMaster));
    }
}



#if 0

/* We currently don't need this code for the IDE.
   If we ever do, uncomment it and change its name so that it starts
   with the "ide_" prefix.  */

/*
 *----------------------------------------------------------------------
 *
 * Tix_DefinePixmap
 *
 *	Define an XPM data structure with an unique name, so that you can
 *	later refer to this pixmap using the -id switch in [image create
 *	pixmap].
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The data is stored in a HashTable.
 *----------------------------------------------------------------------
 */
int
Tix_DefinePixmap(interp, name, data)
    Tcl_Interp * interp;
    Tk_Uid name;		/* Name to use for bitmap.  Must not already
				 * be defined as a bitmap. */
    char **data;
{
    int new;
    Tcl_HashEntry *hshPtr;

    if (!xpmTableInited) {
	xpmTableInited = 1;
	Tcl_InitHashTable(&xpmTable, TCL_ONE_WORD_KEYS);
    }

    hshPtr = Tcl_CreateHashEntry(&xpmTable, name, &new);
    if (!new) {
        Tcl_AppendResult(interp, "pixmap \"", name,
		"\" is already defined", (char *) NULL);
	return TCL_ERROR;
    }
    Tcl_SetHashValue(hshPtr, (char*)data);
    return TCL_OK;
}

#endif

void
ide_create_xpm_image_type ()
{
	Tk_CreateImageType(&tixPixmapImageType);
}
