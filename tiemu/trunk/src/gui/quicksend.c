/* Hey EMACS -*- linux-c -*- */
/* $Id: scroptions.c 2268 2006-11-06 17:18:51Z roms $ */

/*  TiEmu - Tiemu Is an EMUlator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <string.h>

#include "intl.h"
#include "support.h"
#include "paths.h"
#include "struct.h"
#include "ti68k_int.h"

char *qs_filename = NULL;
int qs_enabled = 0;

static GtkWidget *fcb = NULL;

gint display_quicksend_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("quicksend-2.glade"), 
		 "quicksend_dbox",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);

	dbox = glade_xml_get_widget(xml, "quicksend_dbox");

	fcb = glade_xml_get_widget(xml, "filechooserbutton1");
	gtk_widget_set_sensitive(fcb, qs_enabled && qs_filename);
	if(qs_filename)
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(fcb), qs_filename);
	// filter wildcards to set
	//"*.89?;*.92?;*.9x?;*.9X?;*.v2?;*.V2?;*.tig"

	data = glade_xml_get_widget(xml, "checkbutton1");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), qs_enabled);
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) 
	{
	case GTK_RESPONSE_OK:
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}

GLADE_CB void
qs_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	qs_enabled = gtk_toggle_button_get_active(togglebutton);
	if(fcb)
		gtk_widget_set_sensitive(fcb, qs_enabled);
}

GLADE_CB void
qs_filechooserbutton1_current_folder_changed
                                        (GtkFileChooser  *filechooser,
                                        gpointer         user_data)
{
	gchar *fname = gtk_file_chooser_get_filename (filechooser);

	if(!fname)
		return;

	if(!tifiles_file_is_ti(fname) || !tifiles_calc_is_ti9x(tifiles_file_get_model(fname)) ||
		!tifiles_file_is_group(fname) && !tifiles_file_is_single(fname))
	{
		g_free(qs_filename); qs_filename = NULL;
		msg_box1(_("Error"), _("This file is not a valid TI file."));
		return;
	}

	g_free(qs_filename);
	qs_filename = g_strdup(fname); // dup or copy???
}

/* */
