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
#include "gtksdl.h"

#include "support.h"
#include "struct.h"
#include "defs.h"
#include "keymaps.h"

#include "main_intf.h"
#include "prop_intf.h"
#include "boxes_intf.h"

#include "main_cb.h"
#include "boxes_cb.h"

#include "rubberbox.h"
#include "skinops.h"
#include "vti_export.h"
#include "utils.h"

struct skinInfos skin_infos;

extern GtkWidget *lcd_custom_white; /* prop_cb.c */
extern GtkWidget *lcd_custom_black; /* prop_cb.c */

unsigned int lcd_mouse_motion;
unsigned int lcd_button_press;

unsigned int keys_mouse_motion;
unsigned int keys_button_press;


/*
 * MENUBAR
 */

void
on_new_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *filesel;

  /* must save & destroy sdl_area */

  if (skin_infos.changed)
    {
      gtk_widget_show(create_skin_changed(GUINT_TO_POINTER(ACTION_NEW)));

      return;
    }

  if (list_keys_dialog != NULL)
    {
      gtk_widget_destroy(list_keys_dialog);
      list_keys_dialog = NULL;
    }

  if (lcd_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  lcd_mouse_motion);

  if (lcd_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  lcd_button_press);

  if (keys_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  keys_mouse_motion);

  if (keys_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  keys_button_press);

  lcd_mouse_motion = 0;
  lcd_button_press = 0;

  keys_mouse_motion = 0;
  keys_button_press = 0;

  if (sdl_area != NULL)
    {
      gtk_widget_destroy(GTK_WIDGET(sdl_area));
      SDL_Quit();

      sdl_area = NULL;
      area = NULL;

      clear_skin_infos();
    }

  gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);

  filesel = create_filesel();

  gtk_window_set_title(GTK_WINDOW(filesel), _("Select JPEG image"));

  gtk_file_selection_complete(GTK_FILE_SELECTION(filesel), "*.jpg");

  gtk_signal_connect (GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button), "clicked",
                      GTK_SIGNAL_FUNC(on_filesel_new_ok_clicked),
                      NULL);

  gtk_widget_show(filesel);
}


void
on_open_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *filesel;

  /* must save & destroy sdl_area */

  if (skin_infos.changed)
    {
      gtk_widget_show(create_skin_changed(GUINT_TO_POINTER(ACTION_OPEN)));
      
      return;
    }

  if (list_keys_dialog != NULL)
    {
      gtk_widget_destroy(list_keys_dialog);
      list_keys_dialog = NULL;
    }

  if (lcd_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  lcd_mouse_motion);

  if (lcd_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  lcd_button_press);

  if (keys_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  keys_mouse_motion);

  if (keys_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  keys_button_press);

  lcd_mouse_motion = 0;
  lcd_button_press = 0;

  keys_mouse_motion = 0;
  keys_button_press = 0;

  if (sdl_area != NULL)
    {
      gtk_widget_destroy(GTK_WIDGET(sdl_area));
      SDL_Quit();

      sdl_area = NULL;
      area = NULL;

      clear_skin_infos();
    }

  gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);

  filesel = create_filesel();

  gtk_window_set_title(GTK_WINDOW(filesel), _("Select skin"));

    gtk_file_selection_complete(GTK_FILE_SELECTION(filesel), "*.skn");

  gtk_signal_connect (GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button), "clicked",
                      GTK_SIGNAL_FUNC(on_filesel_open_ok_clicked),
                      NULL);

  gtk_widget_show(filesel);
}


void
on_save_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if (area == NULL)
    return;

  if (skin_infos.skin_path != NULL)
    {
      if (write_skin() == 0)
	skin_infos.changed = 0;
    }
  else
    {
      on_save_as_activate(menuitem, user_data);
    }
}


void
on_save_as_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *filesel;

  if (area == NULL)
    return;

  filesel = create_filesel();
  
  gtk_window_set_title(GTK_WINDOW(filesel), _("Select destination file"));

  gtk_file_selection_complete(GTK_FILE_SELECTION(filesel), "*.skn");
  
  gtk_signal_connect (GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button), "clicked",
		      GTK_SIGNAL_FUNC(on_filesel_save_as_ok_clicked),
		      user_data);
  
  gtk_widget_show(filesel);
}


void
on_vti_v21_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *filesel;

  if (area == NULL)
    return;

  filesel = create_filesel();
  
  gtk_window_set_title(GTK_WINDOW(filesel), _("Select destination file -- VTi v2.1 export"));

    gtk_file_selection_complete(GTK_FILE_SELECTION(filesel), "*.skn");
  
  gtk_signal_connect (GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button), "clicked",
		      GTK_SIGNAL_FUNC(on_filesel_vti_export_ok_clicked),
		      GUINT_TO_POINTER(EXPORT_VTI_2_1));
  
  gtk_widget_show(filesel);
}


