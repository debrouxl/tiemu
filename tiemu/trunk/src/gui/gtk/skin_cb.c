#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef __WIN32__
#include <dirent.h>
#endif

#include <string.h>

#include <gtk/gtk.h>

#include "struct.h"
#include "skin_cb.h"
#include "skin_dbox.h"
#include "support.h"
#include "msg_cb.h"
#include "skn/skin.h"
#include "ticalc.h"
#include "interface.h"
#include "main.h"

static gchar *chosen_file = NULL;
static gint valid = 0;

gint display_skinlist_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *clist;
  GtkListStore *list;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  gchar *filename;

  DIR *dir;
  struct dirent *dirent;
  skinInfos si;
  int i;
  gchar *text[5] = { _("Filename"), _("Version"),
		     _("Skin name"), _("Author name"),
		     _("Calc type") };

  dbox = create_skin_dbox();
  
  clist = lookup_widget(dbox, "clist1");

  /* Set up the GtkTreeView */
  list = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING);
  model = GTK_TREE_MODEL(list);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), TRUE); 
  

  for (i = 0; i < 5; i++)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), i, text[i],
						gtk_cell_renderer_text_new(),
						"text", 0, NULL);
  gtk_list_store_clear(list); 

  if( (dir=opendir(inst_paths.skin_dir)) == NULL)
    {
      msg_box("Error", "Unable to open directory.");
      gtk_widget_destroy(dbox);
      return 0;
    }
  
  while( (dirent=readdir(dir)) != NULL)
    {
      if(!strcmp(dirent->d_name, ".")) { continue; }
      if(!strcmp(dirent->d_name, "..")) { continue; }

      //printf("skin = <%s>\n", dirent->d_name);
      filename = g_strconcat(inst_paths.skin_dir, dirent->d_name, NULL);

      if(!skin_read_header(filename, &si))
	{
	  text[0] = g_strdup(dirent->d_name);
	  switch(si.type)
	    {
	    case SKIN_TYPE_TIEMU: text[1] = g_strdup("TiEmu"); break;
	    case SKIN_TYPE_VTI: text[1] = g_strdup("VTi v2.5"); break;
	    case SKIN_TYPE_OLD_VTI: text[1] = g_strdup("VTi v2.1"); break;
	    case SKIN_TYPE_NEW: text[1] = g_strdup("new ?!"); break;
	    default: text[1] = g_strdup("??"); break;
	    }
	  text[2] = g_strdup(si.name);
	  text[3] = g_strdup(si.author);
	  text[4] = g_strdup(si.calc);
	  text[5] = g_strdup("");

	  gtk_list_store_append(list, &iter);
	  gtk_list_store_set(list, &iter, 0, text[0],
			     1, text[1], 2, text[2],
			     3, text[3], 4, text[4],
			     5, text[5], -1);

	  for(i=0; i<5; i++) g_free(text[i]);
	} 
    }
  
  if(closedir(dir)==-1)
    msg_box("Error", "Unable to close directory.");
  
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist)); 
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);       

  gtk_widget_show_all(dbox);

  g_signal_connect(G_OBJECT(sel), "changed", 
                   G_CALLBACK(on_skin_clist_selection_changed), NULL); 

  return 0;
}

void
on_skin_dbox_destroy                   (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}

void 
on_skin_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data) 
{ 
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *buf;

  if (gtk_tree_selection_get_selected (sel, &model, &iter))
    {
      valid = 0;
      if (chosen_file != NULL)
	g_free(chosen_file);

      gtk_tree_model_get (model, &iter, 0, &chosen_file, 4, &buf, -1);

      if ((!strcmp(buf, "TI-89") && (ti68k_getCalcType() & TI89))
	|| (!strcmp(buf, "TI-92") && (ti68k_getCalcType() & TI92)))
	valid = 1;

      g_free(buf);
    }
}


/* OK button */
void
on_skin_button6_clicked                     (GtkButton       *button,
					     gpointer         user_data)
{
  /* Check for a valid skin */
  if(!valid)
    {
      msg_box("Error", "The skin you have chosen is not compatible with the calculator model. Please choose another skin.");
      return;
    }

  /* Change skin */
  g_free(options.skin_file);
  options.skin_file = g_strconcat(inst_paths.skin_dir,
				  chosen_file, NULL);
  g_free(chosen_file);

  hid_change_skin(options.skin_file);

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "skin_dbox"));
}

/* Cancel button */
void
on_skin_button7_clicked                     (GtkButton       *button,
					     gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "skin_dbox"));
}




