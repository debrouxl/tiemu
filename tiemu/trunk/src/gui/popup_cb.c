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

#include "popup_cb.h"
#include "popup_dbox.h"
#include "support.h"

#include "version.h"
#include "struct.h"
#include "interface.h"

/*
  Display the GTK popup menu and configure some items
*/
GtkWidget* display_popup_menu(void)
{
	GtkWidget *menu;
	GtkWidget *w;
	gchar *s;
  
	menu = create_popup_menu();

	// set version
	w = lookup_widget(menu, "popup_menu_header");
	s = g_strdup_printf("TiEmu, version %s", TIEMU_VERSION);
	gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), s);
	g_free(s);

	// init check buttons
	w = lookup_widget(menu, "sync1");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(w), (options.params)->sync_one);

	w = lookup_widget(menu, "restrict1");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(w), !((options.params)->background));

	return menu;
}

void
on_popup_menu_cancel                   (GtkMenuShell    *menushell,
                                        gpointer         user_data)
{
	// never called (ESC)
	DISPLAY("on_popup_menu_cancel\n");
}


gboolean
on_popup_menu_button_press_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
	GtkAllocation *alloc = &(widget->allocation);

    	//DISPLAY("%i %i %i %i\n", (gint)event->x, (gint)event->y, (gint)event->x_root, (gint)event->y_root);
    	//DISPLAY("%i %i %i %i\n", alloc->x, alloc->y, alloc->width, alloc->height);
    	if((event->x > alloc->width) || (event->y > alloc->height) ||
        	(event->x < alloc->x) || (event->y < alloc->y)) {
           	ti68k_unhalt();
    	}

  	return TRUE;	//return FALSE;
}


gboolean
on_popup_menu_key_press_event          (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
	// never called (ESC)
	DISPLAY("on_popup_menu_key_press_event\n");
  	return FALSE;
}


void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_unhalt();
}


void
on_send_file_to_gtktiemu1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	//test_sendfile();
	display_tifile_fileselection();
}


void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_linktype_dbox();
}


void
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


void
on_load_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	rcfile_read();

  	msg_box(_("Information"), _("Configuration file loaded."));
}


void
on_load_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_fs_load_state();
}


void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_fs_save_state();
}


void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_loadState((options.params)->ram_file);
  	ti68k_unhalt();
}


void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gint i;
  	CODE_BKPT *s;

  	// Suspend emulation core
  	ti68k_halt();
  
  	// Remove all breakpoints so that we can 'single step'
  	// Bkpts are restored when the debugger is exited
  	for(i=0; i<g_list_length(bkpt_address_list); i++)
	{	
      		s = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
      		ti68k_delBreakpointAddress(s->id);
    	}

  	// Launch debugger
  	display_debugger_dbox();
}


void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_reset();
  	ti68k_unhalt();
}


void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	//display_rom_fileselection ();
	display_romversion_dbox ();
}


void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_sync1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    		(options.params)->sync_one = 0;
  	else
    		(options.params)->sync_one = 1;

  	ti68k_unhalt();
}


void
on_number_of_colors1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_2_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_4_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_7_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_blurry1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_normal_view__1x_1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_normal_view();
  	ti68k_unhalt();
}


void
on_large_view__x2_1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_large_view();
  	ti68k_unhalt();
}


void
on_full_screen1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_fullscreen();
  	ti68k_unhalt();
}


void
on_none1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_without_skin();
  	ti68k_unhalt();
}


void
on_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_switch_with_skin();
  	ti68k_unhalt();
}


void
on_set_skin1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_skinlist_dbox();
}


void
on_now__1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	hid_screenshot(NULL);
  	ti68k_unhalt();
}


void
on_rbm_options1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_scopt_dbox();
}


void
on_help3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_help_dbox();
}


void
on_manpage1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_manpage_dbox();
}


void
on_changelog1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_startup_dbox();
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_about_dbox ();
}


void
on_infos1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_infos_dbox();
}


void
on_exit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_saveState((options.params)->ram_file);
  	if(!rcfile_exist())
    		rcfile_write();

  	ti68k_exit();
  	exit(0);
}


void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ti68k_exit();
  	exit(0);
}
