/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 197 2004-05-15 11:40:44Z roms $ */

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
#include <string.h>

#include "intl.h"
#include "filesel.h"
#include "refresh.h"

static G_CONST_RETURN gchar *filename = NULL;

static void store_filename(GtkFileSelection * file_selector,
			   gpointer user_data)
{
	filename = (gchar *)
	    gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
} 

static void cancel_filename(GtkButton * button, gpointer user_data)
{
	filename = "";
} 

const gchar *create_fsel(gchar *dirname, gchar *ext)
{
	GtkWidget *fs;
	gchar *mask;
    
    mask = g_strconcat(dirname, ext, NULL);
	fs = gtk_file_selection_new("Select a skin");
	gtk_file_selection_complete(GTK_FILE_SELECTION(fs), mask);
    g_free(mask);

	g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(fs)->ok_button),
			 "clicked", G_CALLBACK(store_filename), fs);

	g_signal_connect(GTK_OBJECT
			 (GTK_FILE_SELECTION(fs)->cancel_button),
			 "clicked", G_CALLBACK(cancel_filename), fs);

	g_signal_connect_swapped(GTK_OBJECT
				 (GTK_FILE_SELECTION(fs)->ok_button),
				 "clicked",
				 G_CALLBACK(gtk_widget_destroy),
				 (gpointer) fs);

	g_signal_connect_swapped(GTK_OBJECT
				 (GTK_FILE_SELECTION(fs)->cancel_button),
				 "clicked", G_CALLBACK(gtk_widget_destroy),
				 (gpointer) fs);

	filename = NULL;
	gtk_widget_show(fs);
	while (filename == NULL) {
        GTK_REFRESH();
	}

	if (!strcmp(filename, ""))
		return NULL;
	else
		return filename;
}


