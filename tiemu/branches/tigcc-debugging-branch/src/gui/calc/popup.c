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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdlib.h>

#include "intl.h"
#include "support.h"
#include "struct.h"
#include "version.h"
#include "popup.h"
#include "paths.h"
#include "engine.h"
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
#include "dbg_mem.h"

#include "ti68k_int.h"
#include "ti68k_def.h"

GLADE_CB void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	fprintf(stdout, _("* TiEmu version %s (cables=%s, files=%s, calcs=%s)\n"),
	     TIEMU_VERSION, ticable_get_version(), tifiles_get_version(),
	     ticalc_get_version());
}


GLADE_CB void
on_send_file_to_gtktiemu1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
	display_tifile_dbox();
	ti68k_engine_start();
}


GLADE_CB void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
	display_comm_dbox();
	ti68k_engine_start();
}


GLADE_CB void
on_save_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	rcfile_write();

#if defined(__LINUX__)
  	msg_box(_("Information"), 
	  _("Configuration file saved (in ~/.tiemu)."));
#elif defined(__WIN32__)
  	msg_box(_("Information"), 
	  _("Configuration file saved (in tiemu.ini)."));
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
	ti68k_engine_stop();
	display_load_state_dbox();
	ti68k_engine_start();
}


GLADE_CB void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
	display_save_state_dbox();
	ti68k_engine_start();
}


GLADE_CB void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
	ti68k_state_load(params.sav_file);
  	ti68k_engine_start();
}


GLADE_CB void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(dbg_on) return;
	ti68k_engine_stop();
    ti68k_debug_break();
	ti68k_engine_start();
}


GLADE_CB void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
	ti68k_reset();
  	ti68k_engine_start();
}

GLADE_CB void
on_upgrade_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
  	display_set_tib_dbox();
	ti68k_engine_start();
}


GLADE_CB void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(dbg_run) return;
	ti68k_engine_stop();
	display_romversion_dbox (FALSE);
	ti68k_engine_start();
}


GLADE_CB void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    	params.restricted = 0;
  	else
    	params.restricted = 1;
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
on_normal_view1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    //printf("normal !\n");
	//ti68k_engine_stop();
	//hid_switch_normal_view();
  	//ti68k_engine_start();
}


GLADE_CB void
on_large_view1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    //printf("large !\n");
	//ti68k_engine_stop();
	//hid_switch_large_view();
  	//ti68k_engine_start();
}


GLADE_CB void
on_full_screen1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    //printf("full !\n");
	//ti68k_engine_stop();
	//hid_switch_fullscreen();
  	//ti68k_engine_start();
}


GLADE_CB void
on_no_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_without_skin();
}


GLADE_CB void
on_default_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_with_skin();
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


GLADE_CB void
on_exit_and_save_state1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gchar *basename;
	gchar *dot;
	extern int exit_loop;

	// stop emulation engine
	ti68k_engine_stop();

	// build name
	basename = g_path_get_basename(params.rom_file);
	dot = strrchr(basename, '.');
	if(dot != NULL)
		*dot = '\0';

	// set path
	g_free(params.sav_file);
	params.sav_file = g_strconcat(inst_paths.home_dir, basename, ".sav", NULL);
	g_free(basename);

	// save state and and config
	ti68k_state_save(params.sav_file);
    rcfile_write();

	// exit
	exit_loop = !0;
  	gtk_main_quit();
}


GLADE_CB void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	extern int exit_loop;

	exit_loop = !0;
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
    data = glade_xml_get_widget(xml, "restrict1");
	g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_restrict_to_actual_speed1_activate, NULL);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.restricted);
	g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_restrict_to_actual_speed1_activate, NULL);

	data = glade_xml_get_widget(xml, "hw_protection1");
	g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_hw_protection1_activate, NULL);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.hw_protect);
	g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_hw_protection1_activate, NULL);

    switch(options.view)
    {
    case VIEW_NORMAL:
        data = glade_xml_get_widget(xml, "normal_view1");
        g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_normal_view1_activate, NULL);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_normal_view1_activate, NULL);
        break;
    case VIEW_LARGE:
        data = glade_xml_get_widget(xml, "large_view1");
        g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_large_view1_activate, NULL);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_large_view1_activate, NULL);
        break;
    case VIEW_FULL:
        data = glade_xml_get_widget(xml, "full_view1");
        g_signal_handlers_block_by_func(GTK_OBJECT(data), (VCB)on_full_screen1_activate, NULL);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        g_signal_handlers_unblock_by_func(GTK_OBJECT(data), (VCB)on_full_screen1_activate, NULL);
        break;
    default:
        break;
    }

	// if debugger is open, blocks some items
	if(dbg_run)
	{
		data = glade_xml_get_widget(xml, "send_file_to_gtktiemu1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "link_cable1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "calculator_state1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "reset_calc1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "upgrade_calc1");
		gtk_widget_set_sensitive(data, FALSE);

		data = glade_xml_get_widget(xml, "set_rom1");
		gtk_widget_set_sensitive(data, FALSE);
	}

	// 
	data = glade_xml_get_widget(xml, "normal_view1");
	gtk_widget_set_sensitive(data, FALSE);
	data = glade_xml_get_widget(xml, "large_view1");
	gtk_widget_set_sensitive(data, FALSE);
	data = glade_xml_get_widget(xml, "full_view1");
	gtk_widget_set_sensitive(data, FALSE);


	//while(gtk_events_pending()) gtk_main_iteration();

	return menu;
}


/* */

static void go_to_bookmark(const char *link)
{
	gboolean result;
	gchar **argv = g_malloc0(3 * sizeof(gchar *));

#ifdef __LINUX__
	argv[0] = g_strdup("/usr/bin/mozilla");
#else
	argv[0] = g_strdup("C:\\Program Files\\Internet Explorer\\IExplore.exe");
#endif
	argv[1] = g_strdup(link);
	argv[2] = NULL;

	result = g_spawn_async(NULL, argv, NULL, 0, NULL, NULL, NULL, NULL);
	g_strfreev(argv);

	if (result == FALSE) 
	{
		msg_box("Error", "Spawn error: do you have Mozilla/IE installed ?");
	} 
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
