## Process this file with automake to produce Makefile.in

# subdirectories to scan
SUBDIRS = build glade help pedrom pixmaps skins src

if USE_GROFF
SUBDIRS += man
endif

dist_pkgdata_DATA = COPYING RELEASE AUTHORS LICENSES

EXTRA_DIST = README.* TODO docs

# custom targets
install-without-tcl-tk:
	$(MAKE) install SUBDIRS="build glade help pedrom pixmaps skins" DESTDIR="$(DESTDIR)"
	cd src && $(MAKE) install-without-tcl-tk DESTDIR="$(DESTDIR)"
