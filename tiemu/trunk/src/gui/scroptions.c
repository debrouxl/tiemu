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
#include "support.h"
#include "paths.h"
#include "struct.h"
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
    tmp_options.file = g_strdup(options2.file);
	
	switch (tmp_options.format)
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

	switch (tmp_options.type)
	{
	case IMG_BW:
		data = glade_xml_get_widget(xml, "radiobutton10");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	case IMG_COL:
		data = glade_xml_get_widget(xml, "radiobutton11");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	}

	switch (tmp_options.size)
	{
	case IMG_LCD:
		data = glade_xml_get_widget(xml, "radiobutton20");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	case IMG_SKIN:
		data = glade_xml_get_widget(xml, "radiobutton21");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);
		break;
	}

    label = glade_xml_get_widget(xml, "label10");
	refresh_label();
		
	data = glade_xml_get_widget(xml, "entry10");
	gtk_entry_set_text(GTK_ENTRY(data), tmp_options.file);
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
        g_free(options2.file);
		memcpy(&options2, &tmp_options, sizeof(ScrOptions));
		options2.file = g_strdup(tmp_options.file);
        g_free(tmp_options.file);
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);
    ti68k_unhalt();

	return 0;
}

void refresh_label(void)
{
	gchar buffer[MAXCHARS];
	gchar *ext = "???";
    gchar *str;
	
    if(label == NULL)
        return;

	switch(tmp_options.format)
	{
	    case IMG_JPG: ext = "jpg"; break;
	    case IMG_PNG: ext = "png"; break;
	    case IMG_ICO: ext = "ico"; break;
	    default: break;
	}
	
    str = g_strdup_printf("%s%03i.%s", tmp_options.file, tmp_options.counter, ext);
	gtk_label_set_text(GTK_LABEL(label), str);
    g_free(str);
}


GLADE_CB void
on_scopt_radiobutton10_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    tmp_options.type = IMG_BW;
}


GLADE_CB void
on_scopt_radiobutton11_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
        tmp_options.type = IMG_COL;
}


GLADE_CB void
on_scopt_radiobutton20_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
        tmp_options.size = IMG_LCD;
}


GLADE_CB void
on_scopt_radiobutton21_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
        tmp_options.size = IMG_SKIN;
}


GLADE_CB void
on_scopt_radiobutton30_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
        tmp_options.format = IMG_JPG;
    refresh_label();
}


GLADE_CB void
on_scopt_radiobutton31_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
        tmp_options.format = IMG_PNG;
    refresh_label();
}


GLADE_CB void
on_scopt_radiobutton32_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
        tmp_options.format = IMG_ICO;
    refresh_label();
}


GLADE_CB void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *str;

    g_free(tmp_options.file);
    str = gtk_editable_get_chars(editable, 0, -1);
    tmp_options.file = g_strdup(str);
    g_free(str);
    refresh_label();
}

/* */
