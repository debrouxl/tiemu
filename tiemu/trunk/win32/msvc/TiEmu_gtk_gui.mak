# Microsoft Developer Studio Generated NMAKE File, Based on TiEmu_gtk_gui.dsp
!IF "$(CFG)" == ""
CFG=TiEmu_gtk_gui - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TiEmu_gtk_gui - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TiEmu_gtk_gui - Win32 Release" && "$(CFG)" != "TiEmu_gtk_gui - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu_gtk_gui.mak" CFG="TiEmu_gtk_gui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TiEmu_gtk_gui - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TiEmu_gtk_gui - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Release"

OUTDIR=.
INTDIR=E:\msvc\Output\gtktiemu
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Test\GtkTiEmu.exe"

!ELSE 

ALL : "ti_calcs - Win32 Release" "ti_cables - Win32 Release" "lib68k - Win32 Release" "$(OUTDIR)\Test\GtkTiEmu.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"lib68k - Win32 ReleaseCLEAN" "ti_cables - Win32 ReleaseCLEAN" "ti_calcs - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Files.obj"
	-@erase "$(INTDIR)\gtk_about_cb.obj"
	-@erase "$(INTDIR)\gtk_about_dbox.obj"
	-@erase "$(INTDIR)\gtk_adbkpt_cb.obj"
	-@erase "$(INTDIR)\gtk_adbkpt_dbox.obj"
	-@erase "$(INTDIR)\gtk_bkpt_cause.obj"
	-@erase "$(INTDIR)\gtk_colors_cb.obj"
	-@erase "$(INTDIR)\gtk_colors_dbox.obj"
	-@erase "$(INTDIR)\gtk_dbgopt_cb.obj"
	-@erase "$(INTDIR)\gtk_dbgopt_dbox.obj"
	-@erase "$(INTDIR)\gtk_dbkpt_cb.obj"
	-@erase "$(INTDIR)\gtk_dbkpt_dbox.obj"
	-@erase "$(INTDIR)\gtk_debug_cb.obj"
	-@erase "$(INTDIR)\gtk_debug_dbox.obj"
	-@erase "$(INTDIR)\gtk_dump_cb.obj"
	-@erase "$(INTDIR)\gtk_dump_dbox.obj"
	-@erase "$(INTDIR)\gtk_fsel_cb.obj"
	-@erase "$(INTDIR)\gtk_fsel_dbox.obj"
	-@erase "$(INTDIR)\gtk_help_cb.obj"
	-@erase "$(INTDIR)\gtk_help_dbox.obj"
	-@erase "$(INTDIR)\gtk_infos_cb.obj"
	-@erase "$(INTDIR)\gtk_infos_dbox.obj"
	-@erase "$(INTDIR)\gtk_itick_cb.obj"
	-@erase "$(INTDIR)\gtk_itick_dbox.obj"
	-@erase "$(INTDIR)\gtk_link_cb.obj"
	-@erase "$(INTDIR)\gtk_link_dbox.obj"
	-@erase "$(INTDIR)\gtk_logaddr_cb.obj"
	-@erase "$(INTDIR)\gtk_logaddr_dbox.obj"
	-@erase "$(INTDIR)\gtk_main.obj"
	-@erase "$(INTDIR)\gtk_manpage_cb.obj"
	-@erase "$(INTDIR)\gtk_manpage_dbox.obj"
	-@erase "$(INTDIR)\gtk_msg_cb.obj"
	-@erase "$(INTDIR)\gtk_msg_dbox.obj"
	-@erase "$(INTDIR)\gtk_pbar_cb.obj"
	-@erase "$(INTDIR)\gtk_pbar_dbox.obj"
	-@erase "$(INTDIR)\gtk_pgmentbkpt_cb.obj"
	-@erase "$(INTDIR)\gtk_pgmentbkpt_dbox.obj"
	-@erase "$(INTDIR)\gtk_romv_cb.obj"
	-@erase "$(INTDIR)\gtk_romv_dbox.obj"
	-@erase "$(INTDIR)\gtk_scopt_cb.obj"
	-@erase "$(INTDIR)\gtk_scopt_dbox.obj"
	-@erase "$(INTDIR)\gtk_startup_cb.obj"
	-@erase "$(INTDIR)\gtk_startup_dbox.obj"
	-@erase "$(INTDIR)\gtk_state_cb.obj"
	-@erase "$(INTDIR)\gtk_state_dbox.obj"
	-@erase "$(INTDIR)\gtk_thanks_cb.obj"
	-@erase "$(INTDIR)\gtk_thanks_dbox.obj"
	-@erase "$(INTDIR)\gtk_tiemu_cb.obj"
	-@erase "$(INTDIR)\gtk_tiemu_dbox.obj"
	-@erase "$(INTDIR)\gtk_trap_cb.obj"
	-@erase "$(INTDIR)\gtk_trap_dbox.obj"
	-@erase "$(INTDIR)\gtk_user_cb.obj"
	-@erase "$(INTDIR)\gtk_user_dbox.obj"
	-@erase "$(INTDIR)\gtk_viewlog_cb.obj"
	-@erase "$(INTDIR)\gtk_viewlog_dbox.obj"
	-@erase "$(INTDIR)\gtk_wizard_cb.obj"
	-@erase "$(INTDIR)\gtk_wizard_dbox.obj"
	-@erase "$(INTDIR)\gtk_xpm.obj"
	-@erase "$(INTDIR)\gtktiemu.res"
	-@erase "$(INTDIR)\Img_cnv.obj"
	-@erase "$(INTDIR)\Img_jpg.obj"
	-@erase "$(INTDIR)\Img_pcx.obj"
	-@erase "$(INTDIR)\Img_xpm.obj"
	-@erase "$(INTDIR)\Misc.obj"
	-@erase "$(INTDIR)\Rcfile.obj"
	-@erase "$(INTDIR)\sdlspecific.obj"
	-@erase "$(INTDIR)\skn_loader.obj"
	-@erase "$(INTDIR)\support.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Test\GtkTiEmu.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /w /W0 /GX /O2 /I "..\..\ti_libs\cables\src" /I "..\..\ti_libs\calcs\src" /I "..\..\ti_libs\libti68k\src\\" /I "..\dep\win32" /D "NDEBUG" /D "HAVE_LIBJPEG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "PENT_COUNTER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\Gtktiemu.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TiEmu_gtk_gui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gnu-intl.lib glib-1.3.lib gdk-1.3.lib gtk-1.3.lib gmodule-1.3.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\GtkTiEmu.pdb" /machine:I386 /out:"$(OUTDIR)\Test\GtkTiEmu.exe" 
