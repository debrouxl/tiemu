# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
# Copyright (C) 2003 Free Software Foundation, Inc.
#
# This file may be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

PACKAGE = opcodes
VERSION = 2.16.91

SHELL = /bin/sh


srcdir = ../.././opcodes/po
top_srcdir = ../.././opcodes
VPATH = ../.././opcodes/po

prefix = /usr/local
exec_prefix = ${prefix}
datadir = $(prefix)/share
localedir = $(datadir)/locale
gnulocaledir = $(prefix)/share/locale
gettextsrcdir = $(prefix)/share/gettext/po
subdir = po

DESTDIR =

INSTALL = /usr/bin/install -c
INSTALL_DATA = /usr/bin/install -c -m 644
MKINSTALLDIRS = $(srcdir)/../../mkinstalldirs

CC = gcc
GENCAT = @GENCAT@
GMSGFMT = PATH=../src:$$PATH /usr/bin/msgfmt
MSGFMT = /usr/bin/msgfmt
XGETTEXT = PATH=../src:$$PATH /usr/bin/xgettext
MSGMERGE = PATH=../src:$$PATH msgmerge

DEFS = -DHAVE_CONFIG_H
CFLAGS = -g -O2 
CPPFLAGS = 

INCLUDES = -I.. -I$(top_srcdir)/intl

COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(XCFLAGS)

SOURCES = cat-id-tbl.c
POFILES =  fr.po sv.po tr.po es.po da.po de.po id.po pt_BR.po ro.po nl.po fi.po vi.po ga.po
GMOFILES =  fr.gmo sv.gmo tr.gmo es.gmo da.gmo de.gmo id.gmo pt_BR.gmo ro.gmo nl.gmo fi.gmo vi.gmo ga.gmo
DISTFILES = ChangeLog Makefile.in.in POTFILES.in $(PACKAGE).pot \
stamp-cat-id $(POFILES) $(GMOFILES) $(SOURCES)

