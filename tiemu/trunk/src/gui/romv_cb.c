#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>

#include "romv_cb.h"
#include "romv_dbox.h"
#include "support.h"

#include "interface.h"
//#include "platform.h"
#include "struct.h"
#include "hid.h"
#include "msg_cb.h"
#include "paths.h"
#include "tilibs.h"
#include "main.h"
#include "user_cb.h"

gchar *chosen_file = NULL;

gint display_romversion_dbox(void)
{
  GtkWidget *dbox, *dbox2;
  GtkWidget *clist;
  GtkListStore *list;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;

  gchar buffer[MAXCHARS];
  int i;
  FILE *fp;
  gchar *filename;
  struct stat s;

  gchar *text[6] = { _("Filename"), _("Calc"),
		     _("Version"), _("Memory"),
		     _("Size"), _("Type") };

  dbox = create_romversion_dbox();
  
  clist = lookup_widget(dbox, "clist1");

  /* Set up the GtkTreeView */
  list = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING, G_TYPE_STRING,
			    G_TYPE_STRING, G_TYPE_STRING);
  model = GTK_TREE_MODEL(list);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), TRUE); 
  
  for (i = 0; i < 6; i++)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), i, text[i],
						gtk_cell_renderer_text_new(),
						"text", i, NULL);
  gtk_list_store_clear(list); 

  /* List all ROMs available in the ROM directory */
  dbox2 = create_window1();
  gtk_widget_show_all(dbox2);
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  filename = g_strconcat(inst_paths.home_dir, CACHE_FILE, NULL);
  ti68k_scanFiles(inst_paths.rom_dir, filename);
  gtk_widget_destroy(dbox2);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  stat(filename, &s);
  if(s.st_size == 0)
    {
      gtk_widget_show_all(dbox);
      return 0;
    }

  fp = fopen(filename, "rt");
  if(fp == NULL)
    {
      DISPLAY("Unable to open this file: %s\n", filename);
      gtk_widget_destroy(dbox);
      return -1;
    }
  while(!feof(fp))
    {
      for(i=0; i<6; i++)
	{
	  fscanf(fp, "%s\t", buffer);
	  text[i] = g_strdup(buffer);
	}

      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter, 0, text[0],
			 1, text[1], 2, text[2],
			 3, text[3], 4, text[4],
			 5, text[5], -1);

      for(i=0; i<6; i++) g_free(text[i]);
    } 
  fclose(fp);

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

  gtk_widget_show_all(dbox);

  g_signal_connect(G_OBJECT(sel), "changed",
                   G_CALLBACK(on_romv_clist_selection_changed), NULL);

  return 0;
}

#ifdef __WIN32__
# define strcasecmp _stricmp
#endif

void
on_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  char *ext = NULL;
  int ret;

  /* Determine file type and operation */
  ext = strrchr(chosen_file, '.');
  
  if(!strcasecmp(ext, ".rom")) 
    { // ROM image
      g_free((options.params)->rom_file);
      (options.params)->rom_file = g_strconcat(inst_paths.rom_dir,
					       chosen_file, NULL);
      g_free(chosen_file);
      if(ti68k_loadImage((options.params)->rom_file)) 
	{
	  msg_box("Error", "Can not open the ROM image.");
	  return;
	}
      
      /* and restart emulation engine */
      ti68k_restart();
    }
  else 
    { // FLASH upgrade
      ret = user2_box("Question", "Do you want to load it as a fake ROM image or as an FLASH upgrade ? \n\nClick FLASH if you want to directly upgrade the calculator operating system (AMS). \n\nOn the other hand, if you load it as a fake ROM image, TiEmu will convert the FLASH upgrade into a ROM image but your image will suffer from some limitations (no boot block, no certificate, problems with fonts).", "ROM", "FLASH");

      if(ret == 1) 
	{ // ROM
	  g_free((options.params)->rom_file);
	  (options.params)->rom_file = g_strconcat(inst_paths.rom_dir,
						   chosen_file, NULL);
	  g_free(chosen_file);
	  if(ti68k_loadImage((options.params)->rom_file)) 
	    {
	      msg_box("Error", "Can not open the ROM image.");
	      return;
	    }
	  ti68k_restart();
	}
      else
	{ // FLASH
	  g_free((options.params)->tib_file);
	  (options.params)->tib_file = g_strconcat(inst_paths.rom_dir,
						   chosen_file, NULL);
	  g_free(chosen_file);
	  if(ti68k_loadUpgrade((options.params)->tib_file)) 
	    {
	      msg_box("Error", "Can not open the FLASH upgrade.");
	      return;
	    }
	  ti68k_reset();
	}
    }
  
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "romversion_dbox"));
  while( gtk_events_pending() ) { 
    gtk_main_iteration(); 
  }
}


void
on_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "romversion_dbox"));
}


void
on_romversion_dbox_destroy             (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void 
on_romv_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data)
{ 
  GtkTreeModel *model;
  GtkTreeIter iter;

  if (gtk_tree_selection_get_selected (sel, &model, &iter))
    {
      if (chosen_file != NULL)
	g_free(chosen_file);

      gtk_tree_model_get (model, &iter, 0, &chosen_file, -1);
    }
}


/**/
