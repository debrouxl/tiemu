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

#undef GTK_DISABLE_DEPRECATED

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"
#include "dbg_vectors.h"
#include "dbg_data.h"
#include "struct.h"


enum { 
	    COL_SYMBOL, COL_TYPE, COL_STATUS, COL_START, COL_END, COL_MODE,
};
#define CLIST_NVCOLS	(6)		// 7 visible columns
#define CLIST_NCOLS		(6)		// 7 real columns

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { 
		_("Symbol"), _("Type"), _("Status"), _("Start"), _("End"), _("Mode")
    };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	//clist = tree = gtk_tree_view_new_with_model(model);
	//view = GTK_TREE_VIEW(tree);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	for(i = COL_SYMBOL; i <= COL_MODE; i++)
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

static GList** bkpts_mem_access[6] = {
	&bkpts.mem_rb, &bkpts.mem_rw, &bkpts.mem_rl, 
	&bkpts.mem_wb, &bkpts.mem_ww, &bkpts.mem_wl, 
};
static  GList** bkpts_mem_range[2] = { 
	&bkpts.mem_rng_r, &bkpts.mem_rng_w,
};
static const int bkpts_memacc_rw[6] = {
    BK_READ_BYTE, BK_READ_WORD, BK_READ_LONG,
    BK_WRITE_BYTE, BK_WRITE_WORD, BK_WRITE_LONG, 
};
static const int bkpts_memrng_rw[2] = {
	BK_READ, BK_WRITE
};

static void clist_refresh(GtkListStore *store)
{
	GList *l;
    GtkTreeIter iter;
	gint i;

	g_assert(store != NULL);
	gtk_list_store_clear(store);

	// Code breakpoints
	for(l = bkpts.code; l != NULL; l = g_list_next(l))
	{
		uint32_t addr = GPOINTER_TO_INT(l->data);
		gchar *str;
		
		str = g_strdup_printf("0x%06x", addr);
		
		gtk_list_store_append(store, &iter);

		gtk_list_store_set(store, &iter, 
		COL_SYMBOL, str, 
		COL_TYPE, ti68k_bkpt_type_to_string(BK_TYPE_CODE),
		COL_STATUS, _("enabled"),
		COL_START, str,
		COL_END, "",		
        COL_MODE, "",
		-1);
		
		g_free(str);
	}

	// Vector breakpoints
	for(l = bkpts.exception; l != NULL; l = g_list_next(l))
	{
		gint n = GPOINTER_TO_INT(l->data);
		gchar *str1, *str2;
		
		str1 = g_strdup_printf("#%i", n);
		str2 = g_strdup_printf("0x%06x", 4 * n);
		
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
		COL_SYMBOL, str1, 
		COL_TYPE, ti68k_bkpt_type_to_string(BK_TYPE_EXCEPTION),
		COL_STATUS, _("enabled"),
		COL_START, str2,
		COL_END, "",		
		COL_MODE, "",
		-1);
		
		g_free(str1);
		g_free(str2);
	}
	
	// Memory access breakpoints
	for(i = 0; i < 6; i++)
	{
		for(l = *bkpts_mem_access[i]; l != NULL; l = g_list_next(l))
		{
			uint32_t addr = GPOINTER_TO_INT(l->data);
			gchar *str;
			
			str = g_strdup_printf("0x%06x", addr);
			
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 
			COL_SYMBOL, str, 
			COL_TYPE, ti68k_bkpt_type_to_string(BK_TYPE_ACCESS),
			COL_STATUS, _("enabled"),
			COL_START, str,
			COL_END, "",
            COL_MODE, ti68k_bkpt_mode_to_string(BK_TYPE_ACCESS, bkpts_memacc_rw[i]),
			-1);
			
			g_free(str);
		}
	}
	
	// Memory range breakpoints
	for(i = 0; i < 2; i++)
	{
		for(l = *(bkpts_mem_range[i]); l != NULL; l = g_list_next(l))
		{
			ADDR_RANGE *s = l->data;
			gchar *str1, *str2;
			
			str1 = g_strdup_printf("0x%06x", s->val1);
			str2 = g_strdup_printf("0x%06x", s->val2);
			
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 
			COL_SYMBOL, str1, 
			COL_TYPE, ti68k_bkpt_type_to_string(BK_TYPE_RANGE),
			COL_STATUS, _("enabled"),
			COL_START, str1,
			COL_END, str2,
            COL_MODE, ti68k_bkpt_mode_to_string(BK_TYPE_RANGE, bkpts_memrng_rw[i]),
			-1);
			
			g_free(str1);
			g_free(str2);
		}
	}
}

