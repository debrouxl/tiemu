#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "dbkpt_cb.h"
#include "dbkpt_dbox.h"
#include "support.h"

//#include "platform.h"
#include "struct.h"
#include "extern.h"
#include "hid.h"
#include "adbkpt_cb.h"
#include "adbkpt_dbox.h"

#include "interface.h"
#include "bkpts.h"

extern DATA_BKPT db;

static gint sel_row = -1;

gint display_data_bkpts_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *clist;
  GtkListStore *list;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  gint i;
  gchar buffer[MAXCHARS];
  DATA_BKPT *s;

  gchar *text[3] = { _("Type"), _("Mode"),
		     _("Address") };

  dbox = create_data_bkpts_dbox(); 
  data_bkpt_dbox = dbox;

  clist = lookup_widget(dbox, "clist4");

  /* Set up the GtkTreeView */
  list = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING, G_TYPE_POINTER);
  model = GTK_TREE_MODEL(list);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), TRUE); 
  
  for (i = 0; i < 3; i++)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), i, text[i],
						gtk_cell_renderer_text_new(),
						"text", i, NULL);
  gtk_list_store_clear(list); 

  for(i=0; i<g_list_length(bkpt_access_list); i++)
    {
      s = (DATA_BKPT *)g_list_nth_data(bkpt_access_list, i);
      //DISPLAY("%i, s->id: %i\n", i, s->id);
      if(s->mode & BK_READ)
	{
	  sprintf(buffer, "Read of %06X-%06X\n", 
		  s->address, s->address);
	}
      if(s->mode & BK_WRITE)
	{
	  sprintf(buffer, "Write of %06X/%06X\n",
		  s->address, s->address);
	}
     
      if( (s->mode & BK_READ) && !(s->mode & BK_WRITE) )
	sprintf(buffer, "Read only");
      else if( (s->mode & BK_WRITE) && !(s->mode & BK_READ) )
	sprintf(buffer, "Write only");
      else
	sprintf(buffer, "Read & Write");
      text[0] = g_strdup(buffer);

      if(s->mode & BK_BYTE)
	sprintf(buffer, "Byte");
      else if(s->mode & BK_WORD)
	sprintf(buffer, "Word");
      else if(s->mode & BK_LONG)
	sprintf(buffer, "Long");
      else
	sprintf(buffer, "Byte");
      text[1] = g_strdup(buffer);
      text[2] = g_strdup_printf("%06X", s->address);

      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter,
			 0, text[0], 1, text[1],
			 2, text[2], 3, GINT_TO_POINTER(s->id), -1);
    }
  for(i=0; i<3; i++) 
    g_free(text[i]);
  
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

  gtk_widget_show_all(dbox);

  g_signal_connect(G_OBJECT(sel), "changed",
                   G_CALLBACK(on_dbkpt_clist_selection_changed), NULL);
  
  return 0;
}

void 
on_dbkpt_clist_selection_changed (GtkTreeSelection *sel, 
				  gpointer user_data)
{ 
  GList *paths;
  GtkTreeModel *model;

  paths = gtk_tree_selection_get_selected_rows(sel, &model);

  if (paths != NULL)
    sel_row = *gtk_tree_path_get_indices(g_list_nth_data(paths, 0));
  else
    sel_row = -1;

  g_list_foreach(paths, (GFunc)gtk_tree_path_free, NULL);
  g_list_free(paths);
}


/* Add button of the 'Data breakpoints dialog box' */
void
on_button_add_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_show(create_add_data_bkpt_dbox());
}


/* Delete button of the 'Data breakpoints dialog box' */
/* Remove the selected data breakpoint */
void
on_button_del_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gint id;
  gpointer idp;
  GtkWidget *clist;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path;
  int i;
  DATA_BKPT *s = NULL;

  if(sel_row != -1)
    {
      clist = lookup_widget(GTK_WIDGET(button), "clist1");
      
      model = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
      path = gtk_tree_path_new_from_indices(sel_row, -1);
      gtk_tree_model_get_iter(model, &iter, path);
      gtk_tree_path_free(path);

      /* Remove data breakpoint */
      // get id associated with the row
      gtk_tree_model_get(model, &iter,
			 3, &idp, -1);
      id = GPOINTER_TO_INT(idp);
      //DISPLAY("Selected row: %i\n", sel_row);
      //DISPLAY("id: %i\n", id);
      
      // retrieve structure associated with id
      for(i=0; i<g_list_length(bkpt_access_list); i++)
	{
	  s = (DATA_BKPT *)g_list_nth_data(bkpt_access_list, i);
	  if((s->id) == id)
	    {
	      //DISPLAY("id found\n");
	      break;
	    }
	}

      // delete the right id
      ti68k_delBreakpointAccess(s->id, s->mode);
      bkpt_access_list = g_list_remove(bkpt_access_list, s);
      gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
    }
}

/* Ok button of the 'Data breakpoints dialog box' */
void
on_button_ok39_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "dbkpt_dbox"));
}


void
on_data_bkpts_dbox_destroy             (GtkWidget       *widget,
                                        gpointer         user_data)
{

}
