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

gint display_infos_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *label;
	gint result;
	gchar *str;
	const char *p;
	const char *p1, *p2;
	int i = 0;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("infos-2.glade"), "infos_dbox",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "infos_dbox");

	label = glade_xml_get_widget(xml, "label20");
	str = g_strdup_printf("%s", "TiEmu v2.00");
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label21");
	str = g_strdup_printf("%s", skin_infos.name);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label22");
	str = g_strdup_printf("%s", skin_infos.author);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label23");
	str = g_strdup_printf("%s", ti68k_calctype_to_string(ti68k_getCalcType()));
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label24");
	str = g_strdup_printf("%s", ti68k_getRomVersion());
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label25");
	str = g_strdup_printf("%i KB", ti68k_getRamSize());
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label26");
	str = g_strdup_printf("%i KB", ti68k_getRomSize() >> 10);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
	label = glade_xml_get_widget(xml, "label27");
	str = g_strdup_printf("%s", ti68k_romtype_to_string(ti68k_getRomType()));
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
	
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
