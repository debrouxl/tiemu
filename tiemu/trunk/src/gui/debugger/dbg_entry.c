/* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_vectors.c 676 2004-09-21 20:27:05Z roms $ */

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
#include "support.h"
#include "ti68k_int.h"
#include "dbg_entry.h"
#include "dbg_bkpts.h"

enum { 
	    COL_NAME, COL_HANDLE
};
#define CTREE_NVCOLS	(1)		// 1 visible columns
#define CTREE_NCOLS		(2)		// 1 real columns

#define SELTOK	"-> "

static GtkTreeStore* ctree_create(GtkWidget *widget)
{
	GtkTreeView *view = GTK_TREE_VIEW(widget);
	GtkTreeStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CTREE_NVCOLS] = { _("Name") };
    gint i;
	
	store = gtk_tree_store_new(CTREE_NCOLS, G_TYPE_STRING, G_TYPE_INT, -1);
    model = GTK_TREE_MODEL(store);
	
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, FALSE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	for(i = COL_NAME; i <= COL_NAME; i++)
	{
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
			NULL);
	}
    
    for (i = 0; i < CTREE_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	return store;
}

static void ctree_populate(GtkWidget *widget)
{
	GtkTreeView *view = GTK_TREE_VIEW(widget);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkTreeStore *store = GTK_TREE_STORE(model);
	gint i, j;
	GNode *tree;
	uint16_t handle;

	// Parse VAT
	vat_parse(&tree);

	// Retrieve breakpoint
	if(ti68k_bkpt_get_pgmentry(0, &handle))
		handle = -1;

	// and show it
	for (i = 0; i < (int)g_node_n_children(tree); i++) 
	{
		GNode *fol_node = g_node_nth_child(tree, i);
		VatSymEntry *vse = (VatSymEntry *)fol_node->data;
		GtkTreeIter fol_iter;

		gtk_tree_store_append(store, &fol_iter, NULL);
		gtk_tree_store_set(store, &fol_iter, COL_NAME, vse->name, COL_HANDLE, vse->handle, -1);

		for(j = 0; j < (int)g_node_n_children(fol_node); j++)
		{
			GNode *var_node = g_node_nth_child(fol_node, j);
			VatSymEntry *vse = (VatSymEntry *)var_node->data;
			GtkTreeIter var_iter;

			gtk_tree_store_append(store, &var_iter, &fol_iter);
			gtk_tree_store_set(store, &var_iter, COL_NAME, vse->name, COL_HANDLE, vse->handle, -1);
			
			if(vse->handle == handle)
			{
				gchar *name = g_strconcat(SELTOK, vse->name, NULL);
				gtk_tree_store_set(store, &var_iter, COL_NAME, name, -1);
				g_free(name);
			}
		}
	}

	gtk_tree_view_expand_all(view);

	// Free copy of VAT
	vat_free(&tree);
}

GList *sel = NULL;

static void ctree_get_selection(void)
{
	GList *ptr;

	// clear bkpt list
	ti68k_bkpt_clear_pgmentry();

	// create new one
	for(ptr = sel; ptr != NULL; ptr = g_list_next(ptr))
		ti68k_bkpt_add_pgmentry((uint16_t)(GPOINTER_TO_INT(ptr->data)));

	// free data
	g_list_free(sel);
	sel = NULL;
}

gint dbgentry_display_dbox(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_entry-2.glade"), "dbgentry_dbox",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgentry_dbox");
	//gtk_window_resize(GTK_WINDOW(dbox), 320, 240);
		
	data = glade_xml_get_widget(xml, "treeview1");
    ctree_create(data);
	ctree_populate(data);	
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) 
	{
	case GTK_RESPONSE_OK:
		ctree_get_selection();
		dbgbkpts_display_window();
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}

GLADE_CB gboolean
dbgentry_button_press_event        (GtkWidget       *widget,
                                    GdkEventButton  *event,
                                    gpointer         user_data)
{		
	GtkTreeView *view = GTK_TREE_VIEW(widget);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkTreeStore *store = GTK_TREE_STORE(model);
	GtkTreeViewColumn *column;
	GtkTreePath *path;
	GtkTreeIter iter;
	gboolean folder;

	// is double click ?
	if(event->type != GDK_2BUTTON_PRESS)
		return FALSE;

	gtk_tree_view_get_cursor(view, &path, &column);
	if(!path || !column)
		return FALSE;

	gtk_tree_model_get_iter(model, &iter, path);
	folder = gtk_tree_model_iter_has_child(model, &iter);

    if(!folder)
	{
        gchar** row_text = g_malloc0((CTREE_NVCOLS + 1) * sizeof(gchar *));
		uint16_t handle;
		
		// get address
		gtk_tree_model_get(model, &iter, COL_NAME, &row_text[COL_NAME], COL_HANDLE, &handle, -1);

		// toggle token
		if(strstr(row_text[COL_NAME], SELTOK))
		{
			// remove entry
			gchar *name = g_strdup(&row_text[COL_NAME][strlen(SELTOK)]);
			gtk_tree_store_set(store, &iter, COL_NAME, name, -1);
			g_free(name);
			sel = g_list_remove(sel, GINT_TO_POINTER(handle));
		}
		else
		{
			gchar *name;
			
			// no more than 1 bkpt
			if(g_list_length(sel) >= 1) return FALSE;
			
			// add entry
			name = g_strconcat(SELTOK, row_text[COL_NAME], NULL);
			gtk_tree_store_set(store, &iter, COL_NAME, name, -1);
			g_free(name);
			sel = g_list_append(sel, GINT_TO_POINTER(handle));
		}

        g_strfreev(row_text);
	}
	
    return FALSE;
}
