/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
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

#include <glib.h>
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
#include "ti68k_int.h"

#include "hid.h"
#include "engine.h"
#include "refresh.h"
#include "../hid/printl.h"

#include "wizard.h"
#include "popup.h"
#include "splash.h"
#include "tie_error.h"
#include "dboxes.h"


ScrOptions options2;
TieOptions options;		// general tiemu options
TicalcInfoUpdate info_update;	// pbar, msg_box, refresh, ...

//G_LOCK_EXTERN(lcd_flag);
extern int lcd_flag;

static gint hid_refresh (gpointer data)
{
    if(lcd_flag || (tihw.calc_type == HW2))
    {
        hid_update_lcd();
        lcd_flag = 0;

        // Toggles every FS (every time the LCD restarts at line 0)
        tihw.io2[0x1d] |= (1 << 7);
    }
    
    hid_update_keys();

    return TRUE;
}

/* Main function */		
int main(int argc, char **argv) 
{
	GThread *thread = NULL;
	GError *error = NULL;
    int err;

	/*
		Do primary initializations 
	*/
	version();
	initialize_paths();
	rcfile_default();   // (step 2)
	rcfile_read();
	scan_cmdline(argc, argv);

    /* 
		Init GTK+ (popup menu, boxes, ...) and threads
	*/
	g_thread_init(NULL);
	gtk_init(&argc, &argv);

    /*
        Set splash screen
    */
    splash_screen_start();
    splash_screen_set_label(_("Initializing GTK+..."));    
  
	/* 
	   Check the version of libraries 
	 */
    splash_screen_set_label(_("Initializing TiLP framework..."));

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
	    Change logging domains
	*/
	
    //ticable_set_printl(ticables_printl);
	//tifiles_set_printl(tifiles_printl);
    //ticalc_set_printl(ticalcs_printl);

	/* 
		Assign an GUI to the emulation engine 
		and a debugger (step 1)
	*/
	hid_set_callbacks();
	//ti68k_defineDebugger(enter_gtk_debugger);

    /*
        Search for ROM in the image directory
    */
    splash_screen_set_label(_("Searching for ROM dumps..."));
    err = ti68k_scan_files(inst_paths.img_dir, inst_paths.img_dir);
	handle_error();

	/*
		Attempt to load an image (step 3)
	*/
	err = ti68k_load_image(params.rom_file);
	if(err) 
	{
      
		display_wizard_dbox();
		while(!wizard_ok)
			GTK_REFRESH();
	
		g_free(params.rom_file);
		params.rom_file = g_strdup(wizard_rom);
		g_free(wizard_rom);

        splash_screen_set_label(_("Loading image..."));
		err = ti68k_load_image(params.rom_file);
		handle_error();
	}

	/* 
		Initialize emulation engine (step 4)
	*/
    splash_screen_set_label(_("Initializing m68k emulation engine..."));
	err = ti68k_init();
	handle_error();
	if(err)
		return -1;

	/*
		Load FLASH upgrade (if any)
	*/
	if(params.tib_file != NULL) 
	{
		err = ti68k_load_upgrade(params.tib_file);
		handle_error();
	}

	/* 
		Override refresh functions of the libticalcs library 
		(must be done after init of ti68k engine
	*/
	gt_init_refresh_functions();

	/* 
		Reset emulation engine (step 5)
	*/
	ti68k_reset();

	/* 
		Load calculator state image 
	*/
    splash_screen_set_label(_("Loading previously saved state..."));
	if(params.sav_file != NULL) 
	{
		err = ti68k_state_load(params.sav_file);
		handle_error();
	}
  
	/* 
		Start thread (emulation engine) and run main loop 
	*/
    splash_screen_set_label(_("Starting engine..."));
	thread = g_thread_create(ti68k_engine, NULL, FALSE, &error);
	ti68k_engine_unhalt();
    splash_screen_stop();

	/*
		Surprisingly, using gtk_main_iteration is less CPU intensive than 
        gtk_main() and the hid_update_key in a gtk_idle function.
		GTK popup menu is called from SDL in hid.c
        Timeout handler is used to refresh display. This way is 10% less CPU 
        intensive than direct updating from hardware at hardware rate.
	*/
    gtk_timeout_add((params.lcd_rate == -1) ? 10 : params.lcd_rate, 
           (GtkFunction)hid_refresh, NULL);

   	gdk_threads_enter();
    gtk_main();
	gdk_threads_leave();

	/* 
		Close the emulator engine
	*/
	err = ti68k_exit();
	handle_error();
  
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
