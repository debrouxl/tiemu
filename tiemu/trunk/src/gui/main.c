/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 154 2004-05-12 13:28:29Z roms $ */

/*  TiEmu - a TI emulator
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Liévin
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

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#include "intl.h"
#include "tilibs.h"
#include "struct.h"
#include "hid.h"
#include "version.h"
#include "cmdline.h"
#include "files.h"
#include "rcfile.h"
#include "interface.h"

#include "hid.h"
#include "core.h"
#include "refresh.h"
#include "printl.h"

#include "wizard_dbox.h"
#include "popup_cb.h"


extern int wizard_ok;
extern gchar *wizard_rom;

TieOptions options;		// general tiemu options
TicalcInfoUpdate info_update;	// pbar, msg_box, refresh, ...


//#ifdef __WIN32__
//#undef main			// undef main with SDL/Win32
//#endif


/* Main function */		
int main(int argc, char **argv) 
{
	GThread *thread = NULL;
	GError *error = NULL;

	/*
		Do primary initializations 
	*/
	version();
	initialize_paths();
	rcfile_default();
	rcfile_read();
	scan_cmdline(argc, argv);
  
	/* 
	   Check the version of libraries 
	 */
	if (strcmp(tifiles_get_version(), TIEMU_REQUIRES_LIBFILES_VERSION) < 0) {
		printl(0, _("libtifiles library version <%s> mini required.\n"),
			TIEMU_REQUIRES_LIBFILES_VERSION);
		msg_box(_("Error"), _("Libtifiles: version mismatches."));
		exit(-1);
	}
	
	if (strcmp(ticable_get_version(), TIEMU_REQUIRES_LIBCABLES_VERSION) < 0) {
		printl(0, _("libticables library version <%s> mini required.\n"),
			TIEMU_REQUIRES_LIBCABLES_VERSION);
		msg_box(_("Error"), _("Libticables: version mismatches."));
		exit(-1);
	}
	
	if (strcmp(ticalc_get_version(), TIEMU_REQUIRES_LIBCALCS_VERSION) < 0) {
		printl(0, _("libticalcs library version <%s> mini required.\n"),
			TIEMU_REQUIRES_LIBCALCS_VERSION);
		msg_box(_("Error"), _("Libticalcs: version mismatches."));
		exit(-1);
	}

	/* 
	   Initialize the libti* libraries
	 */
	
        //ticable_set_printl(ticables_printl);
	ticable_init();
	//tifiles_set_printl(tifiles_printl);
	tifiles_init();
        //ticalc_set_printl(ticalcs_printl);
	ticalc_init();

	/* 
		Assign an GUI to the emulation engine 
		adn a debugger
	*/
	hid_set_gui_callbacks();
	ti68k_defineDebugger(enter_gtk_debugger);

	/* 
		Init GTK+ (popup menu, boxes, ...) and threads
	*/
	g_thread_init(NULL);
	gtk_init(&argc, &argv);

	/*
		For developers only
	*/
#ifndef __WIN32__
	// linux
	g_free((options.params)->rom_file);
	(options.params)->rom_file = g_strdup("/home/romain/ti92.rom");
#else
	//win32
	g_free((options.params)->rom_file);
	(options.params)->rom_file = g_strdup("C:\\Program Files\\Common Files\\GTK\\2.0\\lib\\ti92.rom");
#endif

	/*
		Attempt to load an image
	*/
	if(ti68k_loadImage((options.params)->rom_file)) {
      
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

	/* 
		Initialize emulation engine
	*/
	if(ti68k_init()) {
		tiemu_error(0, "failed to init the ti68k engine.\n");
		return -1;
	}

	if(options.params->tib_file != NULL) {
		ti68k_loadUpgrade((options.params)->tib_file);
	}

	/* 
		Override refresh functions of the libticalcs library 
		(must be done after init of ti68k engine
	*/
	gt_init_refresh_functions();

	/* 
		Reset emulation engine
	*/
	ti68k_reset();

	/* 
		Load calculator state image 
	*/
	ti68k_loadState((options.params)->ram_file);
  
	/* 
		Close the start-up console (Win32) 
	*/
	//if(options.console == DSP_OFF)
		//close_console();

	/* 
		Start thread (emulation engine) and run main loop 
	*/
	thread = g_thread_create(ti68k_engine, NULL, FALSE, &error);
	ti68k_unhalt();
	
	gdk_threads_enter();
	while(1) {
		// we will remove polling by event later
		if(hid_popup_menu()) {
			gui_popup_menu();
		}
		gtk_main_iteration();
	}
	gdk_threads_leave();
	//gtk_main();

	/* 
		Close the emulator engine
	*/
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
	/* Check whether a TiEmu session is already running */
	HANDLE hMutex = CreateMutex(NULL, TRUE, "TiEmu");

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, _("Error"), _("An TiEmu session is already running. Check the task list."), MB_OK);
	}
  
	return main(__argc, __argv);
}
#endif
