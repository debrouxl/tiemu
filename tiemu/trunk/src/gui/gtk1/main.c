/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/timeb.h>
#include <sys/types.h>
//#include <unistd.h>

#include "tilibs.h"
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "version.h"
#include "misc.h"
#include "files.h"
#include "rcfile.h"
#include "interface.h"

#include "tiemu_cb.h"
#include "wizard_dbox.h"
#include "refresh.h"
#include "main_wnd.h"
#include "main.h"

/* 
   The TI92/89 should approximately execute NB_INSTRUCTIONS_PER_LOOP in 
   TIME_LIMIT milliseconds
   If you think this values are a bit too big, you can slow down 
   the emulator by changing them 
*/
#define NB_INSTRUCTIONS_PER_LOOP 50000 //50000
#define TIME_LIMIT               30 // 30


extern int wizard_ok;
extern gchar *wizard_rom;

TieOptions options;        // general gtktiemu options
TicalcInfoUpdate info_update;    // pbar, msg_box, refresh, ...


/* Main function */		
int main(int argc, char **argv) 
{
  struct timeb tCurrentTime;
  struct timeb tLastTime;
  unsigned long int iCurrentTime;
  unsigned long int iLastTime; 
  gint res=0;

  /* Do primary initializations */
  version();
  initialize_paths();
  load_default_config();
  read_rc_file();
  scan_cmdline(argc, argv);
  
  /* Check libticables & libticalcs library */

  DISPLAY(_("libticables library, version %s\n"),
	  ticable_get_version());
  if(strcmp(ticable_get_version(), LIB_CABLE_VERSION_REQUIRED) < 0)
    {
      DISPLAY("Library version <%s> mini required.\n", 
	      LIB_CABLE_VERSION_REQUIRED);
      exit(-1);
    }

  DISPLAY(_("libticalcs library, version %s\n"),
	  ticalc_get_version());
  if(strcmp(ticalc_get_version(), LIB_CALC_VERSION_REQUIRED) < 0)
    {
      DISPLAY("Library version <%s> mini required.\n", 
	      LIB_CALC_VERSION_REQUIRED);
      exit(-1);
    }

  /* Initialize the libticables library */
  ticable_set_param(options.link_cable);

  /* Assign a GUI to the libti68k library */
  hid_set_gui_callbacks();

  /* and a debugger */
  ti68k_defineDebugger(enter_gtk_debugger);

  /* Load a ROM image */
  gtk_init(&argc, &argv);
  if(ti68k_loadImage((options.params)->rom_file)) 
    {

      gtk_widget_show(create_step1_dbox());
      while(!wizard_ok) {
	while( gtk_events_pending() ) { 
	  gtk_main_iteration(); 
	}
      }
      g_free((options.params)->rom_file);
      (options.params)->rom_file = g_strdup(wizard_rom);
      g_free(wizard_rom);
      ti68k_loadImage((options.params)->rom_file);
    }

  /* Init TI68k engine */
  if(ti68k_init()) return -1;

  /*
  if(options.params->tib_file != NULL)
    ti68k_loadUpgrade((options.params)->tib_file);
  */

  /* Override refresh functions of the libticalcs library (must be done
     after ti68k_init */
  gt_init_refresh_functions();

  /* Start emulation */
  ti68k_reset();

  /* Load calculator state image */
  ti68k_loadState((options.params)->ram_file);
  
  /* Close the start-up console (Win32) */
  options.console = DSP_ON;
  ticable_DISPLAY_settings(options.console);

  /* Create popup menu: must be called after initLib68k ! */
  gtk_main_window();

  /* Run main loop */
  while (1) 
    {
      // Update GUI only if emulator core is halted
      if(is_halted()) {
	gtk_main_iteration_do(FALSE);
	continue;
      }
      
      // Run emulator core
      ftime(&tLastTime);
      if((res = ti68k_doInstructions(NB_INSTRUCTIONS_PER_LOOP)))
	{  // a bkpt has been encountered

	}
      else 
	{ // normal execution

	  ftime(&tCurrentTime);
	  iLastTime    = tLastTime.time*1000+tLastTime.millitm;
	  iCurrentTime = tCurrentTime.time*1000+tCurrentTime.millitm;
	  if ((iCurrentTime - iLastTime) < TIME_LIMIT)
	  {
#if defined(__LINUX__)
	    usleep((TIME_LIMIT - iCurrentTime + iLastTime)*1000);
#elif defined(__WIN32__)
	    Sleep((TIME_LIMIT - iCurrentTime + iLastTime));
#endif
	  }
	}
    }

  /* Close the emulator library */
  ti68k_exit();
  
  return 0;
}


/* 
   If GtkTiEmu is compiled in console mode (_CONSOLE), 
   then we use the 'main' entry point.
   If GtkTiEmu is compiled as a windowed application (_WINDOWS), 
   then we use the 'WinMain' entry point.
*/
#if defined(__WIN32__) && defined(_WINDOWS)// && !defined(_CONSOLE)
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
  /* Check whether a TiLP session is already running */
  HANDLE hMutex = CreateMutex(NULL, TRUE, "GtkTiEmu");
  if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
      g_error("GtkTiEmu is already running.");
    }

//  ticable_DISPLAY_settings(DSP_ON);
  
  return main(__argc, __argv);
}
#endif



/*
  These functions should be removed
*/

static int engine_halted = 0; // emulation engine is halted

int is_halted() 
{
  return engine_halted;
}

void halt(void) 
{
  engine_halted = !0;
}

void unhalt(void) 
{
  engine_halted = 0;
  while( gtk_events_pending() ) { 
    gtk_main_iteration(); 
  }
}
