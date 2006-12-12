/* tclcursor.c - Tcl function to compute the size of a cursor.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

/* Interestingly, there is no way to get the size of a cursor in X.
   We would have to change Tk to keep track of this information if we
   cared about it.  Luckily, we only care for Windows.  */

/* This makes a Tcl command with two subcommands:

   ide_cursor size  - Return size of cursor as list {WIDTH HEIGHT}

   ide_cursor position - Return position of cursor as list {X Y}

   */

#ifdef _WIN32

#include <windows.h>

#include <tcl.h>
#include <stdio.h>

#include "guitcl.h"
#include "subcommand.h"

static int
get_cursor_size (ClientData cd, Tcl_Interp *interp, int argc, char *argv[])
{
  char buf[30];

  sprintf (buf, "%d", GetSystemMetrics (SM_CXCURSOR));
  Tcl_AppendElement (interp, buf);
  sprintf (buf, "%d", GetSystemMetrics (SM_CYCURSOR));
  Tcl_AppendElement (interp, buf);

  return TCL_OK;
}

static int
get_cursor_position (ClientData cd, Tcl_Interp *interp, int argc, char *argv[])
{
  POINT where;
  char buf[30];

  if (! GetCursorPos (&where))
    {
      Tcl_AppendResult (interp, argv[0], ": couldn't get cursor position",
			(char *) NULL);
      return TCL_ERROR;
    }

  sprintf (buf, "%ld", where.x);
  Tcl_AppendElement (interp, buf);
  sprintf (buf, "%ld", where.y);
  Tcl_AppendElement (interp, buf);

  return TCL_OK;
}

static const struct ide_subcommand_table cursor_commands[] =
{
  { "size", get_cursor_size, 2, 2 },
  { "position", get_cursor_position, 2, 2 },
  { NULL, NULL, 0, 0 }
};

int
ide_create_cursor_command (Tcl_Interp *interp)
{
  return ide_create_command_with_subcommands (interp, "ide_cursor",
					      cursor_commands,
					      (ClientData) NULL,
					      NULL);
}

#endif /* _WIN32 */
