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

#include "msg_cb.h"
#include "msg_dbox.h"
#include "support.h"

#include "struct.h"
#include "hid.h"

static gboolean b = TRUE;

/* Msg box with halting */
void msg_box(const gchar *title, gchar *message)
{
  GtkWidget *dbox;
  GtkWidget *w;

  b = TRUE;
  dbox = create_msg_dbox();

  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label2");
  gtk_label_set_text(GTK_LABEL(w), message);

  gtk_widget_show_all(dbox);
}

/* Msg box without halting */
void msg_box2(const gchar *title, gchar *message)
{
  GtkWidget *dbox;
  GtkWidget *w;

  b = FALSE;
  dbox = create_msg_dbox();

  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label2");
  gtk_label_set_text(GTK_LABEL(w), message);

  gtk_widget_show_all(dbox);
}

void
msg_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "msg_dbox"));
}

void
on_msg_dbox_destroy                    (GtkObject       *object,
                                        gpointer         user_data)
{
  if(b)
    ti68k_unhalt(); // free running
}
