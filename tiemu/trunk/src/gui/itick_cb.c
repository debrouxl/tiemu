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

#include "itick_cb.h"
#include "itick_dbox.h"
#include "support.h"

#include "struct.h"
#include "hid.h"

gint tmp_cyclerate, tmp_tickrate, tmp_itick;

gint display_tickrate_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *w;
  gchar buffer[MAXCHARS];

  dbox = create_tickrate_dbox();
  
  w = lookup_widget(dbox, "entry7");
  tmp_tickrate = (options.params)->tick_rate;
  g_snprintf(buffer, MAXCHARS, "%i", (options.params)->tick_rate);
  gtk_entry_set_text(GTK_ENTRY(w), buffer);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_entry7_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;
  
  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "%i", &tmp_tickrate) < 1)
    tmp_tickrate = 40000;
  
  g_free(s);
}

void
on_button33_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  (options.params)->tick_rate = tmp_tickrate;
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "tickrate_dbox"));
}


void
on_button34_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "tickrate_dbox"));
}


void
on_tickrate_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

/*--*/

gint display_cyclerate_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *w;
  gchar buffer[MAXCHARS];

  dbox = create_cyclerate_dbox();
  
  w = lookup_widget(dbox, "entry8");
  tmp_cyclerate = (options.params)->cycle_rate;
  g_snprintf(buffer, MAXCHARS, "%i", (options.params)->cycle_rate);
  gtk_entry_set_text(GTK_ENTRY(w), buffer);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_entry8_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "%i", &tmp_cyclerate) < 1)
    tmp_cyclerate = 40000;

  g_free(s);
}


void
on_button35_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  (options.params)->cycle_rate = tmp_cyclerate;

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "cyclerate_dbox"));
}


void
on_button36_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "cyclerate_dbox"));
}


void
on_cyclerate_dbox_destroy              (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

/* -- */

gint display_itick_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *w;
  gchar buffer[MAXCHARS];

  dbox = create_itick_dbox();
  
  w = lookup_widget(dbox, "entry9");
  tmp_itick = (options.params)->i_tick;
  g_snprintf(buffer, MAXCHARS, "%i", (options.params)->i_tick);
  gtk_entry_set_text(GTK_ENTRY(w), buffer);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_entry9_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "%i", &tmp_itick) < 1)
    tmp_itick = 640;

  g_free(s);
}


void
on_button41_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  (options.params)->i_tick = tmp_itick;

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "itick_dbox"));
}


void
on_button42_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "itick_dbox"));
}


void
on_itick_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

/* */



