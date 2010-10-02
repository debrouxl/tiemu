/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

#if GTK_CHECK_VERSION(2,18,0)
#undef GTK_WIDGET_STATE
#define GTK_WIDGET_STATE(wid) (gtk_widget_get_state(wid))
#endif

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
  GSList *formats;
  gchar *ext_list = NULL;
  unsigned int i;
  
  /* must save & destroy pixbuf */
  
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
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  lcd_mouse_motion);

  if (lcd_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  lcd_button_press);

  if (keys_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  keys_mouse_motion);

  if (keys_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  keys_button_press);

  lcd_mouse_motion = 0;
  lcd_button_press = 0;

  keys_mouse_motion = 0;
  keys_button_press = 0;

  if (pixbuf != NULL)
    {
      g_object_unref(pixbuf);
      pixbuf = NULL;
      clear_skin_infos();
    }

  gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);

  filesel = create_filesel();

  gtk_window_set_title(GTK_WINDOW(filesel), _("Select JPEG image"));

  ext_list = g_strdup("");
  formats = gdk_pixbuf_get_formats ();
  
  for(i=0; i < g_slist_length(formats); i++)
    {
      GSList *elt;
      GdkPixbufFormat *fmt;
      gchar **exts;
      
      elt = g_slist_nth(formats, i);
      fmt = (GdkPixbufFormat *)elt->data;
      exts = gdk_pixbuf_format_get_extensions(fmt);
      
      ext_list = g_strconcat(ext_list, "*.", exts[0], "; ", NULL);
      g_strfreev(exts);
    }
  g_slist_free (formats);
  
  gtk_file_selection_complete(GTK_FILE_SELECTION(filesel), ext_list);
  g_free(ext_list);
  
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

  /* must save & destroy pixbuf */

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
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  lcd_mouse_motion);

  if (lcd_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  lcd_button_press);

  if (keys_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  keys_mouse_motion);

  if (keys_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  keys_button_press);

  lcd_mouse_motion = 0;
  lcd_button_press = 0;

  keys_mouse_motion = 0;
  keys_button_press = 0;

  if (pixbuf != NULL)
    {
      g_object_unref(pixbuf);
      pixbuf = NULL;
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
  if(pixbuf == NULL)
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

  if(pixbuf == NULL)
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

  if(pixbuf == NULL)
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

  if(pixbuf == NULL)
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

  gtk_main_quit();
}


void
on_properties_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *properties;

  uint32_t custom_white = 0xffffff;
  uint32_t custom_black = 0;

  if(pixbuf == NULL)
    return;
  
  properties = create_prop_dialog();
  
  if (skin_infos.name != NULL)
    {
      gtk_entry_set_text(GTK_ENTRY(lookup_widget(properties, "prop_entry_title")), (char *)skin_infos.name);
    }

  if (skin_infos.author != NULL)
    {
      gtk_entry_set_text(GTK_ENTRY(lookup_widget(properties, "prop_entry_author")), (char *)skin_infos.author);
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
  else if (strcmp(skin_infos.calc, CALC_TI84P) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti84p")), TRUE);
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
  else if (strcmp(skin_infos.calc, CALC_TI89T) == 0)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(properties, "ti89t")), TRUE);
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
  GdkRect lcd_cur;

  if(pixbuf == NULL)
    return;

  if (list_keys_dialog != NULL)
    {
      gtk_widget_destroy(list_keys_dialog);
      list_keys_dialog = NULL;
    }


  if (lcd_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  lcd_mouse_motion);

  if (lcd_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  lcd_button_press);

  if (keys_mouse_motion != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  keys_mouse_motion);

  if (keys_button_press != 0)
    gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			  keys_button_press);

  lcd_mouse_motion = 0;
  lcd_button_press = 0;

  keys_mouse_motion = 0;
  keys_button_press = 0;

  erase_rubberbox(drawingarea1);

  if ((skin_infos.lcd_pos.bottom > 0) && (skin_infos.lcd_pos.right > 0))
    {
      lcd_cur.x = skin_infos.lcd_pos.left;
      lcd_cur.y = skin_infos.lcd_pos.top;
      
      lcd_cur.w = skin_infos.lcd_pos.right - skin_infos.lcd_pos.left;
      lcd_cur.h = skin_infos.lcd_pos.bottom - skin_infos.lcd_pos.top;

      draw_rubberbox(drawingarea1, lcd_cur);

      sbar_print(_("Left click + drag to define LCD position, right click when done. Current size : %d x %d"),
		 lcd_cur.w, lcd_cur.h);
    }
  else
    {
      sbar_print(_("Left click + drag to define LCD position, right click when done"));
    }


  if (lcd_mouse_motion == 0)
    lcd_mouse_motion = gtk_signal_connect(GTK_OBJECT(drawingarea1),
					  "motion_notify_event",
					  (GtkSignalFunc)mouse_motion,
					  GUINT_TO_POINTER(ACTION_LCD_COORDS));
  
  if (lcd_button_press == 0)
    lcd_button_press = gtk_signal_connect(GTK_OBJECT(drawingarea1),
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

  if(pixbuf == NULL)
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
    keys_mouse_motion = gtk_signal_connect(GTK_OBJECT(drawingarea1),
					   "motion_notify_event",
					   (GtkSignalFunc)mouse_motion,
					   NULL);
  
  if (keys_button_press == 0)
    keys_button_press = gtk_signal_connect(GTK_OBJECT(drawingarea1),
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
  if(pixbuf == NULL)
    return;

  on_properties_activate(NULL, NULL);
}


void
on_tb_lcd_pos_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  if(pixbuf == NULL)
    return;

  on_lcd_position_activate(NULL, NULL);
}


