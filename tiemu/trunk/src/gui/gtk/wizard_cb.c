#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "wizard_cb.h"
#include "wizard_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "msg_cb.h"
#include "interface.h"

static gint action = 1;
gchar *wizard_rom = NULL;
gint wizard_ok = FALSE;

/**********************/
/**********************/

gint display_flash_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_flash_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "flash_fileselection");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
			      "*.tib;*.89u;*.9xu");
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_romfile_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_romfile_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "rom_fileselection");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
			      "*.rom");
  gtk_widget_show_all(dbox);
  return 0;
}


gint display_step1_dbox()
{
  gtk_widget_show_all(create_step1_dbox());
  return 0;
}

gint display_msg1_dbox()
{
  gtk_widget_show_all(create_msg1_dbox());
  return 0;
}

gint display_step3_dbox()
{
  gtk_widget_show_all(create_step3_dbox());
  return 0;
}

gint display_wait_dbox()
{
  gtk_widget_show_all(create_wait_dbox());
  return 0;
}


/****************************/
/* FLASH file selection box */
/****************************/

void
on_flashfile_fileselection_destroy     (GtkObject       *object,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_flashfile_ok_button2_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  const gchar *filename;
  gchar buffer[1024];
  gchar *ext = NULL;

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  ext = strrchr(filename, '.');
  if(str_cmp(ext, ".tib") && str_cmp(ext, ".89u") && str_cmp(ext, ".9xu"))
    {
      msg_box(_("Error"), _("Invalid file"));
      gtk_widget_destroy(GTK_WIDGET(user_data));
      gtk_widget_show(create_step1_dbox());     
      return;
    }

  gtk_widget_show((w = create_wait_dbox()));
  while( gtk_events_pending() ) { gtk_main_iteration(); }
  ti68k_convertTibToRom(filename, buffer);
  wizard_rom = g_strdup(buffer);
  gtk_widget_destroy(w);
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  gtk_widget_destroy(GTK_WIDGET(user_data));
  gtk_widget_show(create_step3_dbox());
}


void
on_flashfile_cancel_button2_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


/*************/
/* Step1 box */
/*************/

void
step1_on_radiobutton1_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  action = 1;
}


void
step2_on_radiobutton2_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  action = 2;
}


void
step3_on_radiobutton3_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  action = 3;
}


void
step1_b1_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
step1_b2_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  switch(action)
    {
    case 1: gtk_widget_show(create_msg1_dbox());
      break;
    case 2: gtk_widget_show(create_flash_fileselection());
      break;
    case 3: gtk_widget_show(create_romfile_fileselection());
      break;
    default:
      break;
    }
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
step1_b3_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  exit(0);
}


/****************/
/* Message1 box */
/****************/

void
msg1_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  exit(0);
}


/*************/
/* Step3 box */
/*************/

void
step3_b1_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  gtk_widget_show(create_step1_dbox());
}


void
step3_b2_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  wizard_ok = TRUE;
}


void
step3_b3_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  exit(0);
}


/****************************************/
/* Display a box for make the user wait */
/****************************************/


void
wait_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}

/************************/
/* Choose a ROM to load */
/************************/

void
on_romfile_fileselection_destroy       (GtkObject       *object,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_romfile_cancel_button2_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  exit(0);
}


void
on_romfile_ok_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  const gchar *filename;
  gchar *ext = NULL;

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  ext = strrchr(filename, '.');
  if(str_cmp(ext, ".rom"))
    {
      msg_box(_("Error"), _("Invalid file"));
      gtk_widget_destroy(GTK_WIDGET(user_data));
      gtk_widget_show(create_step1_dbox());     
      return;
    }
  wizard_rom = g_strdup(filename);

  gtk_widget_destroy(GTK_WIDGET(user_data));
  gtk_widget_show(create_step3_dbox());
}