LINK32_OBJS= \
	"..\..\DLL & libs\libs\SDL.lib" \
	"..\..\DLL & libs\Libs\Libjpeg.lib" \
	"$(INTDIR)\gtk_about_cb.obj" \
	"$(INTDIR)\gtk_about_dbox.obj" \
	"$(INTDIR)\gtk_adbkpt_cb.obj" \
	"$(INTDIR)\gtk_adbkpt_dbox.obj" \
	"$(INTDIR)\gtk_bkpt_cause.obj" \
	"$(INTDIR)\gtk_colors_cb.obj" \
	"$(INTDIR)\gtk_colors_dbox.obj" \
	"$(INTDIR)\gtk_dbgopt_cb.obj" \
	"$(INTDIR)\gtk_dbgopt_dbox.obj" \
	"$(INTDIR)\gtk_dbkpt_cb.obj" \
	"$(INTDIR)\gtk_dbkpt_dbox.obj" \
	"$(INTDIR)\gtk_debug_cb.obj" \
	"$(INTDIR)\gtk_debug_dbox.obj" \
	"$(INTDIR)\gtk_dump_cb.obj" \
	"$(INTDIR)\gtk_dump_dbox.obj" \
	"$(INTDIR)\gtk_fsel_cb.obj" \
	"$(INTDIR)\gtk_fsel_dbox.obj" \
	"$(INTDIR)\gtk_help_cb.obj" \
	"$(INTDIR)\gtk_help_dbox.obj" \
	"$(INTDIR)\gtk_infos_cb.obj" \
	"$(INTDIR)\gtk_infos_dbox.obj" \
	"$(INTDIR)\gtk_itick_cb.obj" \
	"$(INTDIR)\gtk_itick_dbox.obj" \
	"$(INTDIR)\gtk_link_cb.obj" \
	"$(INTDIR)\gtk_link_dbox.obj" \
	"$(INTDIR)\gtk_logaddr_cb.obj" \
	"$(INTDIR)\gtk_logaddr_dbox.obj" \
	"$(INTDIR)\gtk_main.obj" \
	"$(INTDIR)\gtk_manpage_cb.obj" \
	"$(INTDIR)\gtk_manpage_dbox.obj" \
	"$(INTDIR)\gtk_msg_cb.obj" \
	"$(INTDIR)\gtk_msg_dbox.obj" \
	"$(INTDIR)\gtk_pbar_cb.obj" \
	"$(INTDIR)\gtk_pbar_dbox.obj" \
	"$(INTDIR)\gtk_pgmentbkpt_cb.obj" \
	"$(INTDIR)\gtk_pgmentbkpt_dbox.obj" \
	"$(INTDIR)\gtk_romv_cb.obj" \
	"$(INTDIR)\gtk_romv_dbox.obj" \
	"$(INTDIR)\gtk_scopt_cb.obj" \
	"$(INTDIR)\gtk_scopt_dbox.obj" \
	"$(INTDIR)\gtk_startup_cb.obj" \
	"$(INTDIR)\gtk_startup_dbox.obj" \
	"$(INTDIR)\gtk_state_cb.obj" \
	"$(INTDIR)\gtk_state_dbox.obj" \
	"$(INTDIR)\gtk_thanks_cb.obj" \
	"$(INTDIR)\gtk_thanks_dbox.obj" \
	"$(INTDIR)\gtk_tiemu_cb.obj" \
	"$(INTDIR)\gtk_tiemu_dbox.obj" \
	"$(INTDIR)\gtk_trap_cb.obj" \
	"$(INTDIR)\gtk_trap_dbox.obj" \
	"$(INTDIR)\gtk_user_cb.obj" \
	"$(INTDIR)\gtk_user_dbox.obj" \
	"$(INTDIR)\gtk_viewlog_cb.obj" \
	"$(INTDIR)\gtk_viewlog_dbox.obj" \
	"$(INTDIR)\gtk_wizard_cb.obj" \
	"$(INTDIR)\gtk_wizard_dbox.obj" \
	"$(INTDIR)\gtk_xpm.obj" \
	"$(INTDIR)\Files.obj" \
	"$(INTDIR)\Img_cnv.obj" \
	"$(INTDIR)\Img_jpg.obj" \
	"$(INTDIR)\Img_pcx.obj" \
	"$(INTDIR)\Img_xpm.obj" \
	"$(INTDIR)\Misc.obj" \
	"$(INTDIR)\Rcfile.obj" \
	"$(INTDIR)\skn_loader.obj" \
	"$(INTDIR)\support.obj" \
	"$(INTDIR)\sdlspecific.obj" \
	"$(INTDIR)\gtktiemu.res" \
	"..\..\ti_libs\libti68k\win32\libti68k.lib" \
	"..\..\ti_libs\cables\win32\libticables.lib" \
	"..\..\ti_libs\calcs\win32\libticalcs.lib"