void
on_vti_v25_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *filesel;

  if (area == NULL)
    return;

  filesel = create_filesel();
  
  gtk_window_set_title(GTK_WINDOW(filesel), _("Select destination file -- VTi v2.5 export"));

  gtk_file_selection_complete(GTK_FILE_SELECTION(filesel), "*.skn");
  
  gtk_signal_connect (GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button), "clicked",
		      GTK_SIGNAL_FUNC(on_filesel_vti_export_ok_clicked),
		      GUINT_TO_POINTER(EXPORT_VTI_2_5));
  
  gtk_widget_show(filesel);
}


void
on_exit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if (skin_infos.changed)
    {
      gtk_widget_show(create_skin_changed(GUINT_TO_POINTER(ACTION_EXIT)));

      return;
    }

  clear_skin_infos();

  SDL_Quit();

  gtk_main_quit();
}


void
on_properties_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *properties;

  uint32_t custom_white = 0xffffff;
  uint32_t custom_black = 0;

  if (area == NULL)
    return;

  properties = create_prop_dialog();

  if (skin_infos.name != NULL)
    {
      gtk_entry_set_text(GTK_ENTRY(lookup_widget(properties, "prop_entry_title")), skin_infos.name);
    }

  if (skin_infos.author != NULL)
    {
      gtk_entry_set_text(GTK_ENTRY(lookup_widget(properties, "prop_entry_author")), skin_infos.author);
    }


  if (strcmp(skin_infos.calc, CALC_TI73) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti73")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI82) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti82")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI83) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti83")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI83P) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti83p")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI85) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti85")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI86) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti86")), TRUE);
    } 
  else if (strcmp(skin_infos.calc, CALC_TI89) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti89")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI92) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti92")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_TI92P) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti92p")), TRUE);
    }
  else if (strcmp(skin_infos.calc, CALC_V200) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "v200")), TRUE);
    }


  switch(skin_infos.colortype)
    {
      case LCD_COLORTYPE_HIGH:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "lcd_high_contrast")), TRUE);
	break;
      case LCD_COLORTYPE_LOW:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "lcd_low_contrast")), TRUE);
	break;
      case LCD_COLORTYPE_CUSTOM:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "lcd_custom")), TRUE);
	custom_white = skin_infos.lcd_white;
	custom_black = skin_infos.lcd_black;
	break;
      default:
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "lcd_custom")), TRUE);
	break;
    }

  /* SET THE LOW AND HIGH CONTRAST COLORS */

  gtk_widget_show(properties);

  set_lcd_color(lookup_widget(properties, "lcd_hi_white"), LCD_HI_WHITE);
  set_lcd_color(lookup_widget(properties, "lcd_hi_black"), LCD_HI_BLACK);

  set_lcd_color(lookup_widget(properties, "lcd_low_white"), LCD_LOW_WHITE);
  set_lcd_color(lookup_widget(properties, "lcd_low_black"), LCD_LOW_BLACK);

  lcd_custom_white = lookup_widget(properties, "lcd_custom_white");
  lcd_custom_black = lookup_widget(properties, "lcd_custom_black");

  set_lcd_color(lcd_custom_white, custom_white);
  set_lcd_color(lcd_custom_black, custom_black);
}


void
on_lcd_position_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  SDL_Rect lcd_cur;

  if (area == NULL)
    return;

  if (list_keys_dialog != NULL)
    {
      gtk_widget_destroy(list_keys_dialog);
      list_keys_dialog = NULL;
    }


  if (lcd_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  lcd_mouse_motion);

  if (lcd_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  lcd_button_press);

  if (keys_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  keys_mouse_motion);

  if (keys_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
			  keys_button_press);

  lcd_mouse_motion = 0;
  lcd_button_press = 0;

  keys_mouse_motion = 0;
  keys_button_press = 0;

  erase_rubberbox(sdl_area);

  if ((skin_infos.lcd_pos.top >= 0)
      && (skin_infos.lcd_pos.left >= 0)
      && (skin_infos.lcd_pos.bottom > 0)
      && (skin_infos.lcd_pos.right > 0))
    {
      lcd_cur.x = skin_infos.lcd_pos.left;
      lcd_cur.y = skin_infos.lcd_pos.top;
      
      lcd_cur.w = skin_infos.lcd_pos.right - skin_infos.lcd_pos.left;
      lcd_cur.h = skin_infos.lcd_pos.bottom - skin_infos.lcd_pos.top;

      draw_rubberbox(sdl_area, lcd_cur);

      sbar_print(_("Left click + drag to define LCD position, right click when done. Current size : %d x %d"),
		 lcd_cur.w, lcd_cur.h);
    }
  else
    {
      sbar_print(_("Left click + drag to define LCD position, right click when done"));
    }


  if (lcd_mouse_motion == 0)
    lcd_mouse_motion = gtk_signal_connect(GTK_OBJECT(sdl_eventbox),
					  "motion_notify_event",
					  (GtkSignalFunc)mouse_motion,
					  GUINT_TO_POINTER(ACTION_LCD_COORDS));
  
  if (lcd_button_press == 0)
    lcd_button_press = gtk_signal_connect(GTK_OBJECT(sdl_eventbox),
					  "button_press_event",
					  (GtkSignalFunc)button_press,
					  GUINT_TO_POINTER(ACTION_LCD_COORDS));
}


