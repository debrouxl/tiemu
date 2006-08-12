-Compiling LPG Software with MinGW
-Version 1.99 20060812
- (C) Copyright 2005 - 2006 Kevin Kofler
- (C) Copyright 2001 - 2006 Romain Lievin
- (C) Copyright 2006 Tyler Cassidy
- tilp-users@lists.sf.net

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

This is how you compile LPG software with MinGW. This bugger can be tricky so watch out :)

This will compile for:
-libticables2, libticonv, libtifiles2, libticables2
-TiLP-II
-TiEmu
-GFM

Follow the instructions very closely or you will soon see that Monkeys will start biting ;-)

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------
| Compiling on Windows                |
---------------------------------------

1. Download the following stuff, but do not install it yet!
-- MinGW and MSYS from <http://www.mingw.org/>
-- Bison Binaries, Dependancies and Developer Files from <http://gnuwin32.sf.net/packages/bison.htm>
-- Flex Binaries, Dependancies and Developer Files from <http://gnuwin32.sf.net/packages/flex.htm>

2. Extract the MinGW stuff into C:/mingw and install MSYS into C:/msys, not C:/msys/1.0

3. Extract all the Bison and Flex Archives into the C:/mingw directory

3. Download and Install the GladeWin32 Development Packages from <http://prdownloads.sf.net/gladewin32/gtk-win32-devel-2.6.10-rc1.exe> and make sure you check the install MSYS environment variable checkbox when the installer asks you, if you dont the Monkeys WILL bite :)

4. Create a C:/msys/target folder. This is where the compiles stuff will be placed.

5. Edit the C:/msys/etc/profile file and add the following on a new line at the bottom:
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/target/lib/pkgconfig

6. Reboot MSYS if you done step 5 while MSYS was running.

7. Now lets compile, open up MSYS if you already have not. Make sure all of your source is extracted and ready to compile. With your basic Linux Skills, go into the following directory and type the following:

-Go into libticables2 directory
-Run:
./configure --prefix=/target --disable-nls
make
make install

8. Repeat Step 7 for the additional libti* libraries in the FOLLOWING order. libticonv, libtifiles2, libticalcs2.

9. Ok Now all you have to do is repeat step 7 for TiLP-II, TiEmu, or GFM, or all of them if you so wish!

10. You files are now all located in /target. If it does not work, an instruction may have been missed or performed wrong, or the Source Code is broken.

Have fun :)

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


---------------------------------------
| Cross Compiling on Linux            |
---------------------------------------

1. Download and Install the following RPMs from <http://mirzam.it.vu.nl/mingw/>:
-MinGW binutils
-MinGW w32api
-MinGW runtime

2. Download and Install the MinGW GCC RPM from <http://bitwalk.hp.infoseek.co.jp/download.html>.
   (Make sure you get the package with g++ included, It is needed to build the DCOP server)

3. Installed WINE.
   (Can retrieve with yum on Fedora, "yum install wine")

4. Installed the last GTK+ 2.6 development package (version 2.6.10-rc1) from <http://gladewin32.sf.net> in WINE.
   (WARNING: Do not build against GTK+ 2.8 on Windows, Windows ME/98 Users will have issues running software built agaisnt GTK+ 2.8)

5. Ran the following command:
   ln -s ~/.wine/c/GTK /target
   (This makes pkg-config happy, you can replace ../c/.. with whatever directory/drive you wish to use)

6. Edited the following file, /usr/local/i386-mingw32/lib/libstdc++.la and removed -lmingwthrd from the file.

7. Edit cross-mingw32-gtkaio.sh to your systems needs.

8. Used the following commands to build each of the following packages:
   source cross-mingw32-gtkaio.sh # Needs to be done for each BUILD!!!!!!
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
   
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------
| Running your software on WINE       |
---------------------------------------

If you have built your program against GTK+ 2.6, it will run in WINE.

1. Open WINE regedit and add ";c:\GTK\bin" to HKEY_CURRENT_USER\Environment\PATH.

2. Copied all the .ttf files from /usr/share/fonts/bitstream-vera/ to ~/.wine/c/windows/Fonts.

3. Added "bitstream vera sans" to the aliases for "sans" and likewise for
   "serif", "monospace" and "courier" in ~/.wine/c/GTK/etc/pango/pango.aliases.
   
4. Ran:
   wine 'c:\tiemu\bin\tiemu.exe'
   
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------
| Building an Installer               |
---------------------------------------

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