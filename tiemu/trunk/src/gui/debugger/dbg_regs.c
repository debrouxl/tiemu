/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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


enum { 
	    COL_NAME, COL_VALUE, 
		COL_EDITABLE, COL_COLOR, COL_FONT
};
#define CLIST_NVCOLS	(2)		// 2 visible columns
#define CLIST_NCOLS		(5)		// 5 real columns

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
    GtkWidget *tree = user_data;
	GtkTreeView *view = GTK_TREE_VIEW(tree);
	GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkTreeStore *store = GTK_TREE_STORE(model);

	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;
	
	uint32_t value;
	gint n;

	if (!gtk_tree_model_get_iter(model, &iter, path))
		return;
		
	if (strlen(path_string) < 3)
		return;
	
	// set new value	
	n = path_string[2] - '0';
	switch(path_string[0] - '0')
	{
		case 1:	// Ax
			if(validate_value(new_text, 8))
			{
				sscanf(new_text, "%x", &value);			
				gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
				ti68k_register_set_addr(n, value);
				if((n == 6) || (n == 7))
					dbgstack_refresh_window();
			}
		break;
		case 0:	// Dx
			if(validate_value(new_text, 8))
			{
				sscanf(new_text, "%x", &value);			
				gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
				ti68k_register_set_data(n, value);
			}
		break;
		case 2:	// Others
			switch(n)
			{
				case 0:	// pc
					if(validate_value(new_text, 6))
					{
						sscanf(new_text, "%x", &value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_pc(value);
					}
				break;		
				case 1:	// usp
					if(validate_value(new_text, 6))
					{
						sscanf(new_text, "%x", &value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_usp(value);
					}
				break;
                case 2:	// ssp
					if(validate_value(new_text, 6))
					{
						sscanf(new_text, "%x", &value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_ssp(value);
					}
				break;
				case 3: // sr
					if(validate_value(new_text, 4))
					{
						sscanf(new_text, "%x", &value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_sr(value);

                        // update usp <=> ssp
                        dbgregs_refresh_window();
                        dbgstack_refresh_window();
					}
				break;
				case 4: // super-flags
					if(ti68k_register_set_flags(new_text, NULL))
                    {
                        uint32_t data;
                        gchar *sdata;

						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
                        
                        // update sr, too
                        gtk_tree_path_free(path);	
                        path = gtk_tree_path_new_from_string("2:2");
                        if (!gtk_tree_model_get_iter(model, &iter, path))
		                    return;
                        ti68k_register_get_sr(&data);
                        sdata = g_strdup_printf("%04x", data);
                        gtk_tree_store_set(store, &iter, COL_VALUE, sdata,	-1);
                        g_free(sdata);
                        
                        // update usp <=> ssp
                        dbgregs_refresh_window();
                        dbgstack_refresh_window();
                    }
				case 5: // user-flags
					if(ti68k_register_set_flags(NULL, new_text))
                    {
                        uint32_t data;
                        gchar *sdata;

						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);

                        // update sr, too
                        gtk_tree_path_free(path);	
                        path = gtk_tree_path_new_from_string("2:2");
                        if (!gtk_tree_model_get_iter(model, &iter, path))
		                    return;
                        ti68k_register_get_sr(&data);
                        sdata = g_strdup_printf("%04x", data);
                        gtk_tree_store_set(store, &iter, COL_VALUE, sdata,	-1);
                        g_free(sdata);
                    }
					break;
				break;
			}
		break;
		default:
		break;
	}

	gtk_tree_path_free(path);
}

static gboolean select_func(GtkTreeSelection * selection,
			    GtkTreeModel * model,
			    GtkTreePath * path,
			    gboolean path_currently_selected,
			    gpointer data)
{
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
	
	store = gtk_tree_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, 
				G_TYPE_BOOLEAN, GDK_TYPE_COLOR, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, FALSE);
	gtk_tree_view_set_rules_hint(view, TRUE);
  
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            "", renderer, 
            "text", COL_NAME,
			"font", COL_FONT,
			NULL);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            "", renderer, 
            "text", COL_VALUE,
			"editable", COL_EDITABLE,
			"foreground-gdk", COL_COLOR,
			"font", COL_FONT,
			NULL);
			
	g_signal_connect(G_OBJECT(renderer), "edited",
			G_CALLBACK(renderer_edited), widget);

    
    for (i = 0; i < CLIST_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		//gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
	gtk_tree_selection_set_select_function(selection, select_func, NULL, NULL);

	return store;
}

