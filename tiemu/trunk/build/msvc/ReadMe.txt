This document covers the compilation of TiEmu with the MSVC compiler.

1) Requirements:

- MSVC 6.0 or later (not tested with MSVC 2003 yet)

- Gtk+/Win32 Development Environment Installer 2.6.4-rc2 (runtime, devel, docs, glade, etc.):
		Get it at <http://prdownloads.sourceforge.net/gladewin32/gtk-win32-devel-2.6.4-rc2.exe?download>
		or at <http://gladewin32.sourceforge.net/>
		
- the TiLP framework:
	  Get it at <http://lpg.ticalc.org/prj_tilp> or at <hhttp://www.ticalc.org/archives/files/fileinfo/337/33748.html>
	  If you have SVN, you can do a checkout from <http://svn.technologeek.org/cgi-bin/viewcvs.cgi/?root=tilp>

2) Before compiling anything, you will need to set up a directory layout like this:
- run the GTK installer and set installation folder to "C:\GTK2DEV"
- uncompress or checkout libs in a base folder (I'm using "C:\sources\roms") like this:
		base_folder\tifiles
		base_folder\ticables
	  base_folder\ticalcs
- do the same with TiEmu:
	  base_folder\tiemu

3) Open the MSVC project at "base_folder\tiemu\src\core\uae" and build the uae project.
Next, open the TiEmu project and the build.

4) Resulting files will be produced in "C:\msvc\tilp" and not in "C:\msvc\tiemu".
I use this convenient location because libs are used primarily by TiLP and placed here.

Before running TiEmu, you will need to copy the following folders:
- glade
- help
- pedrom
- pixmaps
- skins

---
Romain Liévin
