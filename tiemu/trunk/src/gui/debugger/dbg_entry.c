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
#include "ti68k_int.h"
#include "dbg_entry.h"

enum { 
	    COL_NAME, COL_HANDLE
};
#define CTREE_NVCOLS	(1)		// 1 visible columns
#define CTREE_NCOLS		(2)		// 1 real columns

static GtkTreeStore* ctree_create(GtkWidget *tree)
{
	GtkTreeView *view = GTK_TREE_VIEW(tree);
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

static void ctree_populate(GtkTreeStore *store)
{
	gint i, j;
	GNode *tree;

	// Parse VAT
	vat_parse(&tree);

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
		}
	}

	// Free copy of VAT
	vat_free(&tree);
}

static void ctree_get_selection(GtkWidget *tree)
{
	GtkTreeView *view = GTK_TREE_VIEW(tree);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GList *l;
	
	// get selection
	selection = gtk_tree_view_get_selection(view);
	for (l = gtk_tree_selection_get_selected_rows(selection, &model);
	     l != NULL; l = l->next) 
	{
		GtkTreeIter iter;
		GtkTreePath *path = l->data;
		char *name;
		int handle;
			
		gtk_tree_model_get_iter(model, &iter, path);
		gtk_tree_model_get(model, &iter, COL_NAME, &name, COL_HANDLE, &handle, -1);
		printf("Selected: %s\n", name);;
		
		//ti68k_bkpt_add_exception(n);
	}	
}

gint dbgentry_display_dbox(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	GtkTreeStore *store;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_entry-2.glade"), "dbgentry_dbox",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgentry_dbox");
	//gtk_window_resize(GTK_WINDOW(dbox), 320, 240);
		
	data = glade_xml_get_widget(xml, "treeview1");
    store = ctree_create(data);
	ctree_populate(store);	
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) 
	{
	case GTK_RESPONSE_OK:
		ctree_get_selection(data);
		//dbgbkpts_display_window();
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}




