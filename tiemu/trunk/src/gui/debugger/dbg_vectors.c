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
#include "ti68k_int.h"

enum { 
	    COL_NUMBER, COL_NAME,
};
#define CLIST_NVCOLS	(2)		// 2 visible columns
#define CLIST_NCOLS		(2)		// 2 real columns

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { _("Number"), _("Name") };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_INT, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	//clist = tree = gtk_tree_view_new_with_model(model);
	//view = GTK_TREE_VIEW(tree);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	for(i = COL_NUMBER; i <= COL_NAME; i++)
	{
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
			NULL);
	}
    
    for (i = 0; i < CLIST_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

	return store;
}

static void clist_populate(GtkListStore *store)
{
    GtkTreeIter iter;
	gint i;

	for(i = 0; i < 128; i++)
	{
	    gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
		COL_NUMBER, i, 
		COL_NAME, ti68k_exception_to_string(i),
		-1);
	}
}

static void clist_get_selection(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
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
		gint n;
			
		gtk_tree_model_get_iter(model, &iter, path);
		gtk_tree_model_get(model, &iter, COL_NUMBER, &n, -1);
		printf("sel: %i\n", n);
		
		ti68k_bkpt_set_exception(n);
	}	
}

gint display_dbgvectors_dbox(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	GtkListStore *store;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_vectors-2.glade"), "dbgvectors_dbox",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgvectors_dbox");
	gtk_window_resize(GTK_WINDOW(dbox), 320, 240);
		
	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_populate(store);	
	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
		clist_get_selection(data);
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}




