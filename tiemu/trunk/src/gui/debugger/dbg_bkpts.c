#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"
#include "dbg_vectors.h"
#include "dbg_data.h"


enum { 
	    COL_SYMBOL, COL_TYPE, COL_STATUS, COL_START, COL_END, COL_RW, COL_SIZE,
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
		_("Symbol"), _("Type"), _("Status"), _("Start"), _("End"), _("Mode"), _("Type")
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

static GList** bkpts_mem_access[6] = {
	&bkpts.mem_rb, &bkpts.mem_rw, &bkpts.mem_rl, 
	&bkpts.mem_wb, &bkpts.mem_ww, &bkpts.mem_wl, 
};
static  GList** bkpts_mem_range[2] = { 
	&bkpts.mem_rng_r, &bkpts.mem_rng_w,
};
static const char* bkpts_memacc_rw[6] = {
	_("read"), _("read"), _("read"), 
	_("write"), _("write"), _("write"), 
};
static const char* bkpts_memrng_rw[2] = {
	_("read"), _("write"),
};
static const char* bkpts_memacc_size[6] = {
	_("byte"), _("word"), _("long"),
	_("byte"), _("word"), _("long"),
};

static void clist_refresh(GtkListStore *store)
{
	GList *l;
    GtkTreeIter iter;
	gchar *str, *str1, *str2;
	gint i;

	gtk_list_store_clear(store);

	// Code breakpoints
	for(l = bkpts.code; l != NULL; l = g_list_next(l))
	{
		uint32_t addr = GPOINTER_TO_INT(l->data);
		
		str = g_strdup_printf("0x%06x", addr);
		
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
		COL_SYMBOL, str, 
		COL_TYPE, _("code"),
		COL_STATUS, _("enabled"),
		COL_START, str,
		COL_END, "",		
		COL_RW, "",
		COL_SIZE, "",
		-1);
		
		g_free(str);
	}

	// Vector breakpoints
	for(l = bkpts.exception; l != NULL; l = g_list_next(l))
	{
		gint n = GPOINTER_TO_INT(l->data);
		
		str1 = g_strdup_printf("#%i", n);
		str2 = g_strdup_printf("0x%06x", 4 * n);
		
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
		COL_SYMBOL, str1, 
		COL_TYPE, _("exception"),
		COL_STATUS, _("enabled"),
		COL_START, str2,
		COL_END, "",		
		COL_RW, "",
		COL_SIZE, "",
		-1);
		
		g_free(str);
	}
	
	// Memory access breakpoints
	for(i = 0; i < 6; i++)
	{
		for(l = *bkpts_mem_access[i]; l != NULL; l = g_list_next(l))
		{
			uint32_t addr = GPOINTER_TO_INT(l->data);
			
			str = g_strdup_printf("0x%06x", addr);
			
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 
			COL_SYMBOL, str, 
			COL_TYPE, _("mem access"),
			COL_STATUS, _("enabled"),
			COL_START, str,
			COL_END, "",		
			COL_RW, bkpts_memacc_rw[i],
			COL_SIZE, bkpts_memacc_size[i],
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
			
			str1 = g_strdup_printf("0x%06x", s->val1);
			str2 = g_strdup_printf("0x%06x", s->val2);
			
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 
			COL_SYMBOL, str1, 
			COL_TYPE, _("mem range"),
			COL_STATUS, _("enabled"),
			COL_START, str1,
			COL_END, str2,		
			COL_RW, bkpts_memrng_rw[i],
			COL_SIZE, _("any"),
			-1);
			
			g_free(str);
		}
	}
}

GtkListStore *store;

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

	gtk_window_resize(GTK_WINDOW(dbox), 320, 120);
    gtk_widget_show(GTK_WIDGET(dbox));

	return 0;
}

gint refresh_dbgbkpts_window(void)
{
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


