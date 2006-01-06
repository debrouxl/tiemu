
#ifdef _WIN32

#include <windows.h>

#include "tkInt.h"
#include "tkWinInt.h"
#include "tkPort.h"
#include "tkText.h"

#define MAXINT 32000000

#define HAS_3D_BORDER	1
#define NEW_LAYOUT	2
#define TOP_LINE	4
#define BOTTOM_LINE	8


#define DINFO_OUT_OF_DATE	1
#define REDRAW_PENDING		2
#define REDRAW_BORDERS		4
#define REPICK_NEEDED		8



/*
 * The following structure describes one line of the display, which may
 * be either part or all of one line of the text.
 */

typedef struct DLine {
    TkTextIndex index;		/* Identifies first character in text
				 * that is displayed on this line. */
    int count;			/* Number of characters accounted for by this
				 * display line, including a trailing space
				 * or newline that isn't actually displayed. */
    int y;			/* Y-position at which line is supposed to
				 * be drawn (topmost pixel of rectangular
				 * area occupied by line). */
    int oldY;			/* Y-position at which line currently
				 * appears on display.  -1 means line isn't
				 * currently visible on display and must be
				 * redrawn.  This is used to move lines by
				 * scrolling rather than re-drawing. */
    int height;			/* Height of line, in pixels. */
    int baseline;		/* Offset of text baseline from y, in
				 * pixels. */
    int spaceAbove;		/* How much extra space was added to the
				 * top of the line because of spacing
				 * options.  This is included in height
				 * and baseline. */
    int spaceBelow;		/* How much extra space was added to the
				 * bottom of the line because of spacing
				 * options.  This is included in height. */
    int length;			/* Total length of line, in pixels. */
    TkTextDispChunk *chunkPtr;	/* Pointer to first chunk in list of all
				 * of those that are displayed on this
				 * line of the screen. */
    struct DLine *nextPtr;	/* Next in list of all display lines for
				 * this window.   The list is sorted in
				 * order from top to bottom.  Note:  the
				 * next DLine doesn't always correspond
				 * to the next line of text:  (a) can have
				 * multiple DLines for one text line, and
				 * (b) can have gaps where DLine's have been
				 * deleted because they're out of date. */
    int flags;			/* Various flag bits:  see below for values. */
} DLine;


typedef struct TextDInfo {
    Tcl_HashTable styleTable;	/* Hash table that maps from StyleValues
				 * to TextStyles for this widget. */
    DLine *dLinePtr;		/* First in list of all display lines for
				 * this widget, in order from top to bottom. */
    GC copyGC;			/* Graphics context for copying from off-
				 * screen pixmaps onto screen. */
    GC scrollGC;		/* Graphics context for copying from one place
				 * in the window to another (scrolling):
				 * differs from copyGC in that we need to get
				 * GraphicsExpose events. */
    int x;			/* First x-coordinate that may be used for
				 * actually displaying line information.
				 * Leaves space for border, etc. */
    int y;			/* First y-coordinate that may be used for
				 * actually displaying line information.
				 * Leaves space for border, etc. */
    int maxX;			/* First x-coordinate to right of available
				 * space for displaying lines. */
    int maxY;			/* First y-coordinate below available
				 * space for displaying lines. */
    int topOfEof;		/* Top-most pixel (lowest y-value) that has
				 * been drawn in the appropriate fashion for
				 * the portion of the window after the last
				 * line of the text.  This field is used to
				 * figure out when to redraw part or all of
				 * the eof field. */

    /*
     * Information used for scrolling:
     */

    int newCharOffset;		/* Desired x scroll position, measured as the
				 * number of average-size characters off-screen
				 * to the left for a line with no left
				 * margin. */
    int curPixelOffset;		/* Actual x scroll position, measured as the
				 * number of pixels off-screen to the left. */
    int maxLength;		/* Length in pixels of longest line that's
				 * visible in window (length may exceed window
				 * size).  If there's no wrapping, this will
				 * be zero. */
    double xScrollFirst, xScrollLast;
				/* Most recent values reported to horizontal
				 * scrollbar;  used to eliminate unnecessary
				 * reports. */
    double yScrollFirst, yScrollLast;
				/* Most recent values reported to vertical
				 * scrollbar;  used to eliminate unnecessary
				 * reports. */

    /*
     * The following information is used to implement scanning:
     */

    int scanMarkChar;		/* Character that was at the left edge of
				 * the window when the scan started. */
    int scanMarkX;		/* X-position of mouse at time scan started. */
    int scanTotalScroll;	/* Total scrolling (in screen lines) that has
				 * occurred since scanMarkY was set. */
    int scanMarkY;		/* Y-position of mouse at time scan started. */

    /*
     * Miscellaneous information:
     */

    int dLinesInvalidated;	/* This value is set to 1 whenever something
				 * happens that invalidates information in
				 * DLine structures;  if a redisplay
				 * is in progress, it will see this and
				 * abort the redisplay.  This is needed
				 * because, for example, an embedded window
				 * could change its size when it is first
				 * displayed, invalidating the DLine that
				 * is currently being displayed.  If redisplay
				 * continues, it will use freed memory and
				 * could dump core. */
    int flags;			/* Various flag values:  see below for
				 * definitions. */
} TextDInfo;

