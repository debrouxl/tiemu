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


#ifndef __MAIN_CB_H__
#define __MAIN_CB_H__

#include <gtk/gtk.h>


void
on_file_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_new_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_open_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_as_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_vti_v21_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_vti_v25_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_exit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_edit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_properties_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_lcd_position_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_key_positions_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_help_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_main_window_delete_destroy_event    (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_tb_properties_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_tb_lcd_pos_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_tb_key_pos_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_filesel_delete_destroy_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_filesel_new_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_filesel_open_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_filesel_save_as_ok_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_filesel_vti_export_ok_clicked       (GtkButton       *button,
					gpointer         version);

void
on_filesel_cancel_clicked              (GtkButton       *button,
                                        gpointer         user_data);


#endif /* !__MAIN_CB_H__ */

gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

void
on_drawingarea1_size_allocate          (GtkWidget       *widget,
                                        GdkRectangle    *allocation,
                                        gpointer         user_data);

void
on_drawingarea1_size_request           (GtkWidget       *widget,
                                        GtkRequisition  *requisition,
                                        gpointer         user_data);
