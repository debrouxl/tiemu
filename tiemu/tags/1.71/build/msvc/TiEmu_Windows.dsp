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
# PROP Intermediate_Dir "E:\msvc\Output\gtktiemu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /w /W0 /GX /O2 /I "..\..\ti_libs\cables\src" /I "..\..\ti_libs\calcs\src" /I "..\..\ti_libs\libti68k\src\\" /I "..\dep\win32" /D "NDEBUG" /D "HAVE_LIBJPEG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "PENT_COUNTER" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gnu-intl.lib glib-1.3.lib gdk-1.3.lib gtk-1.3.lib gmodule-1.3.lib /nologo /subsystem:windows /machine:I386 /out:"Test\GtkTiEmu.exe"

!ELSEIF  "$(CFG)" == "TiEmu_Windows - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\msvc\tiemu"
# PROP Intermediate_Dir "C:\msvc\Output\tiemu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /GX /ZI /Od /I "../../ti_libs/cables/src/" /I "../../ti_libs/calcs/src/" /I "D:\devel\tiemu_project\tiemu\src\core" /I "D:\devel\tiemu_project\tiemu\src\core\uae" /I "D:\devel\tiemu_project\tiemu\src\core\ti_hw" /I "D:\devel\tiemu_project\tiemu\src\hid" /I "D:\devel\tiemu_project\tiemu\src\gui\gtk" /I "D:\devel\tiemu_project\tiemu\src\\" /I "D:\devel\tiemu_project\SDL-1.2.0\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "PENT_COUNTER" /D "HAVE_LIBJPEG" /FD /GZ /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gnu-intl.lib glib-1.3.lib gdk-1.3.lib gtk-1.3.lib gmodule-1.3.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\msvc\tiemu\TiEmu.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TiEmu_Windows - Win32 Release"
# Name "TiEmu_Windows - Win32 Debug"
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=C:\msvc\tilp\libticables.lib
# End Source File
# Begin Source File

SOURCE=C:\msvc\tilp\libticalcs.lib
# End Source File
# Begin Source File

SOURCE="..\..\SDL-1.2.4\lib\SDL.lib"
# End Source File
# Begin Source File

SOURCE="..\..\SDL-1.2.4\lib\SDLmain.lib"
# End Source File
# Begin Source File

SOURCE=.\jpeg.lib
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TiEMu.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "ti_hw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\ti_hw\debugger.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\hardware.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\ioports.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\keyboard.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\lcd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\linkport.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\m68k.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\memory.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\timer.c
# End Source File
# End Group
# Begin Group "uae"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\uae\cpu0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu1.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu2.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu4.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu5.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu6.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu7.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu8.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu9.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuC.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuD.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpudefs.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuE.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuF.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpustbl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\newcpu.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\readcpu.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\core\bkpts.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\callbacks.c
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

SOURCE=..\..\src\core\intlist.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\state.c
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Group "gtk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\gtk\about_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\about_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\adbkpt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\adbkpt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\bkpt_cause.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\colors_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\colors_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\dbgopt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\dbgopt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\dbkpt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\dbkpt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\debug_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\debug_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\dump_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\dump_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\fsel_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\fsel_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\gxpm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\help_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\help_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\infos_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\infos_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\itick_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\itick_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\link_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\link_cb.c.old
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\link_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\logaddr_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\logaddr_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\main.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\main_wnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\manpage_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\manpage_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\msg_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\msg_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\pbar_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\pbar_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\pgmentbkpt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\pgmentbkpt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\refresh.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\romv_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\romv_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\scopt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\scopt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\skin_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\skin_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\startup_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\startup_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\state_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\state_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\support.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\thanks_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\thanks_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\tiemu_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\tiemu_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\trap_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\trap_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\user_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\user_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\viewlog_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\viewlog_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\wizard_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\gtk\wizard_dbox.c
# End Source File
# End Group
# End Group
# Begin Group "hid"

# PROP Default_Filter ""
# Begin Group "img"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\hid\img\bmp.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\img\cnv.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\img\jpg.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\img\pcx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\img\xpm.c
# End Source File
# End Group
# Begin Group "skn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\hid\skn\skin.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\hid\files.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\misc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\paths.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\rcfile.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\ticalc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\tikeys.c
# End Source File
# End Group
# End Group
# End Target
# End Project
