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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdlib.h>
#include <string.h>
#ifdef __MINGW32__
#include <windows.h>
#endif

#include "intl.h"
#include "support.h"
#include "struct.h"
#include "version.h"
#include "popup.h"
#include "paths.h"
#include "../engine.h"
#include "fs_misc.h"
#include "comm.h"
#include "rcfile.h"
#include "dboxes.h"
#include "romversion.h"
#include "calc.h"
#include "release.h"
#include "about.h"
#include "infos.h"
#include "manpage.h"
#include "scroptions.h"
#include "tie_error.h"
#include "dbg_all.h"

#include "ti68k_int.h"
#include "ti68k_def.h"

GLADE_CB void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
#if 1
	fprintf(stdout, _("* TiEmu version %s (cables=%s, files=%s, calcs=%s)\n"),
	     TIEMU_VERSION, ticable_get_version(), tifiles_get_version(),
	     ticalc_get_version());
#else
	gchar **list, **ptr;

	list = create_fsels(inst_paths.base_dir, NULL, "*.txt");
	for(ptr = list; *ptr; ptr++)	printf("<<%s>>\n", *ptr);
#endif
}


GLADE_CB void
on_send_file_to_tiemu1_activate     (GtkMenuItem     *menuitem,
                                     gpointer         user_data)
{
	if(engine_is_stopped()) return;

	engine_stop();
	display_send_files_dbox();
	engine_start();
}

GLADE_CB void
on_recv_file_from_tiemu1_activate     (GtkMenuItem     *menuitem,
                                       gpointer         user_data)
{
	if(engine_is_stopped()) return;
	params.recv_file = GTK_CHECK_MENU_ITEM(menuitem)->active;
}


GLADE_CB void
on_debug_file_with_tiemu1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(engine_is_stopped()) return;

	engine_stop();
	display_debug_dbox();
	engine_start();
}


GLADE_CB void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(engine_is_stopped()) return;

	engine_stop();
	display_comm_dbox();
	engine_start();
}


void window_get_rect(GtkWidget *widget, GdkRect *rect);

GLADE_CB void
on_save_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	// save main window position
	window_get_rect(main_wnd, &options3.calc.rect);

	//write config
	rcfile_write();

#ifdef __WIN32__
  	msg_box(_("Information"), 
	  _("Configuration file saved (in tiemu.ini)."));
#else
	msg_box(_("Information"), 
	  _("Configuration file saved (in ~/.tiemu)."));
#endif
}


GLADE_CB void
on_load_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	rcfile_read();

  	msg_box(_("Information"), _("Configuration file loaded."));
}


GLADE_CB void
on_load_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	engine_stop();
	display_load_state_dbox();
	engine_start();
}


GLADE_CB void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	engine_stop();
	display_save_state_dbox();
	engine_start();
}


GLADE_CB void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	engine_stop();
	ti68k_state_load(params.sav_file);
  	engine_start();
}


GLADE_CB void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
#ifndef __IPAQ__
    if(dbg_on) return;

	engine_stop();
    ti68k_debug_break();
	engine_start();
#endif
}

GLADE_CB void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	engine_stop();
	ti68k_reset();
	if (dbg_on)
		on_quit1_activate(menuitem, user_data);
	else
	  	engine_start();
}

GLADE_CB void
on_upgrade_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	engine_stop();
  	display_set_tib_dbox();
	engine_start();
}


GLADE_CB void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	engine_stop();
	display_romversion_dbox (FALSE);
	engine_start();
}


GLADE_CB void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	int engine_was_stopped = engine_is_stopped();
	engine_stop();
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    	params.restricted = 0;
  	else
    	params.restricted = 1;
	if (!engine_was_stopped) engine_start();
}

GLADE_CB void
on_hw_protection1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    	params.hw_protect = 0;
  	else
    	params.hw_protect = 1;
}

GLADE_CB void
on_high_lcd_update1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	params.lcd_rate = 25;
	hid_lcd_rate_set();
}

GLADE_CB void
on_med_lcd_update1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	params.lcd_rate = 55;
	hid_lcd_rate_set();
}

GLADE_CB void
on_low_lcd_update1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	params.lcd_rate = 85;
	hid_lcd_rate_set();
}

GLADE_CB void
on_normal_view1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(GTK_CHECK_MENU_ITEM(menuitem)->active == TRUE) 
		hid_switch_normal_view();
}


GLADE_CB void
on_large_view1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(GTK_CHECK_MENU_ITEM(menuitem)->active == TRUE) 
		hid_switch_large_view();
}


