#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"

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
		COL_COLOR
};
#define CLIST_NVCOLS	(18)
#define CLIST_NCOLS		(18+17)

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
    sscanf(str_addr, "%lx", &addr);
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
				GDK_TYPE_COLOR,
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
			"foreground-gdk", COL_COLOR,
			NULL);

    for (i = COL_0; i <= COL_F; i++)
    {
    	renderer = gtk_cell_renderer_text_new();

        gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i, 
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

	*st = store;
	return list;
}

static void clist_populate(GtkListStore *store, uint32_t start, int length)
{
    GtkTreeIter iter;
    int i;
    gchar *str;
    char ascii[17];
    uint32_t addr;
	GdkColor color;
	gboolean success;

	gdk_color_parse("DarkGray", &color);
	gdk_colormap_alloc_colors(gdk_colormap_get_system(), &color, 1,
				  FALSE, FALSE, &success);

    for(addr = start; addr < start+length; addr += 0x10)
    {
		uint8_t *mem_ptr;

        gtk_list_store_append(store, &iter);

		str = g_strdup_printf("0x%06x", addr);
		gtk_list_store_set(store, &iter, 
			COL_ADDR, str, 
			COL_COLOR, &color,
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
		gtk_list_store_set(store, &iter, 
			COL_ASCII, ascii, 
			COL_COLOR, &color,
			-1);
    }
}

static void notebook_add_tab(GtkWidget *notebook, const char* tab_name)
{
	GtkListStore *store;
	GtkWidget *label;
	GtkWidget *child;
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
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
		sscanf(tab_name, "0x%06x", &addr);
    	clist_populate(store, addr, 64);
    }
	gtk_widget_show(child);

	gtk_notebook_insert_page(GTK_NOTEBOOK(notebook), child, label, current_page);
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
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_mem-2.glade"), "dbgmem_dbox", PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	entry = glade_xml_get_widget(xml, "entry1");
	gtk_entry_set_text(GTK_ENTRY(entry), "0x000000");
	*addr = 0;
	
	dbox = glade_xml_get_widget(xml, "dbgmem_dbox");	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	
	switch (result) {
	case GTK_RESPONSE_OK:
		str = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
		sscanf(str, "%lx", addr);
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);
	return 0;
}

/*
	Display memory window
*/
gint display_dbgmem_window(void)
{
	GladeXML *xml;
    GtkWidget *data;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_mem-2.glade"), "dbgmem_window",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	data = glade_xml_get_widget(xml, "dbgmem_window");

    notebook = glade_xml_get_widget(xml, "notebook1");
    gtk_notebook_popup_enable(GTK_NOTEBOOK(notebook));
    
    notebook_add_tab(notebook, _("STACK"));

    gtk_widget_show(GTK_WIDGET(data));

	return 0;
}

GLADE_CB void
on_dbgmem_window_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(object));
}


GLADE_CB void
on_add1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	uint32_t addr;
	gchar *str;
	
	display_dbgmem_dbox(&addr);
	
	str = g_strdup_printf("0x%06x", addr);
	notebook_add_tab(notebook, str);
	g_free(str);
}


GLADE_CB void
on_del1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	
	gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), current_page);
}