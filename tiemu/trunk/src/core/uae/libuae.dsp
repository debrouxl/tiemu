# Microsoft Developer Studio Project File - Name="libuae" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libuae - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libuae.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libuae.mak" CFG="libuae - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libuae - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libuae - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libuae - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libuae - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"libuae.lib"

!ENDIF 

# Begin Target

# Name "libuae - Win32 Release"
# Name "libuae - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cpu0.c
# End Source File
# Begin Source File

SOURCE=.\cpu1.c
# End Source File
# Begin Source File

SOURCE=.\cpu2.c
# End Source File
# Begin Source File

SOURCE=.\cpu3.c
# End Source File
# Begin Source File

SOURCE=.\cpu4.c
# End Source File
# Begin Source File

SOURCE=.\cpu5.c
# End Source File
# Begin Source File

SOURCE=.\cpu6.c
# End Source File
# Begin Source File

SOURCE=.\cpu7.c
# End Source File
# Begin Source File

SOURCE=.\cpu8.c
# End Source File
# Begin Source File

SOURCE=.\cpu9.c
# End Source File
# Begin Source File

SOURCE=.\cpuA.c
# End Source File
# Begin Source File

SOURCE=.\cpuB.c
# End Source File
# Begin Source File

SOURCE=.\cpuC.c
# End Source File
# Begin Source File

SOURCE=.\cpuD.c
# End Source File
# Begin Source File

SOURCE=.\cpudefs.c
# End Source File
# Begin Source File

SOURCE=.\cpuE.c
# End Source File
# Begin Source File

SOURCE=.\cpuF.c
# End Source File
# Begin Source File

SOURCE=.\cpustbl.c
# End Source File
# Begin Source File

SOURCE=.\cputbl.c
# End Source File
# Begin Source File

SOURCE=.\readcpu.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cputbl.h
# End Source File
# Begin Source File

SOURCE=.\memory.h
# End Source File
# Begin Source File

SOURCE=.\newcpu.h
# End Source File
# Begin Source File

SOURCE=.\options.h
# End Source File
# Begin Source File

SOURCE=.\readcpu.h
# End Source File
# Begin Source File

SOURCE=.\sysconfig.h
# End Source File
# Begin Source File

SOURCE=.\sysdeps.h
# End Source File
# Begin Source File

SOURCE=.\uae.h
# End Source File
# Begin Source File

SOURCE=.\uconfig.h
# End Source File
# End Group
# End Target
# End Project
