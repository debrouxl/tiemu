This is a new method, greatly simplified by the latest GTK+ package.

Win32
-----

1°) Below, you will find a methodology which is common to:
- libtifiles, libticonv, libticables, libticalcs
- tilp
- tiemu

You should strictly follow it else you will encounter some (very) weird stuffs !

2°) Well, let get's us:

- get MinGW & MSys from <http://www.mingw.org> and install them. You can merge both
folders if you want but this is strongly discouraged by MinGW team. This was my 
way until I discover Tor Lillqvist' method (GiMP for Win32) with a separate 
/target folder.
I assume you installed MinGW as C:\MinGW and MSys as C:\MSys (without 1.0 path).

- get <http://prdownloads.sourceforge.net/gladewin32/gtk-win32-devel-2.6.10-rc1.exe> 
and install it. Don't forget to register Msys environnement when the installer ask you.

- create a MSys\target folder. Compiled packages will be installed into.

- do an 'export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/target/lib/pkgconfig'
	or make changes in your MSys\etc\profile for example.

3°) To compile TiLP framework, TiLP and/or TiEmu, do the following:
	
- ticables, ticonv, tifiles, ticalcs:
	./configure --prefix=/target --disable-nls

- tilp:
	./configure --prefix=/target --disable-nls

- tiemu:
	./configure --prefix=/target --disable-nls

Tested with TiLP & TiEmu, 15/02/05.

---

Romain Liévin (The TiLP Team leader).

============================================================

Linux (cross)
-------------

TiEmu now supports Linux->MinGW cross-compilation again. Here's how I did it:
1. Installed the MinGW binutils, w32api and runtime RPMs from: http://mirzam.it.vu.nl/mingw/
2. Installed the MinGW GCC RPM from: http://bitwalk.hp.infoseek.co.jp/download.html
   (Make sure you get a package which includes g++. It is needed to build oleaut.cpp.)
3. Installed WINE from Fedora Extras.
4. Installed the last GTK+ 2.6 development environment (2.6.10-rc1) from http://gladewin32.sf.net in WINE.
   WARNING: DON'T build against GTK+ 2.8! GTK+ 2.8 uses Cairo, which doesn't work at all on Win9x/Me.
            Nobody seems interested in fixing that. We've learned this the hard way. You have been warned.
5. ln -s ~/.wine/c/GTK /target #(to make pkg-config happy)
6. In order to avoid a needless mingwm10.dll dependency, edited
   /usr/local/i386-mingw32/lib/libstdc++.la to remove -lmingwthrd.
7. Used the following commands to build TiEmu:
   source cross-mingw32-aio.sh #(needs to be done for EACH build!)
   export CFLAGS="-Os -s -fno-exceptions"
   export CXXFLAGS="-Os -s -fno-exceptions"
   cd libticables-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../libticonv-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../libtifiles-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../libticalcs-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../tiemu-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   i386-mingw32-strip ~/.wine/c/tiemu/bin/*
   NOTE: You'll have to substitute an absolute path for ~ to make configure happy.

If you followed the instructions and built against GTK+ 2.6, your executable
will also run under WINE. Here's how to get it to work:
1. Fired up WINE regedit and added ";c:\GTK\bin" to HKEY_CURRENT_USER\Environment\PATH.
2. Copied /usr/share/fonts/bitstream-vera/*.ttf to ~/.wine/c/windows/Fonts so
   GTK+ in WINE has fonts to work with.
3. Added "bitstream vera sans" to the aliases for "sans" and likewise for
   "serif", "monospace" and "courier" in ~/.wine/c/GTK/etc/pango/pango.aliases.
4. wine 'c:\tiemu\bin\tiemu.exe'
WARNING: DON'T use GTK+ 2.8 if you want to be able to test with WINE!

To build the setup wizard, here's what I did:
1. Fetched the latest NSIS source tarball from: http://nsis.sf.net
2. Installed the SCons development version from: http://scons.sf.net
3. Built NSIS:
   scons PREFIX=~/nsis SKIPSTUBS=all SKIPPLUGINS=all SKIPUTILS=all SKIPMISC=all
   scons PREFIX=~/nsis SKIPSTUBS=all SKIPPLUGINS=all SKIPUTILS=all SKIPMISC=all install
4. Fetched the corresponding NSIS ZIP file from: http://nsis.sf.net
5. Extracted some of the contents of the ZIP file into ~/nsis:
   Docs, Examples -> ~/nsis/share/doc/nsis
   Bin, Contrib, Include, Menu, Plugins, Stubs -> ~/nsis/share/nsis
6. Used this /usr/local/bin/makensis script (a symlink will NOT work, because
   makensis expects to be called with a full path!):
   #!/bin/sh
   ~/nsis/bin/makensis $*
7. Installed UPX from Fedora Extras.
8. Used the following commands to build the TiEmu setup wizard:
   cd build/nsis
   makensis tiemu-cross.nsi

Tested with TiEmu 2.81, 2006-07-23.

---

Kevin Kofler (TiEmu co-developer).