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
#include <sys/stat.h>

#include "intl.h"
#include "paths.h"
#include "ti68k_int.h"
#include "refresh.h"
#include "struct.h"
#include "dboxes.h"
#include "fs_misc.h"
#include "rcfile.h"

gchar *chosen_file = NULL;

enum { 
	COLUMN_FILENAME, COLUMN_CALC, COLUMN_VERSION, 
	COLUMN_MEMORY, COLUMN_SIZE, COLUMN_TYPE, COLUMN_NCOLS
};

#define CLIST_NCOLS (COLUMN_NCOLS)

static void clist_selection_changed(GtkTreeSelection * sel,
				   					gpointer user_data)
{ 
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected (sel, &model, &iter))
    {
        if (chosen_file != NULL)
		{
	        g_free(chosen_file);
			chosen_file = NULL;
		}

        gtk_tree_model_get (model, &iter, 0, &chosen_file, -1);
    }
}

GtkListStore* clist_init(GtkWidget *clist)
{
	GtkTreeView *view = GTK_TREE_VIEW(clist);	
	GtkListStore *list;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
	gint i;
	const gchar *text[CLIST_NCOLS] = { 
		_("Filename"), _("Model"), _("Version"), 
		_("Type"), _("Size"), _("Boot") };
	
	list = gtk_list_store_new(6, 
    			G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING, G_TYPE_STRING);
    model = GTK_TREE_MODEL(list);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_headers_clickable(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
    for (i = 0; i < CLIST_NCOLS; i++) 
    {
    	renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(view, -1, text[i],
						renderer, "text", i, NULL);
    }
    
    for (i = 0; i < CLIST_NCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	g_signal_connect(G_OBJECT(selection), "changed",
			 G_CALLBACK(clist_selection_changed), NULL);

	return list;
}

gint display_romversion_dbox()
{
	GtkWidget *dialog;
    GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
    gint result;

    GtkListStore *list;
    GtkTreeIter iter;

    gchar buffer[MAXCHARS];
    int i;
    FILE *fp;
    gchar *filename;
    struct stat s;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("romversion-2.glade"), "romversion_dbox",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);

	// display waiting box
	dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_CLOSE, _("Cache is being built. Please wait..."));
	g_signal_connect_swapped(GTK_OBJECT(dialog), "response",
			G_CALLBACK(gtk_widget_destroy),
			GTK_OBJECT(dialog));
	gtk_widget_show_all(GTK_WIDGET(dialog));
	GTK_REFRESH();
	
    // display list box
	dbox = glade_xml_get_widget(xml, "romversion_dbox");
    
    data = glade_xml_get_widget(xml, "applybutton1");
    gtk_button_set_label(GTK_BUTTON(data), "Import...");

    data = glade_xml_get_widget(xml, "clist1");
    list = clist_init(data);

	// scan ROM images
    filename = g_strconcat(inst_paths.img_dir, CACHE_FILE, NULL);
    ti68k_scan_images(inst_paths.img_dir, filename);

    stat(filename, &s);
    if(s.st_size == 0) 
	{
		gtk_widget_show_all(dbox);
		return 0;
    }

    fp = fopen(filename, "rt");
    if(fp == NULL) 
	{
        DISPLAY("Unable to open this file: %s\n", filename);
        gtk_widget_destroy(dbox);
		ti68k_engine_unhalt();
        return -1;
    }

    while(!feof(fp)) 
	{
		gchar **row_text = g_malloc0((CLIST_NCOLS+1) * sizeof(gchar *));

        for(i=0; i<6; i++) 
		{
	        fscanf(fp, "%s\t", buffer);
	        row_text[i] = g_strdup(buffer);
	    }

        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 
			0, row_text[0],
			1, row_text[1], 
			2, row_text[2],
			3, row_text[3], 
			4, row_text[4],
			5, row_text[5], -1);

			g_strfreev(row_text);
    } 

    fclose(fp);
    
  	// destroy waiting box
	gtk_widget_destroy(GTK_WIDGET(dialog));

	// run main box
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	gtk_widget_destroy(dbox);

	switch (result) 
	{
		case GTK_RESPONSE_OK:
			if(ti68k_is_a_img_file(chosen_file))
			{
                // Remove previous tib file
                g_free(params.tib_file);
				params.tib_file = g_strconcat(inst_paths.img_dir, "", NULL);

                // Set new image
				g_free(params.rom_file);
				params.rom_file = g_strconcat(inst_paths.img_dir, chosen_file, NULL);
				g_free(chosen_file);
                chosen_file = NULL;

				if(ti68k_load_image(params.rom_file)) 
				{
					msg_box("Error", "Can not load the image.");
					return -1;
				}

                // Restart engine
                ti68k_restart();
			} 
			else if(ti68k_is_a_tib_file(chosen_file))
			{
                // Set tib file
				g_free(params.tib_file);
				params.tib_file = g_strconcat(inst_paths.img_dir, chosen_file, NULL);
				g_free(chosen_file);
                chosen_file = NULL;

				if(ti68k_load_upgrade(params.tib_file)) 
				{
					msg_box("Error", "Can not load the upgrade.");
					return -1;
				}
                
                msg_box(_("Information"), _("Your configuration has been saved."));
                rcfile_write();

                // Simply reset, don't restart
                ti68k_reset();
			}
			
		break;

		case GTK_RESPONSE_APPLY:
			display_import_romversion_dbox();
			display_romversion_dbox();	// force rescan but recursive
        break;
		
		default:
		break;
	}

    ti68k_engine_unhalt();

	return 0;
}
