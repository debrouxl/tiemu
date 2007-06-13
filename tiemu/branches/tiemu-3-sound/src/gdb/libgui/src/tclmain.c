/* tclmain.c - a simple main() for IDE programs that use Tk.
   Copyright (C) 1997, 1998 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

#include <config.h>

#include <tcl.h>
#include <tk.h>

#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#endif

#include "guitcl.h"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

/* This is like Tk_Main, except that the resulting program doesn't try
   to act like a script interpreter.  It never reads commands from
   stdin.  */
void
ide_main (int argc, char *argv[], Tcl_AppInitProc *appInitProc)
{
  Tcl_Interp *interp;
  char *args;
  char buf[20];

  Tcl_FindExecutable (argv[0]);
  interp = Tcl_CreateInterp ();

#ifdef TCL_MEM_DEBUG
  Tcl_InitMemory (interp);
#endif

  args = Tcl_Merge (argc - 1, argv + 1);
  Tcl_SetVar (interp, "argv", args, TCL_GLOBAL_ONLY);
  ckfree (args);

  sprintf (buf, "%d", argc-1);
  Tcl_SetVar (interp, "argc", buf, TCL_GLOBAL_ONLY);
  Tcl_SetVar (interp, "argv0", argv[0], TCL_GLOBAL_ONLY);

  /* We set this to "1" so that the console window will work.  */
  Tcl_SetVar (interp, "tcl_interactive", "1", TCL_GLOBAL_ONLY);
 
#if IDE_ENABLED
    Tcl_SetVar (interp, "IDE_ENABLED", "1", TCL_GLOBAL_ONLY);
#else
    Tcl_SetVar (interp, "IDE_ENABLED", "0", TCL_GLOBAL_ONLY);
#endif

  if ((*appInitProc) (interp) != TCL_OK)
    {
      Tcl_Channel err_channel;
      char *msg;

      /* Guarantee that errorInfo is set properly.  */
      Tcl_AddErrorInfo (interp, "");
      msg = Tcl_GetVar (interp, "errorInfo", TCL_GLOBAL_ONLY);

      /* On Windows, we are probably running as a windows app, and
         stderr is the bit bucket, so we call a win32 function to
         display the error.  */

#ifdef _WIN32
      MessageBox (NULL, msg, NULL, MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      err_channel = Tcl_GetStdChannel (TCL_STDERR);
      if (err_channel)
	{

	  Tcl_Write (err_channel, msg, -1);
	  Tcl_Write (err_channel, "\n", 1);
        }
#endif

      Tcl_DeleteInterp (interp);
      Tcl_Exit (EXIT_FAILURE);
    }

  Tcl_ResetResult (interp);

  /* Now just go until the user decides to shut down.  */
  Tk_MainLoop ();
  Tcl_DeleteInterp (interp);
  Tcl_Exit (EXIT_SUCCESS);
}
