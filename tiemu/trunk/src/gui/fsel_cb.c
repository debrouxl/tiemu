/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Liévin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "fsel_cb.h"
#include "fsel_dbox.h"
#include "support.h"

#include "struct.h"
#include "hid.h"
#include "pbars.h"
#include "dboxes.h"
#include "tilibs.h"
#include "interface.h"

gint display_rom_fileselection(void)
{
  GtkWidget *dbox;
   
  dbox = create_rom_fileselection();
  gtk_file_selection_complete(GTK_FILE_SELECTION(dbox), "*.rom;*.tib;*.89u;*.9Xu");
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_tifile_fileselection(void)
{
  GtkWidget *dbox;
  gchar *s;
   
  dbox = create_tifile_fileselection();
  switch(ti68k_getCalcType())
    {
    case TI92:
      s = "*.92?";
      break;
    case TI89:
      s = "*.89?";
      break;
    case TI92 | MODULEPLUS:
      s = "*.9x?";
      break;
    default:
      s = "*.???";
      break;
    }
  gtk_file_selection_complete(GTK_FILE_SELECTION(dbox), s);
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_tib_fileselection(void)
{
  GtkWidget *dbox;
   
  dbox = create_tib_fileselection();
  gtk_file_selection_complete(GTK_FILE_SELECTION(dbox),
			      "*.89u;*.9xu;*.tib");
  gtk_widget_show_all(dbox);
  return 0;
}

gint display_ams_fileselection(void)
{
  GtkWidget *dbox;
   
  dbox = create_ams_fileselection();
  gtk_file_selection_complete(GTK_FILE_SELECTION(dbox),
			      "*.89u;*.9xu;*.tib");
  gtk_widget_show_all(dbox);
  return 0;
}

void
on_rom_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}


void
on_rom_ok_button2_clicked                  (GtkButton       *button,
					    gpointer         user_data)
{
  GtkWidget *w;
  const gchar *src;
  gchar *dst;
  gchar *cmd;

  w = lookup_widget(GTK_WIDGET(button), "rom_fileselection");

  src = gtk_file_selection_get_filename 
    (GTK_FILE_SELECTION(w));

  dst = g_strconcat(inst_paths.rom_dir, g_basename(src), NULL);

  cmd = g_strdup_printf("cp %s %s", src, dst);

  system(cmd); //copy_file(src, dst);

  gtk_widget_destroy(w);
  ti68k_unhalt();
}


void
on_rom_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "rom_fileselection"));
}


void
on_tifile_fileselection_destroy        (GtkObject       *object,
                                        gpointer         user_data)
{
  destroy_pbar();
  ti68k_unhalt();
}


void
on_tifile_ok_button2_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  const gchar *filename;
  gchar *ext;

  w = lookup_widget(GTK_WIDGET(button), "tifile_fileselection");

  /* Retrieve filename */
  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(w));
  gtk_widget_hide(w);
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

  gtk_widget_destroy(w);
}


void
on_tifile_cancel_button2_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "tifile_fileselection"));
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
  ti68k_unhalt();
}


void
on_tib_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  const gchar *filename;

  w = lookup_widget(GTK_WIDGET(button), "tib_fileselection");

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(w));

  g_free((options.params)->tib_file);
  (options.params)->tib_file = g_strdup(filename);
  if(ti68k_loadImage((options.params)->tib_file)) 
    {
      msg_box("Error", "Can not open the ROM/FLASH file.");
      return;
    }
  
  gtk_widget_destroy(w);
}


void
on_tib_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "tib_fileselection"));
}


void
on_ams_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}


void
on_ams_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  /*
  GtkWidget *w;
  gchar *filename;
  gchar buffer[1024];

  w = lookup_widget(GTK_WIDGET(button), "ams_fileselection");

  filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(w));
  //msg_box(_("Information"), _("Conversion in progress. Please wait..."));
  //while( gtk_events_pending() ) { gtk_main_iteration(); }
  ti68k_convertTibToRom(filename, buffer);

  gtk_widget_destroy(w);
  */
}


void
on_ams_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "ams_fileselection"));
}
