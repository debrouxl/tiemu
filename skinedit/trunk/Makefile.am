## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS=-I m4

# subdirectories to scan
SUBDIRS = build man pixmaps po src

if USE_GROFF
SUBDIRS += man
endif

EXTRA_DIST = \
	glade/*.glade	\
	glade/*.gladep	\
	glade/*.str	\
	desktop		\
	docs
