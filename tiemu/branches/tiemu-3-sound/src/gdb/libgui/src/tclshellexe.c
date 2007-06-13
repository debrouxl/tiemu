/* tclshellexe.c - Interface to Windows ShellExecute function.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>;
   Code mostly taken from S-N.  */

#ifdef _WIN32

#include <string.h>

#include <windows.h>

#include <tcl.h>
#include <tk.h>

#include "guitcl.h"

static int
shell_execute_command (ClientData clientData, Tcl_Interp *interp,
		       int argc, char *argv[])
{
  char	*operation;
  char	*file;
  char	*param;
  char	*dir;
  int	ret;

  if (argc < 3 || argc > 5)
    {
      Tcl_AppendResult(interp, "wrong # args:  should be \"",
		       argv[0], " operation file ?parameters? ?directory?\"", NULL);

      return TCL_ERROR;
    }
  operation = argv[1];	/* Mandatory */
  if (!*operation)
    operation = NULL;

  file = argv[2];		/* Mandatory */

  if (argc > 3)
    {
      param = argv[3];
      if (!*param)
	param = NULL;
    }
  else
    param = NULL;

  if (argc > 4)
    {
      dir = argv[4];
      if (!*dir)
	dir = NULL;
    }
  else
    dir = NULL;

  ret = (int)ShellExecute(NULL, operation, file, param, dir, SW_SHOWNORMAL);
  if (ret <= 32)
    {
      Tcl_AppendResult(interp, strerror(ret), NULL);
      return TCL_ERROR;
    }
  return TCL_OK;
}

int
ide_create_shell_execute_command (Tcl_Interp *interp)
{
  if (Tcl_CreateCommand (interp, "ide_shell_execute", shell_execute_command,
			 NULL, NULL) == NULL)
    return TCL_ERROR;
  return TCL_OK;
}

#endif /* _WIN32 */
