## Process this file with automake to produce Makefile.in

# subdirectories to scan
SUBDIRS = . ROMs man pixmaps skins src

EXTRA_DIST = \
	ABOUT-NLS AUTHORS ChangeLog COPYING INSTALL* NEWS 	\
	README README* TODO RELEASE 				\
	glade_projects docs

INCLUDES = -I$(top_srcdir) -I$(includedir) \
	-I../intl -I$(top_srcdir)/intl \
	-DPREFIX=\"${prefix}\" -I. -I../ -I../../