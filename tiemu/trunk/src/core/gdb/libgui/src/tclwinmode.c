/* tclwinmode.c - Tcl access to SetErrorMode function.
   Copyright (C) 1998 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

#include <tcl.h>
#include "guitcl.h"

#ifdef __CYGWIN32__

#include <windows.h>

struct pair
{
  const char *name;
  UINT value;
};

static struct pair values[] =
{
  { "failcriticalerrors", SEM_FAILCRITICALERRORS },
  { "noalignmentfaultexcept", SEM_NOALIGNMENTFAULTEXCEPT },
  { "nogpfaulterrorbox", SEM_NOGPFAULTERRORBOX },
  { "noopenfileerrorbox", SEM_NOOPENFILEERRORBOX },
  { NULL, 0 }
};

#endif

static int
seterrormode_command (ClientData cd, Tcl_Interp *interp,
		      int argc, char *argv[])
{
#ifdef __CYGWIN32__
  int len, i;
  char **list;
  UINT val = 0;

  if (argc != 2)
    {
      Tcl_AppendResult (interp, "wrong # args: should be \"",
			argv[0], " modelist\"", (char *) NULL);
      return TCL_ERROR;
    }

  if (Tcl_SplitList (interp, argv[1], &len, &list) != TCL_OK)
    return TCL_ERROR;

  for (i = 0; i < len; ++i)
    {
      int j, found = 0;
      for (j = 0; values[j].name; ++j)
	{
	  if (! strcmp (values[j].name, list[i]))
	    {
	      found = 1;
	      val |= values[j].value;
	      break;
	    }
	}
      if (! found)
	{
	  Tcl_AppendResult (interp, "unrecognized key \"", list[i],
			    "\"", (char *) NULL);
	  ckfree ((char *) list);
	  return TCL_ERROR;
	}
    }
  ckfree ((char *) list);

  val = SetErrorMode (val);

  for (i = 0; values[i].name; ++i)
    {
      if (val & values[i].value)
	Tcl_AppendElement (interp, values[i].name);
    }
#endif /* __CYGWIN32__ */

  return TCL_OK;
}

int
ide_create_set_error_mode_command (Tcl_Interp *interp)
{
  if (Tcl_CreateCommand (interp, "ide_set_error_mode",
			 seterrormode_command, NULL, NULL) == NULL)
    return TCL_ERROR;
  return TCL_OK;
}
