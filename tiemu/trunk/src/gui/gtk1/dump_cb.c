#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "dump_cb.h"
#include "dump_dbox.h"
#include "support.h"

#include "interface.h"
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "pbar_cb.h"
#include "main.h"

gint display_ramdump_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_ramdump_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "ramdump_fileselection");
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(user_data), 
				  "ram_file.hex");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
			      "*.hex");
  gtk_widget_show_all(dbox);

  return 0;
}

gint display_romdump_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_romdump_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "romdump_fileselection");
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(user_data), 
				  "rom_file.hex");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
			      "*.hex");
  gtk_widget_show_all(dbox);

  return 0;
}

gint display_wait_conversion_dbox()
{
  gtk_widget_show_all(create_wait_conversion_dbox());
  return 0;
}

/* Callbacks */

void
on_ramdump_fileselection_destroy       (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_ramd_ok_button2_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;

  gtk_widget_show((w = create_wait_conversion_dbox()));
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  ti68k_dumpRam(gtk_file_selection_get_filename (GTK_FILE_SELECTION (user_data)));
  gtk_widget_destroy(w);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_ramd_cancel_button2_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_romd_fileselection_destroy          (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_romd_ok_button2_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;

  gtk_widget_show((w = create_wait_conversion_dbox()));
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  ti68k_dumpRom(gtk_file_selection_get_filename (GTK_FILE_SELECTION (user_data)));
  gtk_widget_destroy(w);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_romd_cancel_button2_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}

/* */
