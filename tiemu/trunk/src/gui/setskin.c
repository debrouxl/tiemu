/* Hey EMACS -*- linux-c -*- */
/* $Id: screenshot.c 558 2004-04-09 14:15:30Z roms $ */

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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#include <gtk/gtk.h>

#include "intl.h"
#include "skinops.h"
#include "setskin.h"
#include "refresh.h"
#include "paths.h"
#include "struct.h"
#include "interface.h"

/* File Selection box */

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

static const gchar *create_fsel(gchar *dirname, gchar *ext)
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

gint display_skin_dbox()
{
	const gchar *filename;
    SKIN_INFOS si;
    gint ok;

	filename = create_fsel(inst_paths.skin_dir, "*.skn");
	if (!filename)
		return;

    // Read skin header
    if(skin_read_header(filename, &si) == -1) {

        msg_box(_("Error"), _("Unable to use this skin."));
        return -1;
    }

    // Check skin header
   printf("<%s>\n", si.calc);
    switch(ti68k_getCalcType())
	{
	    case TI92:
            ok = !strcmp(si.calc, SKIN_TI92);
		break;
	    case TI89:
            ok = !strcmp(si.calc, SKIN_TI89);
		break;
	    case TI92 | MODULEPLUS:
            ok = !strcmp(si.calc, SKIN_TI92P);
		break;
	    default: 
            ok = 0;
		break;
	}

        
    if(!ok) {
        msg_box(_("Error"), _("Skin incompatible with the current calc model."));
        ti68k_unhalt();
        return -1;
    }

    // Load new skin
    g_free(options.skin_file);
    options.skin_file = g_strconcat(filename, NULL);
    
    hid_change_skin(options.skin_file);
    ti68k_unhalt();

	return 0;
}

