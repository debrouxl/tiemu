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
#include "hid.h"
#include "help.h"
#include "release.h"
#include "about.h"
#include "infos.h"
#include "manpage.h"
#include "scroptions.h"
#include "tie_error.h"
#include "./debugger/dbg_mem.h"

#include "ti68k_int.h"
#include "ti68k_def.h"

extern int dbg_on;

/* 
   Display a popup menu: entry point used by hid.c (SDL)
*/
void gui_popup_menu(void)
{
	GtkWidget *menu;
	guint button;
	guint32 time;

	// check whether debugger is opened...
    if(dbg_on)
        return;

	// don't have time event due to SDL
	time = gtk_get_current_event_time();
	
	// don't have button event due to SDL
	button = 0;

	// halt emulation engine
	ti68k_engine_halt();

	// display popup menu
	menu = display_popup_menu();
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, button, time);
	gtk_widget_show(menu);
}

/*
  Display the GTK popup menu and configure some items
*/
static GtkWidget* display_popup_menu(void)
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
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.restricted);

	data = glade_xml_get_widget(xml, "sync1");
	gtk_widget_set_sensitive(data, FALSE);
	//gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), params.sync_one);

    // init radio buttons
    switch(params.grayplanes) {
    case 2:
        data = glade_xml_get_widget(xml, "2_colors1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        break;
    case 4:
        data = glade_xml_get_widget(xml, "4_colors1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        break;
    case 7:
        data = glade_xml_get_widget(xml, "7_colors1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        break;
    case 11:
        data = glade_xml_get_widget(xml, "11_colors1");
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data), TRUE);
        break;
    }


	return menu;
}


GLADE_CB void
on_popup_menu_cancel                   (GtkMenuShell    *menushell,
                                        gpointer         user_data)
{
	// never called (ESC)
	DISPLAY("on_popup_menu_cancel\n");
}


GLADE_CB gboolean
on_popup_menu_button_press_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
	GtkAllocation *alloc = &(widget->allocation);

    	//DISPLAY("%i %i %i %i\n", (gint)event->x, (gint)event->y, (gint)event->x_root, (gint)event->y_root);
    	//DISPLAY("%i %i %i %i\n", alloc->x, alloc->y, alloc->width, alloc->height);
    	if((event->x > alloc->width) || (event->y > alloc->height) ||
        	(event->x < alloc->x) || (event->y < alloc->y)) {
           	ti68k_engine_unhalt();
    	}

  	return TRUE;	//return FALSE;
}


GLADE_CB gboolean
on_popup_menu_key_press_event          (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
	// never called (ESC)
	DISPLAY("on_popup_menu_key_press_event\n");
  	return FALSE;
}


GLADE_CB void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_engine_unhalt();
}


GLADE_CB void
on_send_file_to_gtktiemu1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_tifile_dbox();
}


GLADE_CB void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_comm_dbox();
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
	ti68k_engine_unhalt();
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
	display_load_state_dbox();
}


GLADE_CB void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_save_state_dbox();
}


GLADE_CB void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_state_load(params.sav_file);
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    ti68k_debug_break();
	ti68k_engine_unhalt();
}


GLADE_CB void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_reset();
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_romversion_dbox ();
}


GLADE_CB void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    		params.restricted = 0;
  	else
    		params.restricted = 1;

  	ti68k_engine_unhalt();
}


GLADE_CB void
on_sync1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    /*
	if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    		params.sync_one = 0;
  	else
    		params.sync_one = 1;
*/
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_2_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    params.grayplanes = 2;
    ti68k_engine_unhalt();
}


GLADE_CB void
on_4_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    params.grayplanes = 4;
    ti68k_engine_unhalt();
}


GLADE_CB void
on_7_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    params.grayplanes = 7;
    ti68k_engine_unhalt();
}


GLADE_CB void
on_11_colors1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    params.grayplanes = 11;
    ti68k_engine_unhalt();
}


GLADE_CB void
on_normal_view1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_normal_view();
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_large_view1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_large_view();
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_full_screen1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_fullscreen();
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_no_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_without_skin();
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_default_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_with_skin();
  	ti68k_engine_unhalt();
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
  	ti68k_engine_unhalt();
}


GLADE_CB void
on_screen_options1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_scroptions_dbox();
}


GLADE_CB void
on_help1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_help_dbox();
}


GLADE_CB void
on_manpage1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_manpage_dbox();
}


GLADE_CB void
on_changelog1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_release_dbox();
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
	gchar *path;

	// build name
	basename = g_path_get_basename(params.rom_file);
	dot = strrchr(basename, '.');
	if(dot != NULL)
		*dot = '\0';

	// set path
	path = g_strconcat(inst_paths.base_dir, basename, ".sav", NULL);
	g_free(basename);

	// save state and and path
	ti68k_state_save(params.sav_file);
	rcfile_read();
	g_free(params.sav_file);
	params.sav_file = g_strdup(path);
	g_free(path);
    rcfile_write();

	// exits
  	gtk_main_quit();
}


GLADE_CB void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_main_quit();
}
