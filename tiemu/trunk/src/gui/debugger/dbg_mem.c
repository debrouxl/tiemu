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

// set_page, to remove...
#undef GTK_DISABLE_DEPRECATED

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

static GtkWidget *notebook;

enum { 
	    COL_ADDR, 
        COL_0, COL_1, COL_2, COL_3,
        COL_4, COL_5, COL_6, COL_7,
        COL_8, COL_9, COL_A, COL_B,
        COL_C, COL_D, COL_E, COL_F,
		COL_ASCII,
		COL_0b, COL_1b, COL_2b, COL_3b, 
		COL_4b, COL_5b, COL_6b, COL_7b, 
		COL_8b, COL_9b, COL_Ab, COL_Bb, 
		COL_Cb, COL_Db, COL_Eb, COL_Fb,
		COL_COLOR, COL_FONT
};
#define CLIST_NVCOLS	(18)
#define CLIST_NCOLS		(18+18)

#define FONT_NAME	"courier"

static gint column2index(GtkWidget *list, GtkTreeViewColumn * column)
{
	gint i;

	for (i = 0; i < CLIST_NVCOLS; i++) {
		GtkTreeViewColumn *col;

		col = gtk_tree_view_get_column(GTK_TREE_VIEW(list), i);
		if (col == column)
			return i;
	}

	return -1;
}

static void renderer_edited(GtkCellRendererText * cell,
			    const gchar * path_string,
			    const gchar * new_text, gpointer user_data)
{
    GtkWidget *list = user_data;
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkListStore *store = GTK_LIST_STORE(model);

    GtkTreeViewColumn *column;
    GtkTreeIter iter;
	GtkTreePath *path;	
	gint col;
    gchar *str_addr;
    gchar *str_data = (char *)new_text;
    int addr, data;
    uint8_t *mem_ptr;

    // get column
    gtk_tree_view_get_cursor(view, &path, &column);
    if(!path || !column)
        return;

    // get iterator
	if (!gtk_tree_model_get_iter(model, &iter, path))
		return;

    // get old value
	col = column2index(list, column);
    gtk_tree_model_get(model, &iter, COL_ADDR, &str_addr, -1);

    // check for new value
    if(!isxdigit(str_data[0]) || !isxdigit(str_data[1]) || (strlen(str_data) > 2))
    {
        gtk_tree_path_free(path);
        return;
    }

    // set new value
    gtk_list_store_set(store, &iter, col, new_text,	-1);

    // and update memory
    sscanf(str_addr, "%x", &addr);
    sscanf(str_data, "%x", &data);
    addr += (col - COL_0);
    mem_ptr = (uint8_t *)ti68k_get_real_address(addr);
	*mem_ptr = data;

    g_free(str_addr);
	gtk_tree_path_free(path);
}

static GtkWidget* clist_create(GtkListStore **st)
{
	GtkWidget *list;
	GtkTreeView *view;
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
	
	const gchar *text[CLIST_NVCOLS] = { 
            _("Address"), 
            "+0", "+1", "+2", "+3", "+4", "+5", "+6", "+7",
            "+8", "+9", "+A", "+B", "+C", "+D", "+E", "+F",
			"ASCII"
    };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_STRING, 
                G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				G_TYPE_STRING,
				G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN,
				G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN,
				G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN,
				G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN,
				GDK_TYPE_COLOR, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	list = gtk_tree_view_new_with_model(model);
	view = GTK_TREE_VIEW(list);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, FALSE);
	gtk_tree_view_set_rules_hint(view, TRUE);
  
	i = COL_ADDR;
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
			"font", COL_FONT,
			"foreground-gdk", COL_COLOR,
			NULL);

    for (i = COL_0; i <= COL_F; i++)
    {
    	renderer = gtk_cell_renderer_text_new();

        gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i, 
			"font", COL_FONT,
            "editable", i + CLIST_NVCOLS - 1,
            NULL);

        g_signal_connect(G_OBJECT(renderer), "edited",
			 G_CALLBACK(renderer_edited), list);
    }

	i = COL_ASCII;
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
			"font", COL_FONT,
			"foreground-gdk", COL_COLOR,
			NULL);
    
    for (i = 0; i < CLIST_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

	*st = store;
	return list;
}

static void clist_populate(GtkListStore *store, uint32_t start, int length)
{
    GtkTreeIter iter;
    int i;
    gchar *str;
    char ascii[17];
    uint32_t a;
	GdkColor color;
	gboolean success;

	gdk_color_parse("DarkGray", &color);
	gdk_colormap_alloc_colors(gdk_colormap_get_system(), &color, 1,
				  FALSE, FALSE, &success);

    for(a = start; a < start+length; a += 0x10)
    {
		uint32_t addr = a & 0xffffff;
		uint8_t *mem_ptr;

		char *utf;
		gsize bw;

        gtk_list_store_append(store, &iter);

		str = g_strdup_printf("%06x", addr);
		gtk_list_store_set(store, &iter, 
			COL_ADDR, str, 
			COL_COLOR, &color,
			COL_FONT, FONT_NAME,
			-1);
		g_free(str);

		for(i = COL_0; i <= COL_F; i++)
		{
			mem_ptr = ti68k_get_real_address(addr + (i-COL_0));

			str = g_strdup_printf("%02x", *mem_ptr);
			ascii[i-COL_0] = (isprint(*mem_ptr) ? *mem_ptr : '.');

			gtk_list_store_set(store, &iter, 
				i, str, 
				i + CLIST_NVCOLS - COL_0, TRUE, 
				-1);

			g_free(str);
        }
	 
		ascii[16] = '\0';
		utf = g_locale_to_utf8(ascii, -1, NULL, &bw, NULL);
		gtk_list_store_set(store, &iter, 
			COL_ASCII, utf, 
			COL_COLOR, &color,
			-1);
    }
}

