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

enum { 
	    COL_NAME, COL_VALUE, 
		COL_EDITABLE, COL_COLORED,
};
#define CLIST_NVCOLS	(2)		// 2 visible columns
#define CLIST_NCOLS		(4)		// 4 real columns

GtkWidget *ctree;

static int validate_value(const char *str)
{
	int i;
	
	 if(strlen(str) != 8)
	 	return 0;
	
	for(i = 0; i < 8; i++)
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
    GtkWidget *list = ctree;
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkTreeStore *store = user_data;
	GtkTreeModel *model = GTK_TREE_MODEL(store);

	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;
	
	uint32_t value;
	gint n;
	
	if (!gtk_tree_model_get_iter(model, &iter, path))
		return;
		
	if (strlen(path_string) < 3)
		return;
	
	// set new value	
	n = path_string[1] - '0';
	switch(path_string[0] - '0')
	{
		case 0:	// Ax
			if(validate_value(new_text))
			{
				sscanf(new_text, "%lx", value);			
				gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
				ti68k_register_set_addr(n, value);
			}
		break;
		case 1:	// Dx
			if(validate_value(new_text))
			{
				sscanf(new_text, "%lx", value);			
				gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
				ti68k_register_set_data(n, value);
			}
		break;
		case 2:	// Others
			switch(n)
			{
				case 0:	// pc
					if(validate_value(new_text))
					{
						sscanf(new_text, "%lx", value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_pc(value);
					}
				break;		
				case 1:	// usp
					if(validate_value(new_text))
					{
						sscanf(new_text, "%lx", value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_usp(value);
					}
				break;
				case 2:	// ssp
					if(validate_value(new_text))
					{
						sscanf(new_text, "%lx", value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_ssp(value);
					}
				break;
				case 3: // sr
					if(validate_value(new_text))
					{
						sscanf(new_text, "%lx", value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_sr(value);
					}
				break;
				case 4: // flags
					/*if(validate_value(new_text))
					{
						sscanf(new_text, "%lx", value);			
						gtk_tree_store_set(store, &iter, COL_VALUE, new_text,	-1);
						ti68k_register_set_flags(value);
					}*/
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


static GtkTreeStore* ctree_create(GtkWidget *tree)
{
	GtkTreeView *view = GTK_TREE_VIEW(tree);
	GtkTreeStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
	
	const gchar *text[CLIST_NVCOLS] = { _("Name"), _("Value") };
    gint i;
	
	store = gtk_tree_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, 
				G_TYPE_BOOLEAN, GDK_TYPE_COLOR,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	//clist = tree = gtk_tree_view_new_with_model(model);
	//view = GTK_TREE_VIEW(tree);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, FALSE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[0], renderer, 
            "text", COL_NAME,
			NULL);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[1], renderer, 
            "text", COL_VALUE,
			"editable", COL_EDITABLE,
			"foreground-gdk", COL_COLORED,
			NULL);
			
	//g_signal_connect(G_OBJECT(renderer), "edited",
	//		G_CALLBACK(renderer_edited), store);

    
    for (i = 0; i < CLIST_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
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
    const char *others[] = { "PC", "USP", "SSP", "SR" , "Flags"};

	// clear tree
	gtk_tree_store_clear(store);
	
	// set the 3 main nodes
	gtk_tree_store_append(store, &node1, NULL);
	gtk_tree_store_set(store, &node1, 
		COL_NAME, "Adress", 
		COL_VALUE, "",  
		COL_EDITABLE, FALSE,
		-1);
		
	gtk_tree_store_set(store, &node2, 
		COL_NAME, "Data", 
		COL_VALUE, "",  
		COL_EDITABLE, FALSE,
		-1);
		
	gtk_tree_store_set(store, &node3, 
		COL_NAME, "Other", 
		COL_VALUE, "",  
		COL_EDITABLE, FALSE,
		-1);
		
	// populate Ax node
	for(i = 0; i < 8; i++)
	{
		gchar *str = g_strdup_printf("A%i", i);
		
		gtk_tree_store_append(store, &iter, &node1);
		gtk_tree_store_set(store, &iter,
			COL_NAME, str,
			COL_VALUE, "0x000000",
			COL_EDITABLE, TRUE,
	   		-1);
	   		
	   	g_free(str);
	}
	
	// populate Dx node
	for(i = 0; i < 8; i++)
	{
		gchar *str = g_strdup_printf("D%i", i);
		
		gtk_tree_store_append(store, &iter, &node2);
		gtk_tree_store_set(store, &iter,
			COL_NAME, str,
			COL_VALUE, "0x000000",
			COL_EDITABLE, TRUE,
	   		-1);
	   		
	   	g_free(str);
	}
	
	// populate Others node
	for(i = 0; i < 5; i++)
	{
		gtk_tree_store_append(store, &iter, &node3);
		gtk_tree_store_set(store, &iter,
			COL_NAME, others[i],
			COL_VALUE, "0x000000",
			COL_EDITABLE, TRUE,
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
	gchar *sdata;    

	// refresh Ax nodes
	for(i = 0; i < 8; i++)
	{
		spath = g_strdup_printf("0:%i", i);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			continue;
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_addr(i));
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata,	-1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);
	}
	
	// refresh Dx nodes
	for(i = 0; i < 8; i++)
	{
		spath = g_strdup_printf("1:%i", i);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			continue;
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_data(i));
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
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
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_pc());
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
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
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_addr(8));
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
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
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_sp());
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
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
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_sr());
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);	
	}
	
	// refresh Others node (flags)
	{
		spath = g_strdup_printf("2:%i", 0);
		path = gtk_tree_path_new_from_string(spath);
		if(!gtk_tree_model_get_iter(model, &iter, path))
			return;
		
		sdata = g_strdup_printf("%06x", ti68k_register_get_flag());
		gtk_tree_store_set(store, &iter, COL_VALUE, sdata, -1);
		g_free(sdata);
	   		
	   	g_free(spath);
	   	gtk_tree_path_free(path);	
	}
}


/*
	Display registers window
*/
gint display_dbgregs_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	GtkTreeStore *store;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_regs-2.glade"), "dbgregs_window",
		 PACKAGE);
	if (!xml)
		g_error("GUI loading failed !\n");
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgregs_window");

	ctree = data = glade_xml_get_widget(xml, "treeview1");
    store = ctree_create(data);
	ctree_populate(store);
	gtk_widget_show(data);

    gtk_widget_show(GTK_WIDGET(dbox));

	return 0;
}

GLADE_CB void
on_dbgregs_window_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(object));
}
