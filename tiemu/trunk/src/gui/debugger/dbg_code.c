#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"

enum { 
	    COL_ICON, COL_ADDR, COL_DISASM,
};
#define CLIST_NVCOLS	(3)		// 3 visible columns
#define CLIST_NCOLS		(3)		// 4 real columns

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { _(""), _("Address"), _("Disassembly") };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING,
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
  
	for(i = COL_ADDR; i <= COL_DISASM; i++)
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
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);	// SINGLE ?

	return store;
}

static void clist_populate(GtkListStore *store)
{
    GtkTreeIter iter;
	gint i;

	for(i = 0; i < 128; i++)
	{
	    gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
		COL_ADDR, i, 
		COL_DISASM, ti68k_exception_to_string(i),
		-1);
	}
	/*
	pix1 = create_pixbuf("up.ico");
	COL_ICON, pix1
	*/
}

/*
	Display source code window
*/
gint display_dbgcode_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	GtkListStore *store;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_code-2.glade"), "dbgcode_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgcode_window");

	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_populate(store);
	//clist_refresh(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));
	gtk_widget_show(data);

	gtk_window_resize(GTK_WINDOW(dbox), 320, 240);
    gtk_widget_show(GTK_WIDGET(dbox));

	return 0;
}

GLADE_CB void
on_dbgcode_window_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(object));
}

GLADE_CB void
dbgcode_dbgcode_window_destroy              (GtkObject       *object,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgcode_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgcode_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgcode_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgcode_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgcode_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgcode_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}

