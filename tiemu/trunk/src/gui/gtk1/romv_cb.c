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
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "msg_cb.h"
#include "paths.h"
#include "tilibs.h"
#include "main.h"
#include "user_cb.h"

gchar *chosen_file = NULL;

gint display_romversion_dbox()
{
  GtkWidget *dbox, *dbox2;
  gpointer user_data;
  GtkWidget *list = NULL;
  gchar *text[6]; // 6 columns
  gchar buffer[MAXCHARS];
  int i;
  FILE *fp;
  gchar *filename;
  struct stat s;

  dbox = create_romversion_dbox();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "clist1");
  list = GTK_WIDGET(user_data);

  /* List all ROMs available in the ROM directory */
  gtk_widget_show_all(dbox2 = create_window1());
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  build_home_path(&filename, CACHE_FILE);
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
      return -1;
    }
  while(!feof(fp))
    {
      for(i=0; i<6; i++)
	{
	  fscanf(fp, "%s\t", buffer);
	  text[i] = g_strdup(buffer);
	}
      gtk_clist_append(GTK_CLIST(list), text);	  
      for(i=0; i<6; i++) g_free(text[i]);
    } 
  fclose(fp);
  
  gtk_widget_show_all(dbox);
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
  
  gtk_widget_destroy(GTK_WIDGET(user_data));
  while( gtk_events_pending() ) { 
    gtk_main_iteration(); 
  }
}


void
on_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_romversion_dbox_destroy             (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_romv_clist1_select_row              (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gchar *filename = NULL;
  gtk_clist_get_text(clist, row, 0, &filename);
  chosen_file = g_strdup(filename);
}


void
on_romv_clist1_unselect_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
}


gboolean
on_romv_clist1_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

  return FALSE;
}

/**/
