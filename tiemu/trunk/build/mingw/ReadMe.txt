This is a new method, greatly simplified by the latest GTK+ package.

Win32
-----

1°) Below, you will find a methodology which is common to:
- libtifiles, libticables, libticalcs
- tilp
- tiemu

You should strictly follow it else you will encounter some (very) weird stuffs !

2°) Well, let get's us:

- get MinGW & MSys from <http://www.mingw.org> and install them. You can merge both
folders if you want but this is strongly discouraged by MinGW team. This was my 
way until I discover Tor Lillqvist' method (GiMP for Win32) with a separate 
/target folder.
I assume you installed MinGW as C:\MinGW and MSys as C:\MSys (without 1.0 path).

- get <http://prdownloads.sourceforge.net/gladewin32/gtk-win32-devel-2.4-rc26.exe> 
and install it. Don't forget to register Msys environnement when the installer ask you.

- create a MSys\target folder. Compiled packages will be installed into.

- do an 'export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/target/lib/pkgconfig'
	or make changes in your MSys\etc\profile for example.

3°) To compile TiLP framework, TiLP and/or TiEmu, do the following:
	
- ticables, tifiles, ticalcs:
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
1. Installed the MinGW RPMs from: http://mirzam.it.vu.nl/mingw/
2. Installed WINE from Fedora Extras.
3. Installed the latest GTK+ development environment from http://gladewin32.sf.net in WINE.
4. ln -s ~/.wine/c/GTK /target #(to make pkg-config happy)
5. Used the following commands to build TiEmu:
   source cross-mingw32-aio.sh #(needs to be done for EACH build!)
   export CFLAGS="-Os -s -fno-exceptions"
   cd libticables2-mingw-build
   LIBUSB_CONFIG=no ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../libtifiles2-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../libticalcs2-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   cd ../tiemu-mingw-build
   ./configure --prefix=~/.wine/c/tiemu --disable-nls --host=i386-mingw32 --build=i686-redhat-linux-gnu
   make
   make install
   #NOTE: The LIBUSB_CONFIG=no is because configure picks up the libusb from the
   #      build host (which the linker then chokes on) if I don't do that.

Unfortunately, it does NOT seem to work in WINE. Here's the instructions getting
it up to the wizard, but showing way too large fonts and then crashing:
1. Fired up WINE regedit and added ";c:\GTK\bin" to HKEY_CURRENT_USER\Environment\PATH.
2. Copied /usr/share/fonts/bitstream-vera/*.ttf to ~/.wine/c/windows/Fonts so
   GTK+ in WINE has fonts to work with.
3. Added "bitstream vera sans" to the aliases for "sans" and likewise for
   "serif" and "monospace" in ~/.wine/c/GTK/etc/pango/pango.aliases.
4. wine 'c:\tiemu\bin\tiemu.exe'
So you can't count on WINE for testing.

To build the setup wizard, here's what I did:
1. Fetched the latest NSIS source tarball from: http://nsis.sf.net
2. Installed the SCons development version from: http://scons.sf.net
3. Built NSIS:
   scons PREFIX=~/nsis SKIPSTUBS=all SKIPPLUGINS=all SKIPUTILS=all SKIPMISC=all
   scons PREFIX=~/nsis SKIPSTUBS=all SKIPPLUGINS=all SKIPUTILS=all SKIPMISC=all install
4. Fetched the corresponding NSIS ZIP file from: http://nsis.sf.net
5. Extracted everything except the root directory (which contains only Windows
   executables) and the files which were already there from the ZIP into ~/nsis.
6. Used this /usr/local/bin/makensis script (a symlink will NOT work, because
   makensis expects to be called with a full path!):
   #!/bin/sh
   ~/nsis/makensis $*
7. Installed UPX from Fedora Extras.
8. Used the following commands to build the TiEmu setup wizard:
   cd build/nsis
   makensis tiemu-cross.nsi

Tested with TiEmu 2.80, 2006-01-07.

---

Kevin Kofler (TiEmu co-developer).