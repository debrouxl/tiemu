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


#ifndef __RUBBERBOX_H__
#define __RUBBERBOX_H__


gboolean
mouse_motion(GtkWidget *drawing_area, GdkEventMotion *event, gpointer action);

gboolean
button_press(GtkWidget *drawing_area, GdkEventButton *event, gpointer action);

void
draw_rubberbox(GtkWidget *drawing_area, GdkRect rect);

void
erase_rubberbox(GtkWidget *drawing_area);

extern int clist_row;
extern int clist_max;

#endif /* __RUBBERBOX_H__ */
