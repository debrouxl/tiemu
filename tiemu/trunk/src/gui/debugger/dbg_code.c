/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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
#include "dbg_bkpts.h"
#include "struct.h"

enum { 
	    COL_ICON, COL_ADDR, COL_OPCODE, COL_OPERAND,
        COL_HEXADDR
};
#define CLIST_NVCOLS	(4)		// 3 visible columns
#define CLIST_NCOLS		(5)		// 4 real columns

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { _(""), _("Address"), _("Opcode"), _("Operand") };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				GDK_TYPE_PIXBUF, 
                G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                G_TYPE_INT
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	//clist = tree = gtk_tree_view_new_with_model(model);
	//view = GTK_TREE_VIEW(tree);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
  	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[COL_ICON], renderer, 
            "pixbuf", COL_ICON,
			NULL);
  
	for(i = COL_ADDR; i <= COL_OPERAND; i++)
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
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	return store;
}

static void clist_populate(GtkListStore *store)
{
    GtkTreeIter iter;
    GdkPixbuf *pix;
    gint i;
    uint32_t addr;

    gtk_list_store_clear(store);
    addr = ti68k_debug_get_pc();

    for(i = 0; i < 10; i++)
    {
        char output[128];
        int offset;
        gchar** row_text;
        uint32_t value;
        
        // disassemble at 'addr' address into 'output' and returns offset to the
        // next instruction address
        offset = ti68k_debug_disassemble(addr, output);

        row_text = g_strsplit(output, " ", 3);
        if(row_text[2] == NULL)
            row_text[2] = g_strdup("");
        if(row_text[1] == NULL)
            row_text[1] = g_strdup("");
        sscanf(row_text[0], "%lx", &value);

        pix = create_pixbuf("void.xpm");

        gtk_list_store_append(store, &iter);
	    gtk_list_store_set(store, &iter, 
        COL_ICON, pix,
	    COL_ADDR, row_text[0], 
		COL_OPCODE, row_text[1],
        COL_OPERAND, row_text[2],
        COL_HEXADDR, value,
		-1);

        g_strfreev(row_text);
        addr += offset;
    }
}

static void clist_refresh(GtkListStore *store)
{
    GtkTreeModel *model = GTK_TREE_MODEL(store);
    gboolean valid;
    GtkTreeIter iter;

    uint32_t addr;
    uint32_t pc;
    int found = 0;

    pc = ti68k_debug_get_pc();

    // check for refresh
    for(valid = gtk_tree_model_get_iter_first(model, &iter);
        valid; 
        valid = gtk_tree_model_iter_next(model, &iter))
    {
        gchar *str;

        gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
        sscanf(str, "%lx", &addr);

        if(addr == pc)
            found = !0;

        g_free(str);
    }

    if(!found)
        clist_populate(store);

    // look for pc and matching bkpt
    for(valid = gtk_tree_model_get_iter_first(model, &iter);
        valid; 
        valid = gtk_tree_model_iter_next(model, &iter))
        {
            GdkPixbuf *pix;
            gchar *str;

            gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
            sscanf(str, "%lx", &addr);

            if(addr == pc)
            {
                pix = create_pixbuf("run.xpm");
            }
            else if(g_list_find(bkpts.code, GINT_TO_POINTER(addr)) != NULL)
            {
                pix = create_pixbuf("bkpt.xpm");
            }
            else
            {
                pix = create_pixbuf("void.xpm");
            }

            gtk_list_store_set(store, &iter, COL_ICON, pix, -1);
            g_free(str);
        }
}

static GtkListStore *store;
static gint already_open = 0;
typedef struct {
	GtkWidget *b1;
	GtkWidget *b2;
	GtkWidget *b3;
	GtkWidget *b4;
	GtkWidget *b5;
	GtkWidget *b6;
} TB;
static TB tb;

static void tb_set_states(int s1, int s2, int s3, int s4, int s5, int s6)
{
	gtk_widget_set_sensitive(tb.b1, s1);
	gtk_widget_set_sensitive(tb.b2, s2);
	gtk_widget_set_sensitive(tb.b3, s3);
	gtk_widget_set_sensitive(tb.b4, s4);
	gtk_widget_set_sensitive(tb.b5, s5);
	gtk_widget_set_sensitive(tb.b6, s6);
}