GLADE_CB void
on_full_view1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(GTK_CHECK_MENU_ITEM(menuitem)->active == TRUE) 
		hid_switch_fullscreen();
}

GLADE_CB void
on_custom_view1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


GLADE_CB void
on_no_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
#ifndef __IPAQ__
	hid_switch_without_skin();
#endif
}


GLADE_CB void
on_default_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
#ifndef __IPAQ__
	hid_switch_with_skin();
#endif
}


GLADE_CB void
on_set_skin1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_skin_dbox();
}


GLADE_CB void
on_now1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_screenshot(NULL);
}


GLADE_CB void
on_screen_options1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_scroptions_dbox();
}

static void go_to_bookmark(const char *link);

GLADE_CB void
on_help1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gchar *path = g_strconcat(inst_paths.help_dir, _("Manual_en.html"), NULL);

	go_to_bookmark(path);
	g_free(path);
}


GLADE_CB void
on_manpage1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_manpage_dbox();
}

GLADE_CB void
on_bookmarks1_activate				   (GtkMenuItem		*menuitem,
										gpointer		user_data)
{
	GtkTooltipsData* data = gtk_tooltips_data_get(GTK_WIDGET(menuitem));
	go_to_bookmark(data->tip_text);
}


GLADE_CB void
on_bugreport1_activate				   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget *dialog;
	const gchar *message =
    "There are several ways to get in touch if you encounter a problem with TiEMu or if you have questions, suggestions, bug reports, etc:\n- if you have general questions or problems, please consider the users' mailing list first (http://tiemu-users@list.sf.net).\n- if you want to discuss about TiEmu, you can use the TiEmu forum (http://sourceforge.net/forum/?group_id=23169).\n- for bug reports, use the 'Bug Tracking System' (http://sourceforge.net/tracker/?group_id=23169).\n\nBefore e-mailing the TiEmu team, make sure you have read the manual and/or the FAQ....";
  
	dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
				  GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
				  message);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

GLADE_CB void
on_changelog1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_release_dbox ();
}


GLADE_CB void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_about_dbox ();
}


GLADE_CB void
on_infos1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_infos_dbox();
}

void exit_main_loop(void);

GLADE_CB void
on_exit_and_save_state1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gchar *basename;
	gchar *dot;

	// stop emulation engine
	engine_stop();

	// build name
	basename = g_path_get_basename(params.rom_file);
	dot = strrchr(basename, '.');
	if(dot != NULL)
		*dot = '\0';

	// set path
	g_free(params.sav_file);
	params.sav_file = g_strconcat(inst_paths.home_dir, basename, ".sav", NULL);
	g_free(basename);

	// save state
	ti68k_state_save(params.sav_file);

	// and config
	window_get_rect(main_wnd, &options3.calc.rect);
    rcfile_write();

	// exit
	exit_main_loop();
  	gtk_main_quit();
}


GLADE_CB void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
#if 0
	{
		//testing...
		GdkEvent* event = gdk_event_new(GDK_KEY_PRESS);

		event->key.type = GDK_KEY_PRESS;
		event->key.window = main_wnd->window;
		event->key.send_event = FALSE;
		event->key.time = GDK_CURRENT_TIME;
		event->key.state = GDK_LOCK_MASK;
		event->key.keyval = -1;
		event->key.length = 0;
		event->key.string = "";
		event->key.hardware_keycode = 0x14;
		event->key.group = 0;	
		
		gdk_event_put(event);
		while(gtk_events_pending()) gtk_main_iteration_do(FALSE);
		//gdk_event_free(event);
	}
#endif

	exit_main_loop();
	gtk_main_quit();
}

typedef void (*VCB) (void);