/*
 * The following structure describes how to display a range of characters.
 * The information is generated by scanning all of the tags associated
 * with the characters and combining that with default information for
 * the overall widget.  These structures form the hash keys for
 * dInfoPtr->styleTable.
 */

typedef struct StyleValues {
    Tk_3DBorder border;		/* Used for drawing background under text.
				 * NULL means use widget background. */
    int borderWidth;		/* Width of 3-D border for background. */
    int relief;			/* 3-D relief for background. */
    Pixmap bgStipple;		/* Stipple bitmap for background.  None
				 * means draw solid. */
    XColor *fgColor;		/* Foreground color for text. */
    Tk_Font tkfont;		/* Font for displaying text. */
    Pixmap fgStipple;		/* Stipple bitmap for text and other
				 * foreground stuff.   None means draw
				 * solid.*/
    int justify;		/* Justification style for text. */
    int lMargin1;		/* Left margin, in pixels, for first display
				 * line of each text line. */
    int lMargin2;		/* Left margin, in pixels, for second and
				 * later display lines of each text line. */
    int offset;			/* Offset in pixels of baseline, relative to
				 * baseline of line. */
    int overstrike;		/* Non-zero means draw overstrike through
				 * text. */
    int rMargin;		/* Right margin, in pixels. */
    int spacing1;		/* Spacing above first dline in text line. */
    int spacing2;		/* Spacing between lines of dline. */
    int spacing3;		/* Spacing below last dline in text line. */
    TkTextTabArray *tabArrayPtr;/* Locations and types of tab stops (may
				 * be NULL). */
    int underline;		/* Non-zero means draw underline underneath
				 * text. */
    Tk_Uid wrapMode;		/* How to handle wrap-around for this tag.
				 * One of tkTextCharUid, tkTextNoneUid,
				 * or tkTextWordUid. */
} StyleValues;

/*
 * The following structure extends the StyleValues structure above with
 * graphics contexts used to actually draw the characters.  The entries
 * in dInfoPtr->styleTable point to structures of this type.
 */

typedef struct TextStyle {
    int refCount;		/* Number of times this structure is
				 * referenced in Chunks. */
    GC bgGC;			/* Graphics context for background.  None
				 * means use widget background. */
    GC fgGC;			/* Graphics context for foreground. */
    StyleValues *sValuePtr;	/* Raw information from which GCs were
				 * derived. */
    Tcl_HashEntry *hPtr;	/* Pointer to entry in styleTable.  Used
				 * to delete entry. */
} TextStyle;




