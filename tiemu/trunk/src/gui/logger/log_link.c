/* Hey EMACS -*- linux-c -*- */
/* $Id: manpage.c 1455 2005-05-31 18:38:03Z roms $ */

/*  tilp - Ti Linking Program
 *  Copyright (C) 1999-2004  Romain Lievin
 *
 *  This program is free software you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#include <gtk/gtk.h>
#include <glade/glade.h>

#include <stdio.h>
#include <string.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_def.h"
#include "filesel.h"

static GtkTextBuffer *txtbuf;
static logger_enabled = 0;

static void udpate_widgets(GtkWidget *button, GtkWidget *spin)
{
	gtk_button_set_use_stock(GTK_BUTTON(button), TRUE);
	gtk_button_set_label(GTK_BUTTON(button), logger_enabled ? GTK_STOCK_STOP : GTK_STOCK_OK);	// GTK_STOCK_START

	gtk_widget_set_sensitive(spin, !logger_enabled);
}

gint display_loglink_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *text;
	gpointer data, data2;
	int i;

	xml = glade_xml_new
		(tilp_paths_build_glade("log_link-2.glade"), "linklog_dbox", PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);

	text = glade_xml_get_widget(xml, "textview1");
	txtbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));

	data = glade_xml_get_widget(xml, "spinbutton1");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), logger.link_size >> 10);

	data = glade_xml_get_widget(xml, "checkbutton1");
	gtk_toggle_button_set_active(data, logger.link_mask & 1);

	data = glade_xml_get_widget(xml, "checkbutton2");
	gtk_toggle_button_set_active(data, logger.link_mask & 2);

	data = glade_xml_get_widget(xml, "button10");
	data2 = glade_xml_get_widget(xml, "spinbutton1");
	udpate_widgets(data, data2);

	for(i = 0; i < logger.link_ptr; i++)
	{
		uint16_t word = (logger.link_buf)[i];
		char *str;

		str = g_strdup_printf("%c: %02x\n", (MSB(word) & 1) ? 'S' : 'R', LSB(word));
		gtk_text_buffer_insert_at_cursor(txtbuf, str, strlen(str));
		g_free(str);                                           
	}

	dbox = glade_xml_get_widget(xml, "linklog_dbox");
	gtk_widget_show(dbox);

	return 0;
}

GLADE_CB void
ll_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

	if(gtk_toggle_button_get_active(togglebutton)) 
		logger.link_mask |= 1;
	else
		logger.link_mask &= ~1;
}


GLADE_CB void
ll_checkbutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	if(gtk_toggle_button_get_active(togglebutton)) 
		logger.link_mask |= 2;
	else
		logger.link_mask &= ~2;
}


GLADE_CB void
on_button9_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	const gchar *filename;
	FILE *f;
	gchar *txt;
	GtkTextIter start, end;

	filename = create_fsel(inst_paths.home_dir, "log_link.txt", "*.txt", TRUE);
	if (!filename)
		return;

	f = fopen(filename, "wt");
	if(f == NULL)
		return;

	gtk_text_buffer_get_bounds(txtbuf, &start, &end);
	txt = gtk_text_buffer_get_text(txtbuf, &start, &end, TRUE);
	printf("<%s>\n", txt);
	fwrite(txt, strlen(txt), 1, f);

	fclose(f);
}


GLADE_CB void
on_button10_clicked                    (GtkButton       *butto,
                                        gpointer         user_dat)
{
	GtkWidget *button = user_dat;
	GtkWidget *spinbutton = GTK_WIDGET(butto);

	logger_enabled = !logger_enabled;
	if(logger.link_size == 0)
		logger.link_size = 1024 * gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton));
	logger.link_ptr = 0;

	if(logger_enabled)
	{
		g_free(logger.link_buf);
		logger.link_buf = (uint16_t *)g_malloc0(logger.link_size * sizeof(uint16_t));
	}
	else
	{
		g_free(logger.link_buf);
		logger.link_buf = NULL;
	}

	udpate_widgets(button, spinbutton);
}


GLADE_CB void
on_button11_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkTextIter start, end;
	GtkWidget *text = GTK_WIDGET(button);
	GtkTextBuffer *txtbuf;

	txtbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
	gtk_text_buffer_get_bounds(txtbuf, &start, &end);
	gtk_text_buffer_delete(txtbuf, &start, &end);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text), 15);
}


GLADE_CB void
on_button12_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy(GTK_WIDGET(button));
}

GLADE_CB void
ll_spinbutton1_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
	if(!logger_enabled)
		logger.link_size = 1024 * gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(editable));
}