/*
  Display the GTK popup menu and configure some items
*/
GtkWidget* display_popup_menu(void)
{
	GladeXML *xml;
	GtkWidget *menu;
	GtkWidget *data;
	gchar *s;
  
	//menu = create_popup_menu();
	xml = glade_xml_new
	    (tilp_paths_build_glade("popup-2.glade"), "popup_menu",
	     PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);

	menu = glade_xml_get_widget(xml, "popup_menu");

	// set version
	data = glade_xml_get_widget(xml, "popup_menu_header");
	s = g_strdup_printf("TiEmu, version %s", TIEMU_VERSION);
	gtk_label_set_text(GTK_LABEL(GTK_BIN(data)->child), s);
	g_free(s);

	// init check buttons
	data = glade_xml_get_widget(xml, "recv_file_from_tiemu1");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.recv_file);

    data = glade_xml_get_widget(xml, "restrict1");
	g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_restrict_to_actual_speed1_activate, NULL);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.restricted);
	g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_restrict_to_actual_speed1_activate, NULL);

	data = glade_xml_get_widget(xml, "hw_protection1");
	g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_hw_protection1_activate, NULL);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.hw_protect);
	g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_hw_protection1_activate, NULL);

	// hide the custom view radio button
	data = glade_xml_get_widget(xml, "custom_view1");
	gtk_widget_hide(data);

	// init radio buttons
    switch(options.view)
    {
    case VIEW_NORMAL:
        data = glade_xml_get_widget(xml, "normal_view1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        break;
    case VIEW_LARGE:
        data = glade_xml_get_widget(xml, "large_view1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        break;
	case VIEW_FULL:
		data = glade_xml_get_widget(xml, "full_view1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
		break;
	case VIEW_CUSTOM:
		data = glade_xml_get_widget(xml, "custom_view1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
		break;
    default:
        break;
    }

	if(params.lcd_rate >= 10 && params.lcd_rate < 40)
	{
		data = glade_xml_get_widget(xml, "high_lcd_update1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
	}
	else if(params.lcd_rate >= 40 && params.lcd_rate < 70 || params.lcd_rate == -1)
	{
		data = glade_xml_get_widget(xml, "med_lcd_update1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
	}
	else if(params.lcd_rate >= 70 && params.lcd_rate < 100)
	{
		data = glade_xml_get_widget(xml, "low_lcd_update1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
	}

	// if debugger is open, blocks some items
	if(dbg_on)
	{
		data = glade_xml_get_widget(xml, "send_file_to_tiemu1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "recv_file_to_tiemu1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "link_cable1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "upgrade_calc1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "set_rom1");
		gtk_widget_set_sensitive(data, FALSE);

		if (reset_disabled)
		{
			data = glade_xml_get_widget(xml, "reset_calc1");
			gtk_widget_set_sensitive(data, FALSE);
		}
	}

	return menu;
}


/* */

static void go_to_bookmark(const char *link)
{
#ifdef __WIN32__
	HINSTANCE hInst;

	// Windows do the whole work for us, let's go...
	hInst = ShellExecute(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
	if((int)hInst <= 32)
	{
		msg_box("Error", "Unable to run ShellExecture extension.");
	}
#else
	// Kevin's list:
	// * /usr/bin/gnome-open (GNOME 2.6+ default browser, this really should be
	// first on the list to try, as this will honor the user's choice rather than
	// guessing an arbitrary one)
	// * /usr/bin/sensible-browser (Debian's browser script)
	// * /usr/bin/htmlview (old RHL/Fedora default browser script)
	// * /usr/bin/firefox (Mozilla Firefox)
	// * /usr/bin/mozilla (Mozilla Seamonkey)
	// * /usr/bin/konqueror (Konqueror)
	//
	gboolean result;
	char *apps[] = { 
			"/usr/bin/gnome-open",
			"/usr/bin/sensible-browser",
			"/usr/bin/htmlview",
			"/usr/bin/firefox",
			"/usr/bin/mozilla",
			"/usr/bin/konqueror",
	};
	gint i, n;

	n = sizeof(apps) / sizeof(char *);
	for(i = 0; i < n; i++)
	{
		gchar **argv = g_malloc0(3 * sizeof(gchar *));

		argv[0] = g_strdup(apps[i]);
		argv[1] = g_strdup(link);
		argv[2] = NULL;

		result = g_spawn_async(NULL, argv, NULL, 0, NULL, NULL, NULL, NULL);
		g_strfreev(argv);

		if(result != FALSE)
			break;
	}

	if (i == n) 
	{
		msg_box("Error", "Spawn error: do you have Mozilla installed ?");
	} 
#endif
	else 
	{
		GtkWidget *dialog;
		GTimer *timer;
		const gchar *message = "A web browser has been launched: this may take a while before it appears. If it is already launched, the page will be opened in the existing frame.";

		dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
					   GTK_MESSAGE_INFO,
					   GTK_BUTTONS_CLOSE, message);
		g_signal_connect_swapped(GTK_OBJECT(dialog), "response",
					 G_CALLBACK(gtk_widget_destroy),
					 GTK_OBJECT(dialog));
		gtk_widget_show_all(GTK_WIDGET(dialog));
		
		while(gtk_events_pending()) gtk_main_iteration();
		for(timer = g_timer_new(); g_timer_elapsed(timer, NULL) < 3.0;);

		g_timer_destroy(timer);
		gtk_widget_destroy(GTK_WIDGET(dialog));
	}
}
