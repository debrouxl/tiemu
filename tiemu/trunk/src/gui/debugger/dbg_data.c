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
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
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
#include "ti68k_int.h"
#include "support.h"
#include "dbg_bkpts.h"

static gint menu;
static GladeXML *xml;

gint display_dbgdata_dbox(void)
{
	//GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;

	gint mode, type, access;
	G_CONST_RETURN gchar *s_start, *s_stop;
	uint32_t start, stop;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_data-2.glade"), "dbgdata_dbox",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgdata_dbox");
	gtk_window_resize(GTK_WINDOW(dbox), 320, 240);

	data = glade_xml_get_widget(xml, "radiobutton20");
	gtk_signal_emit_by_name(GTK_OBJECT(data), "toggled");

loop:
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
		// Retrieve settings from fields
		data = glade_xml_get_widget(xml, "radiobutton10");
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
			mode = BK_READ;
		data = glade_xml_get_widget(xml, "radiobutton11");
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
			mode = BK_WRITE;
		data = glade_xml_get_widget(xml, "radiobutton12");
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
			mode = BK_READ | BK_WRITE;

		data = glade_xml_get_widget(xml, "radiobutton20");
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
			type = 1;

		data = glade_xml_get_widget(xml, "radiobutton21");
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
			type = 2;
	
		if(type == 1)
		{
			data = glade_xml_get_widget(xml, "optionmenu1");
			switch(gtk_option_menu_get_history(GTK_OPTION_MENU(data)))
			{
			case 0: access = BK_BYTE; break;
			case 1: access = BK_WORD; break;
			case 2: access = BK_LONG; break;
			}

			data = glade_xml_get_widget(xml, "entry3");
			s_start = s_stop = gtk_entry_get_text(GTK_ENTRY(data));
		} else if(type == 2)
		{
			data = glade_xml_get_widget(xml, "entry1");
			s_start = gtk_entry_get_text(GTK_ENTRY(data));

			data = glade_xml_get_widget(xml, "entry2");
			s_stop = gtk_entry_get_text(GTK_ENTRY(data));			
		}

		// Convert values and check
		result = sscanf(s_start, "%lx", &start);
		if(result < 1)
			goto loop;

		result = sscanf(s_stop, "%lx", &stop);
		if((result < 1) && (type == 2))
			goto loop;

		if((start >= stop) && (type == 2))
			goto loop;

		// Add breakpoint
		if(type == 1)
		{
			ti68k_bkpt_set_access(start, mode | access) ;
		} 
		else if(type == 2)
		{
			ti68k_bkpt_set_range(start, stop, mode);
		}

		refresh_dbgbkpts_window();

		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}


GLADE_CB void
on_radiobutton20_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *data;

	data = glade_xml_get_widget(xml, "optionmenu1");
	gtk_widget_set_sensitive(data, TRUE);
	data = glade_xml_get_widget(xml, "entry3");
	gtk_widget_set_sensitive(data, TRUE);

	data = glade_xml_get_widget(xml, "entry1");
	gtk_widget_set_sensitive(data, FALSE);
	data = glade_xml_get_widget(xml, "entry2");
	gtk_widget_set_sensitive(data, FALSE);
}

GLADE_CB void
on_radiobutton21_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *data;

	data = glade_xml_get_widget(xml, "optionmenu1");
	gtk_widget_set_sensitive(data, FALSE);
	data = glade_xml_get_widget(xml, "entry3");
	gtk_widget_set_sensitive(data, FALSE);

	data = glade_xml_get_widget(xml, "entry1");
	gtk_widget_set_sensitive(data, TRUE);
	data = glade_xml_get_widget(xml, "entry2");
	gtk_widget_set_sensitive(data, TRUE);
}



