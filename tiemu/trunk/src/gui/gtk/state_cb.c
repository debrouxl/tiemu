#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include <gtk/gtk.h>

#include "state_cb.h"
#include "state_dbox.h"
#include "support.h"

#include "interface.h"
#include "platform.h"
#include "struct.h"
#include "rcfile.h"
#include "ticalc.h"
#include "msg_cb.h"
#include "paths.h"
#include "main.h"

/**************/
/* Load state */
/**************/

gint display_fs_load_state()
{
  GtkWidget *dbox;
  gpointer user_data;
   
  dbox = create_fs_load_state();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "fs_load_state");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
			      "*.sav");
  
  gtk_widget_show_all(dbox);
  return 0;
}


void
load_state_on_ok_button1_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  const gchar *s;

  s = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  g_free((options.params)->ram_file);
  (options.params)->ram_file =g_strdup(s);
  ti68k_loadState((options.params)->ram_file);

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
load_state_on_cancel_button1_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


/**************/
/* Save state */
/**************/

gint display_fs_save_state()
{
  GtkWidget *dbox;
  gpointer user_data;
  gchar *s;
   
  dbox = create_fs_save_state();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "fs_save_state");
  s = (options.params)->ram_file;
#ifdef __LINUX__
  if(!strcmp(s, ""))
    s = g_strconcat("~/", CONF_DIR, "/", "default.sav", NULL);
#else
  if(!strcmp(s, ""))
    s = g_strconcat("default.sav", NULL);
#endif
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), s);
  
  gtk_widget_show_all(dbox);
  if(!strcmp((options.params)->ram_file, ""))
    g_free(s);
  return 0;
}


void
save_state_on_ok_button2_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  const gchar *s;

  s = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  g_free((options.params)->ram_file);
  (options.params)->ram_file = g_strdup(s);
  ti68k_saveState((options.params)->ram_file);
  if(!is_rcfile_exist())
    {
      write_rc_file();

#if defined(__LINUX__)
      msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in ~/.gtktiemu)."));
#elif defined(__WIN32__)
      msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in gtktiemu.ini)."));
#endif
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
save_state_on_cancel_button2_pressed   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}



void
on_fs_load_state_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_fs_save_state_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}

