# Microsoft Developer Studio Generated NMAKE File, Based on SkinEdit_Windows.dsp
!IF "$(CFG)" == ""
CFG=SkinEdit_Windows - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SkinEdit_Windows - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SkinEdit_Windows - Win32 Release" && "$(CFG)" != "SkinEdit_Windows - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SkinEdit_Windows.mak" CFG="SkinEdit_Windows - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SkinEdit_Windows - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SkinEdit_Windows - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SkinEdit_Windows - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\SkinEdit.exe"


CLEAN :
	-@erase "$(INTDIR)\boxes_cb.obj"
	-@erase "$(INTDIR)\boxes_intf.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main_cb.obj"
	-@erase "$(INTDIR)\main_intf.obj"
	-@erase "$(INTDIR)\prop_cb.obj"
	-@erase "$(INTDIR)\prop_intf.obj"
	-@erase "$(INTDIR)\rubberbox.obj"
	-@erase "$(INTDIR)\skinedit.res"
	-@erase "$(INTDIR)\skinops.obj"
	-@erase "$(INTDIR)\support.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vti_export.obj"
	-@erase ".\SkinEdit.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W2 /GX /O2 /I "..\..\build\msvc" /I "C:\GTK2DEV\INCLUDE\GTK-2.0" /I "C:\GTK2DEV\LIB\GTK-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\GLIB-2.0" /I "C:\GTK2DEV\LIB\GLIB-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\PANGO-1.0" /I "C:\GTK2DEV\INCLUDE\ATK-1.0" /I "c:\gtk2dev\include" /I "C:\Gtk2Dev\include\libglade-2.0" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /Fp"$(INTDIR)\SkinEdit_Windows.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\skinedit.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SkinEdit_Windows.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SkinEdit.pdb" /machine:I386 /out:"SkinEdit.exe" 
LINK32_OBJS= \
	"$(INTDIR)\boxes_cb.obj" \
	"$(INTDIR)\boxes_intf.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\main_cb.obj" \
	"$(INTDIR)\main_intf.obj" \
	"$(INTDIR)\prop_cb.obj" \
	"$(INTDIR)\prop_intf.obj" \
	"$(INTDIR)\rubberbox.obj" \
	"$(INTDIR)\skinops.obj" \
	"$(INTDIR)\support.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\vti_export.obj" \
	"$(INTDIR)\skinedit.res" \
	"..\..\..\..\..\Gtk2Dev\lib\glib-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gdk-win32-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gtk-win32-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gobject-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gdk_pixbuf-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gthread-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gmodule-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\pango-1.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\libglade-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\atk-1.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\intl.lib"

".\SkinEdit.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SkinEdit_Windows - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SkinEdit.exe"


CLEAN :
	-@erase "$(INTDIR)\boxes_cb.obj"
	-@erase "$(INTDIR)\boxes_intf.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main_cb.obj"
	-@erase "$(INTDIR)\main_intf.obj"
	-@erase "$(INTDIR)\prop_cb.obj"
	-@erase "$(INTDIR)\prop_intf.obj"
	-@erase "$(INTDIR)\rubberbox.obj"
	-@erase "$(INTDIR)\skinedit.res"
	-@erase "$(INTDIR)\skinops.obj"
	-@erase "$(INTDIR)\support.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vti_export.obj"
	-@erase "$(OUTDIR)\SkinEdit.exe"
	-@erase "$(OUTDIR)\SkinEdit.ilk"
	-@erase "$(OUTDIR)\SkinEdit.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W2 /Gm /GX /ZI /Od /I "..\..\build\msvc" /I "C:\SDL-1.2.7\include" /I "C:\GTK2DEV\INCLUDE\GTK-2.0" /I "C:\GTK2DEV\LIB\GTK-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\GLIB-2.0" /I "C:\GTK2DEV\LIB\GLIB-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\PANGO-1.0" /I "C:\GTK2DEV\INCLUDE\ATK-1.0" /I "c:\gtk2dev\include" /I "C:\Gtk2Dev\include\libglade-2.0" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "ENABLE_NLS" /Fp"$(INTDIR)\SkinEdit_Windows.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\skinedit.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SkinEdit_Windows.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SkinEdit.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SkinEdit.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\boxes_cb.obj" \
	"$(INTDIR)\boxes_intf.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\main_cb.obj" \
	"$(INTDIR)\main_intf.obj" \
	"$(INTDIR)\prop_cb.obj" \
	"$(INTDIR)\prop_intf.obj" \
	"$(INTDIR)\rubberbox.obj" \
	"$(INTDIR)\skinops.obj" \
	"$(INTDIR)\support.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\vti_export.obj" \
	"$(INTDIR)\skinedit.res" \
	"..\..\..\..\..\Gtk2Dev\lib\glib-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gdk-win32-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gtk-win32-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gobject-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gdk_pixbuf-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gthread-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\gmodule-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\pango-1.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\libglade-2.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\atk-1.0.lib" \
	"..\..\..\..\..\Gtk2Dev\lib\intl.lib"

"$(OUTDIR)\SkinEdit.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SkinEdit_Windows.dep")
!INCLUDE "SkinEdit_Windows.dep"
!ELSE 
!MESSAGE Warning: cannot find "SkinEdit_Windows.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SkinEdit_Windows - Win32 Release" || "$(CFG)" == "SkinEdit_Windows - Win32 Debug"
SOURCE=..\..\src\boxes_cb.c

"$(INTDIR)\boxes_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\boxes_intf.c

"$(INTDIR)\boxes_intf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\main.c

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\main_cb.c

"$(INTDIR)\main_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\main_intf.c

"$(INTDIR)\main_intf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\prop_cb.c

"$(INTDIR)\prop_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\prop_intf.c

"$(INTDIR)\prop_intf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\rubberbox.c

"$(INTDIR)\rubberbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\skinops.c

"$(INTDIR)\skinops.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\support.c

"$(INTDIR)\support.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\utils.c

"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\vti_export.c

"$(INTDIR)\vti_export.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\skinedit.rc

"$(INTDIR)\skinedit.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

