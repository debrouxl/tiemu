## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = cygnus

noinst_LIBRARIES = libgui.a

if INSTALL_LIBGUI

include_HEADERS = \
	guitcl.h subcommand.h

endif

datadir = @datadir@
guidir  = $(datadir)/redhat/gui

# tkTable version info
include $(srcdir)/tkTable_version.in

# This sets the name that tkTable will define for itself when loaded
# If you change this, then the demos won't work, but it might be necessary
# for those with another built-in "table" command
TBL_COMMAND     = table

tkTable.tcl.h: $(srcdir)/tkTable.tcl
	sed -e '/^$\#/d' -e '/^$$/d' -e 's/\"/\\"/g' -e 's/^/"/' -e 's/$$/\\n"/' <$(srcdir)/tkTable.tcl >tkTable.tcl.h || rm tkTable.tcl.h 



# Defining lib_LIBRARIES conditionally doesn't do the right thing.
install-exec-local:
if INSTALL_LIBGUI
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(libdir)
	$(INSTALL_DATA) libgui.a $(libdir)/libgui.a
	@$(POST_INSTALL)
	$(RANLIB) $(libdir)/libgui.a
endif

LIBGUI_CFLAGS=@LIBGUI_CFLAGS@

## Some of the files in this directory want to see Tk internals.
## Nasty.
INCLUDES = $(LIBGUI_CFLAGS) $(TCLHDIR) \
$(TKHDIR) \
$(TK_XINCLUDES) $(TCL_DEFS) $(TK_DEFS) \
$(TKHDIR)/../unix $(TKHDIR)/../win \
-DTBL_VERSION=\"$(TBL_VERSION)\"\
-DTBL_COMMAND=\"$(TBL_COMMAND)\"\
-DTBL_RUNTIME=\"tkTable.tcl\" -DTBL_RUNTIME_DIR=\"$(guidir)\"\
-DSTATIC_BUILD

TKTABLE_SOURCES = tkTable.c tkTableCell.c tkTableCellSort.c \
tkTableCmds.c tkTableEdit.c tkTableTag.c tkTableWin.c tkTableUtil.c

libgui_a_SOURCES = guitcl.h subcommand.c subcommand.h \
tclwinprint.c tclshellexe.c paths.c \
tclwingrab.c tclwinpath.c tclmsgbox.c tclcursor.c \
tkWinPrintText.c tkWinPrintCanvas.c tkWarpPointer.c $(TKTABLE_SOURCES)

## Dependencies

paths.$(OBJEXT): paths.c guitcl.h
subcommand.$(OBJEXT): subcommand.c subcommand.h
tkCanvEdge.$(OBJEXT): tkCanvEdge.c ../config.h
tkCanvLayout.$(OBJEXT): tkCanvLayout.c ../config.h tkCanvLayout.h
tkGraphCanvas.$(OBJEXT): tkGraphCanvas.c tkCanvLayout.h
xpmlib.$(OBJEXT): xpmlib.c guitcl.h
assertions.$(OBJEXT): assertions.c ../config.h assertions.h
tclcursor.$(OBJEXT): tclcursor.c ../config.h guitcl.h subcommand.h
tclhelp.$(OBJEXT): tclhelp.c ../config.h guitcl.h subcommand.h
tclgetdir.$(OBJEXT): tclgetdir.c guitcl.h
tclmain.$(OBJEXT): tclmain.c guitcl.h
tclwinprint.$(OBJEXT): tclwinprint.c guitcl.h subcommand.h
tclsizebox.$(OBJEXT): tclsizebox.c guitcl.h
tclshellexe.$(OBJEXT): tclshellexe.c guitcl.h
tclmapi.$(OBJEXT): tclmapi.c guitcl.h subcommand.h
tclwinfont.$(OBJEXT): tclwinfont.c guitcl.h
tclwingrab.$(OBJEXT): tclwingrab.c guitcl.h
tclwinpath.$(OBJEXT): tclwinpath.c guitcl.h subcommand.h
tclwinmode.$(OBJEXT): tclwinmode.c guitcl.h
tkTable.$(OBJEXT): tkTable.c tkTable.h tkTableInitScript.h tkTable.tcl.h
tkTableCell.$(OBJEXT): tkTableCell.c tkTable.h
tkTableCellSort.$(OBJEXT): tkTableCellSort.c tkTable.h
tkTableCmds.$(OBJEXT): tkTableCmds.c tkTable.h
tkTableEdit.$(OBJEXT): tkTableEdit.c tkTable.h
tkTableTag.$(OBJEXT): tkTableTag.c tkTable.h
tkTablePs.$(OBJECT): tkTablePs.c tkTable.h
tkTableWin.$(OBJEXT):tkTableWin.c  tkTable.h
tkTableUtil.$(OBJEXT): tkTableUtil.c tkTable.h
tkTable.tcl.h: tkTable.tcl

