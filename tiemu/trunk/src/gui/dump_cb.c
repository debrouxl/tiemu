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

#include "dump_cb.h"
#include "dump_dbox.h"
#include "support.h"

#include "interface.h"
#include "struct.h"
#include "hid.h"
#include "pbars.h"

gint display_ramdump_fileselection(void)
{
  GtkWidget *dbox;
  GtkWidget *w;

  dbox = create_ramdump_fileselection();
  w = lookup_widget(dbox, "ramdump_fileselection");
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(w),
				  "ram_file.hex");
  gtk_file_selection_complete(GTK_FILE_SELECTION(w),
			      "*.hex");
  gtk_widget_show_all(dbox);

  return 0;
}

gint display_romdump_fileselection(void)
{
  GtkWidget *dbox;
  GtkWidget *w;

  dbox = create_romdump_fileselection();
  w = lookup_widget(dbox, "romdump_fileselection");
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(w),
				  "rom_file.hex");
  gtk_file_selection_complete(GTK_FILE_SELECTION(w),
			      "*.hex");
  gtk_widget_show_all(dbox);

  return 0;
}

gint display_wait_conversion_dbox(void)
{
  gtk_widget_show_all(create_wait_conversion_dbox());
  return 0;
}

/* Callbacks */

void
on_ramdump_fileselection_destroy       (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}


void
on_ramd_ok_button2_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  GtkWidget *f;

  gtk_widget_show((w = create_wait_conversion_dbox()));
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  f = lookup_widget(GTK_WIDGET(button), "ramdump_fileselection");
  ti68k_dumpRam(gtk_file_selection_get_filename (GTK_FILE_SELECTION(w)));
  gtk_widget_destroy(w);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  gtk_widget_destroy(f);
}


void
on_ramd_cancel_button2_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "ramdump_fileselection"));
}


void
on_romd_fileselection_destroy          (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}


void
on_romd_ok_button2_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  GtkWidget *f;

  gtk_widget_show((w = create_wait_conversion_dbox()));
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  f = lookup_widget(GTK_WIDGET(button), "romdump_fileselection");
  ti68k_dumpRom(gtk_file_selection_get_filename (GTK_FILE_SELECTION(f)));
  gtk_widget_destroy(w);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  gtk_widget_destroy(f);
}


void
on_romd_cancel_button2_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "romdump_fileselection"));
}

/* */
