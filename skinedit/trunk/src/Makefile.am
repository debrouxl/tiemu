## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/intl
AM_CFLAGS = -Wall

## programs and files to compile and install
bin_PROGRAMS = tiemu-skinedit

# build instructions
tiemu_skinedit_CPPFLAGS = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	 @GLIB_CFLAGS@ @GTK_CFLAGS@

tiemu_skinedit_LDADD = \
        @GLIB_LIBS@ @GTK_LIBS@ @LTLIBINTL@

tiemu_skinedit_SOURCES = \
	main.c defs.h struct.h keymaps.h \
	support.c support.h \
	main_intf.c main_intf.h \
	main_cb.c main_cb.h \
	boxes_intf.c boxes_intf.h \
	boxes_cb.c boxes_cb.h \
	prop_intf.c prop_intf.h \
	prop_cb.c prop_cb.h \
	rubberbox.c rubberbox.h \
	skinops.c skinops.h \
	utils.c utils.h \
	vti_export.c vti_export.h \
	intl.h

if OS_WIN32
  tiemu_skinedit_DEPENDENCIES = ../build/mingw/skinedit-rc.o
  tiemu_skinedit_LDFLAGS = -Wl,../build/mingw/skinedit-rc.o
endif
