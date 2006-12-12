/* tclwingrab.c -- Tcl routines to enable and disable windows on Windows.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.

   This file contains routines to enable and disable windows on
   Windows.  This is used to support grabs on Windows in Tk 8.0.

   The routines in this file are expected to be invoked from
   ide_grab_support, which is defined in libide/library/wingrab.tcl.
   They are not expected to be invoked directly, so they are not
   really documented.  */

#ifdef _WIN32

#include <windows.h>

#include <tcl.h>
#include <tk.h>

#include "guitcl.h"

/* FIXME: We need to dig into the Tk window implementation internals
   to convert a Tk window to an HWND.  */

#include <tkWinInt.h>

/* Enable or disable a window.  If the clientdata argument is NULL, we
   disable the window.  Otherwise we enable the window.  This is just
   a quick hack; if we ever need to do something else, we can use a
   more serious method to distinguish the commands.  */

static int
wingrab_command (ClientData cd, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const *objv)
{
  long l;
  HWND hwnd;

  /* Note that here we understand the return value of wm frame.  */

  if (Tcl_GetLongFromObj (interp, objv[1], &l) != TCL_OK)
    return TCL_ERROR;

  hwnd = (HWND) l;
  EnableWindow (hwnd, cd != NULL);

  return TCL_OK;
}

/* Create the ide_grab_support_disable and ide_grab_support_enable
   commands.  */

int
ide_create_win_grab_command (Tcl_Interp *interp)
{
  if (Tcl_CreateObjCommand (interp, "ide_grab_support_disable",
			    wingrab_command, NULL, NULL) == NULL
      || Tcl_CreateObjCommand (interp, "ide_grab_support_enable",
			       wingrab_command, (ClientData) 1, NULL) == NULL)
    return TCL_ERROR;
  return TCL_OK;
}

#endif /* _WIN32 */