"$(OUTDIR)\Test\GtkTiEmu.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Debug"

OUTDIR=E:\msvc\gtktiemu
INTDIR=E:\msvc\Output\gtktiemu

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\msvc\gtktiemu\GtkTiEmu.exe"

!ELSE 

ALL : "ti_calcs - Win32 Debug" "ti_cables - Win32 Debug" "lib68k - Win32 Debug" "..\..\..\..\msvc\gtktiemu\GtkTiEmu.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"lib68k - Win32 DebugCLEAN" "ti_cables - Win32 DebugCLEAN" "ti_calcs - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Files.obj"
	-@erase "$(INTDIR)\gtk_about_cb.obj"
	-@erase "$(INTDIR)\gtk_about_dbox.obj"
	-@erase "$(INTDIR)\gtk_adbkpt_cb.obj"
	-@erase "$(INTDIR)\gtk_adbkpt_dbox.obj"
	-@erase "$(INTDIR)\gtk_bkpt_cause.obj"
	-@erase "$(INTDIR)\gtk_colors_cb.obj"
	-@erase "$(INTDIR)\gtk_colors_dbox.obj"
	-@erase "$(INTDIR)\gtk_dbgopt_cb.obj"
	-@erase "$(INTDIR)\gtk_dbgopt_dbox.obj"
	-@erase "$(INTDIR)\gtk_dbkpt_cb.obj"
	-@erase "$(INTDIR)\gtk_dbkpt_dbox.obj"
	-@erase "$(INTDIR)\gtk_debug_cb.obj"
	-@erase "$(INTDIR)\gtk_debug_dbox.obj"
	-@erase "$(INTDIR)\gtk_dump_cb.obj"
	-@erase "$(INTDIR)\gtk_dump_dbox.obj"
	-@erase "$(INTDIR)\gtk_fsel_cb.obj"
	-@erase "$(INTDIR)\gtk_fsel_dbox.obj"
	-@erase "$(INTDIR)\gtk_help_cb.obj"
	-@erase "$(INTDIR)\gtk_help_dbox.obj"
	-@erase "$(INTDIR)\gtk_infos_cb.obj"
	-@erase "$(INTDIR)\gtk_infos_dbox.obj"
	-@erase "$(INTDIR)\gtk_itick_cb.obj"
	-@erase "$(INTDIR)\gtk_itick_dbox.obj"
	-@erase "$(INTDIR)\gtk_link_cb.obj"
	-@erase "$(INTDIR)\gtk_link_dbox.obj"
	-@erase "$(INTDIR)\gtk_logaddr_cb.obj"
	-@erase "$(INTDIR)\gtk_logaddr_dbox.obj"
	-@erase "$(INTDIR)\gtk_main.obj"
	-@erase "$(INTDIR)\gtk_manpage_cb.obj"
	-@erase "$(INTDIR)\gtk_manpage_dbox.obj"
	-@erase "$(INTDIR)\gtk_msg_cb.obj"
	-@erase "$(INTDIR)\gtk_msg_dbox.obj"
	-@erase "$(INTDIR)\gtk_pbar_cb.obj"
	-@erase "$(INTDIR)\gtk_pbar_dbox.obj"
	-@erase "$(INTDIR)\gtk_pgmentbkpt_cb.obj"
	-@erase "$(INTDIR)\gtk_pgmentbkpt_dbox.obj"
	-@erase "$(INTDIR)\gtk_romv_cb.obj"
	-@erase "$(INTDIR)\gtk_romv_dbox.obj"
	-@erase "$(INTDIR)\gtk_scopt_cb.obj"
	-@erase "$(INTDIR)\gtk_scopt_dbox.obj"
	-@erase "$(INTDIR)\gtk_startup_cb.obj"
	-@erase "$(INTDIR)\gtk_startup_dbox.obj"
	-@erase "$(INTDIR)\gtk_state_cb.obj"
	-@erase "$(INTDIR)\gtk_state_dbox.obj"
	-@erase "$(INTDIR)\gtk_thanks_cb.obj"
	-@erase "$(INTDIR)\gtk_thanks_dbox.obj"
	-@erase "$(INTDIR)\gtk_tiemu_cb.obj"
	-@erase "$(INTDIR)\gtk_tiemu_dbox.obj"
	-@erase "$(INTDIR)\gtk_trap_cb.obj"
	-@erase "$(INTDIR)\gtk_trap_dbox.obj"
	-@erase "$(INTDIR)\gtk_user_cb.obj"
	-@erase "$(INTDIR)\gtk_user_dbox.obj"
	-@erase "$(INTDIR)\gtk_viewlog_cb.obj"
	-@erase "$(INTDIR)\gtk_viewlog_dbox.obj"
	-@erase "$(INTDIR)\gtk_wizard_cb.obj"
	-@erase "$(INTDIR)\gtk_wizard_dbox.obj"
	-@erase "$(INTDIR)\gtk_xpm.obj"
	-@erase "$(INTDIR)\gtktiemu.res"
	-@erase "$(INTDIR)\Img_cnv.obj"
	-@erase "$(INTDIR)\Img_jpg.obj"
	-@erase "$(INTDIR)\Img_pcx.obj"
	-@erase "$(INTDIR)\Img_xpm.obj"
	-@erase "$(INTDIR)\Misc.obj"
	-@erase "$(INTDIR)\Rcfile.obj"
	-@erase "$(INTDIR)\sdlspecific.obj"
	-@erase "$(INTDIR)\skn_loader.obj"
	-@erase "$(INTDIR)\support.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\GtkTiEmu.pdb"
	-@erase "..\..\..\..\msvc\gtktiemu\GtkTiEmu.exe"
	-@erase "..\..\..\..\msvc\gtktiemu\GtkTiEmu.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /w /W0 /Gm /GX /ZI /Od /I "../../ti_libs/cables/src/" /I "../" /I "./" /I "../../ti_libs/calcs/src/" /I "../../ti_libs/libti68k/src/" /I "../dep/win32/" /I "..\..\ti_libs\cables\src" /I "..\..\ti_libs\calcs\src" /I "..\..\ti_libs\libti68k\src\\" /I "..\dep\win32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "PENT_COUNTER" /D "HAVE_LIBJPEG" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\Gtktiemu.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TiEmu_gtk_gui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gnu-intl.lib glib-1.3.lib gdk-1.3.lib gtk-1.3.lib gmodule-1.3.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\GtkTiEmu.pdb" /debug /machine:I386 /out:"D:\msvc\gtktiemu\GtkTiEmu.exe" /pdbtype:sept 
