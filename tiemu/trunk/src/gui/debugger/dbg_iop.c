/* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_regs.c 1615 2005-08-03 12:44:13Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"
#include "ioports.h"

enum 
{
	    COL_NAME, COL_VALUE, COL_ADDR, COL_MASK, COL_FONT, COL_EDIT, COL_S,
};
#define CTREE_NVCOLS	(4)		// 2 visible columns
#define CTREE_NCOLS		(7)		// 5 real columns

#define FONT_NAME	"courier"

static gint column2index(GtkWidget *list, GtkTreeViewColumn * column)
{
	gint i;

	for (i = 0; i < CTREE_NVCOLS; i++) 
	{
		GtkTreeViewColumn *col;

		col = gtk_tree_view_get_column(GTK_TREE_VIEW(list), i);
		if (col == column)
			return i;
	}

	return -1;
}

static int validate_value(const char *str, int ndigits)
{
	int i;
	
	if((int)strlen(str) > ndigits)
	 	return 0;
	
	for(i = 0; (i < ndigits) && (i < (int)strlen(str)); i++)
	{
		if(!isxdigit(str[i]))
			return 0;
	}
	
	return !0;
}

static void renderer_edited(GtkCellRendererText * cell,
			    const gchar * path_string,
			    const gchar * new_text, gpointer user_data)
{
	/*
    GtkWidget *tree = user_data;
	GtkTreeView *view = GTK_TREE_VIEW(tree);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkTreeStore *store = GTK_TREE_STORE(model);

	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;
	*/
}

static gboolean select_func(GtkTreeSelection * selection,
			    GtkTreeModel * model,
			    GtkTreePath * path,
			    gboolean path_currently_selected,
			    gpointer data)
{
	/*
	GtkTreeIter iter;
	gchar *str;

	if (!gtk_tree_model_get_iter(model, &iter, path))
		return FALSE;

	gtk_tree_model_get(model, &iter, COL_VALUE, &str, -1);
	
	if(!strcmp(str, ""))
	{
		g_free(str);
		return FALSE;
	}
	
	g_free(str);	
	*/
	return TRUE;
}

static GtkTreeStore* ctree_create(GtkWidget *widget)
{
	GtkTreeView *view = GTK_TREE_VIEW(widget);
	GtkTreeStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    gint i;
	
	store = gtk_tree_store_new(CTREE_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_POINTER,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_rules_hint(view, TRUE);
  
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            "Name", renderer, 
            "text", COL_NAME,
			"font", COL_FONT,
			NULL);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            "Value", renderer, 
            "text", COL_VALUE,
			"editable", COL_EDIT,
			"font", COL_FONT,
			NULL);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            "Address", renderer, 
            "text", COL_ADDR,
			"font", COL_FONT,
			NULL);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            "Mask", renderer, 
            "text", COL_MASK,
			"font", COL_FONT,
			NULL);
			
	g_signal_connect(G_OBJECT(renderer), "edited",
			G_CALLBACK(renderer_edited), widget);

    
    for (i = 0; i < CTREE_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
	gtk_tree_selection_set_select_function(selection, select_func, NULL, NULL);

	return store;
}

static void ctree_populate(GtkTreeStore *store)
{
	GNode* node0;
	GtkTreeIter parent, iter;
	int i, j;

	ioports_load("c:\\msvc\\tilp\\");
	node0 = ioports_tree();

	if(node0 == NULL)
		return;

	for (i = 0; i < (int)g_node_n_children(node0); i++) 
	{
		GNode *node1 = g_node_nth_child(node0, i);
		IOPORT *s = (IOPORT *)(node1->data);

		gtk_tree_store_append(store, &parent, NULL);
		gtk_tree_store_set(store, &parent, COL_NAME, s->name, -1);

		for (j = 0; j < (int)g_node_n_children(node1); j++) 
		{
			GNode *node2 = g_node_nth_child(node1, j);
			IOPORT *t = (IOPORT *)(node2->data);

			gchar **row_text = g_malloc0((CTREE_NCOLS + 1) * sizeof(gchar *));

			row_text[0] = g_strdup(t->name);
			switch(t->size)
			{
			case 1: row_text[1] = g_strdup_printf("%02xh", mem_rd_byte(t->addr)); break;
			case 2: row_text[1] = g_strdup_printf("%04xh", mem_rd_word(t->addr)); break;
			case 3: row_text[1] = g_strdup_printf("%08xh", mem_rd_long(t->addr)); break;
			default: row_text[1] = g_strdup("???"); break;
			}
			row_text[2] = g_strdup_printf("$%06x", t->addr);
			row_text[3] = g_strdup(t->all_bits ? "" : t->bit_str);

			gtk_tree_store_append(store, &iter, &parent);
			gtk_tree_store_set(store, &iter, 
						COL_NAME, row_text[0],
					   COL_VALUE, row_text[1], 
					   COL_ADDR,  row_text[2], 
					   COL_MASK,  row_text[3],
					   COL_S, (gpointer)t,
					   COL_FONT, FONT_NAME,
					   -1);
			g_strfreev(row_text);
		}
	}
}

static void ctree_refresh(GtkTreeStore *store)
{
	GtkTreeModel *model = GTK_TREE_MODEL(store);
    //int i;
}

static GtkTreeStore *store;

/*
	Display registers window
*/
GtkWidget* dbgiop_create_window(void)
{
	GladeXML *xml = NULL;
	GtkWidget *dbox;
    GtkWidget *data;	
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_ioports-2.glade"), "dbgioports_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgioports_window");
#ifdef WND_TRANSIENT
	gtk_window_set_transient_for(GTK_WINDOW(dbox), GTK_WINDOW(main_wnd));
#endif

	data = glade_xml_get_widget(xml, "treeview1");
    store = ctree_create(data);
	ctree_populate(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));

	return dbox;
}

GtkWidget* dbgiop_display_window(void)
{
#ifdef WND_STATE
	if(!options3.iop.minimized)
	{
		gtk_window_resize(GTK_WINDOW(dbgw.iop), options3.iop.rect.w, options3.iop.rect.h);
		gtk_window_move(GTK_WINDOW(dbgw.iop), options3.iop.rect.x, options3.iop.rect.y);
	}
	else
		gtk_window_iconify(GTK_WINDOW(dbgw.iop));
#endif
    
	ctree_refresh(store);

	if(!GTK_WIDGET_VISIBLE(dbgw.iop) && !options3.iop.closed)
		gtk_widget_show(dbgw.iop);

	return dbgw.iop;
}

void dbgiop_refresh_window(void)
{
	if(!options3.iop.closed)
	{
		ctree_refresh(store);
	}
}