static void ctree_populate(GtkTreeStore *store)
{
	GtkTreeIter node1, node2, node3;
    GtkTreeIter iter;
    int i;
    const char *others[] = { "PC", "USP", "SSP", "SR" , "sf", "uf"};
	GdkColor color;
	gboolean success;

	gdk_color_parse("Blue", &color);
	gdk_colormap_alloc_colors(gdk_colormap_get_system(), &color, 1,
				  FALSE, FALSE, &success);

	// set the 3 main nodes
	gtk_tree_store_append(store, &node1, NULL);
	gtk_tree_store_set(store, &node1, 
		COL_NAME, "Data", 
		COL_VALUE, "",  
		COL_EDITABLE, FALSE,
		COL_FONT, FONT_NAME,
		-1);
		
	gtk_tree_store_append(store, &node2, NULL);
	gtk_tree_store_set(store, &node2, 
		COL_NAME, "Addr", 
		COL_VALUE, "",  
		COL_EDITABLE, FALSE,
		COL_FONT, FONT_NAME,
		-1);
		
	gtk_tree_store_append(store, &node3, NULL);
	gtk_tree_store_set(store, &node3, 
		COL_NAME, "Other", 
		COL_VALUE, "",  
		COL_EDITABLE, FALSE,
		COL_FONT, FONT_NAME,
		-1);
		
	// populate Dx node
	for(i = 0; i < 8; i++)
	{
		gchar *str = g_strdup_printf("D%i", i);
		
		gtk_tree_store_append(store, &iter, &node1);
		gtk_tree_store_set(store, &iter,
			COL_NAME, str,
			COL_VALUE, "000000",
			COL_EDITABLE, TRUE,
			COL_FONT, FONT_NAME,
			COL_COLOR, &color,
	   		-1);
	   		
	   	g_free(str);
	}
	
	// populate Ax node
	for(i = 0; i < 8; i++)
	{
		gchar *str = g_strdup_printf("A%i", i);
		
		gtk_tree_store_append(store, &iter, &node2);
		gtk_tree_store_set(store, &iter,
			COL_NAME, str,
			COL_VALUE, "000000",
			COL_EDITABLE, TRUE,
			COL_FONT, FONT_NAME,
			COL_COLOR, &color,
	   		-1);
	   		
	   	g_free(str);
	}
	
	// populate Others node
	for(i = 0; i < 6; i++)
	{
		gtk_tree_store_append(store, &iter, &node3);
		gtk_tree_store_set(store, &iter,
			COL_NAME, others[i],
			COL_VALUE, "000000",
			COL_EDITABLE, TRUE,
			COL_FONT, FONT_NAME,
	   		-1);
	}
}

static void ctree_refresh(GtkTreeStore *store)
{
	GtkTreeModel *model = GTK_TREE_MODEL(store);
    int i;

	gchar *spath;
	GtkTreePath *path;
	GtkTreeIter iter;
	uint32_t data;
	gchar *sdata;
	int changed;

	GdkColor red, blue;
	gboolean success;
	GdkColor *color;

	gdk_color_parse("Blue", &blue);
	gdk_colormap_alloc_colors(gdk_colormap_get_system(), &blue, 1,
				  FALSE, FALSE, &success);
	gdk_color_parse("Red", &red);
	gdk_colormap_alloc_colors(gdk_colormap_get_system(), &red, 1,
				  FALSE, FALSE, &success);

	// refresh Ax nodes
	for(i = 0; i < 8; i++)
	{
		spath = g_strdup_printf("1:%i", i);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			continue;
		
		changed = ti68k_register_get_addr(i, &data);
		sdata = g_strdup_printf("%08x", data);
		color = changed ? &red : &blue;

		gtk_tree_store_set(store, &iter, COL_VALUE, sdata,	-1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color,	-1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);
	}
	
	// refresh Dx nodes
	for(i = 0; i < 8; i++)
	{
		spath = g_strdup_printf("0:%i", i);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			continue;
		
		changed = ti68k_register_get_data(i, &data);
		sdata = g_strdup_printf("%08x", data);
		color = changed ? &red : &blue;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);
	}
	
	// refresh Others node (PC)
	{
		spath = g_strdup_printf("2:%i", 0);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;
		
		changed = ti68k_register_get_pc(&data);
		sdata = g_strdup_printf("%06x", data);
		color = changed ? &red : &blue;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);	
	}
	
	// refresh Others node (USP)
	{
		spath = g_strdup_printf("2:%i", 1);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;
		
		changed = ti68k_register_get_usp(&data);
		sdata = g_strdup_printf("%06x", data);
		color = changed ? &red : &blue;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);	
	}

    // refresh Others node (SSP)
	{
		spath = g_strdup_printf("2:%i", 2);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;
		
		changed = ti68k_register_get_ssp(&data);
		sdata = g_strdup_printf("%06x", data);
		color = changed ? &red : &blue;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);	
	}
	
	// refresh Others node (SR)
	{
		spath = g_strdup_printf("2:%i", 3);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;

		changed = ti68k_register_get_sr(&data);
		sdata = g_strdup_printf("%04x", data);
		color = changed ? &red : &blue;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);	
	}

	// refresh Others node (S & U flags)
	{
		char s_flags[32], u_flags[32];

		changed = ti68k_register_get_flags(s_flags, u_flags);		
		color = changed ? &red : &blue;

		spath = g_strdup_printf("2:%i", 4);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, s_flags, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);

		spath = g_strdup_printf("2:%i", 5);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;
		
		gtk_tree_store_set(store, &iter, COL_VALUE, u_flags, -1);
		gtk_tree_store_set(store, &iter, COL_COLOR, color, -1);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);
	}

}

