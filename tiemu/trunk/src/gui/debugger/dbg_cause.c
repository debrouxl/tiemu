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

gint display_dbgcause_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *label;
	gint result;
	gchar *str;
	gint type, id, mode;
	uint32_t value;

    // get context
	ti68k_bkpt_get_cause(&type, &mode, &id);
    if(!type && !mode && !id)
        return 0;
	
    // load GUI
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_cause-2.glade"), "dbgcause_dbox",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgcause_dbox");

	// set PC
	label = glade_xml_get_widget(xml, "label21");
	ti68k_register_get_pc(&value);
	str = g_strdup_printf("0x%06x", value);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);

	// set type
	label = glade_xml_get_widget(xml, "label22");
	str = g_strdup_printf("%s", ti68k_bkpt_type_to_string(type));
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);

	// set mode
	label = glade_xml_get_widget(xml, "label23");
	str = g_strdup_printf("%s", ti68k_bkpt_mode_to_string(type, mode));
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);

	// set id
	label = glade_xml_get_widget(xml, "label24");
	str = g_strdup_printf("%i", id);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}