LINK32_OBJS= \
	"..\..\DLL & libs\libs\SDL.lib" \
	"..\..\DLL & libs\Libs\Libjpeg.lib" \
	"$(INTDIR)\gtk_about_cb.obj" \
	"$(INTDIR)\gtk_about_dbox.obj" \
	"$(INTDIR)\gtk_adbkpt_cb.obj" \
	"$(INTDIR)\gtk_adbkpt_dbox.obj" \
	"$(INTDIR)\gtk_bkpt_cause.obj" \
	"$(INTDIR)\gtk_colors_cb.obj" \
	"$(INTDIR)\gtk_colors_dbox.obj" \
	"$(INTDIR)\gtk_dbgopt_cb.obj" \
	"$(INTDIR)\gtk_dbgopt_dbox.obj" \
	"$(INTDIR)\gtk_dbkpt_cb.obj" \
	"$(INTDIR)\gtk_dbkpt_dbox.obj" \
	"$(INTDIR)\gtk_debug_cb.obj" \
	"$(INTDIR)\gtk_debug_dbox.obj" \
	"$(INTDIR)\gtk_dump_cb.obj" \
	"$(INTDIR)\gtk_dump_dbox.obj" \
	"$(INTDIR)\gtk_fsel_cb.obj" \
	"$(INTDIR)\gtk_fsel_dbox.obj" \
	"$(INTDIR)\gtk_help_cb.obj" \
	"$(INTDIR)\gtk_help_dbox.obj" \
	"$(INTDIR)\gtk_infos_cb.obj" \
	"$(INTDIR)\gtk_infos_dbox.obj" \
	"$(INTDIR)\gtk_itick_cb.obj" \
	"$(INTDIR)\gtk_itick_dbox.obj" \
	"$(INTDIR)\gtk_link_cb.obj" \
	"$(INTDIR)\gtk_link_dbox.obj" \
	"$(INTDIR)\gtk_logaddr_cb.obj" \
	"$(INTDIR)\gtk_logaddr_dbox.obj" \
	"$(INTDIR)\gtk_main.obj" \
	"$(INTDIR)\gtk_manpage_cb.obj" \
	"$(INTDIR)\gtk_manpage_dbox.obj" \
	"$(INTDIR)\gtk_msg_cb.obj" \
	"$(INTDIR)\gtk_msg_dbox.obj" \
	"$(INTDIR)\gtk_pbar_cb.obj" \
	"$(INTDIR)\gtk_pbar_dbox.obj" \
	"$(INTDIR)\gtk_pgmentbkpt_cb.obj" \
	"$(INTDIR)\gtk_pgmentbkpt_dbox.obj" \
	"$(INTDIR)\gtk_romv_cb.obj" \
	"$(INTDIR)\gtk_romv_dbox.obj" \
	"$(INTDIR)\gtk_scopt_cb.obj" \
	"$(INTDIR)\gtk_scopt_dbox.obj" \
	"$(INTDIR)\gtk_startup_cb.obj" \
	"$(INTDIR)\gtk_startup_dbox.obj" \
	"$(INTDIR)\gtk_state_cb.obj" \
	"$(INTDIR)\gtk_state_dbox.obj" \
	"$(INTDIR)\gtk_thanks_cb.obj" \
	"$(INTDIR)\gtk_thanks_dbox.obj" \
	"$(INTDIR)\gtk_tiemu_cb.obj" \
	"$(INTDIR)\gtk_tiemu_dbox.obj" \
	"$(INTDIR)\gtk_trap_cb.obj" \
	"$(INTDIR)\gtk_trap_dbox.obj" \
	"$(INTDIR)\gtk_user_cb.obj" \
	"$(INTDIR)\gtk_user_dbox.obj" \
	"$(INTDIR)\gtk_viewlog_cb.obj" \
	"$(INTDIR)\gtk_viewlog_dbox.obj" \
	"$(INTDIR)\gtk_wizard_cb.obj" \
	"$(INTDIR)\gtk_wizard_dbox.obj" \
	"$(INTDIR)\gtk_xpm.obj" \
	"$(INTDIR)\Files.obj" \
	"$(INTDIR)\Img_cnv.obj" \
	"$(INTDIR)\Img_jpg.obj" \
	"$(INTDIR)\Img_pcx.obj" \
	"$(INTDIR)\Img_xpm.obj" \
	"$(INTDIR)\Misc.obj" \
	"$(INTDIR)\Rcfile.obj" \
	"$(INTDIR)\skn_loader.obj" \
	"$(INTDIR)\support.obj" \
	"$(INTDIR)\sdlspecific.obj" \
	"$(INTDIR)\gtktiemu.res" \
	"..\..\..\..\msvc\gtktiemu\libti68k.lib" \
	"$(OUTDIR)\libticables.lib" \
	"$(OUTDIR)\libticalcs.lib"