/*
	Display source code window
*/
GtkWidget* display_dbgcode_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_code-2.glade"), "dbgcode_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgcode_window");

	tb.b1 = glade_xml_get_widget(xml, "button1");
	tb.b2 = glade_xml_get_widget(xml, "button2");
	tb.b3 = glade_xml_get_widget(xml, "button3");
	tb.b4 = glade_xml_get_widget(xml, "button4");
	tb.b5 = glade_xml_get_widget(xml, "button5");
	tb.b6 = glade_xml_get_widget(xml, "button6");

	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_populate(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));
	gtk_widget_show(data);

	//gtk_window_resize(GTK_WINDOW(dbox), 320, 240);
	gtk_widget_set_usize(GTK_WIDGET(dbox), options3.code.w, options3.code.h);
	gtk_widget_set_uposition(GTK_WIDGET(dbox), options3.code.x, options3.code.y);
    gtk_widget_show(GTK_WIDGET(dbox));

	already_open = !0;

	return data;
}

gint refresh_dbgcode_window(void)
{
	static GtkWidget *list = NULL;

	if(!already_open)
		list = display_dbgcode_window();

	gtk_widget_set_sensitive(list, TRUE);	
	clist_refresh(store);

    return 0;
}

GLADE_CB gboolean
on_dbgcode_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	gdk_window_get_size(widget->window, &options3.code.w, &options3.code.h);
	gdk_window_get_root_origin(widget->window, &options3.code.x, &options3.code.y);

	return FALSE;
}

GLADE_CB void
on_dbgcode_window_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
	already_open = 0;
	// Closing the debugger starts the emulator
    ti68k_engine_unhalt();
}


// Run
GLADE_CB void
dbgcode_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *list = GTK_WIDGET(button);   // arg are swapped, why ?

	tb_set_states(1, 0, 0, 0, 1, 0);
    gtk_widget_set_sensitive(list, FALSE);
    ti68k_engine_unhalt();
}


// Step
GLADE_CB void
dbgcode_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    ti68k_debug_step();
	ti68k_engine_unhalt();
}


// Step over
GLADE_CB void
dbgcode_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    // not implemented yet
}


// Run to cursor
GLADE_CB void
dbgcode_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *list = GTK_WIDGET(button);   // arg are swapped, why ?
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkListStore *store = GTK_LIST_STORE(model);
    GtkTreeSelection *selection;
    GtkTreeIter iter;
    gboolean valid;
    gchar *str;
    uint32_t addr;

    selection = gtk_tree_view_get_selection(view);
    valid = gtk_tree_selection_get_selected(selection, NULL, &iter);

    // Get address to go
    gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
    sscanf(str, "%lx", &addr);

	tb_set_states(1, 0, 0, 0, 1, 0);
    ti68k_debug_skip(addr);
	tb_set_states(1, 1, 1, 1, 0, 1);
    clist_refresh(store);
}


// Break
GLADE_CB void
dbgcode_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	// Mode 1 is fastest
#if 1
    GtkWidget *list = GTK_WIDGET(button);   // arg are swapped, why ?
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkListStore *store = GTK_LIST_STORE(model);

    ti68k_engine_halt();
    gtk_widget_set_sensitive(list, TRUE);
	tb_set_states(1, 1, 1, 1, 0, 1);
    clist_refresh(store);
#else
    ti68k_debug_break();
#endif
}


// Toggle breakpoint
GLADE_CB void
dbgcode_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *list = GTK_WIDGET(button);   // arg are swapped, why ?
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkListStore *store = GTK_LIST_STORE(model);
    GtkTreeSelection *selection;
    GtkTreeIter iter;
    gboolean valid;
    gchar *str;
    uint32_t addr;

    selection = gtk_tree_view_get_selection(view);
    valid = gtk_tree_selection_get_selected(selection, NULL, &iter);

    gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
    sscanf(str, "%lx", &addr);

    if(g_list_find(bkpts.code, GINT_TO_POINTER(addr)) == NULL)
        ti68k_bkpt_set_address(addr);
    else
        ti68k_bkpt_del_address(addr);

    clist_refresh(store);
    refresh_dbgbkpts_window();
}