POTFILES = \
	../.././opcodes/alpha-dis.c \
	../.././opcodes/alpha-opc.c \
	../.././opcodes/arc-dis.c \
	../.././opcodes/arc-ext.c \
	../.././opcodes/arc-opc.c \
	../.././opcodes/arm-dis.c \
	../.././opcodes/avr-dis.c \
	../.././opcodes/bfin-dis.c \
	../.././opcodes/cgen-asm.c \
	../.././opcodes/cgen-dis.c \
	../.././opcodes/cgen-opc.c \
	../.././opcodes/cgen-ops.h \
	../.././opcodes/cgen-types.h \
	../.././opcodes/cris-dis.c \
	../.././opcodes/cris-opc.c \
	../.././opcodes/crx-dis.c \
	../.././opcodes/crx-opc.c \
	../.././opcodes/d10v-dis.c \
	../.././opcodes/d10v-opc.c \
	../.././opcodes/d30v-dis.c \
	../.././opcodes/d30v-opc.c \
	../.././opcodes/disassemble.c \
	../.././opcodes/dis-buf.c \
	../.././opcodes/dis-init.c \
	../.././opcodes/dlx-dis.c \
	../.././opcodes/fr30-asm.c \
	../.././opcodes/fr30-desc.c \
	../.././opcodes/fr30-desc.h \
	../.././opcodes/fr30-dis.c \
	../.././opcodes/fr30-ibld.c \
	../.././opcodes/fr30-opc.c \
	../.././opcodes/fr30-opc.h \
	../.././opcodes/frv-asm.c \
	../.././opcodes/frv-desc.c \
	../.././opcodes/frv-desc.h \
	../.././opcodes/frv-dis.c \
	../.././opcodes/frv-ibld.c \
	../.././opcodes/frv-opc.c \
	../.././opcodes/frv-opc.h \
	../.././opcodes/h8300-dis.c \
	../.././opcodes/h8500-dis.c \
	../.././opcodes/h8500-opc.h \
	../.././opcodes/hppa-dis.c \
	../.././opcodes/i370-dis.c \
	../.././opcodes/i370-opc.c \
	../.././opcodes/i386-dis.c \
	../.././opcodes/i860-dis.c \
	../.././opcodes/i960-dis.c \
	../.././opcodes/ia64-asmtab.c \
	../.././opcodes/ia64-asmtab.h \
	../.././opcodes/ia64-dis.c \
	../.././opcodes/ia64-gen.c \
	../.././opcodes/ia64-opc-a.c \
	../.././opcodes/ia64-opc-b.c \
	../.././opcodes/ia64-opc.c \
	../.././opcodes/ia64-opc-d.c \
	../.././opcodes/ia64-opc-f.c \
	../.././opcodes/ia64-opc.h \
	../.././opcodes/ia64-opc-i.c \
	../.././opcodes/ia64-opc-m.c \
	../.././opcodes/ip2k-asm.c \
	../.././opcodes/ip2k-desc.c \
	../.././opcodes/ip2k-desc.h \
	../.././opcodes/ip2k-dis.c \
	../.././opcodes/ip2k-ibld.c \
	../.././opcodes/ip2k-opc.c \
	../.././opcodes/ip2k-opc.h \
	../.././opcodes/iq2000-asm.c \
	../.././opcodes/iq2000-desc.c \
	../.././opcodes/iq2000-desc.h \
	../.././opcodes/iq2000-dis.c \
	../.././opcodes/iq2000-ibld.c \
	../.././opcodes/iq2000-opc.c \
	../.././opcodes/iq2000-opc.h \
	../.././opcodes/m10200-dis.c \
	../.././opcodes/m10200-opc.c \
	../.././opcodes/m10300-dis.c \
	../.././opcodes/m10300-opc.c \
	../.././opcodes/m32c-asm.c \
	../.././opcodes/m32c-desc.c \
	../.././opcodes/m32c-desc.h \
	../.././opcodes/m32c-dis.c \
	../.././opcodes/m32c-ibld.c \
	../.././opcodes/m32c-opc.c \
	../.././opcodes/m32c-opc.h \
	../.././opcodes/m32r-asm.c \
	../.././opcodes/m32r-desc.c \
	../.././opcodes/m32r-desc.h \
	../.././opcodes/m32r-dis.c \
	../.././opcodes/m32r-ibld.c \
	../.././opcodes/m32r-opc.c \
	../.././opcodes/m32r-opc.h \
	../.././opcodes/m32r-opinst.c \
	../.././opcodes/m68hc11-dis.c \
	../.././opcodes/m68hc11-opc.c \
	../.././opcodes/m68k-dis.c \
	../.././opcodes/m68k-opc.c \
	../.././opcodes/m88k-dis.c \
	../.././opcodes/maxq-dis.c \
	../.././opcodes/mcore-dis.c \
	../.././opcodes/mcore-opc.h \
	../.././opcodes/mips16-opc.c \
	../.././opcodes/mips-dis.c \
	../.././opcodes/mips-opc.c \
	../.././opcodes/mmix-dis.c \
	../.././opcodes/mmix-opc.c \
	../.././opcodes/ms1-asm.c \
	../.././opcodes/ms1-desc.c \
	../.././opcodes/ms1-desc.h \
	../.././opcodes/ms1-dis.c \
	../.././opcodes/ms1-ibld.c \
	../.././opcodes/ms1-opc.c \
	../.././opcodes/ms1-opc.h \
	../.././opcodes/ns32k-dis.c \
	../.././opcodes/openrisc-asm.c \
	../.././opcodes/openrisc-desc.c \
	../.././opcodes/openrisc-desc.h \
	../.././opcodes/openrisc-dis.c \
	../.././opcodes/openrisc-ibld.c \
	../.././opcodes/openrisc-opc.c \
	../.././opcodes/openrisc-opc.h \
	../.././opcodes/or32-dis.c \
	../.././opcodes/or32-opc.c \
	../.././opcodes/pdp11-dis.c \
	../.././opcodes/pdp11-opc.c \
	../.././opcodes/pj-dis.c \
	../.././opcodes/pj-opc.c \
	../.././opcodes/ppc-dis.c \
	../.././opcodes/ppc-opc.c \
	../.././opcodes/s390-dis.c \
	../.././opcodes/s390-mkopc.c \
	../.././opcodes/s390-opc.c \
	../.././opcodes/sh64-dis.c \
	../.././opcodes/sh64-opc.c \
	../.././opcodes/sh64-opc.h \
	../.././opcodes/sh-dis.c \
	../.././opcodes/sh-opc.h \
	../.././opcodes/sparc-dis.c \
	../.././opcodes/sparc-opc.c \
	../.././opcodes/sysdep.h \
	../.././opcodes/tic30-dis.c \
	../.././opcodes/tic4x-dis.c \
	../.././opcodes/tic54x-dis.c \
	../.././opcodes/tic54x-opc.c \
	../.././opcodes/tic80-dis.c \
	../.././opcodes/tic80-opc.c \
	../.././opcodes/v850-dis.c \
	../.././opcodes/v850-opc.c \
	../.././opcodes/vax-dis.c \
	../.././opcodes/w65-dis.c \
	../.././opcodes/w65-opc.h \
	../.././opcodes/xstormy16-asm.c \
	../.././opcodes/xstormy16-desc.c \
	../.././opcodes/xstormy16-desc.h \
	../.././opcodes/xstormy16-dis.c \
	../.././opcodes/xstormy16-ibld.c \
	../.././opcodes/xstormy16-opc.c \
	../.././opcodes/xstormy16-opc.h \
	../.././opcodes/xtensa-dis.c \
	../.././opcodes/z8k-dis.c \
	../.././opcodes/z8kgen.c \
	../.././opcodes/z8k-opc.h

