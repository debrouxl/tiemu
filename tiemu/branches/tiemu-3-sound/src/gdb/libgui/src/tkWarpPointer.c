
/* -----------------------------------------------------------------------------
* NAME:	
*		WarpPointer
*
* SYNOPSIS:	int WarpPointer (clientData, interp, objc, objv)
*		Implements tcl function:
*		warp_pointer win x y
*
* DESC:	
*		Forces the pointer to a specific location.  There is probably
*		no good reason to use this except in the testsuite!
*
* ARGS:	
*		win (objv[1]) - tk window name that coordinates are relative to.
*				Use "." for absolute coordinates
*
*		x (obvj[2])   - X coordinate
*		y (objv[3])   - Y coordinate
* RETURNS:	
*	
*
* NOTES:	
*	
*
* ---------------------------------------------------------------------------*/
#include "tk.h"
#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#endif

int
WarpPointer (clientData, interp, objc, objv)
    ClientData clientData;
    Tcl_Interp *interp;
    int objc;
    Tcl_Obj *CONST objv[];
{
    Tk_Window tkwin;
    int x, y;
    char *str;

    if (objc != 4) {
      Tcl_WrongNumArgs(interp, 1, objv, "widgetId x y");
      return TCL_ERROR;
    }

    if ((Tcl_GetIntFromObj (interp, objv[2], &x) == TCL_ERROR) ||
	(Tcl_GetIntFromObj (interp, objv[3], &y) == TCL_ERROR))
      return TCL_ERROR;
    
    tkwin = Tk_NameToWindow(interp, Tcl_GetStringFromObj(objv[1], NULL), \
			 Tk_MainWindow (interp));
    if (tkwin == NULL) 
      return TCL_ERROR;

    {
#ifdef _WIN32
      int wx, wy;
      Tk_GetRootCoords (tkwin, &wx, &wy);
      SetCursorPos (wx + x, wy + y);
#else
      Window win = Tk_WindowId(tkwin);
      XWarpPointer(Tk_Display(tkwin), None, win, 0, 0, 0, 0, x, y); 
#endif
    }

    return TCL_OK;
}

int
cyg_create_warp_pointer_command (Tcl_Interp *interp)
{
  if (!Tcl_CreateObjCommand (interp, "warp_pointer", WarpPointer, NULL, NULL))
    return TCL_ERROR;
  return TCL_OK;
}
