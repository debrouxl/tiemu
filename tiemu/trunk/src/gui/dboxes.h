/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __DBOXES_H__
#define __DBOXES_H__

#include <gtk/gtk.h>

#define BUTTON1  1
#define BUTTON2  2
#define BUTTON3  3

int msg_box1(const gchar * title, gchar * message);
int msg_box2(const char *title, char *message);
int msg_box3(const char *title, char *message,
	      const char *button1, const char *button2,
	      const char *button3);
int msg_box4(const char *title, char *message);
char *msg_entry(const char *title,
		const char *message, const char *content);

#define msg_box	msg_box1

#endif
