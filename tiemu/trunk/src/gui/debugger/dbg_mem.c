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
#include <gdk/gdkkeysyms.h>

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

#define DUMP_SIZE       128

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
    int addr, data, i;
    uint8_t *mem_ptr;

    // get column
    gtk_tree_view_get_cursor(view, &path, &column);
    if(!path || !column)
        return;

    // get iterator
	if (!gtk_tree_model_get_iter(model, &iter, path))
		return;

    // get address
	col = column2index(list, column);
    gtk_tree_model_get(model, &iter, COL_ADDR, &str_addr, -1);

    // check for new value
    if((strlen(str_data) % 2) != 0)
    {
        gtk_tree_path_free(path);
        return;
    }

    for(i = 0; i < (int)strlen(str_data); i++)
        if(!isxdigit(str_data[i]))
            {
                gtk_tree_path_free(path);
                return;
            }

    // set new value(s) and update memory
    for(i = 0; (i < (int)strlen(str_data)/2) && ((col+i) <= COL_F); i++)
    {
        char digits[3];

        strncpy(digits, &new_text[2*i], 2); 
        digits[2] = '\0';

        gtk_list_store_set(store, &iter, col+i, digits, -1);

        sscanf(str_addr, "%x", &addr);
        sscanf(digits, "%x", &data);
        addr += (col - COL_0) + i;

        mem_ptr = (uint8_t *)ti68k_get_real_address(addr);
	    *mem_ptr++ = data;
    }

    g_free(str_addr);
	gtk_tree_path_free(path);
}

static void refresh_page(int page, int offset);
static gboolean
on_treeview_key_press_event            (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);
static gboolean
on_treeview_btn_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

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
    gtk_tree_view_set_headers_visible(view, TRUE);
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
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    g_signal_connect(G_OBJECT(list), "key_press_event",
			 G_CALLBACK(on_treeview_key_press_event), list);
    g_signal_connect(G_OBJECT(list), "button_press_event",
			 G_CALLBACK(on_treeview_btn_press_event), list);

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
	
	label = gtk_label_new(tab_name);
	gtk_widget_show(label);

    child = clist_create(&store);

	// display normal
	sscanf(tab_name, "%06x", &addr);
   	clist_populate(store, addr, DUMP_SIZE);

	gtk_widget_show(child);

	gtk_notebook_insert_page(nb, child, label, page);
	gtk_notebook_set_page(nb, page);
}

static gint already_open = 0;

/*
	Display memory window
*/
GtkWidget* dbgmem_create_window(void)
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
    
	notebook_add_page(notebook, "0x000000");

    gtk_window_resize(GTK_WINDOW(dbox), options3.mem.w, options3.mem.h);
    gtk_window_move(GTK_WINDOW(dbox), options3.mem.x, options3.mem.y);

	already_open = !0;

	return dbox;
}

GtkWidget* dbgmem_display_window(void)
{
    static GtkWidget *wnd = NULL;

	if(!already_open)
		wnd = dbgmem_create_window();
    gtk_widget_show(wnd);

	refresh_page(0, 0);

    return wnd;
}

void dbgmem_refresh_window(void)
{
	if(dbgs.mem)
	{
        GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	    gint page = gtk_notebook_get_current_page(nb);

        refresh_page(page, 0);
	}
}

void dbgmem_add_tab(uint32_t addr)
{
    gchar *str;
	
	str = g_strdup_printf("%06x", addr);
	notebook_add_page(notebook, str);
	g_free(str);
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
	uint32_t addr, len = DUMP_SIZE;

	GList *l, *elt;
	GtkWidget *list;
	GtkTreeView *view;
	GtkTreeModel *model;
	GtkListStore *store;
	gchar *str;

    static uint8_t *cpy[DUMP_SIZE] = { 0 };


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

    // get new address
		sscanf(text, "%x", &addr);
		len = DUMP_SIZE;

	addr += offset;
	addr &= 0xffffff;

    // refresh only if mem changed (speed-up)
    if(!offset)
    {
        uint8_t *mem = (uint8_t *)ti68k_get_real_address(addr);

        if(!memcmp(mem, cpy, DUMP_SIZE))
            return;

        memcpy(cpy, mem, DUMP_SIZE);
    }

    // refresh tab
	str = g_strdup_printf("%06x", addr);
	gtk_label_set_text(GTK_LABEL(label), str);
	g_free(str);

    // and list
   	clist_refresh(store, addr, len <= DUMP_SIZE ? len : DUMP_SIZE);
}

