/* paths.c - Find IDE and application Tcl libraries.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

#include <tk.h>
#include <tcl.h>

#include "guitcl.h"



/* This Tcl code sets up all the path information we care about.

   We first look for the gui library.  This can be set by the
   REDHAT_GUI_LIBRARY environment variable.  Otherwise, it is named
   gui, and is found in $prefix/share/redhat, where $prefix is
   determined by looking at the directory where the running executable
   is installed.

   We then look for the ide library.  This can be set by the
   REDHAT_IDE_LIBRARY environment variable.  Otherwise, it is named
   ide, and is also found in $prefix/share/redhat.

   It is OK if only one of these libraries exist.  If neither exists,
   we report an error.

   We also set the following elements in the global Paths array.

   prefix         -- as in the configure argument
   exec_prefix    -- ditto
   bindir         -- ditto
   libexecdir     -- ditto
   guidir         -- the gui directory (not set if it does not exist)
   idedir         -- the ide directory (not set if it does not exist)
   appdir         -- see below
   bitmapdir      -- see below

   Paths(appdir) is set based on the ide_initialize_paths APPNAME
   parameter.  If a directory $prefix/share/redhat/APPNAME exists, we
   set Paths(appdir) to it.  More precisely, we set Paths(appdir) if
   an APPNAME directory exists which is a sibling directory of the gui
   or ide directory.  For convenience of some tools, we also check for
   $prefix/share/APPNAME, or, more precisely, we check whether APPNAME
   is a sibling directory of the parent of the gui or ide directory.

   Paths(bitmapdir) is set if gui or ide have a sibling directory
   named bitmaps.  */

