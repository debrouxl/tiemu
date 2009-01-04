# Microsoft Developer Studio Project File - Name="TiEmu3_Windows" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TiEmu3_Windows - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu3_Windows.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu3_Windows.mak" CFG="TiEmu3_Windows - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TiEmu3_Windows - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TiEmu3_Windows - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TiEmu3_Windows - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "C:\lpg\msvc\Output\tiemu3"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "c:\lpg\\tiemu3\src\com_ole" /I "c:\lpg\\tiemu3\src\ipc\com" /I "c:\lpg\\tiemu3\src\misc" /I "c:\lpg\\tiemu3\src\gui\calc" /I "c:\lpg\\tiemu3\src\core\dbg" /I "c:\lpg\\tiemu3\src\core\ti_sw" /I "c:\lpg\\tiemu3\src\gui\debugger" /I "." /I "../../../ticables2/src" /I "../../../tifiles2/src" /I "../../../ticalcs2/src" /I "../../../ticonv/src" /I "../../src/win32" /I "C:\LPG\GTK\INCLUDE\GTK-2.0" /I "C:\LPG\GTK\LIB\GTK-2.0\INCLUDE" /I "C:\LPG\GTK\INCLUDE\GLIB-2.0" /I "C:\LPG\GTK\LIB\GLIB-2.0\INCLUDE" /I "C:\LPG\GTK\INCLUDE\PANGO-1.0" /I "C:\LPG\GTK\INCLUDE\ATK-1.0" /I "C:\LPG\GTK\include" /I "C:\LPG\GTK\include\libglade-2.0" /I "c:\lpg\\tiemu3\src\core\uae" /I "c:\lpg\\tiemu3\src\core" /I "c:\lpg\\tiemu3\src\core\ti_hw" /I "c:\lpg\\tiemu3\src\hid" /I "c:\lpg\\tiemu3\src\gui" /I ".\libjpeg" /I "c:\lpg\\tiemu3\src\sound" /I "c:\lpg\sdl\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "GTK_DISABLE_DEPRECATED" /D "NO_GDB" /D "ENABLE_NLS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib   /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib   /nologo /subsystem:windows /machine:I386 /out:"tiemu.exe"

!ELSEIF  "$(CFG)" == "TiEmu3_Windows - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\lpg\msvc"
# PROP Intermediate_Dir "C:\lpg\msvc\Output\tiemu3"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /w /W0 /GX /ZI /Od /I "C:\LPG\GTK\include\cairo" /I "c:\lpg\\tiemu3\src\com_ole" /I "c:\lpg\\tiemu3\src\ipc\com" /I "c:\lpg\\tiemu3\src\misc" /I "c:\lpg\\tiemu3\src\gui\calc" /I "c:\lpg\\tiemu3\src\core\dbg" /I "c:\lpg\\tiemu3\src\core\ti_sw" /I "c:\lpg\\tiemu3\src\gui\debugger" /I "." /I "../../../ticables2/src" /I "../../../tifiles2/src" /I "../../../ticalcs2/src" /I "../../../ticonv/src" /I "../../src/win32" /I "C:\LPG\GTK\INCLUDE\GTK-2.0" /I "C:\LPG\GTK\LIB\GTK-2.0\INCLUDE" /I "C:\LPG\GTK\INCLUDE\GLIB-2.0" /I "C:\LPG\GTK\LIB\GLIB-2.0\INCLUDE" /I "C:\LPG\GTK\INCLUDE\PANGO-1.0" /I "C:\LPG\GTK\INCLUDE\ATK-1.0" /I "C:\LPG\GTK\include" /I "C:\LPG\GTK\include\libglade-2.0" /I "c:\lpg\\tiemu3\src\core\uae" /I "c:\lpg\\tiemu3\src\core" /I "c:\lpg\\tiemu3\src\core\ti_hw" /I "c:\lpg\\tiemu3\src\hid" /I "c:\lpg\\tiemu3\src\gui" /I ".\libjpeg" /I "c:\lpg\\tiemu3\src\sound" /I "c:\lpg\sdl\include" /D "_DEBUG" /D "ENABLE_NLS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "GTK_DISABLE_DEPRECATED" /D "NO_GDB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib   /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib   /nologo /subsystem:windows /debug /machine:I386 /out:"c:\lpg\msvc\tiemu.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TiEmu3_Windows - Win32 Release"
# Name "TiEmu3_Windows - Win32 Debug"
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BUGS
# End Source File
# Begin Source File

SOURCE=..\..\ChangeLog
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

SOURCE="C:\LPG\GTK\lib\glib-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\gdk-win32-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\gtk-win32-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\gobject-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\gdk_pixbuf-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\gthread-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\gmodule-2.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\pango-1.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\atk-1.0.lib"
# End Source File
# Begin Source File

SOURCE="C:\LPG\GTK\lib\glade-2.0.lib"
# End Source File
# Begin Source File

SOURCE=C:\LPG\GTK\lib\intl.lib
# End Source File
# Begin Source File

SOURCE=C:\LPG\GTK\lib\z.lib
# End Source File
# Begin Source File

SOURCE="c:\lpg\sdl\lib\SDL.lib"
# End Source File
# Begin Source File

SOURCE="c:\lpg\sdl\lib\SDLmain.lib"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tiemu.ico
# End Source File
# Begin Source File

SOURCE=..\mingw\tiemu.rc
# End Source File
# Begin Source File

SOURCE=.\TiEmu3.ico
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
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
# Begin Source File

SOURCE=..\..\src\core\ti_hw\rtc_hw3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\tichars.c
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

SOURCE=..\..\src\core\ti_sw\iodefs.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_sw\mem_map.c
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

SOURCE=..\..\src\core\engine.c
# End Source File
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

SOURCE=..\..\src\gui\debugger\dbg_bits.c
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

SOURCE=..\..\src\gui\debugger\dbg_dock.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_entry.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_heap.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_iop.c
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
# Begin Source File

SOURCE=..\..\src\gui\debugger\dbg_wnds.c
# End Source File
# End Group
# Begin Group "calc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\calc\calc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\dnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\keyboard.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\keymap.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\keynames.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\calc\keypress.c
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

SOURCE=..\..\src\gui\dboxes.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\device.c
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

SOURCE=..\..\src\gui\quicksend.c
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
# Begin Group "logger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\logger\log_link.c
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

SOURCE=..\..\src\gdb\libiberty\gettimeofday.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\logging.c
# End Source File
# Begin Source File

SOURCE=..\..\src\misc\paths.c
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
# Begin Group "ipc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\ipc\com\oleaut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ipc\com\oleaut_i.c
# End Source File
# Begin Source File

SOURCE=..\..\src\ipc\com\registry.cpp
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\sound\audio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sound\stream.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\gui\main.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\tsource.c
# End Source File
# End Group
# End Target
# End Project
