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


#ifndef __PROP_CB_H__
#define __PROP_CB_H__

#include <gtk/gtk.h>


gboolean
on_lcd_color_select_delete_destroy_event (GtkWidget       *widget,
					  GdkEvent        *event,
					  gpointer         user_data);


void
on_lcd_color_ok_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_lcd_color_cancel_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_lcd_color_help_clicked              (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_prop_dialog_delete_destroy_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_lcd_custom_white_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_lcd_custom_black_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_prop_dlg_ok_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_prop_dlg_cancel_clicked             (GtkButton       *button,
                                        gpointer         user_data);


#endif /* !__PROP_CB_H__ */