#ifndef _MSC_VER
static char init_script[] = "\
proc initialize_paths {} {\n\
  global ide_application_name auto_path env Paths\n\
  global tcl_library\n\
  rename initialize_paths {}\n\
  # First find the GUI library.\n\
  set guidirs {}\n\
  set prefdirs {}\n\
  if [info exists env(REDHAT_GUI_LIBRARY)] {\n\
    lappend guidirs $env(REDHAT_GUI_LIBRARY)\n\
  }\n\
  set here [pwd]\n\
  set exec_name [info nameofexecutable]\n\
  if {[string compare [file type $exec_name] \"link\"] == 0} {\n\
    set exec_name [file readlink $exec_name]\n\
    if {[string compare [file pathtype $exec_name] \"relative\"] == 0} {\n\
        set exec_name [file join [pwd] $exec_name]\n\
    }\n\
  }\n\
  cd [file dirname $exec_name]\n\
  # Handle build with --exec-prefix and build without.\n\
  set d [file join [file dirname [pwd]] usr share]\n\
  lappend prefdirs $d\n\
  lappend guidirs [file join $d redhat gui]\n\
  set d [file join [file dirname [pwd]] share]\n\
  lappend prefdirs $d\n\
  lappend guidirs [file join $d redhat gui]\n\
  set d [file join [file dirname [file dirname [pwd]]] share]\n\
  lappend prefdirs $d\n\
  lappend guidirs [file join $d redhat gui]\n\
  set Paths(bindir) [pwd]\n\
 	# Base `prefix' on where the `share' dir is found\n\
 	foreach sd $prefdirs {\n\
 	  if [file isdirectory $sd] {\n\
 			set Paths(prefix) [file dirname $sd]\n\
 			break\n\
 		}\n\
 	}\n\
  if {[file isdirectory [file join [file dirname [pwd]] libexec]]} {\n\
    set Paths(libexecdir) [file join [file dirname [pwd]] libexec]\n\
  } else {\n\
    set Paths(libexecdir) $Paths(bindir)\n\
  }\n\
  set Paths(exec_prefix) [file dirname [pwd]]\n\
  cd $here\n\
  # Try to handle running from the build tree:\n\
  lappend guidirs [file join [file dirname [file dirname $tcl_library]] libgui library]\n\
  foreach sd $guidirs {\n\
    if {[file exists [file join $sd tclIndex]]} {\n\
      lappend auto_path $sd\n\
      set Paths(guidir) $sd\n\
      break\n\
    }\n\
  }\n\
  # Now find the IDE library, if it exists.\n\
  set idedirs {}\n\
  if [info exists env(REDHAT_IDE_LIBRARY)] {\n\
    lappend idedirs $env(REDHAT_IDE_LIBRARY)\n\
  }\n\
  foreach d $prefdirs {\n\
    lappend idedirs [file join $d redhat ide]\n\
  }\n\
  # Try to handle running from the build tree:\n\
  lappend idedirs [file join [file dirname [file dirname $tcl_library]] libide library]\n\
  foreach sd $idedirs {\n\
    if {[file exists [file join $sd tclIndex]]} {\n\
      lappend auto_path $sd\n\
      set Paths(idedir) $sd\n\
      break\n\
    }\n\
  }\n\
  # Now set the bitmap directory:\n\
  foreach v [list guidir idedir] {\n\
    if {[info exists Paths($v)]} {\n\
      set d [file dirname $Paths($v)]\n\
      if {[file isdirectory [file join $d bitmaps]]} {\n\
        set Paths(bitmapdir) [file join $d bitmaps]\n\
      }\n\
    }\n\
  }\n\
  \n\
  # We do things in a somewhat roundabout way here.  This lets us\n\
  # run from the source directory, if we're willing to be a little messy\n\
  # in our test scripts.  FIXME: find a cleaner way.\n\
  # This routine is really meant to find the libgui & libide library directories.\n\
  #\n\
  # The client may not want it trying to find the application library\n\
  # Signal that by setting ide_application_name to empty string\n\
  if {$ide_application_name != \"\"} {\n\
    foreach v [list guidir idedir] {\n\
      if {[info exists Paths($v)]} {\n\
        set d [file dirname $Paths($v)]\n\
        if {[file isdirectory [file join $d $ide_application_name]]} {\n\
	  lappend auto_path [file join $d $ide_application_name]\n\
	  set Paths(appdir) [file join $d $ide_application_name]\n\
        }\n\
      }\n\
    }\n\
    if {! [info exists Paths(appdir)]} {\n\
      foreach v [list guidir idedir] {\n\
        if {[info exists Paths($v)]} {\n\
	  set d [file dirname [file dirname $Paths($v)]]\n\
	  if {[file isdirectory [file join $d $ide_application_name]]} {\n\
	    lappend auto_path [file join $d $ide_application_name]\n\
	    set Paths(appdir) [file join $d $ide_application_name]\n\
	  }\n\
        }\n\
      }\n\
    }\n\
  }\n\
  if {[info exists Paths(guidir)] || [info exists Paths(idedir)]} {\n\
    return\n\
  }\n\
  # FIXME: must run this message through gettext.\n\
  # Can only do this once gettext is in C and not just a stub.\n\
  set msg \"Can't find the GUI Tcl library in the following directories:\n\"\n\
  append msg \"    $guidirs $idedirs\n\"\n\
  error $msg\n\
}\n\
initialize_paths";
#else
static char init_script[] = "\
proc initialize_paths {} {\n\
  global ide_application_name auto_path env Paths\n\
  global tcl_library\n\
  rename initialize_paths {}\n\
  set guidirs {}\n\
  set here [pwd]\n\
  cd [file dirname [info nameofexecutable]]\n\
  set d [file join [file dirname [pwd]] share]\n\
  lappend guidirs [file join $d redhat gui]\n\
  set d [file join [file dirname [file dirname [pwd]]] share]\n\
  lappend guidirs [file join $d redhat gui]\n\
  lappend guidirs [file join [file dirname [file dirname $tcl_library]] libgui library]\n\
  foreach sd $guidirs {\n\
    if {[file exists [file join $sd tclIndex]]} {\n\
      lappend auto_path $sd\n\
      set Paths(guidir) $sd\n\
      break\n\
    }\n\
  }\n\
  foreach v [list guidir] {\n\
    if {[info exists Paths($v)]} {\n\
      set d [file dirname $Paths($v)]\n\
      if {[file isdirectory [file join $d bitmaps]]} {\n\
        set Paths(bitmapdir) [file join $d bitmaps]\n\
      }\n\
    }\n\
  }\n\
  \n\
  if {$ide_application_name != \"\"} {\n\
    foreach v [list guidir ] {\n\
      if {[info exists Paths($v)]} {\n\
        set d [file dirname $Paths($v)]\n\
        if {[file isdirectory [file join $d $ide_application_name]]} {\n\
	  lappend auto_path [file join $d $ide_application_name]\n\
	  set Paths(appdir) [file join $d $ide_application_name]\n\
        }\n\
      }\n\
    }\n\
    if {! [info exists Paths(appdir)]} {\n\
      foreach v [list guidir] {\n\
        if {[info exists Paths($v)]} {\n\
	  set d [file dirname [file dirname $Paths($v)]]\n\
	  if {[file isdirectory [file join $d $ide_application_name]]} {\n\
	    lappend auto_path [file join $d $ide_application_name]\n\
	    set Paths(appdir) [file join $d $ide_application_name]\n\
	  }\n\
        }\n\
      }\n\
    }\n\
  }\n\
  if {[info exists Paths(guidir)]} {\n\
    return\n\
  }\n\
  set msg \"Can't find the GUI Tcl library in the following directories:\n\"\n\
  append msg \"    $guidirs\n\"\n\
  error $msg\n\
}\n\
initialize_paths";
#endif