CATALOGS =  fr.gmo sv.gmo tr.gmo es.gmo da.gmo de.gmo id.gmo pt_BR.gmo ro.gmo nl.gmo fi.gmo vi.gmo ga.gmo
CATOBJEXT = .gmo
INSTOBJEXT = .mo

.SUFFIXES:
.SUFFIXES: .c .o .po .pox .gmo .mo .msg .cat

.c.o:
	$(COMPILE) $<

.po.pox:
	$(MAKE) $(PACKAGE).pot
	$(MSGMERGE) $< $(srcdir)/$(PACKAGE).pot -o $*.pox

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=$(srcdir)/`echo $* | sed 's,.*/,,'`.gmo \
	  && rm -f $$file && $(GMSGFMT) -o $$file $<

.po.cat:
	sed -f ../intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && $(GENCAT) $@ $*.msg


all: all-yes

all-yes: $(CATALOGS) # $(PACKAGE).pot
all-no:

$(srcdir)/$(PACKAGE).pot: $(POTFILES)
	$(XGETTEXT) --default-domain=$(PACKAGE) --directory=$(top_srcdir) \
	  --add-comments --keyword=_ --keyword=N_ \
	  --files-from=$(srcdir)/POTFILES.in
	rm -f $(srcdir)/$(PACKAGE).pot
	mv $(PACKAGE).po $(srcdir)/$(PACKAGE).pot

$(srcdir)/cat-id-tbl.c: stamp-cat-id; @:
$(srcdir)/stamp-cat-id: $(PACKAGE).pot
	rm -f cat-id-tbl.tmp
	sed -f ../intl/po2tbl.sed $(srcdir)/$(PACKAGE).pot \
		| sed -e "s/@PACKAGE NAME@/$(PACKAGE)/" > cat-id-tbl.tmp
	if cmp -s cat-id-tbl.tmp $(srcdir)/cat-id-tbl.c; then \
	  rm cat-id-tbl.tmp; \
	else \
	  echo cat-id-tbl.c changed; \
	  rm -f $(srcdir)/cat-id-tbl.c; \
	  mv cat-id-tbl.tmp $(srcdir)/cat-id-tbl.c; \
	fi
	cd $(srcdir) && rm -f stamp-cat-id && echo timestamp > stamp-cat-id