static GtkTreeStore *store;

static GtkWidget *wnd = NULL;
static gint already_open = 0;

/*
	Display registers window
*/
GtkWidget* dbgregs_create_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;	
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_regs-2.glade"), "dbgregs_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgregs_window");
	gtk_window_set_transient_for(GTK_WINDOW(dbox), GTK_WINDOW(main_wnd));

	data = glade_xml_get_widget(xml, "treeview1");
    store = ctree_create(data);
	ctree_populate(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));

	already_open = !0;

	return wnd = dbox;
}

GtkWidget* dbgregs_display_window(void)
{
	if(!already_open)
		wnd = dbgregs_create_window();

	if(!options3.regs.minimized)
	{
		gtk_window_resize(GTK_WINDOW(wnd), options3.regs.rect.w, options3.regs.rect.h);
		gtk_window_move(GTK_WINDOW(wnd), options3.regs.rect.x, options3.regs.rect.y);
	}
	else
		gtk_window_iconify(GTK_WINDOW(wnd));
    
	ctree_refresh(store);
	gtk_widget_show(wnd);

	return wnd;
}

void dbgregs_refresh_window(void)
{
	if(options3.regs.visible)
	{
		ctree_refresh(store);
	}
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
	    (tilp_paths_build_glade("dbg_regs-2.glade"), "dbgregs_popup",
	     PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);

	menu = glade_xml_get_widget(xml, "dbgregs_popup");
	return menu;
}

static uint32_t value = -1; //I'm lazy today !

GLADE_CB gboolean
on_treeview3_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    GtkTreeView *view = GTK_TREE_VIEW(widget);
    GtkTreeModel *model = gtk_tree_view_get_model(view);
	GtkTreePath *path;
	GtkTreeViewColumn *column;
    GtkTreeIter iter;
    gboolean ret;
    gchar *spath;
    gint i, j;
    gint col;

    if(event->type != GDK_BUTTON_PRESS)
        return FALSE;

    {
        // cell selection
        gint tx = (gint) event->x;
	    gint ty = (gint) event->y;
	    gint cx, cy;

        ret = gtk_tree_view_get_path_at_pos(view, tx, ty, &path, &column, &cx, &cy);
        if(ret == FALSE)
            return FALSE;

        col = column2index((GtkWidget *)view, column);
        spath = gtk_tree_path_to_string(path);
        sscanf(spath, "%i:%i", &i, &j);
        
        //gtk_tree_path_free(path);
        g_free(spath);
    }

    if(event->button == 1)  // first button clicked
    {
        if(!((col == 0) && (i == 2) && ((j == 4) ||(j ==5))))
            return FALSE;

        if(dbgsr_display_dbox() == -1)
            return TRUE;

        dbgregs_refresh_window();
        dbgstack_refresh_window();

        return TRUE;
    }

	if (event->button == 3)     // third button clicked
    {
        GdkEventButton *bevent;
        GtkWidget *menu;        

        // check for click on regs
        if(! 
            (
                (col == 1) && 
                (i>= 0) && (i <= 1) && 
                (j >= 0) && (j <= 7)
            ) 
          )
            return FALSE;

        // get iterator
	    if (!gtk_tree_model_get_iter(model, &iter, path))
		    return FALSE;
        gtk_tree_path_free(path);
        gtk_tree_model_get(model, &iter, COL_VALUE, &spath, -1);
        sscanf(spath, "%x", &value);
        printf("value = %x\n", value);

        // popup menu
       	bevent = (GdkEventButton *) (event);
        menu = display_popup_menu();

		gtk_menu_popup(GTK_MENU(menu),
				   NULL, NULL, NULL, NULL,
				   bevent->button, bevent->time);
	    gtk_widget_show(menu);

		return TRUE;
	}

    return FALSE;
}

GLADE_CB void
on_go_to_address4_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(value == -1)
        return;
    
    dbgmem_add_tab(value);

    value = -1;
}

/***** Status Register window *****/

static GtkWidget *btns[16];
static uint32_t tmp_sr;

gint dbgsr_display_dbox(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
	gint result;
    gint i;
    gint ret = -1;

    // load GUI
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_regs-2.glade"), "dbgsr_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgsr_window");

    // set initial states
    ti68k_register_get_sr(&tmp_sr);
    for(i = 0; i < 16; i++)
    {
        gchar *str = g_strdup_printf("checkbutton%i", i+1);
        btns[i] = glade_xml_get_widget(xml, str);
        g_free(str);

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btns[i]), 
                tmp_sr & (1 << i) ? TRUE : FALSE);
    }

	// run dialog
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
        ti68k_register_set_sr(tmp_sr);
        ret = 0;
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return ret;
}

GLADE_CB void
on_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    gint i;
    
    for(i = 0; i< 16; i++)
        if(togglebutton == (GtkToggleButton *)btns[i])
            break;
    
    if(GTK_TOGGLE_BUTTON(btns[i])->active == TRUE)
        tmp_sr |=  (1 << i);
    else
        tmp_sr &= ~(1 << i);
}
