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
	    COL_SYMBOL, COL_TYPE, COL_START, COL_END, COL_STATUS, COL_RW, COL_SIZE,
};
#define CLIST_NVCOLS	(7)		// 7 visible columns
#define CLIST_NCOLS		(7)		// 7 real columns

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { 
		_("Symbol"), _("Type"), _("Start"), _("End"), _("Status"), _("R/W"), _("Size")
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
  
	for(i = COL_SYMBOL; i <= COL_SIZE; i++)
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

static void clist_refresh(GtkListStore *store)
{
    GtkTreeIter iter;
	gchar *str;

	gtk_list_store_clear(store);
/*
	str = g_strdup("foo");
    gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, str, -1);
	g_free(str);
*/


}

/*
	Display registers window
*/
gint display_dbgbkpts_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	GtkListStore *store;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_bkpts-2.glade"), "dbgbkpts_window",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgbkpts_window");

	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_refresh(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));
	gtk_widget_show(data);

	gtk_window_resize(GTK_WINDOW(dbox), 320, 60);
    gtk_widget_show(GTK_WIDGET(dbox));

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


GLADE_CB void
on_dbgbkpts_window_destroy             (GtkObject       *object,
                                        gpointer         user_data)
{

}


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


GLADE_CB void
dbgbkpts_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

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
dbgbkpts_access1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgbkpts_range1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgbkpts_trap1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgbkpts_exception1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


GLADE_CB void
dbgbkpts_autoint1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

