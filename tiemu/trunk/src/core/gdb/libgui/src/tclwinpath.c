/* tclwinpath.c -- Tcl routines to convert paths under cygwin32.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.

   This file contains Tcl interface routines to do path translation
   when using cygwin32.  */

#ifdef __CYGWIN32__

#include <windows.h>

#include <tcl.h>

#include "guitcl.h"
#include "subcommand.h"

/* The path conversion routines are not declared anywhere that I know
   of.  */

extern void cygwin32_conv_to_win32_path (const char *, char *);
extern void cygwin32_conv_to_full_win32_path (const char *, char *);
extern void cygwin32_conv_to_posix_path (const char *, char *);
extern void cygwin32_conv_to_full_posix_path (const char *, char *);
extern int cygwin32_posix_path_list_p (const char *);
extern int cygwin32_win32_to_posix_path_list_buf_size (const char *);
extern int cygwin32_posix_to_win32_path_list_buf_size (const char *);
extern void cygwin32_win32_to_posix_path_list (char *, char *);
extern void cygwin32_posix_to_win32_path_list (char *, char *);
extern void cygwin32_split_path (const char *, char *, char *);

/* This file declares a Tcl command with subcommands.

   Each of the following subcommands returns a string based on the
   PATH argument.  If PATH is already in the desired form, these
   commands just return it unchanged.

   ide_cygwin_path to_win32 PATH
       Return PATH converted to a win32 pathname.

   ide_cygwin_path to_full_win32 PATH
       Return PATH converted to an absolute win32 pathname.

   ide_cygwin_path to_posix PATH
       Return PATH converted to a POSIX pathname.

   ide_cygwin_path to_full_posix PATH
       Return PATH converted to an absolute POSIX pathname.

   The following subcommand returns a boolean value.

   ide_cygwin_path posix_path_list_p PATHLIST
       Return whether PATHLIST is a POSIX style path list.

   The following subcommands return strings.

   ide_cygwin_path posix_to_win32_path_list PATHLIST
       Return PATHLIST converted from POSIX style to win32 style.

   ide_cygwin_path win32_to_posix_path_list PATHLIST
       Return PATHLIST converted from win32 style to POSIX style.

   */

/* Handle ide_cygwin_path to_win32.  */

static int
path_to_win32 (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  char buf[MAX_PATH];

  cygwin32_conv_to_win32_path (argv[2], buf);
  Tcl_SetResult (interp, buf, TCL_VOLATILE);
  return TCL_OK;
}

/* Handle ide_cygwin_path to_full_win32.  */

static int
path_to_full_win32 (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  char buf[MAX_PATH];

  cygwin32_conv_to_full_win32_path (argv[2], buf);
  Tcl_SetResult (interp, buf, TCL_VOLATILE);
  return TCL_OK;
}

/* Handle ide_cygwin_path to_posix.  */

static int
path_to_posix (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  char buf[MAX_PATH];

  cygwin32_conv_to_posix_path (argv[2], buf);
  Tcl_SetResult (interp, buf, TCL_VOLATILE);
  return TCL_OK;
}

/* Handle ide_cygwin_path to_full_posix.  */

static int
path_to_full_posix (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  char buf[MAX_PATH];

  cygwin32_conv_to_full_posix_path (argv[2], buf);
  Tcl_SetResult (interp, buf, TCL_VOLATILE);
  return TCL_OK;
}

/* Handle ide_cygwin_path posix_path_list_p.  */

static int
path_posix_path_list_p (ClientData cd, Tcl_Interp *interp, int argc,
			char **argv)
{
  int ret;

  ret = cygwin32_posix_path_list_p (argv[2]);
  Tcl_ResetResult (interp);
  Tcl_SetBooleanObj (Tcl_GetObjResult (interp), ret);
  return TCL_OK;
}

/* Handle ide_cygwin_path posix_to_win32_path_list.  */

static int
path_posix_to_win32_path_list (ClientData cd, Tcl_Interp *interp, int argc,
			       char **argv)
{
  int size;
  char *buf;

  size = cygwin32_posix_to_win32_path_list_buf_size (argv[2]);
  buf = ckalloc (size);
  cygwin32_posix_to_win32_path_list (argv[2], buf);
  Tcl_SetResult (interp, buf, TCL_DYNAMIC);
  return TCL_OK;
}

/* Handle ide_cygwin_path win32_to_posix_path_list.  */

static int
path_win32_to_posix_path_list (ClientData cd, Tcl_Interp *interp, int argc,
			       char **argv)
{
  int size;
  char *buf;

  size = cygwin32_win32_to_posix_path_list_buf_size (argv[2]);
  buf = ckalloc (size);
  cygwin32_win32_to_posix_path_list (argv[2], buf);
  Tcl_SetResult (interp, buf, TCL_DYNAMIC);
  return TCL_OK;
}

/* The subcommand table.  */

static const struct ide_subcommand_table path_commands[] =
{
  { "to_win32",		path_to_win32,		3, 3 },
  { "to_full_win32",	path_to_full_win32,	3, 3 },
  { "to_posix",		path_to_posix,		3, 3 },
  { "to_full_posix",	path_to_full_posix,	3, 3 },
  { "posix_path_list_p", path_posix_path_list_p, 3, 3 },
  { "posix_to_win32_path_list", path_posix_to_win32_path_list, 3, 3 },
  { "win32_to_posix_path_list", path_win32_to_posix_path_list, 3, 3 },
  { NULL, NULL, 0, 0}
};

/* Create the ide_cygwin_path command.  */

int
ide_create_cygwin_path_command (Tcl_Interp *interp)
{
  return ide_create_command_with_subcommands (interp, "ide_cygwin_path",
					      path_commands, NULL, NULL);
}

#endif /* __CYGWIN32__ */
