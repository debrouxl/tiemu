/*  GtkTiEmu - a TI emulator
 *  Originally written by Jonas Minsberg
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __SPECIFIC_H__
#define __SPECIFIC_H__


/**********************/
/* Exported functions */
/**********************/

int hid_init(void);
int hid_exit(void);

void hid_switch_with_skin(void);
void hid_switch_without_skin(void);
void hid_change_skin(const char *filename);

void hid_switch_fullscreen(void);
void hid_switch_windowed(void);

void hid_switch_normal_view(void);
void hid_switch_large_view(void);

int hid_set_gui_callbacks(void);

int do_screenshot(int format, int type, int size, char *filename);

#endif





