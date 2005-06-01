# Microsoft Developer Studio Project File - Name="TiEmu_Windows" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TiEmu_Windows - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu_Windows.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu_Windows.mak" CFG="TiEmu_Windows - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TiEmu_Windows - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TiEmu_Windows - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TiEmu_Windows - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "C:\msvc\Output\tiemu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "C:\sources\roms\tiemu\src\misc" /I "C:\sources\roms\tiemu\src\gui\calc" /I "C:\sources\roms\tiemu\src\core\dbg" /I "C:\sources\roms\tiemu\src\core\ti_sw" /I "C:\sources\roms\tiemu\src\gui\debugger" /I "." /I "../../../ticables/src" /I "../../../tifiles/src" /I "../../../ticalcs/src" /I "../../src/win32" /I "C:\GTK2DEV\INCLUDE\GTK-2.0" /I "C:\GTK2DEV\LIB\GTK-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\GLIB-2.0" /I "C:\GTK2DEV\LIB\GLIB-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\PANGO-1.0" /I "C:\GTK2DEV\INCLUDE\ATK-1.0" /I "c:\gtk2dev\include" /I "C:\Gtk2Dev\include\libglade-2.0" /I "C:\sources\roms\tiemu\src\core\uae" /I "C:\sources\roms\tiemu\src\core" /I "C:\sources\roms\tiemu\src\core\ti_hw" /I "C:\sources\roms\tiemu\src\hid" /I "C:\sources\roms\tiemu\src\gui" /I ".\libjpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "GTK_DISABLE_DEPRECATED" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"tiemu.exe"

!ELSEIF  "$(CFG)" == "TiEmu_Windows - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\msvc\tilp"
# PROP Intermediate_Dir "C:\msvc\Output\tiemu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /GX /ZI /Od /I "C:\sources\roms\tiemu\src\misc" /I "C:\sources\roms\tiemu\src\gui\calc" /I "C:\sources\roms\tiemu\src\core\dbg" /I "C:\sources\roms\tiemu\src\core\ti_sw" /I "C:\sources\roms\tiemu\src\gui\debugger" /I "." /I "../../../ticables/src" /I "../../../tifiles/src" /I "../../../ticalcs/src" /I "../../src/win32" /I "C:\GTK2DEV\INCLUDE\GTK-2.0" /I "C:\GTK2DEV\LIB\GTK-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\GLIB-2.0" /I "C:\GTK2DEV\LIB\GLIB-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\PANGO-1.0" /I "C:\GTK2DEV\INCLUDE\ATK-1.0" /I "c:\gtk2dev\include" /I "C:\Gtk2Dev\include\libglade-2.0" /I "C:\sources\roms\tiemu\src\core\uae" /I "C:\sources\roms\tiemu\src\core" /I "C:\sources\roms\tiemu\src\core\ti_hw" /I "C:\sources\roms\tiemu\src\hid" /I "C:\sources\roms\tiemu\src\gui" /I ".\libjpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "GTK_DISABLE_DEPRECATED" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\msvc\tilp\TiEmu.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TiEmu_Windows - Win32 Release"
# Name "TiEmu_Windows - Win32 Debug"
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ChangeLog
# End Source File
# Begin Source File

SOURCE=..\..\README
# End Source File
# Begin Source File

SOURCE=..\..\RELEASE
# End Source File
# Begin Source File

SOURCE=..\..\TODO
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\glib-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gdk-win32-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gtk-win32-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gobject-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gdk_pixbuf-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gthread-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gmodule-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\pango-1.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\atk-1.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\GTK2DEV\lib\glade-2.0.lib"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TiEmu.ico
# End Source File
# Begin Source File

SOURCE=.\TiEMu.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\uae\memory.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\newcpu.h
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\version.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "uae"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\uae\newcpu.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\table68k
# End Source File
# End Group
# Begin Group "ti_hw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\ti_hw\dbus.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\flash.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\gscales.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\hw.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\hwprot.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\kbd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\m68k.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\mem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\mem89.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\mem89tm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\mem92.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\mem92p.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\memv2.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\ports.c
# End Source File
# End Group
# Begin Group "dbg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\dbg\bkpts.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\dbg\debug.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\dbg\disasm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\dbg\fpudasm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\dbg\m68kdasm.c
# End Source File
# End Group
# Begin Group "ti_sw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\ti_sw\er_codes.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_sw\handles.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\hwpm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_sw\registers.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_sw\romcalls.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_sw\timem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_sw\vat.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\libuae.lib
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\core\error.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\images.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\interface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\state.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\type2str.c
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Group "debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_all.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_bkpts.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_cause.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_code.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_data.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_entry.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_heap.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_pclog.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_regs.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_romcall.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_stack.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_vectors.c
# End Source File
# End Group
# Begin Group "calc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\calc\calc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\kbd_mapper.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\keyboard.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\keynames.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\popup.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\screen.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\skinkeys.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\skinops.c
# End Source File
# End Group
# Begin Group "dlgbox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\about.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\comm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\dboxes.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\filesel.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\fs_misc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\infos.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\manpage.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\pbars.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\refresh.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\release.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\romversion.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\scroptions.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\splash.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\support.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\wizard.c
# End Source File
# End Group
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\misc\cmdline.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\files.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\paths.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\printl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\rcfile.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\screenshot.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\tie_error.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\gui\engine.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\main.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\tsource.c
# End Source File
# End Group
# End Target
# End Project