install: install-exec install-data
install-exec:
install-info:
install-data: install-data-yes
install-data-no: all
install-data-yes: all
	if test -r $(MKINSTALLDIRS); then \
	  $(MKINSTALLDIRS) $(DESTDIR)$(datadir); \
	else \
	  $(top_srcdir)/mkinstalldirs $(DESTDIR)$(datadir); \
	fi
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  case "$$cat" in \
	    *.gmo) destdir=$(gnulocaledir);; \
	    *)     destdir=$(localedir);; \
	  esac; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  dir=$(DESTDIR)$$destdir/$$lang/LC_MESSAGES; \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $$dir; \
	  else \
	    $(top_srcdir)/mkinstalldirs $$dir; \
	  fi; \
	  if test -r $$cat; then \
	    $(INSTALL_DATA) $$cat $$dir/$(PACKAGE)$(INSTOBJEXT); \
	    echo "installing $$cat as $$dir/$(PACKAGE)$(INSTOBJEXT)"; \
	  else \
	    $(INSTALL_DATA) $(srcdir)/$$cat $$dir/$(PACKAGE)$(INSTOBJEXT); \
	    echo "installing $(srcdir)/$$cat as" \
		 "$$dir/$(PACKAGE)$(INSTOBJEXT)"; \
	  fi; \
	  if test -r $$cat.m; then \
	    $(INSTALL_DATA) $$cat.m $$dir/$(PACKAGE)$(INSTOBJEXT).m; \
	    echo "installing $$cat.m as $$dir/$(PACKAGE)$(INSTOBJEXT).m"; \
	  else \
	    if test -r $(srcdir)/$$cat.m ; then \
	      $(INSTALL_DATA) $(srcdir)/$$cat.m \
		$$dir/$(PACKAGE)$(INSTOBJEXT).m; \
	      echo "installing $(srcdir)/$$cat as" \
		   "$$dir/$(PACKAGE)$(INSTOBJEXT).m"; \
	    else \
	      true; \
	    fi; \
	  fi; \
	done
	if test "$(PACKAGE)" = "gettext"; then \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $(DESTDIR)$(gettextsrcdir); \
	  else \
	    $(top_srcdir)/mkinstalldirs $(DESTDIR)$(gettextsrcdir); \
	  fi; \
	  $(INSTALL_DATA) $(srcdir)/Makefile.in.in \
			  $(DESTDIR)$(gettextsrcdir)/Makefile.in.in; \
	else \
	  : ; \
	fi

# Define this as empty until I found a useful application.
installcheck:

uninstall:
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  rm -f $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT); \
	  rm -f $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT).m; \
	  rm -f $(DESTDIR)$(gnulocaledir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT); \
	  rm -f $(DESTDIR)$(gnulocaledir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT).m; \
	done
	rm -f $(DESTDIR)$(gettextsrcdir)/po-Makefile.in.in

check: all

cat-id-tbl.o: ../intl/libgettext.h

dvi info tags TAGS ID:

mostlyclean:
	rm -f core core.* *.pox $(PACKAGE).po *.old.po cat-id-tbl.tmp
	rm -fr *.o

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES *.mo *.msg *.cat *.cat.m

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f $(GMOFILES)

distdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir: update-po $(DISTFILES)
	dists="$(DISTFILES)"; \
	for file in $$dists; do \
	  ln $(srcdir)/$$file $(distdir) 2> /dev/null \
	    || cp -p $(srcdir)/$$file $(distdir); \
	done

update-po: Makefile
	$(MAKE) $(PACKAGE).pot
	PATH=`pwd`/../src:$$PATH; \
	cd $(srcdir); \
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  mv $$lang.po $$lang.old.po; \
	  echo "$$lang:"; \
	  if $(MSGMERGE) $$lang.old.po $(PACKAGE).pot -o $$lang.po; then \
	    rm -f $$lang.old.po; \
	  else \
	    echo "msgmerge for $$cat failed!"; \
	    rm -f $$lang.po; \
	    mv $$lang.old.po $$lang.po; \
	  fi; \
	done

POTFILES: POTFILES.in
	( if test 'x$(srcdir)' != 'x.'; then \
	    posrcprefix='$(top_srcdir)/'; \
	  else \
	    posrcprefix="../"; \
	  fi; \
	  rm -f $@-t $@ \
	    && (sed -e '/^#/d' -e '/^[ 	]*$$/d' \
		    -e "s@.*@	$$posrcprefix& \\\\@" < $(srcdir)/$@.in \
		| sed -e '$$s/\\$$//') > $@-t \
	    && chmod a-w $@-t \
	    && mv $@-t $@ )

POTFILES.in: # ../Makefile
	cd .. && $(MAKE) po/POTFILES.in

Makefile: Make-in ../config.status POTFILES
	cd .. \
	  && CONFIG_FILES=$(subdir)/Makefile.in:$(subdir)/Make-in \
	     CONFIG_HEADERS= $(SHELL) ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