/* Initialize the global Paths variable.  */
int
ide_initialize_paths (Tcl_Interp *interp, char *appname)
{
  if (Tcl_SetVar (interp, "ide_application_name", appname,
		  TCL_GLOBAL_ONLY) == NULL)
    return (TCL_ERROR);
  return (Tcl_GlobalEval (interp, init_script));
}

#ifdef TCLPRO_DEBUGGER
static char run_app_script[] = "\
proc initialize_find_app_script {} {\n\
  global Paths env ide_application_name\n\
  rename initialize_find_app_script {}\n\
  if {[info exists env(TCLPRO_DEBUG_DIR)]} {\n\
     source [file join $env(TCLPRO_DEBUG_DIR) prodebug.tcl]\n\
     debugger_init\n\
  }\n\
  # Look in idedir for the sake of test apps like idetrace.\n\
  foreach v [list appdir idedir] {\n\
    if {[info exists Paths($v)]} {\n\
      set file [file join $Paths($v) ${ide_application_name}.tcl]\n\
      if {[file exists $file]} {\n\
        if {[info exists env(TCLPRO_DEBUG_DIR)]} {\n\
          # Right now, only one process can be debugged at a time.\n\
          # Unset the debug dir, so we won't try to debug any\n\
          # child processes...\n\
          unset env(TCLPRO_DEBUG_DIR)\n\
          uplevel #0 debugger_eval [list source $file]\n\
        } else {\n\
          uplevel #0 [list source $file]\n\
        }\n\
        return\n\
      }\n\
    }\n\
  }\n\
  # FIXME: must run message through gettext.\n\
  error \"Can't find ${ide_application_name}.tcl\"\n\
}\n\
initialize_find_app_script";
#else
static char run_app_script[] = "\
proc initialize_find_app_script {} {\n\
  global Paths ide_application_name\n\
  rename initialize_find_app_script {}\n\
  # Look in idedir for the sake of test apps like idetrace.\n\
  foreach v [list appdir idedir] {\n\
    if {[info exists Paths($v)]} {\n\
      set file [file join $Paths($v) ${ide_application_name}.tcl]\n\
      if {[file exists $file]} {\n\
        uplevel #0 [list source $file]\n\
        return\n\
      }\n\
    }\n\
  }\n\
  # FIXME: must run message through gettext.\n\
  error \"Can't find ${ide_application_name}.tcl\"\n\
}\n\
initialize_find_app_script";
#endif

/* Run the application-specific init script.  */
int
ide_run_app_script (Tcl_Interp *interp)
{
  return (Tcl_GlobalEval (interp, run_app_script));
}
