man_MANS = tiemu.1

EXTRA_DIST = $(man_MANS) cleaner.c manpage.txt

view: $(man_MANS)
	groff -Tascii -man $(man_MANS) | less

dist_win: $(man_MANS)
	groff -Tascii -man $(man_MANS) > manpage
	gcc cleaner.c -o cleaner
	./cleaner manpage
	dos2unix manpage.txt
