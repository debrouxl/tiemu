man_MANS = skinedit.1
pkgdata_DATA = Manpage.txt

EXTRA_DIST = $(man_MANS) cleaner.c Manpage.txt

view: $(man_MANS)
	groff -Tascii -man $(man_MANS) | less

dist_win: $(man_MANS)
	groff -Tascii -man $(man_MANS) > Manpage
	gcc cleaner.c -o cleaner
	./cleaner Manpage
	rm Manpage cleaner
