/* Hey EMACS -*- linux-c -*- */
/* $Id: hid.c 163 2004-05-13 06:40:57Z roms $ */

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

#include <gtk/gtk.h>

#include "scopt_cb.h"
#include "scopt_dbox.h"
#include "support.h"

#include "struct.h"
#include "hid.h"
#include "screenshot.h"

static gint i_image_format;
static gint i_image_type;
static gint i_image_size;
static gint i_screen_counter;
static gchar *i_screen_file;

static GtkWidget *label = NULL;
void refresh_label(void);

gint display_scopt_dbox(void)
{
  GtkWidget *dbox;

  dbox = create_scopt_dbox();

  i_image_format = options.img_format;
  switch (options.img_format)
    {
      case IMG_PCX:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton18")),
				     TRUE);
	break;
      case IMG_XPM:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton19")),
				     TRUE);
	break;
      case IMG_JPG:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton20")),
				     TRUE);
	break;
      case IMG_BMP:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton21")),
				     TRUE);
	break;
      default:
	break;
    }

  i_image_type = options.img_type;
  switch (options.img_type)
    {
      case IMG_BW:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton22")),
				     TRUE);
	break;
      case IMG_COL:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton23")),
				     TRUE);
	break;
      default:
	break;
    }

  i_image_size = options.img_size;
  switch (options.img_size)
    {
      case IMG_LCD:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton24")),
				     TRUE);
	break;
      case IMG_SKIN:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "radiobutton25")),
				     TRUE);
	break;
      default:
	break;
    }
  
  label = lookup_widget(dbox, "label3");
  refresh_label();

  i_screen_file = g_strdup(options.img_file);
  i_screen_counter = options.img_counter;
  gtk_entry_set_text(GTK_ENTRY(lookup_widget(dbox, "entry1")),
		     options.img_file);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_scopt_radiobutton18_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_PCX;
  refresh_label();
}


void
on_scopt_radiobutton19_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_XPM;
  refresh_label();
}


void
on_scopt_radiobutton20_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_JPG;
  refresh_label();
}


void
on_scopt_radiobutton21_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_BMP;
  refresh_label();
}


void
on_scopt_radiobutton22_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_type = IMG_BW;
}


void
on_scopt_radiobutton23_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_type = IMG_COL;
}


void
on_scopt_radiobutton24_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_size = IMG_LCD;
}


void
on_scopt_radiobutton25_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_size = IMG_SKIN;
}


void
on_scopt_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  options.img_format = i_image_format;
  options.img_type = i_image_type;
  options.img_size = i_image_size;
  options.img_counter = i_screen_counter;
  g_free(options.img_file);
  options.img_file = g_strdup(i_screen_file);
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "scopt_dbox"));
}


void
on_scopt_cancel_button_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "scopt_dbox"));
}

void
on_scopt_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

void refresh_label(void)
{
  gchar buffer[MAXCHARS];
  gchar *ext = "???";

  switch(i_image_format)
    {
    case IMG_XPM: ext = "xpm"; break;
    case IMG_PCX: ext = "pcx"; break;
    case IMG_JPG: ext = "jpg"; break;
    case IMG_BMP: ext = "bmp"; break;
    default: break;
    }
  
  sprintf(buffer, "%s%03i.%s", i_screen_file, i_screen_counter, ext);
  gtk_label_set_text(GTK_LABEL(label), buffer);
}

void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  g_free(options.img_file);
  s = gtk_editable_get_chars(editable, 0, -1);
  i_screen_file = g_strdup(s);
  refresh_label();
}

/* */
