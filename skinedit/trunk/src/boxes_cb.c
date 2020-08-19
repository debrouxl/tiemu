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

#include "support.h"
#include "struct.h"
#include "defs.h"

#include "main_intf.h"
#include "prop_intf.h"
#include "boxes_intf.h"

#include "boxes_cb.h"
#include "main_cb.h"

#include "rubberbox.h"
#include "skinops.h"
#include "vti_export.h"
#include "utils.h"


extern unsigned int keys_mouse_motion; /* main_cb.c */
extern unsigned int keys_button_press; /* main_cb.c */

int clist_row;

/*
 * KEYS LIST
 */

void
on_list_keys_selection_changed (GtkTreeSelection *sel,
				gpointer user_data)
{
  int row;
  GdkRect key_cur;
  GList *paths;
  GtkTreeModel *model;

  erase_rubberbox(drawingarea1);

  paths = gtk_tree_selection_get_selected_rows(sel, &model);

  row = *gtk_tree_path_get_indices(g_list_nth_data(paths, 0));

  g_list_foreach(paths, (GFunc)gtk_tree_path_free, NULL);
  g_list_free(paths);

  if (row > SKIN_KEYS - 1) /* should not happen */
    return;

  clist_row = row;

  if ((skin_infos.keys_pos[row].bottom > 0)
      && (skin_infos.keys_pos[row].right > 0))
    {
      key_cur.x = skin_infos.keys_pos[row].left;
      key_cur.y = skin_infos.keys_pos[row].top;
      
      key_cur.w = skin_infos.keys_pos[row].right - skin_infos.keys_pos[row].left;
      key_cur.h = skin_infos.keys_pos[row].bottom - skin_infos.keys_pos[row].top;

      draw_rubberbox(drawingarea1, key_cur);

      sbar_print(_("Current size : %d x %d at (%d;%d) -- Left click + drag to define key position, right click to save and go on. Close the list window when done (save the key before !)"),
		 key_cur.w, key_cur.h, key_cur.x, key_cur.y);
    }
  else
    {
      sbar_print(_("Left click + drag to define key position, right click to save and go on. Close the list window when done (save the key before !)"));
    }
}


gboolean
on_list_keys_dialog_delete_destroy_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			keys_mouse_motion);

  gtk_signal_disconnect(GTK_OBJECT(drawingarea1),
			keys_button_press);

  keys_mouse_motion = 0;
  keys_button_press = 0;

  erase_rubberbox(drawingarea1);

  gtk_widget_destroy(widget);

  list_keys_dialog = NULL;

  sbar_print(_("Ready"));
  
  return FALSE;
}



/*
 * SKIN CHANGED
 */

gboolean
on_skin_changed_delete_destroy_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_destroy(widget);

  return FALSE;
}


void
on_skin_changed_save_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "skin_changed"));


  if (skin_infos.skin_path != NULL)
    {
      if (write_skin() == 0)
	skin_infos.changed = 0;

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
  else
    {
      on_save_as_activate(NULL, user_data);
    }
}


void
on_skin_changed_ignore_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  skin_infos.changed = 0;

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "skin_changed"));

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
on_skin_changed_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "skin_changed"));
}


/*
 * ABOUT BOX
 */


gboolean
on_about_box_delete_destroy_event              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_destroy(widget);

  return FALSE;
}


void
on_about_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "about_box"));
}