static void clist_refresh(GtkListStore *store, uint32_t start, int length)
{
	gtk_list_store_clear(store);
	clist_populate(store, start, length);
}

static void notebook_add_page(GtkWidget *notebook, const char* tab_name)
{
	GtkListStore *store;
	GtkWidget *label;
	GtkWidget *child;
	GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	gint page = gtk_notebook_get_current_page(nb);
	uint32_t addr;
	uint32_t len;
	
	label = gtk_label_new(tab_name);
	gtk_widget_show(label);

    child = clist_create(&store);
    if(!strcmp(tab_name, _("STACK")))
    {
		// display stack
		uint32_t sp_start, sp_end;
		uint32_t *mem_ptr = (uint32_t *)ti68k_get_real_address(0x000000);

		sp_start = GUINT32_SWAP_LE_BE(*mem_ptr);
		ti68k_register_get_sp(&sp_end);
		len = sp_end - sp_start;

		clist_populate(store, sp_start, len <= 128 ? len : 128);
    }
    else
    {
		// display normal
		sscanf(tab_name, "%06x", &addr);
    	clist_populate(store, addr, 128);
    }
	gtk_widget_show(child);

	gtk_notebook_insert_page(nb, child, label, page);
	gtk_notebook_set_page(nb, page);
}


/*
	Type address in a box.
*/
gint display_dbgmem_dbox(uint32_t *addr)
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *entry;
	gint result;
	gchar *str;
	gint ret = -1;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_mem-2.glade"), "dbgmem_dbox", PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	entry = glade_xml_get_widget(xml, "entry1");
	gtk_entry_set_text(GTK_ENTRY(entry), "000000");
	*addr = 0;
	
	dbox = glade_xml_get_widget(xml, "dbgmem_dbox");	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	
	switch (result) {
	case GTK_RESPONSE_OK:
		str = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
		sscanf(str, "%x", addr);
		ret = 0;
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);
	return ret;
}

static gint already_open = 0;

/*
	Display memory window
*/
GtkWidget* create_dbgmem_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_mem-2.glade"), "dbgmem_window",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgmem_window");

    notebook = glade_xml_get_widget(xml, "notebook1");
    gtk_notebook_popup_enable(GTK_NOTEBOOK(notebook));
    
    notebook_add_page(notebook, _("STACK"));
	notebook_add_page(notebook, "0x000000");

    gtk_window_resize(GTK_WINDOW(dbox), options3.mem.w, options3.mem.h);
    gtk_window_move(GTK_WINDOW(dbox), options3.mem.x, options3.mem.y);

	already_open = !0;

	return dbox;
}

static void refresh_page(int page, int offset);

GtkWidget* display_dbgmem_window(void)
{
    static GtkWidget *wnd = NULL;

	if(!already_open)
		wnd = create_dbgmem_window();
    gtk_widget_show(wnd);

	refresh_page(0, 0);

    return wnd;
}

GLADE_CB void
dbgmem_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	uint32_t addr;
	gchar *str;
	
	if(display_dbgmem_dbox(&addr) == -1)
		return;
	
	str = g_strdup_printf("%06x", addr);
	notebook_add_page(notebook, str);
	g_free(str);
}

GLADE_CB void
dbgmem_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	gint page = gtk_notebook_get_current_page(nb);
	
	gtk_notebook_remove_page(nb, page);
}

GLADE_CB void
dbgmem_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	gint page = gtk_notebook_get_current_page(nb);

	refresh_page(page, -0x10);
}

GLADE_CB void
dbgmem_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	gint page = gtk_notebook_get_current_page(nb);

	refresh_page(page, +0x10);
}

GLADE_CB void
on_notebook1_switch_page               (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
	refresh_page(page_num, 0);
}

static void refresh_page(int page, int offset)
{
	GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	GtkWidget *tab;
	GtkWidget *label;
	G_CONST_RETURN gchar *text;
	uint32_t addr, len = 128;

	GList *l, *elt;
	GtkWidget *list;
	GtkTreeView *view;
	GtkTreeModel *model;
	GtkListStore *store;
	gchar *str;

	// retrieve addr by tab name
	tab = gtk_notebook_get_nth_page(nb, page);
	label = gtk_notebook_get_tab_label(nb, tab);
	text = gtk_label_get_text(GTK_LABEL(label));

	// get list pointer (we have 1 child)
	l = gtk_container_get_children(GTK_CONTAINER(nb));
	elt = g_list_nth(l, page);
	list = GTK_WIDGET(elt->data);
	view = GTK_TREE_VIEW(list);
	model = gtk_tree_view_get_model(view);
	store = GTK_LIST_STORE(model);

	if(!strcmp(text, "STACK"))
	{
		uint32_t sp_start, sp_end;
		uint32_t *mem_ptr = (uint32_t *)ti68k_get_real_address(0x000000);

		sp_start = GUINT32_SWAP_LE_BE(*mem_ptr);
		ti68k_register_get_sp(&sp_end);
		len = sp_end - sp_start;
		addr = sp_start;
	}
	else
	{
		sscanf(text, "%x", &addr);
		len = 128;
	}

	addr += offset;
	addr &= 0xffffff;

	str = g_strdup_printf("%06x", addr);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);
   	clist_refresh(store, addr, len <= 128 ? len : 128);
}
