/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin, Kevin Kofler
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

#if WITH_KDE
#include "kde.h"
#endif

#include "intl.h"
#include "tilibs.h"
#include "struct.h"
#include "calc.h"
#include "version.h"
#include "cmdline.h"
#include "files.h"
#include "rcfile.h"
#include "ti68k_int.h"
#include "printl.h"

#include "engine.h"
#include "refresh.h"
#include "calc.h"

#include "wizard.h"
#include "popup.h"
#include "splash.h"
#include "tie_error.h"
#include "dboxes.h"
#include "dbg_all.h"
#include "romversion.h"


ScrOptions options2;
TieOptions options;		// general tiemu options
TicalcInfoUpdate info_update;	// pbar, msg_box, refresh, ...

/* Main function */		

gint exit_loop = 0;

extern gboolean ti68k_test(gpointer data);

int main(int argc, char **argv) 
{
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
		Init GTK+ (popup menu, boxes, ...)
	*/
	gtk_init(&argc, &argv);
    add_pixmap_directory(inst_paths.pixmap_dir);

    /*
        Set splash screen
    */
    splash_screen_start();
    splash_screen_set_label(_("Initializing GTK+..."));    

#if WITH_KDE
    splash_screen_set_label(_("Initializing KDE..."));
    sp_kde_init(argc, argv, "tiemu", _("TiEmu"), VERSION, _("TI calculator emulator"), "Copyright (c) 2000, Thomas Corvazier, Romain Lievin\nCopyright (c) 2001-2002, Romain Lievin, Julien Blache\nCopyright (c) 2003-2004, Romain Liévin\nCopyright (c) 2005, Romain Liévin, Kevin Kofler", "http://lpg.ticalc.org/prj_tiemu/", "gtktiemu-users@lists.sf.net");
    atexit(sp_kde_finish);
#endif

	/* 
	   Check the version of libraries 
	 */
    splash_screen_set_label(_("Initializing TiLP framework..."));

	if (strcmp(tifiles_get_version(), TIEMU_REQUIRES_LIBFILES_VERSION) < 0) 
	{
		printl(0, _("libtifiles library version <%s> mini required (<%s> found).\n"),
			TIEMU_REQUIRES_LIBFILES_VERSION, tifiles_get_version());
		msg_box(_("Error"), _("Libtifiles: version mismatches."));
		exit(-1);
	}
	
	if (strcmp(ticable_get_version(), TIEMU_REQUIRES_LIBCABLES_VERSION) < 0) 
	{
		printl(0, _("libticables library version <%s> mini required (<%s> found).\n"),
			TIEMU_REQUIRES_LIBCABLES_VERSION, ticable_get_version());
		msg_box(_("Error"), _("Libticables: version mismatches."));
		exit(-1);
	}
	
	if (strcmp(ticalc_get_version(), TIEMU_REQUIRES_LIBCALCS_VERSION) < 0) 
	{
		printl(0, _("libticalcs library version <%s> mini required (<%s> found).\n"),
			TIEMU_REQUIRES_LIBCALCS_VERSION, ticalc_get_version());
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
        Search for dumps or upgrades to convert (in the image directory)
    */
    splash_screen_set_label(_("Searching for ROM dumps..."));
    err = ti68k_scan_files(inst_paths.img_dir, inst_paths.img_dir, !0);
	handle_error();

	/*
		Attempt to load an image (step 3)
	*/
	while(!exit_loop)
	{

		err = ti68k_load_image(params.rom_file);
		if(err) 
		{
	rescan:
			if(ti68k_find_image(inst_paths.img_dir, NULL))
			{
				// search for an available image to use
				if(display_romversion_dbox(TRUE) == -1)
					goto wizard;
			}
			else
			{
				// launch wizard
	wizard:
				display_wizard_dbox();
				while(!wizard_ok)
					GTK_REFRESH();

				if(wizard_ok == 2)
					goto rescan;
			
				g_free(params.rom_file);
				params.rom_file = g_strdup(wizard_rom);
				g_free(wizard_rom);
			}

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
		if(err)	return -1;

		err = hid_init();
		handle_error();
		if(err)	return -1;

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
			(must be done after init of ti68k engine)
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
			Cache debugger windows to speed-up display and install custom event
		*/
		splash_screen_set_label(_("Pre-loading debugger..."));
		gtk_debugger_preload();
  
		/* 
			Start emulation engine and run main loop 
		*/
		splash_screen_stop();
		ti68k_engine_start();

		gtk_main();

		/* 
			Close the emulator engine
		*/
		ti68k_engine_stop();

		err = hid_exit();
		handle_error();

		err = ti68k_exit();
		handle_error();

		ti68k_unload_image_or_upgrade();
	}

	return 0;
}

/* 
   If GtkTiEmu is compiled in console mode (_CONSOLE), 
   then we use the 'main' entry point.
   If GtkTiEmu is compiled as a windowed application (_WINDOWS), 
   then we use the 'WinMain' entry point.
*/
#if defined(__WIN32__) && defined(_WINDOWS)// && !defined(_CONSOLE)

#ifdef __MINGW32__
#include <windows.h>
#endif

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	return main(__argc, __argv);
}
#endif
