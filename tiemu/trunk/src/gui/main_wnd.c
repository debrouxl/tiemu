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

#include <gtk/gtk.h>

#ifndef __WIN32__
# include <SDL/SDL.h>
#else
# include "C:\SDL-1.2.7\include\SDL.h"
#endif

#include "tiemu_cb.h"
#include "refresh.h"

int halt(void);

static GtkWidget *window;

/* 
   Display a popup menu: entry point used by ticalc.c (SDL)
*/
void gui_popup_menu(void)
{
	guint32 time;

  time = gtk_get_current_event_time();
  halt();
  gtk_menu_popup(GTK_MENU(display_popup_menu()),
		 NULL, NULL, NULL, NULL,
		 GDK_3BUTTON_PRESS, time);
}


/* 
   GTK callback: displays a popup menu if the auxiliary 
   window has been enabled (#define EXT_WIN). Used by GTK.
*/
gboolean
button_press_event        (GtkWidget       *widget,
                           GdkEventButton  *event,
                           gpointer         user_data)
{
  GtkWidget *menu;

  if(event->button == 3)
    {
      halt();
      menu = display_popup_menu();
      gtk_widget_grab_focus(menu);
      gdk_event_get();
      gtk_menu_popup(GTK_MENU(menu), 
		     NULL, NULL, NULL, NULL, 
		     event->button, event->time);
    }
  
  return FALSE;
}

/* 
   The GTK auxiliary window: may not be shown but used for 
   using GTK with SDL.
   A better way should be to use a GtkSDL plugin...
*/
int gtk_main_window(void)
{
  GtkWidget *eventbox;
  GtkWidget *label;
  
  /* The main window */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(window), "TiEmu");
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 50);

  /* The event box */
  eventbox = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER (window), eventbox);
  GTK_WIDGET_SET_FLAGS (eventbox, GTK_CAN_FOCUS);
  GTK_WIDGET_SET_FLAGS (eventbox, GTK_CAN_DEFAULT);
  gtk_widget_set_events (eventbox, GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS_MASK);
  gtk_widget_show (eventbox);

  label = gtk_label_new ("Click here to display a menu");
  gtk_container_add (GTK_CONTAINER (eventbox), label);
  gtk_widget_show (label);
  
  g_signal_connect ((gpointer)window, "destroy",
		    G_CALLBACK(gtk_main_quit),
		    NULL);
  g_signal_connect ((gpointer)eventbox, "button_press_event",
		    G_CALLBACK(button_press_event),
		    NULL);

  //gtk_widget_grab_focus (eventbox);
  gtk_widget_grab_default (eventbox);

#ifdef EXT_WIN  
  gtk_widget_show_all(window);
#endif

  return 0;
}