"..\..\..\..\msvc\gtktiemu\GtkTiEmu.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TiEmu_gtk_gui.dep")
!INCLUDE "TiEmu_gtk_gui.dep"
!ELSE 
!MESSAGE Warning: cannot find "TiEmu_gtk_gui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TiEmu_gtk_gui - Win32 Release" || "$(CFG)" == "TiEmu_gtk_gui - Win32 Debug"
SOURCE=..\src\gtk_about_cb.c

"$(INTDIR)\gtk_about_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_about_dbox.c

"$(INTDIR)\gtk_about_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_adbkpt_cb.c

"$(INTDIR)\gtk_adbkpt_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_adbkpt_dbox.c

"$(INTDIR)\gtk_adbkpt_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_bkpt_cause.c

"$(INTDIR)\gtk_bkpt_cause.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_colors_cb.c

"$(INTDIR)\gtk_colors_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_colors_dbox.c

"$(INTDIR)\gtk_colors_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_dbgopt_cb.c

"$(INTDIR)\gtk_dbgopt_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_dbgopt_dbox.c

"$(INTDIR)\gtk_dbgopt_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_dbkpt_cb.c

"$(INTDIR)\gtk_dbkpt_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_dbkpt_dbox.c

"$(INTDIR)\gtk_dbkpt_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_debug_cb.c

