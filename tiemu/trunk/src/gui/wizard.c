/* Hey EMACS -*- linux-c -*- */
/* $Id: wizard_cb.c 187 2004-05-14 14:22:36Z roms $ */

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
#include <glade/glade.h>

#include "intl.h"

static gint action = 1;
gchar *wizard_rom = NULL;
gint wizard_ok = FALSE;

gint display_wizard_dbox(void)
{
    display_step1_dbox();
}

gint display_step1_dbox(void)
{
    GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;

    xml = glade_xml_new
	    (tilp_paths_build_glade("wizard-2.glade"), "step1_dbox", PACKAGE);
	if (!xml)
		g_error(_("comm.c: GUI loading failed !\n"));
	glade_xml_signal_autoconnect(xml);

	dbox = glade_xml_get_widget(xml, "step1_dbox");

    data = glade_xml_get_widget(xml, "applybutton1");
    gtk_button_set_label(data, "<= Back");
    data = glade_xml_get_widget(xml, "applybutton2");
    gtk_button_set_label(data, "<= Cancel");
    data = glade_xml_get_widget(xml, "applybutton3");
    gtk_button_set_label(data, "Next =>");

    result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
		ti68k_unhalt();
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}

/*
gint display_msg1_dbox(void)
{
  gtk_widget_show_all(create_msg1_dbox());
  return 0;
}

gint display_step3_dbox(void)
{
  gtk_widget_show_all(create_step3_dbox());
  return 0;
}

gint display_wait_dbox(void)
{
  gtk_widget_show_all(create_wait_dbox());
  return 0;
}


void
on_flashfile_fileselection_destroy     (GtkObject       *object,
                                        gpointer         user_data)
{

}


void
on_flashfile_ok_button2_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  GtkWidget *f;
  const gchar *filename;
  gchar buffer[1024];
  gchar *ext = NULL;

  f = lookup_widget(GTK_WIDGET(button), "flash_fileselection");

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(f));
  ext = strrchr(filename, '.');
  if(str_cmp(ext, ".tib") && str_cmp(ext, ".89u") && str_cmp(ext, ".9xu"))
    {
      msg_box(_("Error"), _("Invalid file"));
      gtk_widget_destroy(f);
      gtk_widget_show(create_step1_dbox());     
      return;
    }

  gtk_widget_show((w = create_wait_dbox()));
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  ti68k_convertTibToRom(filename, buffer);
  wizard_rom = g_strdup(buffer);
  gtk_widget_destroy(w);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  gtk_widget_destroy(f);
  gtk_widget_show(create_step3_dbox());
}


void
on_flashfile_cancel_button2_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "flash_fileselection"));
}


void
step1_on_radiobutton1_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  action = 1;
}


void
step1_on_radiobutton2_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  action = 2;
}


void
step1_on_radiobutton3_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  action = 3;
}


void
step1_b1_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
step1_b2_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  switch(action)
    {
    case 1: gtk_widget_show(create_msg1_dbox());
      break;
    case 2: gtk_widget_show(create_flash_fileselection());
      break;
    case 3: gtk_widget_show(create_romfile_fileselection());
      break;
    default:
      break;
    }
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "step1_dbox"));
}


void
step1_b3_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  exit(0);
}


void
msg1_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "msg1_dbox"));
  exit(0);
}



void
step3_b1_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "step3_dbox"));
  gtk_widget_show(create_step1_dbox());
}


void
step3_b2_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "step3_dbox"));
  wizard_ok = TRUE;
}


void
step3_b3_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  exit(0);
}




void
wait_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_romfile_fileselection_destroy       (GtkObject       *object,
                                        gpointer         user_data)
{

}


void
on_romfile_cancel_button2_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "romfile_fileselection"));
  exit(0);
}


void
on_romfile_ok_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *f;
  const gchar *filename;
  gchar *ext = NULL;

  f = lookup_widget(GTK_WIDGET(button), "romfile_fileselection");

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(f));
  ext = strrchr(filename, '.');
  if(str_cmp(ext, ".rom"))
    {
      msg_box(_("Error"), _("Invalid file"));
      gtk_widget_destroy(f);
      gtk_widget_show(create_step1_dbox());     
      return;
    }
  wizard_rom = g_strdup(filename);

  gtk_widget_destroy(f);
  gtk_widget_show(create_step3_dbox());
}

*/