static GtkListStore *store = NULL;
static gint already_open = 0;

/*
	Display registers window
*/
gint display_dbgbkpts_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_bkpts-2.glade"), "dbgbkpts_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgbkpts_window");

	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_refresh(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));
	gtk_widget_show(data);

	gtk_window_resize(GTK_WINDOW(dbox), options3.bkpts.w, options3.bkpts.h);
    gtk_widget_show(GTK_WIDGET(dbox));

	already_open = !0;

	return 0;
}

gint refresh_dbgbkpts_window(void)
{
	if(!already_open)
		display_dbgbkpts_window();

	clist_refresh(store);

	return 0;
}


static GtkWidget* display_popup_menu(void)
{
	GladeXML *xml;
	GtkWidget *data;
  
	//menu = create_popup_menu();
	xml = glade_xml_new
	    (tilp_paths_build_glade("dbg_bkpts-2.glade"), "dbgbkpts_popup",
	     PACKAGE);
	if (!xml)
		g_error(_("dbg_bkpts-2.glade: GUI loading failed !\n"));
	glade_xml_signal_autoconnect(xml);

	data = glade_xml_get_widget(xml, "dbgbkpts_popup");

	return data;
}

GLADE_CB gboolean
on_dbgbkpts_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	gdk_window_get_size(widget->window, &options3.bkpts.w, &options3.bkpts.h);
	gdk_window_get_root_origin(widget->window, &options3.bkpts.x, &options3.bkpts.y);

	return FALSE;
}

GLADE_CB void
on_dbgbkpts_window_destroy             (GtkObject       *object,
                                        gpointer         user_data)
{
	already_open = 0;
}

/* Add bkpt */
GLADE_CB void
dbgbkpts_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *menu;
	guint butt = 0;
	guint32 time;

	time = gtk_get_current_event_time();
	menu = display_popup_menu();
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, butt, time);
	gtk_widget_show(menu);
}

/* Remove bkpt */
GLADE_CB void
dbgbkpts_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *list = GTK_WIDGET(button);   // arg are swapped, why ?
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
        gchar** row_text = g_malloc0((CLIST_NVCOLS + 1) * sizeof(gchar *));
        uint32_t n, type, min, max, mode;
			
		gtk_tree_model_get_iter(model, &iter, path);
		gtk_tree_model_get(model, &iter, 
            COL_SYMBOL, &row_text[COL_SYMBOL], 
            COL_TYPE, &row_text[COL_TYPE], 
            COL_START, &row_text[COL_START], 
            COL_END, &row_text[COL_END],
            COL_MODE, &row_text[COL_MODE],
            -1);
		
        type = ti68k_string_to_bkpt_type(row_text[COL_TYPE]);
        switch(type)
        {
        case BK_TYPE_CODE:
            sscanf(row_text[COL_START], "%lx", (long *)&min);
            ti68k_bkpt_del_address(min);
            break;
        case BK_TYPE_EXCEPTION:
            sscanf(row_text[COL_SYMBOL], "#%i", &n);
            ti68k_bkpt_del_exception(n);
            break;
        case BK_TYPE_ACCESS:
            mode = ti68k_string_to_bkpt_mode(row_text[COL_MODE]);
            sscanf(row_text[COL_START], "%lx", (long *)&min);
            ti68k_bkpt_del_access(min, mode);
            break;
        case BK_TYPE_RANGE:
            mode = ti68k_string_to_bkpt_mode(row_text[COL_MODE]);
            sscanf(row_text[COL_START], "%lx", (long *)&min);
            sscanf(row_text[COL_END], "%lx", (long *)&max);
            ti68k_bkpt_del_range(min, max, mode);        
            break;
        }
        g_strfreev(row_text);
    }

    refresh_dbgbkpts_window();
}


GLADE_CB void
dbgbkpts_button3_clicked                     (GtkButton       *button,
                                        GtkWidget       *widget,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgbkpts_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgbkpts_data_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_dbgdata_dbox();
}



GLADE_CB void
dbgbkpts_vector_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	display_dbgvectors_dbox();
}