"$(INTDIR)\gtk_debug_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_debug_dbox.c

"$(INTDIR)\gtk_debug_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_dump_cb.c

"$(INTDIR)\gtk_dump_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_dump_dbox.c

"$(INTDIR)\gtk_dump_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_fsel_cb.c

"$(INTDIR)\gtk_fsel_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_fsel_dbox.c

"$(INTDIR)\gtk_fsel_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_help_cb.c

"$(INTDIR)\gtk_help_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_help_dbox.c

"$(INTDIR)\gtk_help_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_infos_cb.c

"$(INTDIR)\gtk_infos_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_infos_dbox.c

"$(INTDIR)\gtk_infos_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_itick_cb.c

"$(INTDIR)\gtk_itick_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_itick_dbox.c

"$(INTDIR)\gtk_itick_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_link_cb.c

"$(INTDIR)\gtk_link_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_link_dbox.c

"$(INTDIR)\gtk_link_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_logaddr_cb.c

"$(INTDIR)\gtk_logaddr_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_logaddr_dbox.c

"$(INTDIR)\gtk_logaddr_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_main.c

"$(INTDIR)\gtk_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_manpage_cb.c

"$(INTDIR)\gtk_manpage_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_manpage_dbox.c

"$(INTDIR)\gtk_manpage_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_msg_cb.c

