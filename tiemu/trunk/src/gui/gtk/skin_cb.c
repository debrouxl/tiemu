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

gint display_skinlist_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;
  GtkWidget *list = NULL;
  gchar *filename;

  DIR *dir;
  struct dirent *dirent;
  gchar *text[6]; // 6 columns
  skinInfos si;
  int i;

  dbox = create_skin_dbox();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "clist1");
  list = GTK_WIDGET(user_data);

  if( (dir=opendir(inst_paths.skin_dir)) == NULL)
    {
      msg_box("Error", "Unable to open directory.");
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

	  gtk_clist_append(GTK_CLIST(list), text);	  
	  for(i=0; i<6; i++) g_free(text[i]);
	} 
    }
  
  if(closedir(dir)==-1)
    {
      msg_box("Error", "Unable to close directory.");
      return 0;
    }
  
  gtk_widget_show_all(dbox);
  return 0;
}

void
on_skin_dbox_destroy                   (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_skin_clist1_select_row              (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gchar *buffer = NULL;
  gtk_clist_get_text(clist, row, 0, &buffer);
  chosen_file = g_strdup(buffer);
  //printf("chosen_file = <%s>\n", chosen_file);

  valid = 0;
  gtk_clist_get_text(clist, row, 4, &buffer);
  if(!strcmp(buffer, "TI-89") && (ti68k_getCalcType() & TI89))
    valid = 1;
  if(!strcmp(buffer, "TI-92") && (ti68k_getCalcType() & TI92))
    valid = 1;
}


void
on_skin_clist1_unselect_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  return;
}


gboolean
on_skin_clist1_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  return FALSE;
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

  gtk_widget_destroy(GTK_WIDGET(user_data));
}

/* Cancel button */
void
on_skin_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}




