## Process this file with automake to produce Makefile.in

# subdirectories to scan
SUBDIRS = \
	build glade man pixmaps ROMs skins src

EXTRA_DIST = \
	ABOUT-NLS AUTHORS ChangeLog COPYING INSTALL* NEWS 	\
	README README* TODO RELEASE 				\
	docs

INCLUDES = -I$(top_srcdir) -I$(includedir) \
	-I../intl -I$(top_srcdir)/intl \
	-DPREFIX=\"${prefix}\" -I. -I../ -I../../