"$(INTDIR)\gtk_msg_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_msg_dbox.c

"$(INTDIR)\gtk_msg_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_pbar_cb.c

"$(INTDIR)\gtk_pbar_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_pbar_dbox.c

"$(INTDIR)\gtk_pbar_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_pgmentbkpt_cb.c

"$(INTDIR)\gtk_pgmentbkpt_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_pgmentbkpt_dbox.c

"$(INTDIR)\gtk_pgmentbkpt_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_romv_cb.c

"$(INTDIR)\gtk_romv_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_romv_dbox.c

"$(INTDIR)\gtk_romv_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_scopt_cb.c

"$(INTDIR)\gtk_scopt_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_scopt_dbox.c

"$(INTDIR)\gtk_scopt_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_startup_cb.c

"$(INTDIR)\gtk_startup_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_startup_dbox.c

"$(INTDIR)\gtk_startup_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_state_cb.c

"$(INTDIR)\gtk_state_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_state_dbox.c

"$(INTDIR)\gtk_state_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_thanks_cb.c

"$(INTDIR)\gtk_thanks_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_thanks_dbox.c

"$(INTDIR)\gtk_thanks_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_tiemu_cb.c

"$(INTDIR)\gtk_tiemu_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_tiemu_dbox.c

"$(INTDIR)\gtk_tiemu_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_trap_cb.c

"$(INTDIR)\gtk_trap_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_trap_dbox.c

"$(INTDIR)\gtk_trap_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_user_cb.c

"$(INTDIR)\gtk_user_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_user_dbox.c

"$(INTDIR)\gtk_user_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_viewlog_cb.c

"$(INTDIR)\gtk_viewlog_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_viewlog_dbox.c

