Compiling LPG Software with MSVC v6
Version 20090103
Copyright (C) 2005-2007 Kevin Kofler
Copyright (C) 2001-2009 Romain Lievin
Copyright (C) 2006 Tyler Cassidy

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

This is how to compile LPG software with MSVC. This bugger can be tricky so watch out. :)

These compiling instructions will work for:
- libticables2, libticonv, libtifiles2, libticables2
- TiLP 2
- TiEmu 3
- GFM
- SkinEdit

Follow the instructions very closely or you will soon see that monkeys will start biting. ;-)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------
| Retrieving sources                  |
---------------------------------------

There are two ways to retrieve source code:
- downloads tarballs from <http://sourceforge.net/project/showfiles.php?group_id=23169>
- uses SubVersion (svn for short) from <svn.tilp.info>

1) from tarballs: create a folder and extract all of your tarballs

2) from subversion: you will have to install svn-tools (linux) or a client (TortoiseSVN/Win32).
Take a look at <http://tortoisesvn.tigris.org> and <http://svnbook.red-bean.com>.
 
There is a specific tree to use for build:

	C:\LPG\
	|
	+- GTK
	+- SDL
	+- libusb-win32
	+- gfm
	+- skinedit
	+- tfdocgen
	+- ticables2
	+- ticalcs2
	+- ticonv
	+- tiemu3
	+- tifile2
	+- ticables2
	+- tilp2

Next, check-out the following URLs for each target folder: 
- svn co http://svn.tilp.info/repos/tilp/libticables/trunk ticables2 
- svn co http://svn.tilp.info/repos/tilp/libticalcs/trunk ticalcs2 
- svn co http://svn.tilp.info/repos/tilp/libtifiles/trunk tifiles2 
- svn co http://svn.tilp.info/repos/tilp/libticonv/trunk ticonv  
- svn co http://svn.tilp.info/repos/tilp/tilp/trunk tilp2
- svn co http://svn.tilp.info/repos/tilp/tiemu/trunk tiemu3
- svn co http://svn.tilp.info/repos/tilp/tfdocgen/trunk tfdocgen
- svn co http://svn.tilp.info/repos/tiemu/skinedit/trunk skinedit
- svn co http://svn.tilp.info/repos/tilp/gfm/trunk gfm

---------------------------------------
| Compiling on Windows                |
---------------------------------------

1) Have MSVC 6.0  properly installed

2) Download and install the GladeWin32 Development Packages from
   http://prdownloads.sf.net/gladewin32/gtk-win32-devel-2.6.10-rc1.exe and make sure you check the
   "Install MSVC environment variables" checkbox when the installer asks you, if you don't, the
   monkeys WILL bite :)
   
   Download and install the GladeWin32 Development Packages from
   http://prdownloads.sf.net/gladewin32/gtk-win32-devel-2.6.10-rc1.exe and make sure you check the
   "Install MSYS environment variables" checkbox when the installer asks you, if you don't, the
   monkeys WILL bite :)

3) Download and untar libusb-win32-device-bin from
   <http://sourceforge.net/project/showfiles.php?group_id=78138>
   
4) Download and unzip libSDL from <http://www.libsdl.org/release/SDL-devel-1.2.13-VC6.zip>.

5) Open an MSVC project (tilp2, tiemu3, ...) at c:\lpg\project\build\msvc and build.

Note for TiEmu: the MSVC project at "base_folder\tiemu\src\core\uae" has to be built before.

6) Object and executables files will be produced in "C:\lpg\msvc". The generated executables will
not run without some resource files. To copy them, you will find in c:\lpg\tilp or c:\lpg\tiemu
a script named install.bat.

Have fun :)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
