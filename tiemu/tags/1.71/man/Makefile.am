man_MANS = tiemu.1
pkgdata_DATA = Manpage.txt

EXTRA_DIST = $(man_MANS) cleaner.c Manpage.txt #dos2unix.c

all: dist_win

view: $(man_MANS)
	groff -Tascii -man $(man_MANS) | less

dist_win: $(man_MANS)
	groff -Tascii -man $(man_MANS) > Manpage
	gcc cleaner.c -o cleaner
#       gcc dos2unix.c -o dos2unix
	./cleaner Manpage
#       ./dos2unix Manpage.txt
	rm Manpage cleaner
