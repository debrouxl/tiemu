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

#include <gtk/gtk.h>

#include "user_cb.h"
#include "user_dbox.h"
#include "support.h"

static gint ret_val = 0;

/* Dialog box, type 1 */
gint user1_box(const char *title, char *message, const char *button1)
{
  GtkWidget *dbox;
  GtkWidget *w;
  ret_val = 0;

  dbox = create_user1_dbox();
  
  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label2");
  gtk_label_set_text(GTK_LABEL(w), message);

  w = lookup_widget(dbox, "label3");
  gtk_label_set_text(GTK_LABEL(w), button1);
  
  gtk_widget_show_all(dbox);

  while(!ret_val)
    {
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
  gtk_widget_destroy(dbox);
  
  return ret_val;  
}

/* This a general function used to display a message with 2 buttons */
gint user2_box(const char *title, char *message, 
	       const char *button1, const char *button2)
{
  GtkWidget *dbox;
  GtkWidget *w;
  ret_val = 0;

  dbox = create_user2_dbox();
  
  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label3");
  gtk_label_set_text(GTK_LABEL(w), message);

  w = lookup_widget(dbox, "label4");
  gtk_label_set_text(GTK_LABEL(w), button1);

  w = lookup_widget(dbox, "label5");
  gtk_label_set_text(GTK_LABEL(w), button2);
  
  gtk_widget_show_all(dbox);

  while(!ret_val)
    {
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
  gtk_widget_destroy(dbox);
  
  return ret_val;  
}

/* This a general function used to display a message with 3 buttons */
gint user3_box(const char *title, char *message, 
	       const char *button1, const char *button2, const char *button3)
{
  GtkWidget *dbox;
  GtkWidget *w;
  ret_val = 0;

  dbox = create_user3_dbox();
  
  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label6");
  gtk_label_set_text(GTK_LABEL(w), message);

  w = lookup_widget(dbox, "label7");
  gtk_label_set_text(GTK_LABEL(w), button1);

  w = lookup_widget(dbox, "label8");
  gtk_label_set_text(GTK_LABEL(w), button2);

  w = lookup_widget(dbox, "label9");
  gtk_label_set_text(GTK_LABEL(w), button3);
  
  gtk_widget_show_all(dbox);

  while(!ret_val)
    {
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
  gtk_widget_destroy(dbox);
  
  return ret_val;  
}

/* First button clicked */
void
user1_ok_button_clicked                  (GtkButton       *button,
					  gpointer         user_data)
{
  ret_val = 1;
}



void
user2_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  ret_val = 2;
}

void
user3_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  ret_val = 3;
}