"$(INTDIR)\gtk_viewlog_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_wizard_cb.c

"$(INTDIR)\gtk_wizard_cb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_wizard_dbox.c

"$(INTDIR)\gtk_wizard_dbox.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\gtk_xpm.c

"$(INTDIR)\gtk_xpm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Files.c

"$(INTDIR)\Files.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Img_cnv.c

"$(INTDIR)\Img_cnv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Img_jpg.c

"$(INTDIR)\Img_jpg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Img_pcx.c

"$(INTDIR)\Img_pcx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Img_xpm.c

"$(INTDIR)\Img_xpm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Misc.c

"$(INTDIR)\Misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Rcfile.c

"$(INTDIR)\Rcfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\skn_loader.c

"$(INTDIR)\skn_loader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\support.c

"$(INTDIR)\support.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\sdlspecific.c

"$(INTDIR)\sdlspecific.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Gtktiemu.rc

"$(INTDIR)\gtktiemu.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Release"

"lib68k - Win32 Release" : 
   cd "\devel\gtktiemu_project\ti_libs\libti68k\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\lib68k.mak CFG="lib68k - Win32 Release" 
   cd "..\..\..\gtktiemu\win32"

"lib68k - Win32 ReleaseCLEAN" : 
   cd "\devel\gtktiemu_project\ti_libs\libti68k\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\lib68k.mak CFG="lib68k - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\gtktiemu\win32"

!ELSEIF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Debug"

"lib68k - Win32 Debug" : 
   cd "\devel\gtktiemu_project\ti_libs\libti68k\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\lib68k.mak CFG="lib68k - Win32 Debug" 
   cd "..\..\..\gtktiemu\win32"

"lib68k - Win32 DebugCLEAN" : 
   cd "\devel\gtktiemu_project\ti_libs\libti68k\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\lib68k.mak CFG="lib68k - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\gtktiemu\win32"

!ENDIF 

!IF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Release"

"ti_cables - Win32 Release" : 
   cd "\devel\gtktiemu_project\ti_libs\cables\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_cables.mak CFG="ti_cables - Win32 Release" 
   cd "..\..\..\gtktiemu\win32"

"ti_cables - Win32 ReleaseCLEAN" : 
   cd "\devel\gtktiemu_project\ti_libs\cables\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_cables.mak CFG="ti_cables - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\gtktiemu\win32"

!ELSEIF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Debug"

"ti_cables - Win32 Debug" : 
   cd "\devel\gtktiemu_project\ti_libs\cables\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_cables.mak CFG="ti_cables - Win32 Debug" 
   cd "..\..\..\gtktiemu\win32"

"ti_cables - Win32 DebugCLEAN" : 
   cd "\devel\gtktiemu_project\ti_libs\cables\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_cables.mak CFG="ti_cables - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\gtktiemu\win32"

!ENDIF 

!IF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Release"

"ti_calcs - Win32 Release" : 
   cd "\devel\gtktiemu_project\ti_libs\calcs\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_calcs.mak CFG="ti_calcs - Win32 Release" 
   cd "..\..\..\gtktiemu\win32"

"ti_calcs - Win32 ReleaseCLEAN" : 
   cd "\devel\gtktiemu_project\ti_libs\calcs\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_calcs.mak CFG="ti_calcs - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\gtktiemu\win32"

!ELSEIF  "$(CFG)" == "TiEmu_gtk_gui - Win32 Debug"

"ti_calcs - Win32 Debug" : 
   cd "\devel\gtktiemu_project\ti_libs\calcs\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_calcs.mak CFG="ti_calcs - Win32 Debug" 
   cd "..\..\..\gtktiemu\win32"

"ti_calcs - Win32 DebugCLEAN" : 
   cd "\devel\gtktiemu_project\ti_libs\calcs\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\ti_calcs.mak CFG="ti_calcs - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\gtktiemu\win32"

!ENDIF 


!ENDIF 

