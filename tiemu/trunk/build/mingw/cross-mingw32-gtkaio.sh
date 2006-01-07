# This script sets up variables for cross-compilation of GTK+ programs to MinGW.
# It assumes a GTK+ development environment installation from gladewin32.sf.net in WINE.
# You'll also need a: ln -s ~/.wine/c/GTK /target
export CC=i386-mingw32-gcc
export RC=i386-mingw32-windres
export GTK_BASEPATH=~/.wine/c/GTK
export PATH=$GTK_BASEPATH/bin:$PATH
export ACLOCAL_FLAGS="-I $GTK_BASEPATH/share/aclocal $ACLOCAL_FLAGS"
export C_INCLUDE_PATH=$GTK_BASEPATH/include:/usr/local/i386-mingw32/include
export LIBRARY_PATH=$GTK_BASEPATH/lib:/usr/local/i386-mingw32/lib/w32api:/usr/local/i386-mingw32/lib
export PKG_CONFIG_PATH=/usr/local/i386-mingw32/lib/pkgconfig:$GTK_BASEPATH/lib/pkgconfig:~/.wine/c/tiemu/lib/pkgconfig

