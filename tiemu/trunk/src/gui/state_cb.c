/* Hey EMACS -*- linux-c -*- */
/* $Id: hid.c 163 2004-05-13 06:40:57Z roms $ */

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

#include <string.h>

#include <gtk/gtk.h>

#include "state_cb.h"
#include "state_dbox.h"
#include "support.h"

#include "interface.h"
#include "struct.h"
#include "rcfile.h"
#include "hid.h"
#include "msg_cb.h"
#include "paths.h"

/**************/
/* Load state */
/**************/

gint display_fs_load_state(void)
{
  GtkWidget *dbox;
   
  dbox = create_fs_load_state();
  
  gtk_file_selection_complete(GTK_FILE_SELECTION(dbox), 
			      "*.sav");
  
  gtk_widget_show_all(dbox);
  return 0;
}


void
load_state_on_ok_button1_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  const gchar *s;

  w = lookup_widget(GTK_WIDGET(button), "fs_load_state");

  s = gtk_file_selection_get_filename(GTK_FILE_SELECTION(w));
  g_free((options.params)->ram_file);
  (options.params)->ram_file =g_strdup(s);
  ti68k_loadState((options.params)->ram_file);

  gtk_widget_destroy(w);
}


void
load_state_on_cancel_button1_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "fs_load_state"));
}


/**************/
/* Save state */
/**************/

gint display_fs_save_state(void)
{
  GtkWidget *dbox;
  gchar *s;
   
  dbox = create_fs_save_state();
  
  s = (options.params)->ram_file;
#ifdef __LINUX__
  if(!strcmp(s, ""))
    s = g_strconcat("~/", CONF_DIR, "/", "default.sav", NULL);
#else
  if(!strcmp(s, ""))
    s = g_strconcat("default.sav", NULL);
#endif
  gtk_file_selection_complete(GTK_FILE_SELECTION(dbox), s);
  
  gtk_widget_show_all(dbox);
  if(!strcmp((options.params)->ram_file, ""))
    g_free(s);
  return 0;
}


void
save_state_on_ok_button2_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  const gchar *s;

  w = lookup_widget(GTK_WIDGET(button), "fs_save_state");

  s = gtk_file_selection_get_filename(GTK_FILE_SELECTION(w));
  g_free((options.params)->ram_file);
  (options.params)->ram_file = g_strdup(s);
  ti68k_saveState((options.params)->ram_file);
  if(!rcfile_exist())
    {
      rcfile_write();

#if defined(__LINUX__)
      msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in ~/.gtktiemu)."));
#elif defined(__WIN32__)
      msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in gtktiemu.ini)."));
#endif
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }

  gtk_widget_destroy(w);
}


void
save_state_on_cancel_button2_pressed   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "fs_save_state"));
}



void
on_fs_load_state_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}


void
on_fs_save_state_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

