/* guitcl.h - Interface to Tcl layer of GUI support code.
   Copyright (C) 1997, 1998 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

#ifndef GUITCL_H
#define GUITCL_H

#ifdef __cplusplus
extern "C" {
#endif

/* This is like Tk_Main, but it doesn't assume that the program wants
   to act like an interactive interpreter.  */
extern void
ide_main (int ide_argc, char *ide_argv[], Tcl_AppInitProc *);

/* Set up the XPM image reader.  This requires Tk to be linked in.
   However, it does not require Tk to be initialized before calling.  */
extern void
ide_create_xpm_image_type (void);

/* This locates the libide and application-specific Tcl libraries.  It
   sets the global Tcl variable `ide_application_name' to IDE_APPNAME,
   and initializes a global Paths array with useful path information.
   The application-specific Tcl library is assumed to be in the
   directory $datadir/IDE_APPNAME/.
   Returns a standard Tcl result.  */
extern int
ide_initialize_paths (Tcl_Interp *, char *ide_appname);

/* This tries to find the application-specific startup file.  If it is
   found, it is sourced.  If not, an error results.  The file is
   assumed to be named $datadir/IDE_APPNAME/IDE_APPNAME.tcl, where
   IDE_APPNAME is the name that was previously passed to
   ide_initialize_paths.
   Returns a standard Tcl result.  */
extern int
ide_run_app_script (Tcl_Interp *);

/* This adds the new graph command for manipulating graphs to the
   interpreter IDE_INTERP.  
   Returns a standard Tcl result.  */
extern int
create_graph_command (Tcl_Interp *ide_interp);

/* This function creates the ide_help Tcl command.  */
int
ide_create_help_command (Tcl_Interp *);

/* This function creates the ide_get_directory Tcl command.  */
int
ide_create_get_directory_command (Tcl_Interp *);

/* This function creates the ide_winprint Tcl command.  */
int
ide_create_winprint_command (Tcl_Interp *);

/* This function creates the ide_sizebox Tcl command.  */
int
ide_create_sizebox_command (Tcl_Interp *);

/* This function creates the ide_shell_execute command.  */
int
ide_create_shell_execute_command (Tcl_Interp *);

/* This function creates the `ide_mapi' command.  */
int
ide_create_mapi_command (Tcl_Interp *);

/* This function creates the `ide_win_choose_font' command.  */
int
ide_create_win_choose_font_command (Tcl_Interp *);

/* This function creates internal commands used by ide_grab_support on
   Windows.  */
int
ide_create_win_grab_command (Tcl_Interp *);

/* This function creates the `ide_cygwin_path' command.  */
int
ide_create_cygwin_path_command (Tcl_Interp *);

/* This function creates the ide_cursor command on Windows.  */
int
ide_create_cursor_command (Tcl_Interp *);

/* This function creates the ide_set_error_mode command.  On Windows,
   this translates into a call to SetErrorMode.  On Unix, this command
   is a no-op.  */
int
ide_create_set_error_mode_command (Tcl_Interp *);

/* This function creates the ide_messageBox command.  */
int
ide_create_messagebox_command (Tcl_Interp *);

/* This function creates the "warp_pointer" command. Warp_pointer
   forces the pointer to a specific location.  There is probably no
   good reason to use this except in the testsuite!  */
int
cyg_create_warp_pointer_command (Tcl_Interp *interp);

#ifdef __cplusplus
}
#endif

#endif /* GUITCL_H */
