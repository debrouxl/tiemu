#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdint.h>

#include "intl.h"
#include "paths.h"
#include "support.h"

enum { 
	    COLUMN_ADDR, 
        COLUMN_0, COLUMN_1, COLUMN_2, COLUMN_3,
        COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7,
        COLUMN_8, COLUMN_9, COLUMN_A, COLUMN_B,
        COLUMN_C, COLUMN_D, COLUMN_E, COLUMN_F,
        COLUMN_NCOLS
};

#define CLIST_NCOLS (COLUMN_NCOLS)

static void renderer_edited(GtkCellRendererText * cell,
			    const gchar * path_string,
			    const gchar * new_text, gpointer user_data)
{
	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;
/*
	if (!gtk_tree_model_get_iter(model2, &iter, path))
		return;

	gtk_tree_model_get(model2, &iter, COL_MENU, &menu, -1);
	sym = menu->sym;

	gtk_tree_model_get(model2, &iter, COL_VALUE, &old_def, -1);
	new_def = new_text;

	sym_set_string_value(sym, new_def);

	config_changed = TRUE;
	update_tree(&rootmenu, NULL);
*/
	gtk_tree_path_free(path);
}

static GtkListStore* clist_init(GtkWidget *clist)
{
	GtkTreeView *view = GTK_TREE_VIEW(clist);	
	GtkListStore *list;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
	
	const gchar *text[CLIST_NCOLS] = { 
            _("Address"), 
            "0", "1", "2", "3", "4", "5", "6", "7",
            "8", "9", "A", "B", "C", "D", "E", "F"
    };
    gint i;
	
	list = gtk_list_store_new(CLIST_NCOLS, G_TYPE_STRING, 
                G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
                G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
                G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
                G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT
            );
    model = GTK_TREE_MODEL(list);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_headers_clickable(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
    for (i = 0; i < CLIST_NCOLS; i++)
    {
    	renderer = gtk_cell_renderer_text_new();

        gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i, 
            /*"editable", i,*/
            NULL);

        g_signal_connect(G_OBJECT(renderer), "edited",
			 G_CALLBACK(renderer_edited), NULL);
    }
    
    for (i = 0; i < CLIST_NCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	return list;
}

static void clist_populate(GtkListStore *list)
{
    GtkTreeIter iter;
    gchar *str;
    uint32_t addr;
    uint8_t  data;
    int i;

    for(addr = 0x000000; addr <= 0x000400; addr += 0x10)
    {
        str = g_strdup_printf("0x%06x", addr);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 
			    0, str,
			    1, 1, 
			    2, 2,
			    3, 3, 
			    4, 4,
			    5, 5, 
                6, 6, 
                7, 7, 
                8, 8, 
                9, 9, 
                10, 10,
                11, 11,
                12, 12,
                13, 13,
                14, 14,
                15, 15,
                16, 16,
                -1);
        g_free(str);

	    //g_strfreev(row_text);
    }
}

gint display_dbgmem_window(void)
{
	GladeXML *xml;
    GtkWidget *win;
	GtkWidget *data;
	gint result;

    GtkListStore *list;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_mem-2.glade"), "dbgmem_window",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	win = glade_xml_get_widget(xml, "dbgmem_window");

	data = glade_xml_get_widget(xml, "treeview1");
    list = clist_init(data);
    clist_populate(list);

    data = glade_xml_get_widget(xml, "notebook1");
    gtk_notebook_popup_enable(data);

    gtk_notebook_append_page(data, gtk_label_new("1"), gtk_label_new("1"));
    gtk_notebook_append_page(data, gtk_label_new("1"), gtk_label_new("1"));

    gtk_widget_show(GTK_WIDGET(win));

	return 0;
}

GLADE_CB void
on_dbgmem_window_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(object));
}

GLADE_CB gboolean
on_notebook1_select_page               (GtkNotebook     *notebook,
                                        gboolean         move_focus,
                                        gpointer         user_data)
{
    printf("!\n");

  return FALSE;
}