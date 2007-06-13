
#ifdef _WIN32

#include <windows.h>

#include "tkWinInt.h"
#include "tkCanvas.h"

#include <tcl.h>
#include <tk.h>



/*
 *--------------------------------------------------------------
 * 
 * PrintCanvasCmd -- 
 *      When invoked with the correct args this will bring up a
 *      standard Windows print dialog box and then print the
 *	contence of the canvas.
 *
 * Results:
 *      Standard Tcl result.
 * 
 *--------------------------------------------------------------
 */


int
PrintCanvasCmd(clientData, interp, argc, argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     char **argv;
{
    PRINTDLG pd;
    Tcl_CmdInfo canvCmd;
    TkCanvas *canvasPtr;
    TkWinDrawable *PrinterDrawable;
    Tk_Window tkwin;/* = canvasPtr->tkwin;*/
    Tk_Item *itemPtr;
    Pixmap pixmap;
    HDC hDCpixmap;
    TkWinDCState pixmapState;
    DEVMODE dm;
    float Ptr_pixX,Ptr_pixY,Ptr_mmX,Ptr_mmY;
    float canv_pixX,canv_pixY,canv_mmX,canv_mmY;

    int widget_X_size = 0;
    int widget_Y_size = 0;
    int page_Y_size, page_X_size;
    int tiles_wide,tiles_high;
    int tile_y, tile_x;
    int screenX1, screenX2, screenY1, screenY2, width, height;
    DOCINFO *lpdi = (DOCINFO *) ckalloc(sizeof(DOCINFO));

    if (argc < 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " canvas \"",
			 (char *) NULL);
	goto error;
    }

    /* The second arg is the canvas widget */
    if (!Tcl_GetCommandInfo(interp, argv[1], &canvCmd)) {
	Tcl_AppendResult(interp, "couldn't get canvas information for \"",
			 argv[1], "\"", (char *) NULL);
	goto error;
    }
    
    memset(&dm,0,sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    dm.dmScale = 500;

    memset(lpdi,0,sizeof(DOCINFO));
    lpdi->cbSize=sizeof(DOCINFO);
    lpdi->lpszDocName= (LPCSTR) ckalloc(255);
    sprintf((char*)lpdi->lpszDocName,"SN - Printing\0");
    lpdi->lpszOutput=NULL;

    canvasPtr = (TkCanvas *)(canvCmd.clientData);
  tkwin = canvasPtr->tkwin;
    memset(&pd,0,sizeof( PRINTDLG ));
    pd.lStructSize  = sizeof( PRINTDLG );
    pd.hwndOwner    = NULL;
    pd.hDevMode	    = NULL;
    pd.hDevNames    = NULL;
    /* pd.hDC = */
    pd.Flags	    = PD_RETURNDC;

    /* Get printer details. */
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
    screenX2=canvasPtr->width; screenY2=canvasPtr->height;
	canvasPtr->drawableXOrigin = screenX1 - 30;
	canvasPtr->drawableYOrigin = screenY1 - 30;
	pixmap = Tk_GetPixmap(Tk_Display(tkwin), Tk_WindowId(tkwin),
	    (screenX2 + 30 - canvasPtr->drawableXOrigin),
	    (screenY2 + 30 - canvasPtr->drawableYOrigin),
	    Tk_Depth(tkwin));
 	width = screenX2 - screenX1;
	height = screenY2 - screenY1;

    hDCpixmap = TkWinGetDrawableDC(Tk_Display(tkwin), pixmap, &pixmapState);
    canv_pixX=(float)GetDeviceCaps(hDCpixmap,HORZRES);
    canv_pixY=(float)GetDeviceCaps(hDCpixmap,VERTRES);
    canv_mmX=(float)GetDeviceCaps(hDCpixmap,HORZSIZE);
    canv_mmY=(float)GetDeviceCaps(hDCpixmap,VERTSIZE);

    
    SetMapMode(PrinterDrawable->winDC.hdc,MM_ISOTROPIC);
    SetWindowExtEx(PrinterDrawable->winDC.hdc,(int)((float)canv_pixX),(int)((float)canv_pixY),NULL);
    SetViewportExtEx(PrinterDrawable->winDC.hdc,(int)((float)Ptr_pixX),
			    (int)((float)Ptr_pixY),
			    NULL);

    /* max X and Y for canvas  */
    for (itemPtr = canvasPtr->firstItemPtr; itemPtr != NULL;
	    itemPtr = itemPtr->nextPtr) {
	if (itemPtr->x1 > widget_X_size) {
	    widget_X_size = itemPtr->x1;
	}
	if (itemPtr->y1 > widget_Y_size) {
	    widget_Y_size = itemPtr->y1;
	}
    }

    /* Calculate the number of tiles high */
    page_Y_size = GetDeviceCaps(hDCpixmap,LOGPIXELSY)*(Ptr_mmY/22);
    page_X_size = GetDeviceCaps(hDCpixmap,LOGPIXELSX)*(Ptr_mmX/22);

    tiles_high = ( widget_Y_size / page_Y_size ); /* start at zero */
    tiles_wide = ( widget_X_size / page_X_size ); /* start at zero */

    StartDoc(pd.hDC,lpdi);

    for (tile_x = 0; tile_x <= tiles_wide;tile_x++) {
    for (tile_y = 0; tile_y <= tiles_high;tile_y++) {
	SetViewportOrgEx(pd.hDC,-(tile_x*Ptr_pixX),-(tile_y*Ptr_pixY),NULL);
        StartPage(pd.hDC);

 	for (itemPtr = canvasPtr->firstItemPtr; itemPtr != NULL;
		itemPtr = itemPtr->nextPtr) {
	    (*itemPtr->typePtr->displayProc)((Tk_Canvas) canvasPtr, itemPtr,
		    canvasPtr->display, (unsigned long) PrinterDrawable/*pixmap*/, screenX1, screenY1, width,
		    height);
	}
    
    EndPage(pd.hDC);
    }
    }
    EndDoc(pd.hDC);

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
ide_delete_print_canvas_command(ClientData clientData)
{
  /* destructor code here.*/
}

int
ide_create_printcanvas_command (Tcl_Interp *interp)
{

  /* initialization code here */
  
        if (Tcl_CreateCommand(interp, "ide_print_canvas", PrintCanvasCmd, 
			  NULL, ide_delete_print_canvas_command) == NULL)
	return TCL_ERROR;

	    return TCL_OK;
}

#endif /* _WIN32 */
