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


#ifndef __BOXES_CB_H__
#define __BOXES_CB_H__

#include <gtk/gtk.h>


void
on_list_keys_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_list_keys_dialog_delete_destroy_event (GtkWidget       *widget,
					  GdkEvent        *event,
					  gpointer         user_data);


gboolean
on_skin_changed_delete_destroy_event   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_skin_changed_save_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_skin_changed_ignore_clicked        (GtkButton       *button,
				       gpointer         user_data);

void
on_skin_changed_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data);


gboolean
on_about_box_delete_destroy_event      (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_about_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data);


#endif /* !__BOXES_CB_H__ */