static void
DisplayDLineToDrawable(TkText *textPtr, DLine *dlPtr, DLine *prevPtr, TkWinDrawable *drawable);

/*
 *--------------------------------------------------------------
 * 
 * PrintTextCmd -- 
 *      When invoked with the correct args this will bring up a
 *      standard Windows print dialog box and then print the
 *	contence of the text wiget.
 *
 * Results:
 *      Standard Tcl result.
 * 
 *--------------------------------------------------------------
 */

static int
PrintTextCmd(clientData, interp, argc, argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     char **argv;
{
    PRINTDLG pd; 
    Tcl_CmdInfo textCmd;
    TkText *textPtr;
    TextDInfo *dInfoPtr;
    DLine *dlPtr;
    TkWinDrawable *PrinterDrawable;
    Tk_Window tkwin;
    Tk_Item *itemPtr;
    int maxHeight;
    DLine *prevPtr;
    Pixmap pixmap;
    int bottomY = 0;		/* Initialization needed only to stop
				 * compiler warnings. */
    DOCINFO *lpdi = (DOCINFO *) ckalloc(sizeof(DOCINFO));
    TkTextIndex first, last;
    int numLines;
    HDC hDCpixmap;
    TkWinDCState pixmapState;
    DEVMODE dm;
    float Ptr_pixX,Ptr_pixY,Ptr_mmX,Ptr_mmY;
    float canv_pixX,canv_pixY,canv_mmX,canv_mmY;
    int page_Y_size,tiles_high,tile_y;
    int screenX1, screenX2, screenY1, screenY2, width, height;

    int saved_x;
    int saved_y;
    int saved_w;
    int saved_h;
    int saved_maxX;
    int saved_maxY;
    int saved_eof;


    if (argc < 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " text \"",
			 (char *) NULL);
	goto error;
    }

    /*
     * The second arg is the canvas widget.
     */
    if (!Tcl_GetCommandInfo(interp, argv[1], &textCmd)) {
	Tcl_AppendResult(interp, "couldn't get text information for \"",
			 argv[1], "\"", (char *) NULL);
	goto error;
    }
    
    memset(&dm,0,sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    dm.dmScale = 500;

    memset(lpdi,0,sizeof(DOCINFO));
    lpdi->cbSize=sizeof(DOCINFO);
    lpdi->lpszDocName = (LPCSTR) ckalloc(255);
    sprintf((char*)lpdi->lpszDocName,"SN - Printing\0");
    lpdi->lpszOutput=NULL;

    textPtr = (TkText *)(textCmd.clientData);
 
    tkwin = textPtr->tkwin;
    dInfoPtr = textPtr->dInfoPtr;
    dlPtr=dInfoPtr->dLinePtr;
    memset(&pd,0,sizeof( PRINTDLG ));
    pd.lStructSize  = sizeof( PRINTDLG );
    pd.hwndOwner    = NULL;
    pd.hDevMode	    = NULL;
    pd.hDevNames    = NULL;
    pd.Flags	    = PD_RETURNDC|PD_NOSELECTION;

    /*
     * Get printer details.
     */
    if (!PrintDlg(&pd)) {
	goto done;
    }

    PrinterDrawable = (TkWinDrawable *) ckalloc(sizeof(TkWinDrawable));
    PrinterDrawable->type = TWD_WINDC;
    PrinterDrawable->winDC.hdc = pd.hDC;

    Ptr_pixX=(float)GetDeviceCaps(PrinterDrawable->winDC.hdc,HORZRES);
    Ptr_pixY=(float)GetDeviceCaps(PrinterDrawable->winDC.hdc,VERTRES);
    Ptr_mmX=(float)GetDeviceCaps(PrinterDrawable->winDC.hdc,HORZSIZE);
    Ptr_mmY=(float)GetDeviceCaps(PrinterDrawable->winDC.hdc,VERTSIZE);
 
    screenX1=0; screenY1=0;
    screenX2=dInfoPtr->maxX; screenY2=dInfoPtr->maxY;
    pixmap = Tk_GetPixmap(Tk_Display(tkwin), Tk_WindowId(tkwin),
	    (screenX2 + 30),
	    (screenY2 + 30),
	    Tk_Depth(tkwin));
    width = screenX2 - screenX1;
    height = screenY2 - screenY1;

    hDCpixmap = TkWinGetDrawableDC(Tk_Display(tkwin), pixmap, &pixmapState);
    canv_pixX=(float)GetDeviceCaps(hDCpixmap,HORZRES);
    canv_pixY=(float)GetDeviceCaps(hDCpixmap,VERTRES);
    canv_mmX=(float)GetDeviceCaps(hDCpixmap,HORZSIZE);
    canv_mmY=(float)GetDeviceCaps(hDCpixmap,VERTSIZE);

    /*
     * Save text widget data.
     */
    dInfoPtr = textPtr->dInfoPtr;
    saved_x = dInfoPtr->x;
    saved_y = dInfoPtr->y;
    saved_w = Tk_Width(textPtr->tkwin);
    saved_h = Tk_Height(textPtr->tkwin);
    saved_maxX = dInfoPtr->maxX;
    saved_maxY = dInfoPtr->maxY;
    saved_eof = dInfoPtr->topOfEof;
    dInfoPtr->maxX = MAXINT;
    Tk_Width(textPtr->tkwin) = MAXINT;
 
    dInfoPtr->maxY  = MAXINT;
    Tk_Height(textPtr->tkwin) = MAXINT;

    /* Make the text widget big enough for all the
    text to be seen. */

    numLines = TkBTreeNumLines(textPtr->tree);
#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 1)
    TkTextMakeByteIndex(textPtr->tree, 0, 0, &first);
    TkTextMakeByteIndex(textPtr->tree, numLines, 100, &last);
#else
    TkTextMakeIndex(textPtr->tree, 0, 0, &first);
    TkTextMakeIndex(textPtr->tree, numLines, 100, &last);
#endif
    TkTextChanged(textPtr, &first, &last);

    /*
     * Set the display info flag to out-of-date.
     */

    textPtr->dInfoPtr->flags|=DINFO_OUT_OF_DATE;

    /*
     *TkTextXviewCmd will call	UpdateDisplayInfo.
     */

    TkTextXviewCmd(textPtr, interp, 2, NULL);
    dInfoPtr = textPtr->dInfoPtr;

    SetMapMode(PrinterDrawable->winDC.hdc,MM_ISOTROPIC);
    SetWindowExtEx(PrinterDrawable->winDC.hdc,(int)((float)canv_pixX),(int)((float)canv_pixY),NULL);
    SetViewportExtEx(PrinterDrawable->winDC.hdc,(int)((float)Ptr_pixX),
			    (int)((float)Ptr_pixY),
			    NULL);

    /*
     * Get max Y for text widget.
     */
    maxHeight = -1;
    for (dlPtr = dInfoPtr->dLinePtr; dlPtr != NULL;
	    dlPtr = dlPtr->nextPtr) {
	maxHeight = dlPtr->y + dlPtr->height;
    }

    /*
     * Calculate the number of tiles high.
     */
    page_Y_size = GetDeviceCaps(hDCpixmap,LOGPIXELSY)*(Ptr_mmY/22);

    tiles_high = ( maxHeight / page_Y_size ); /* start at page zero */

    StartDoc(pd.hDC,lpdi);
    for (tile_y = 0; tile_y <= tiles_high;tile_y++) {
	SetViewportOrgEx(pd.hDC,0,-(tile_y*Ptr_pixY),NULL);

	StartPage(pd.hDC);

	if (maxHeight > 0) {
	    for (prevPtr = NULL, dlPtr = textPtr->dInfoPtr->dLinePtr;
			(dlPtr != NULL) && (dlPtr->y < dInfoPtr->maxY);
			prevPtr = dlPtr, dlPtr = dlPtr->nextPtr) {
	        DisplayDLineToDrawable(textPtr, dlPtr, prevPtr, PrinterDrawable);
	    
	    }
	}

    
	EndPage(pd.hDC);
    }
    EndDoc(pd.hDC);

    /*
     * Restore text widget data.
     */

    dInfoPtr->x = saved_x;
    dInfoPtr->y = saved_y;
    Tk_Width(textPtr->tkwin) = saved_w;
    Tk_Height(textPtr->tkwin) = saved_h;
    dInfoPtr->maxY = saved_maxY;
    dInfoPtr->maxX = saved_maxX;
    dInfoPtr->topOfEof = saved_eof;
    /*
     * Pitch the info again.
     */
    TkTextChanged(textPtr, &first, &last);

    /*
     * Display info not valid anymore.
     */

    textPtr->dInfoPtr->flags|=DINFO_OUT_OF_DATE;

done:
    ckfree ((char*) lpdi->lpszDocName);
    ckfree ((char*) lpdi);
    return TCL_OK;
error:
    ckfree ((char*) lpdi->lpszDocName);
    ckfree ((char*) lpdi);
    return TCL_ERROR;
}



static void 
ide_delete_print_text_command(ClientData clientData)
{
  /* destructor code here.*/
}

int
ide_create_print_text_command (Tcl_Interp *interp)
{

    if (Tcl_CreateCommand(interp, "ide_print_text", 
	PrintTextCmd, 
			  NULL, NULL) == NULL)
	return TCL_ERROR;

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * DisplayDLineToDrawable --
 *
 *	This procedure is invoked to draw a single line to a HDC
 *
 *----------------------------------------------------------------------
 */

static void
DisplayDLineToDrawable(textPtr, dlPtr, prevPtr, drawable)
    TkText *textPtr;		/* Text widget in which to draw line. */
    register DLine *dlPtr;	/* Information about line to draw. */
    DLine *prevPtr;		/* Line just before one to draw, or NULL
				 * if dlPtr is the top line. */
    TkWinDrawable *drawable;	/* drawable to use for displaying.
				 * Caller must make sure it's large enough
				 * to hold line. */
{
    register TkTextDispChunk *chunkPtr;
    TextDInfo *dInfoPtr = textPtr->dInfoPtr;
    Display *display;
    int height, x;

    /*
     * First, clear the area of the line to the background color for the
     * text widget.
     */

    display = Tk_Display(textPtr->tkwin);

    for (chunkPtr = dlPtr->chunkPtr; (chunkPtr != NULL);
	    chunkPtr = chunkPtr->nextPtr) {
	if (chunkPtr->displayProc == TkTextInsertDisplayProc) {
	    /*
	     * Already displayed the insertion cursor above.  Don't
	     * do it again here.
	     */

	    continue;
	} else {
	    x = chunkPtr->x + dInfoPtr->x - dInfoPtr->curPixelOffset;
	    if ((x + chunkPtr->width <= 0) || (x >= dInfoPtr->maxX)) {
	        (*chunkPtr->displayProc)(chunkPtr, -chunkPtr->width,
		    dlPtr->y,
		    dlPtr->height - dlPtr->spaceAbove - dlPtr->spaceBelow,
		    dlPtr->baseline - dlPtr->spaceAbove, display, (unsigned long)drawable,
		    dlPtr->y + dlPtr->spaceAbove);
	    } else {
	        (*chunkPtr->displayProc)(chunkPtr, x, dlPtr->y,
		    dlPtr->height - dlPtr->spaceAbove - dlPtr->spaceBelow,
		    dlPtr->baseline - dlPtr->spaceAbove, display, (unsigned long)drawable,
		    dlPtr->y + dlPtr->spaceAbove);
	    }
	}
    }

}

#endif /* _WIN */
