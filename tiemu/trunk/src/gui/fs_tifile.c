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

#include "intl.h"
#include "skinops.h"
#include "fs_tifile.h"
#include "interface.h"
#include "paths.h"
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

gint display_tifile_dbox()
{
	const gchar *filename;
    const gchar *ext;

    // set mask
    switch(ti68k_getCalcType()) {
    case TI92:
        ext = "*.92?";
    case TI89:
        ext = "*.89?";
        break;
    case TI92 | MODULEPLUS:
        ext = "*.9x?";
        break;
    default:
        ext = "*.???";
        break;
    }

    // get filename
	filename = create_fsel(inst_paths.base_dir, ext);
	if (!filename)
		return;

    // check extension
    if(!tifiles_is_a_ti_file(filename) || 
        !tifiles_is_ti9x(tifiles_which_calc_type(filename))) {
        msg_box(_("Error"), _("This file is not a valid TI file."));
        ti68k_unhalt();
        return -1;
    }

    // set pbar title
    if(tifiles_is_a_tib_file(filename) || tifiles_is_a_flash_file(filename)) {
        create_pbar_type5(_("Flash"), "");
    } else if(tifiles_is_a_backup_file(filename)) {
        create_pbar_type3(_("Backup"));
    } else if(tifiles_is_a_group_file(filename)) {
        create_pbar_type5(_("Sending group file"), "");
    } else if(tifiles_is_a_single_file(filename)) {
        create_pbar_type4(_("Sending variable"), "");
    }

    ti68k_unhalt();
    ti68k_sendFile(filename);

	return 0;
}

