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

#include "dbgopt_cb.h"
#include "dbgopt_dbox.h"
#include "support.h"

//#include "platform.h"
#include "struct.h"


static gint tmp_stack_lines;
static gint tmp_mem_lines;
static gint tmp_code_lines;

int display_dbgopt_dbox()
{
  GtkWidget *dbox;
  GtkWidget *w;

  dbox = create_dbgopt_dbox();
  
  w = lookup_widget(dbox, "spinbutton1");
  tmp_code_lines = options.code_lines;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), options.stack_lines);
  w = lookup_widget(dbox, "spinbutton2");
  tmp_mem_lines = options.mem_lines;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), options.mem_lines);
  w = lookup_widget(dbox, "spinbutton3");
  tmp_stack_lines = options.stack_lines;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), options.stack_lines);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_dbgopt_ok_button_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  options.code_lines = tmp_code_lines;
  options.mem_lines = tmp_mem_lines;
  options.stack_lines = tmp_stack_lines;

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "dbgopt_dbox"));
}


void
on_dbgopt_cancel_button_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "dbgopt_dbox"));
}


void
on_dbgopt_spinbutton1_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *w;

  w = lookup_widget(GTK_WIDGET(editable), "spinbutton1");
  tmp_code_lines = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w));
}


void
on_dbgopt_spinbutton2_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *w;

  w = lookup_widget(GTK_WIDGET(editable), "spinbutton2");
  tmp_mem_lines = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w));
}


void
on_dbgopt_spinbutton3_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *w;

  w = lookup_widget(GTK_WIDGET(editable), "spinbutton3");
  tmp_stack_lines = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w));
}

void
on_dbgopt_dbox_destroy                 (GtkObject       *object,
                                        gpointer         user_data)
{
  //ti68k_unhalt();
}

/* */
