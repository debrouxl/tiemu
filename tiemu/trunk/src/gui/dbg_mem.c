#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdio.h>
#include <stdint.h>

#include "intl.h"
#include "paths.h"
#include "support.h"

static GtkWidget *notebook;
static GtkListStore *store;

enum { 
	    COLUMN_ADDR, 
        COLUMN_0, COLUMN_1, COLUMN_2, COLUMN_3,
        COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7,
        COLUMN_8, COLUMN_9, COLUMN_A, COLUMN_B,
        COLUMN_C, COLUMN_D, COLUMN_E, COLUMN_F,
		COLUMN_ASCII
};
#define CLIST_NVCOLS	(COLUMN_ASCII+1)
#define CLIST_NCOLS		(CLIST_NVCOLS+16)

static void renderer_edited(GtkCellRendererText * cell,
			    const gchar * path_string,
			    const gchar * new_text, gpointer user_data)
{
	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;
	GtkTreeModel *model = GTK_TREE_MODEL(store);
	gint value;

	if (!gtk_tree_model_get_iter(model, &iter, path))
		return;

	printf("path = <%s>\n", path_string);

	gtk_tree_model_get(model, &iter, COLUMN_ADDR, &value, -1);

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
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	list = gtk_tree_view_new_with_model(model);
	view = GTK_TREE_VIEW(list);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_headers_clickable(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	i = COLUMN_ADDR;
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
			NULL);

    for (i = COLUMN_0; i <= COLUMN_F; i++)
    {
    	renderer = gtk_cell_renderer_text_new();

        gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i, 
            "editable", i + CLIST_NVCOLS - 1,
            NULL);

        g_signal_connect(G_OBJECT(renderer), "edited",
			 G_CALLBACK(renderer_edited), NULL);
    }

	i = COLUMN_ASCII;
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
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
    uint32_t addr = start;
        
	const uint8_t sample[16] = { 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
			'A', 'B', 'C', 'D', 'E', 'F' };

    for(addr = 0x000000; addr < start+length; addr += 0x10)
    {
        gtk_list_store_append(store, &iter);

		str = g_strdup_printf("0x%06x", addr);
		gtk_list_store_set(store, &iter, COLUMN_ADDR, str, -1);
		g_free(str);

		for(i = COLUMN_0; i <= COLUMN_F; i++)
		{
			str = g_strdup_printf("%02x", sample[i-1]);
			ascii[i-1] = isascii(sample[i-1]) ? sample[i-1] : ' ';
			gtk_list_store_set(store, &iter, 
				i, str, 
				i + CLIST_NVCOLS - 1, TRUE, 
				-1);
			g_free(str);
        }
		
		ascii[16] = '\0';
		gtk_list_store_set(store, &iter, COLUMN_ASCII, ascii, -1);
    }
}

static void notebook_add_tab(GtkWidget *notebook, const char* tab_name)
{
	GtkWidget *label;
	GtkWidget *child;
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	
	label = gtk_label_new(tab_name);
	gtk_widget_show(label);

    child = clist_create(&store);
    if(!strcmp(tab_name, _("STACK")))
    {
    	clist_populate(store, 0x000000, 64);
    }
    else
    {
    	clist_populate(store, 0x000000, 64);
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
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	entry = glade_xml_get_widget(xml, "entry1");
	gtk_entry_set_text(GTK_ENTRY(entry), "0x000000");
	*addr = 0;
	
	dbox = glade_xml_get_widget(xml, "dbgmem_dbox");	
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	
	switch (result) {
	case GTK_RESPONSE_OK:
		str = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
		sscanf(str, "%0x", addr);
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

GLADE_CB gboolean
on_notebook1_select_page               (GtkNotebook     *notebook,
                                        gboolean         move_focus,
                                        gpointer         user_data)
{
    printf("!\n");

  return FALSE;
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