## Process this file with automake to produce Makefile.in

INCLUDES = \
	-I$(top_srcdir)/intl \
	@GTK_CFLAGS@

bin_PROGRAMS = skinedit

skinedit_SOURCES = \
	main.c struct.h \
	defs.h keymaps.h \
	support.c support.h \
	main_intf.c main_intf.h \
	main_cb.c main_cb.h \
	prop_intf.c prop_intf.h \
	prop_cb.c prop_cb.h \
	boxes_intf.c boxes_intf.h \
	boxes_cb.c boxes_cb.h \
	rubberbox.c rubberbox.h \
	gtksdl.c gtksdl.h \
	skinops.c skinops.h \
	vti_export.c vti_export.h \
	utils.c utils.h

skinedit_LDADD = @GTK_LIBS@ @SDL_LIBS@ $(INTLLIBS)

