#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "fsel_cb.h"
#include "fsel_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "pbar_cb.h"
#include "msg_cb.h"
#include "tilibs.h"
#include "interface.h"
#include "main.h"

gint display_rom_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;
   
  dbox = create_rom_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "rom_fileselection");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), "*.rom;*.tib;*.89u;*.9Xu");
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_tifile_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;
   
  dbox = create_tifile_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "tifile_fileselection");
  switch(ti68k_getCalcType())
    {
    case TI92:
      gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
				  "*.92?");
      break;
    case TI89:
      gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
				  "*.89?");
      break;
    case TI92 | MODULEPLUS:
      gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
				  "*.9x?");
      break;
    default:
      gtk_file_selection_complete(GTK_FILE_SELECTION(user_data), 
				  "*.???");
      break;
    }
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_tib_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;
   
  dbox = create_tib_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "tib_fileselection");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data),
			      "*.89u;*.9xu;*.tib");
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_ams_fileselection()
{
  GtkWidget *dbox;
  gpointer user_data;
   
  dbox = create_ams_fileselection();
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "ams_fileselection");
  gtk_file_selection_complete(GTK_FILE_SELECTION(user_data),
			      "*.89u;*.9xu;*.tib");
  gtk_widget_show_all(dbox);
  return 0;
}

void
on_rom_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_rom_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  const gchar *src;
  gchar *dst;
  gchar *cmd;

  src = gtk_file_selection_get_filename 
    (GTK_FILE_SELECTION(user_data));

  dst = g_strconcat(inst_paths.rom_dir, g_basename(src), NULL);

  cmd = g_strdup_printf("cp %s %s", src, dst);

  system(cmd); //copy_file(src, dst);

  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}


void
on_rom_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_tifile_fileselection_destroy        (GtkObject       *object,
                                        gpointer         user_data)
{
  destroy_pbar();
  unhalt();
}


void
on_tifile_ok_button2_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  const gchar *filename;
  gchar *ext;

  /* Retrieve filename */
  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  gtk_widget_hide(GTK_WIDGET(user_data));
  while( gtk_events_pending() ) { gtk_main_iteration(); }

  /* Get extension */
  ext = strrchr(filename, '.');
  if(ext == NULL)
    return;
  else
    ext++;
  
  /* TI file ? */
  if(0)
    /*
      if(!(
      (!strstr(ext, "89") && (ti68k_getCalcType() == TI89)) ||
      (!strstr(ext, "92") && (ti68k_getCalcType() == TI92)) ||
      (!strstr(ext, "9x") && (ti68k_getCalcType() == TI92p)) ||
      (!strstr(ext, "9X") && (ti68k_getCalcType() == TI92p))
      ))*/
    {
      printf("exit: <%s> %i\n", ext, ti68k_getCalcType());
      return;
    }
  
  /* FLASH APP file ? */
  else if( (ext[2] == 'k') || (ext[2] =='K'))
    {
      create_pbar_type5(_("Flash"), "");
    }

  /* FLASH OS file ? */
  else if( (ext[2] == 'u') || (ext[2] == 'U'))
    {
      create_pbar_type5(_("Flash"), "");
    }
  
  /* Backup file ? */
  else if( (ext[2] == 'b') || (ext[2] == 'B'))
    {
      create_pbar_type3(_("Backup"));
    }

  /* Group file ? */
  else if( (ext[2] == 'g') || (ext[2] == 'G'))
    {
      create_pbar_type5(_("Sending group file"), "");
    }

  /* Single file */
  else
    {
      create_pbar_type4(_("Sending variable"), "");
    }

  while( gtk_events_pending() ) { 
	  gtk_main_iteration(); 
	}

  ti68k_sendFile(filename);

  while( gtk_events_pending() ) { 
    gtk_main_iteration(); 
  }

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_tifile_cancel_button2_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_update_file1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_tib_fileselection();
}


void
on_tib_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_tib_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));

  g_free((options.params)->tib_file);
  (options.params)->tib_file = g_strdup(filename);
  if(ti68k_loadImage((options.params)->tib_file)) 
    {
      msg_box("Error", "Can not open the ROM/FLASH file.");
      return;
    }
  
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_tib_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_ams_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_ams_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  /*
  gchar *filename;
  gchar buffer[1024];

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  //msg_box(_("Information"), _("Conversion in progress. Please wait..."));
  //while( gtk_events_pending() ) { gtk_main_iteration(); }
  ti68k_convertTibToRom(filename, buffer);

  gtk_widget_destroy(GTK_WIDGET(user_data));
  */
}


void
on_ams_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}
