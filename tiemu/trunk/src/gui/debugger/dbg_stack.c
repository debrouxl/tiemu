 /* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_code.c 450 2004-06-13 13:48:31Z roms $ */

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
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"

enum { 
	    COL_ADDR, COL_DATA, COL_FONT
};
#define CLIST_NVCOLS	(2)		// 1 visible columns
#define CLIST_NCOLS		(3)		// 1 real columns

#define FONT_NAME	"courier"

#define DUMP_SIZE       32

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { _("Address"), _("Value") };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	//clist = tree = gtk_tree_view_new_with_model(model);
	//view = GTK_TREE_VIEW(tree);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, FALSE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	for(i = COL_ADDR; i <= COL_DATA; i++)
	{
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
            "font", COL_FONT,
			NULL);
	}
    
    for (i = 0; i < CLIST_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

	return store;
}

static void clist_populate(GtkListStore *store)
{
    int i;
    GtkTreeIter iter;
    gchar *str;

    uint32_t sp;
    uint16_t *mem;
    uint16_t data;

    ti68k_register_get_sp(&sp);
	mem = (uint16_t *)ti68k_get_real_address(sp);

    for(i = 0; i < DUMP_SIZE; i++)
    {
        gtk_list_store_append(store, &iter);

        str = g_strdup_printf("0x%06x:", sp + 2*i);
		gtk_list_store_set(store, &iter, COL_ADDR, str, -1);
        g_free(str);

        data =  GUINT16_SWAP_LE_BE(mem[i]);
		str = g_strdup_printf("%04x", data);
        gtk_list_store_set(store, &iter, COL_DATA, str, -1);
        g_free(str);
		
        gtk_list_store_set(store, &iter, COL_FONT, FONT_NAME, -1);
    }
}

static GtkWidget *label;

static void clist_refresh(GtkListStore *store)
{
	gtk_list_store_clear(store);
	clist_populate(store);

    if(ti68k_debug_is_supervisor())
        gtk_label_set_text(GTK_LABEL(label), "SSP");
    else
        gtk_label_set_text(GTK_LABEL(label), "USP");
}

static GtkListStore *store = NULL;
static gint already_open = 0;

/*
	Display registers window
*/
GtkWidget* dbgstack_create_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_stack-2.glade"), "dbgstack_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgstack_window");

	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_populate(store);

    label = glade_xml_get_widget(xml, "label2");

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));
	gtk_widget_show(data);

	gtk_window_resize(GTK_WINDOW(dbox), options3.stack.w, options3.stack.h);
	gtk_window_move(GTK_WINDOW(dbox), options3.stack.x, options3.stack.y);

    gtk_window_set_transient_for(GTK_WINDOW(dbox), GTK_WINDOW(main_wnd));

	already_open = !0;

	return dbox;
}

GtkWidget* dbgstack_display_window(void)
{
    static GtkWidget *wnd = NULL;

	if(!already_open)
		wnd = dbgstack_create_window();
    gtk_widget_show(wnd);

	clist_refresh(store);

	return wnd;
}

void dbgstack_refresh_window(void)
{
	if(dbgs.stack)
	{
		clist_refresh(store);
	}
}

