/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 187 2004-05-14 14:22:36Z roms $ */

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
#endif				/*  */

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "skinops.h"
#include "interface.h"
#include "screenshot.h"

static ScrOptions tmp_options;

static GtkWidget *label = NULL;
void refresh_label(void);

gint display_scroptions_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("scroptions-2.glade"), 
		 "scroptions_dbox",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "scroptions_dbox");
	memcpy(&tmp_options, &options2, sizeof(ScrOptions));
	
	switch (options2.format)
	{
	case IMG_JPG: 
		data = glade_xml_get_widget(xml, "radiobutton30");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	case IMG_PNG: 
		data = glade_xml_get_widget(xml, "radiobutton31");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	case IMG_ICO: 
		data = glade_xml_get_widget(xml, "radiobutton32");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break; 
	}

	switch (options2.type)
	{
	case IMG_BW:
		data = glade_xml_get_widget(xml, "radiobutton10");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	case IMG_COL:
		data = glade_xml_get_widget(xml, "radiobutton11");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		BREAK;
	}

	switch (options2.size)
	{
	case IMG_LCD:
		data = glade_xml_get_widget(xml, "radiobutton20");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	case IMG_SKN:
		data = glade_xml_get_widget(xml, "radiobutton21");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		BREAK;
	}

	refresh_label();
	tmp_options.file = g_strdup(options2.file);
	i_screen_counter = options.img_counter;
	
	data = glade_xml_get_widget(xml, "entry10");
	gtk_entry_set_text(GTK_ENTRY(data), options.img_file);
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
		memcpy(&options2, &tmp_options, sizeof(ScrOptions));
		g_free(options2.file);
		options2.file = g_strdup(tmp_options.file);

		ti68k_unhalt();
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}

void refresh_label(void)
{
	gchar buffer[MAXCHARS];
	gchar *ext = "???";
	
	switch(i_image_format)
	{
	case IMG_XPM: ext = "xpm"; break;
	case IMG_PCX: ext = "pcx"; break;
	case IMG_JPG: ext = "jpg"; break;
	case IMG_BMP: ext = "bmp"; break;
	default: break;
	}
	
	sprintf(buffer, "%s%03i.%s", i_screen_file, i_screen_counter, ext);
	gtk_label_set_text(GTK_LABEL(label), buffer);
}

void
on_scopt_radiobutton18_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_PCX;
  refresh_label();
}


void
on_scopt_radiobutton19_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_XPM;
  refresh_label();
}


void
on_scopt_radiobutton20_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_JPG;
  refresh_label();
}


void
on_scopt_radiobutton21_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_BMP;
  refresh_label();
}


void
on_scopt_radiobutton22_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_type = IMG_BW;
}


void
on_scopt_radiobutton23_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_type = IMG_COL;
}


void
on_scopt_radiobutton24_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_size = IMG_LCD;
}


void
on_scopt_radiobutton25_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_size = IMG_SKIN;
}


void
on_scopt_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  options.img_format = i_image_format;
  options.img_type = i_image_type;
  options.img_size = i_image_size;
  options.img_counter = i_screen_counter;
  g_free(options.img_file);
  options.img_file = g_strdup(i_screen_file);
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "scopt_dbox"));
}


void
on_scopt_cancel_button_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "scopt_dbox"));
}

void
on_scopt_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

void refresh_label(void)
{
  gchar buffer[MAXCHARS];
  gchar *ext = "???";

  switch(i_image_format)
    {
    case IMG_XPM: ext = "xpm"; break;
    case IMG_PCX: ext = "pcx"; break;
    case IMG_JPG: ext = "jpg"; break;
    case IMG_BMP: ext = "bmp"; break;
    default: break;
    }
  
  sprintf(buffer, "%s%03i.%s", i_screen_file, i_screen_counter, ext);
  gtk_label_set_text(GTK_LABEL(label), buffer);
}

void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  g_free(options.img_file);
  s = gtk_editable_get_chars(editable, 0, -1);
  i_screen_file = g_strdup(s);
  refresh_label();
}

/* */