void
on_key_positions_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  int i;
  GtkWidget *view;
  GtkListStore *list;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;

  if (area == NULL)
    return;

  if (list_keys_dialog != NULL)
    {
      gtk_widget_show(list_keys_dialog);
      return;
    }

  create_list_keys_dialog();

  view = lookup_widget(GTK_WIDGET(list_keys_dialog), "list_keys");

  list = gtk_list_store_new(1, G_TYPE_STRING);

  model = GTK_TREE_MODEL(list);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);

  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 0, "",
					      gtk_cell_renderer_text_new(),
					      "text", 0, NULL);

  gtk_list_store_clear(list);

  for (i = 0; keynames[skin_infos.keymap][i] != NULL; i++)
    {
      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter, 0, keynames[skin_infos.keymap][i], -1);
    }

  clist_max = i;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

  gtk_widget_show(list_keys_dialog);

  g_signal_connect(G_OBJECT(sel), "changed",
		   G_CALLBACK(on_list_keys_selection_changed), NULL);

  if (keys_mouse_motion == 0)
    keys_mouse_motion = gtk_signal_connect(GTK_OBJECT(sdl_eventbox),
					   "motion_notify_event",
					   (GtkSignalFunc)mouse_motion,
					   NULL);
  
  if (keys_button_press == 0)
    keys_button_press = gtk_signal_connect(GTK_OBJECT(sdl_eventbox),
					   "button_press_event",
					   (GtkSignalFunc)button_press,
					   NULL);
}


void
on_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show(create_about_box());
}


/*
 * MAIN WINDOW
 */


gboolean
on_main_window_delete_destroy_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  if (skin_infos.changed)
    {
      gtk_widget_show(create_skin_changed(GUINT_TO_POINTER(ACTION_EXIT)));

      return TRUE;
    }

  clear_skin_infos();

  SDL_Quit();

  gtk_main_quit();

  return FALSE;
}


/*
 * TOOLBAR
 */


void
on_tb_properties_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  if (area == NULL)
    return;

  on_properties_activate(NULL, NULL);
}


void
on_tb_lcd_pos_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  if (area == NULL)
    return;

  on_lcd_position_activate(NULL, NULL);
}


void
on_tb_key_pos_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  if (area == NULL)
    return;

  on_key_positions_activate(NULL, NULL);
}


/*
 * FILESEL
 */

gboolean
on_filesel_delete_destroy_event                (GtkWidget       *widget,
						GdkEvent        *event,
						gpointer         user_data)
{
  gtk_widget_destroy(widget);

  return FALSE;
}


void
on_filesel_new_ok_clicked                  (GtkButton       *button,
					    gpointer         user_data)
{
  FILE *fp = NULL;
  const char *jpeg = NULL;

  jpeg = gtk_file_selection_get_filename(GTK_FILE_SELECTION(lookup_widget(GTK_WIDGET(button), "filesel")));

  if (jpeg != NULL)
    {
      fp = fopen(jpeg, "rb");
      
      if (fp != NULL)
	{
	  if (load_jpeg(fp) == 0)
	    {
	      skin_infos.changed = 1;
	      skin_infos.type = SKIN_TYPE_NEW;
	      skin_infos.jpeg_path = strdup(jpeg);
	    }

	  fclose(fp);
	}
    }

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "filesel"));

  on_properties_activate(NULL ,NULL);
}


void
on_filesel_open_ok_clicked                  (GtkButton       *button,
					     gpointer         user_data)
{
  const char *skin = NULL;

  skin = gtk_file_selection_get_filename(GTK_FILE_SELECTION(lookup_widget(GTK_WIDGET(button), "filesel")));

  if (skin != NULL)
    {
      if(load_skin(skin) == 0)
	skin_infos.changed = 1;
    }

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "filesel"));

  on_properties_activate(NULL, NULL);
}


void
on_filesel_save_as_ok_clicked                  (GtkButton       *button,
						gpointer         user_data)
{
  const char *path = NULL;

  path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(lookup_widget(GTK_WIDGET(button), "filesel")));

  if (path != NULL)
    {
      if (write_skin_as(path) == 0)
	skin_infos.changed = 0;
    }

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "filesel"));

  switch(GPOINTER_TO_UINT(user_data))
    {
      case ACTION_NEW:
	on_new_activate(NULL, NULL);
	break;
      case ACTION_OPEN:
	on_open_activate(NULL, NULL);
	break;
      case ACTION_EXIT:
	on_exit_activate(NULL, NULL);
	break;
      default:
	return;
    }
}


void
on_filesel_vti_export_ok_clicked                  (GtkButton       *button,
						   gpointer         version)
{
  const char *path = NULL;
  
  path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(lookup_widget(GTK_WIDGET(button), "filesel")));
  
  if (path != NULL)
    {
      if ((GPOINTER_TO_UINT(version) == EXPORT_VTI_2_1)
	  || (GPOINTER_TO_UINT(version) == EXPORT_VTI_2_5))
	{
	  export_skin_vti(path, GPOINTER_TO_UINT(version));
	}
    }
  
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "filesel"));
}


void
on_filesel_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "filesel"));
}

