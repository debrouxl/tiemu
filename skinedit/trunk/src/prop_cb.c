/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <gtk/gtk.h>

#include "support.h"
#include "struct.h"
#include "defs.h"

#include "main_intf.h"
#include "prop_intf.h"
#include "boxes_intf.h"

#include "prop_cb.h"

#include "rubberbox.h"
#include "skinops.h"
#include "vti_export.h"
#include "utils.h"

extern struct skinInfos skin_infos;

GtkWidget *lcd_custom_white;
GtkWidget *lcd_custom_black;


/*
 * COLOR SELECTION
 */


gboolean
on_lcd_color_select_delete_destroy_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_destroy(widget);

  return FALSE;
}


void
on_lcd_color_ok_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *colorsel;
  GtkColorSelectionDialog *csd;
  GtkColorSelection *cs;
  
  gdouble gcolor[4];
  unsigned char r, g, b;
  
  colorsel = lookup_widget(GTK_WIDGET(button), "lcd_color_select");
  
  csd = GTK_COLOR_SELECTION_DIALOG(colorsel);
  cs = GTK_COLOR_SELECTION(csd->colorsel);
  gtk_color_selection_get_color(cs, gcolor);
  
  r = (unsigned char)(gcolor[0] * 0xff);
  g = (unsigned char)(gcolor[1] * 0xff);
  b = (unsigned char)(gcolor[2] * 0xff);
  
  switch(GPOINTER_TO_UINT(user_data))
    {
    case ACTION_LCD_WHITE:
      skin_infos.lcd_white = (r << 16) | (g << 8) | b; 
      set_lcd_color(lcd_custom_white, skin_infos.lcd_white); 
      break;
    case ACTION_LCD_BLACK:
      skin_infos.lcd_black = (r << 16) | (g << 8) | b;
      set_lcd_color(lcd_custom_black, skin_infos.lcd_black); 
      break;
    default:
      return;
    }
  
  gtk_widget_destroy(colorsel);
}


void
on_lcd_color_cancel_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "lcd_color_select"));
}

/*
 * PROPERTIES
 */


gboolean
on_prop_dialog_delete_destroy_event            (GtkWidget       *widget,
						GdkEvent        *event,
						gpointer         user_data)
{
  gtk_widget_destroy(widget);

  lcd_custom_white = NULL;
  lcd_custom_black = NULL;

  return FALSE;
}


gboolean
on_lcd_custom_white_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  GtkWidget *colorsel;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(widget, "lcd_custom")), TRUE);

  colorsel = create_lcd_color_select(GUINT_TO_POINTER(ACTION_LCD_WHITE));

  gtk_window_set_title(GTK_WINDOW(colorsel), _("Select custom WHITE color"));

  //  gtk_color_selection_set_opacity(GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(colorsel)->colorsel), FALSE);

  gtk_widget_show(colorsel);

  return FALSE;
}


gboolean
on_lcd_custom_black_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  GtkWidget *colorsel;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(widget, "lcd_custom")), TRUE);

  colorsel = create_lcd_color_select(GUINT_TO_POINTER(ACTION_LCD_BLACK));

  gtk_window_set_title(GTK_WINDOW(colorsel), _("Select custom BLACK color"));
  
  //gtk_color_selection_set_opacity(GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(colorsel)->colorsel), FALSE);

  gtk_widget_show(colorsel);

  return FALSE;
}


void
on_prop_dlg_ok_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  const char *text = NULL;

  if (skin_infos.name != NULL)
    free(skin_infos.name);

  text = gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "prop_entry_title")));

  if (text != NULL)
    skin_infos.name = strdup(text);

  if (skin_infos.author != NULL)
    free(skin_infos.author);

  text = NULL;
  text = gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "prop_entry_author")));

  if (text != NULL)
    skin_infos.author = strdup(text);


  memset(skin_infos.calc, 0, 8);

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti73"))))
    {
      strcpy(skin_infos.calc, CALC_TI73);
      skin_infos.keymap = 0;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti82"))))
    {
      strcpy(skin_infos.calc, CALC_TI82);
      skin_infos.keymap = 1;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti83"))))
    {
      strcpy(skin_infos.calc, CALC_TI83);
      skin_infos.keymap = 1;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti83p"))))
    {
      strcpy(skin_infos.calc, CALC_TI83P);
      skin_infos.keymap = 1;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti85"))))
    {
      strcpy(skin_infos.calc, CALC_TI85);
      skin_infos.keymap = 2;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti86"))))
    {
      strcpy(skin_infos.calc, CALC_TI86);
      skin_infos.keymap = 2;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti89"))))
    {
      strcpy(skin_infos.calc, CALC_TI89);
      skin_infos.keymap = 3;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti92"))))
    {
      strcpy(skin_infos.calc, CALC_TI92);
      skin_infos.keymap = 4;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "ti92p"))))
    {
      strcpy(skin_infos.calc, CALC_TI92P);
      skin_infos.keymap = 4;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "v200"))))
    {
      strcpy(skin_infos.calc, CALC_V200);
      skin_infos.keymap = 4;
    }
  
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "lcd_high_contrast"))))
    {
      skin_infos.colortype = LCD_COLORTYPE_HIGH;

      skin_infos.lcd_white = LCD_HI_WHITE;
      skin_infos.lcd_black = LCD_HI_BLACK;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "lcd_low_contrast"))))
    {
      skin_infos.colortype = LCD_COLORTYPE_LOW;

      skin_infos.lcd_white = LCD_LOW_WHITE;
      skin_infos.lcd_black = LCD_LOW_BLACK;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(GTK_WIDGET(button), "lcd_custom"))))
    {
      skin_infos.colortype = LCD_COLORTYPE_CUSTOM;

      /* the values are already in skin_infos.lcd_{white,black} */
    }

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "prop_dialog"));

  lcd_custom_white = NULL;
  lcd_custom_black = NULL;
}


void
on_prop_dlg_cancel_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "prop_dialog"));

  lcd_custom_white = NULL;
  lcd_custom_black = NULL;
}
