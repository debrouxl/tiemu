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

#include "colors_cb.h"
#include "colors_dbox.h"
#include "support.h"

#include "struct.h"
#include "hid.h"

/***************************/
/* 'Number of colors' dbox */
/***************************/

gint tmp_gray_scales;

gint display_ncolors_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *w;

  dbox = create_ncolors_dbox();
  tmp_gray_scales = (options.params)->n_grayplanes;
  
  w = lookup_widget(dbox, "radiobutton24");
  if(tmp_gray_scales == 0)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(w), TRUE);

  w = lookup_widget(dbox, "radiobutton25");
  if(tmp_gray_scales == 2)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(w), TRUE);

  w = lookup_widget(dbox, "radiobutton26");
  if(tmp_gray_scales == 5)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(w), TRUE);

  w = lookup_widget(dbox, "radiobutton27");
  if(tmp_gray_scales == 11)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(w), TRUE);

  gtk_widget_show_all(dbox);
  return 0;
}

void
on_radiobutton24_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_gray_scales = 0;
}


void
on_radiobutton25_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_gray_scales = 2;
}


void
on_radiobutton26_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_gray_scales = 5;
}


void
on_radiobutton27_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_gray_scales = 11;
}


void
on_button29_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  (options.params)->n_grayplanes = tmp_gray_scales;
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "colors_dbox"));
}


void
on_button30_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "colors_dbox"));
}


void
on_ncolors_dbox_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

/* */
