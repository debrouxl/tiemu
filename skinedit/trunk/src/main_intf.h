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


#ifndef __MAIN_INTF_H__
#define __MAIN_INTF_H__

#include <gtk/gtk.h>

#include "gtksdl.h"

GtkWidget*
create_main_window (void);


GtkWidget*
create_filesel (void);


GtkWidget *sdl_eventbox;
GtkWidget *statusbar;

GtkWidget *area;
GtkSDL *sdl_area;


#endif /* !__MAIN_INTF_H__ */
