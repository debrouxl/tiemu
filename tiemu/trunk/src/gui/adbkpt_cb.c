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

#include <string.h>

#include <gtk/gtk.h>

#include "adbkpt_cb.h"
#include "adbkpt_dbox.h"
#include "support.h"

#include "struct.h"
#include "extern.h"
#include "hid.h"
#include "ti68k_int.h"
#include "bkpts.h"

DATA_BKPT db;
static gint single_or_range = 1;
extern GtkWidget *data_bkpt_dbox;

gint display_add_data_bkpt_dbox(void)
{
  db.mode = BK_READ | BK_BYTE;
  db.address  = 0x0;
  db.address2 = 0x0;
  db.id = -1;

  gtk_widget_show_all(create_add_data_bkpt_dbox());
  return 0;
}


/* Read toggle button of the 'Add data breakpoint' dbox */
void
on_radiobutton19_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  db.mode |= BK_READ;
  db.mode &= ~BK_WRITE;
}

/* Write toggle button of the 'Add data breakpoint' dbox */
void
on_radiobutton20_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  db.mode |= BK_WRITE;
  db.mode &= ~BK_READ;
}

/* Either toggle button of the 'Add data breakpoint' dbox */
void
on_radiobutton21_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  db.mode |= BK_READ;
  db.mode |= BK_WRITE;
}

/* 'Single address' toggle button of the 'Add data breakpoint' dbox */
void
on_radiobutton22_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  single_or_range = 1;
}

/* 'Range address' toggle button of the 'Add data breakpoint' dbox */
void
on_radiobutton23_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  single_or_range = 2;
}


/* Combo box of the 'Add data breakpoint' dbox */
void
on_combo_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *buffer;

  buffer = gtk_editable_get_chars(editable, 0, -1);
  db.mode &= ~(BK_BYTE | BK_WORD | BK_LONG);
  if(!strcmp(buffer, "Byte"))
    db.mode |= BK_BYTE;
  else if(!strcmp(buffer, "Word"))
    db.mode |= BK_WORD;
  else if(!strcmp(buffer, "Long"))
    db.mode |= BK_LONG;
  else
    db.mode |= BK_BYTE;
}


/* Range 'Range' text field of the 'Add data breakpoint' dbox */
void
on_entry2_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *buffer;
  
  buffer = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(buffer, "0x%06x", &(db.address)) < 1)
    db.address = 0x0;
  //DISPLAY("addr: $%06X\n", db.address);
}


/* Left 'Range' text field of the 'Add data breakpoint' dbox */
void
on_entry3_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *buffer;
  
  buffer = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(buffer, "0x%06x", &(db.address2)) < 1)
    db.address2 = 0x0;
  //DISPLAY("addr: $%06X\n", db.address2);
}


/* 'Single address' text field of the 'Add data breakpoint' dbox */
void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *buffer;
  
  buffer = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(buffer, "0x%06x", &(db.address)) < 1)
    db.address = 0x0;
  //DISPLAY("addr: $%06X\n", db.address);
}


/* OK button of the 'Add data breakpoint' dialog box */
void
on_button_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  gint i;
  DATA_BKPT *s;

  /* Place a breakpoint */
  if(single_or_range == 1)
    {
      i = ti68k_bkpt_set_access(db.address, db.mode);
      //DISPLAY("Breakpoint access id: %i\n", i);
      s = (DATA_BKPT *)g_malloc(sizeof(DATA_BKPT));
      s->id = i;
      s->mode = db.mode;
      s->address = db.address;
      bkpt_access_list = g_list_append(bkpt_access_list, s);
      //DISPLAY("list_length: %i\n", g_list_length(bkpt_access_list));
    }
  else
    {
      /*
      i = setBreakpointAccessRange(db.address, db.address2, db.mode);
      //DISPLAY"Breakpoint access id: %i\n", i);
      s = (DATA_BKPT *)g_malloc(sizeof(DATA_BKPT));
      s->id = i;
      s->mode = db.mode;
      s->address  = db.address;
      s->address2 = db.address2;
      bkpt_access_list = g_list_append(bkpt_access_list, s);
      //DISPLAY("%i\n", g_list_length(bkpt_access_list));
      */
    }

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "adbkpt_dbox"));
}

/* Cancel button of the 'Add data breakpoint' dialog box */
void
on_button_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "adbkpt_dbox"));
}

void
on_add_data_bkpt_dbox_destroy          (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}