static gboolean
on_treeview_key_press_event            (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
    GtkTreeView *view = GTK_TREE_VIEW(widget);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkListStore *store = GTK_LIST_STORE(model);
    GtkTreeSelection *selection;
    GtkTreeIter iter;
    gboolean valid;
    gchar *str;
    gchar *row;
    gint row_idx, row_max;
    uint32_t addr;
    uint32_t min, max;
    gint n;

    GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	gint page = gtk_notebook_get_current_page(nb);

    // get min address
    gtk_tree_model_get_iter_first(model, &iter);
    gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
    sscanf(str, "%x", &min);

    // get max address
    n = gtk_tree_model_iter_n_children(model, NULL);
    gtk_tree_model_iter_nth_child(model, &iter, NULL, n-1);
    gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
    sscanf(str, "%x", &max);

    // retrieve selection
    selection = gtk_tree_view_get_selection(view);
    valid = gtk_tree_selection_get_selected(selection, NULL, &iter);
    if(valid)
    {
        gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
        sscanf(str, "%x", &addr);

        row = gtk_tree_model_get_string_from_iter(model, &iter);
        sscanf(row, "%i", &row_idx);
        row_max = gtk_tree_model_iter_n_children(model, NULL) - 1;
    }
    else
        row_idx = row_max = -1;

    switch(event->keyval) 
	{
    case GDK_Up:
        if(row_max == -1)
            break;

        if(row_idx > 0)
            break;

        refresh_page(page, -0x10);

        return FALSE;

    case GDK_Down:
        if(row_max == -1)
            break;

        if(row_idx < row_max)
            break;

        refresh_page(page, +0x10);

        {
            GtkTreePath *path = gtk_tree_path_new_from_string("7");
            gtk_tree_selection_select_path(selection, path);
        }
        
        return TRUE;

    case GDK_Page_Up:
        if(row_max == -1)
            break;

        if(row_idx > 0)
            break;

        refresh_page(page, -DUMP_SIZE);

        {
            GtkTreePath *path = gtk_tree_path_new_from_string("0");
            gtk_tree_selection_select_path(selection, path);
        }

        return TRUE;

    case GDK_Page_Down:
        if(row_max == -1)
            break;

        if(row_idx < row_max)
            break;

        refresh_page(page, +DUMP_SIZE);

        {
            GtkTreePath *path = gtk_tree_path_new_from_string("7");
            gtk_tree_selection_select_path(selection, path);
        }

        return TRUE;

	default:
		return FALSE;
	}

    return FALSE;
}

/***** Popup menu *****/

/*
	Display popup menu (right click)
*/
static GtkWidget* display_popup_menu(void)
{
	GladeXML *xml;
	GtkWidget *menu;

	xml = glade_xml_new
	    (tilp_paths_build_glade("dbg_mem-2.glade"), "dbgmem_popup",
	     PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);

	menu = glade_xml_get_widget(xml, "dbgmem_popup");
	return menu;
}

static gboolean
on_treeview_btn_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    GtkTreeView *view = GTK_TREE_VIEW(widget);
	GtkTreePath *path;
	GtkTreeViewColumn *column;
    gboolean ret;

    switch (event->type) 
    {
    case GDK_BUTTON_PRESS:	// third button clicked
	    if (event->button == 3) 
        {
            GdkEventButton *bevent;
            GtkWidget *menu;

            //--- set cell focus
            gint tx = (gint) event->x;
	        gint ty = (gint) event->y;
	        gint cx, cy;

	        ret = gtk_tree_view_get_path_at_pos(view, tx, ty, &path, &column, &cx, &cy);
            gtk_tree_view_set_cursor(view, path, column, FALSE);
            gtk_tree_path_free(path);
            //printf("%i %i %i %i (%i)\n", tx, ty, cx, cy, ret);
            //---

		    bevent = (GdkEventButton *) (event);
            menu = display_popup_menu();

		    gtk_menu_popup(GTK_MENU(menu),
				       NULL, NULL, NULL, NULL,
				       bevent->button, bevent->time);
	        gtk_widget_show(menu);

		    return TRUE;
	    }
	    break;
    default:
        break;
    }

    return FALSE;
}

GLADE_CB void
on_find1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


GLADE_CB void
on_find_next1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


GLADE_CB void
on_go_to_address2_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkNotebook *nb = GTK_NOTEBOOK(notebook);
	gint page = gtk_notebook_get_current_page(nb);

    uint32_t addr;
	gchar *str;
	
	if(display_dbgmem_dbox(&addr) == -1)
		return;

    gtk_notebook_remove_page(nb, page);
	
	str = g_strdup_printf("%06x", addr);
	notebook_add_page(notebook, str);
	g_free(str);
}


GLADE_CB void
on_dissassemble1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkNotebook *nb = GTK_NOTEBOOK(notebook);
    gint page = gtk_notebook_get_current_page(nb);

	GList *l, *elt;
	GtkWidget *list;
	GtkTreeView *view;
	GtkTreeModel *model;
	GtkListStore *store;
	gchar *str;

    GtkTreePath *path;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    gint col;
    uint32_t addr;

	// get list pointer (we have 1 child)
	l = gtk_container_get_children(GTK_CONTAINER(nb));
	elt = g_list_nth(l, page);
	list = GTK_WIDGET(elt->data);
	view = GTK_TREE_VIEW(list);
	model = gtk_tree_view_get_model(view);
	store = GTK_LIST_STORE(model);

    // get column
    gtk_tree_view_get_cursor(view, &path, &column);
    if(!path || !column)
        return;

    // get iterator
	if (!gtk_tree_model_get_iter(model, &iter, path))
		return;

    // get old value
	col = column2index(list, column);
    gtk_tree_model_get(model, &iter, COL_ADDR, &str, -1);
    sscanf(str, "%06x", &addr);
    
    // populate code
    dbgcode_disasm_at(addr + (col-1));
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