void
on_tb_key_pos_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  if(pixbuf == NULL)
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


static char* convert_image_to_jpeg(char *filename)
{
  GdkPixbuf *img;
  GError *error = NULL;
  gboolean status;
  gchar *s, *d;
  
  /* Load image */
  img = gdk_pixbuf_new_from_file(filename, &error);
  if (img == NULL) 
    {
      fprintf(stderr, "Failed to load pixbuf file: %s: %s\n", filename, error->message);
      g_error_free(error);
      return NULL;
    }
  
  /* Check for file with extension */
  d = strrchr(filename, '.');
  if(d == NULL)
    return NULL;
  
  /* JPEG file ? Don't convert. */
  s = g_strdup(filename);
  if(!strcmp(d, ".jpg") || !strcmp(d, ".jpeg"))
    {	
      g_object_unref(img);
      return s;
    }	
  
  /* Replace filename extension by jpg */
  s = g_strdup(filename);
  d = strrchr(s, '.');
  strcpy(++d, "jpg");
  
  /* Save image as JPEG */
  status = gdk_pixbuf_save (img, s, "jpeg", &error, "quality", "100", NULL);
  if(status == FALSE) 
    {
      fprintf(stderr, "Failed to save pixbuf file: %s: %s\n", filename, error->message);
      g_error_free(error);
      g_object_unref(img);
      return NULL;
    }
  
  return s;
}

void
on_filesel_new_ok_clicked                  (GtkButton       *button,
					    gpointer         user_data)
{
  FILE *fp = NULL;
  const char *fn = NULL;
  gchar *s;

  fn = gtk_file_selection_get_filename(GTK_FILE_SELECTION(lookup_widget(GTK_WIDGET(button), "filesel")));
  
  if (fn != NULL)
    {
      s = convert_image_to_jpeg((char *)fn);
      if(s != NULL)
	{
	  fp = fopen(fn, "rb");
	  if (fp != NULL)
	    {
	      if (load_image(fp) == 0)
		{
		  skin_infos.changed = 1;
		  skin_infos.type = SKIN_TYPE_NEW;
		  skin_infos.jpeg_path = strdup(s);	//strdup(jpeg);
		  g_free(s);
		}
	      
	      fclose(fp);
	    }
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

/* Backing pixbuf for drawing area */
GdkPixbuf *pixbuf = NULL;

/* Create a new backing pixbuf of the appropriate size */
gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  return FALSE;
}

/* Redraw the screen from the backing pixbuf */
gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  GdkRect rect;

  if(pixbuf == NULL)
    return FALSE;

  memcpy(&rect, &(event->area), sizeof(GdkRectangle));
  /*
  if (rect.x < 0) 
    rect.x = 0;
  if (rect.y < 0) 
    rect.y = 0;	
  */
  if (rect.x > (gint)skin_infos.width - 1) 
    rect.x = (gint)skin_infos.width - 1;
  if (rect.y > (gint)skin_infos.height - 1) 
    rect.y = (gint)skin_infos.height - 1;

  if (rect.x + rect.w > (gint)skin_infos.width - 1) 
    rect.w = (gint)skin_infos.width - rect.x - 1;
  if (rect.y + rect.h > (gint)skin_infos.height - 1) 
    rect.h = (gint)skin_infos.height - rect.y - 1;
  
  gdk_draw_pixbuf(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  pixbuf, 
		  rect.x, rect.y,
		  rect.x, rect.y,
		  rect.w, rect.h,
		  GDK_RGB_DITHER_NONE, 0, 0);
  
  return TRUE;
}
