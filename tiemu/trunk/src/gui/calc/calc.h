/* Hey EMACS -*- linux-c -*- */
/* $Id: hid.h 557 2004-07-14 08:17:39Z roms $ */

/*  TiEmu - a TI emulator
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Liévin
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

#ifndef __CALC_H__
#define __CALC_H__

int  hid_init(void);
int  hid_exit(void);

int hid_update_keys(void);
int hid_update_lcd(void);

int hid_switch_with_skin(void);
int hid_switch_without_skin(void);
int hid_change_skin(const char *filename);

int hid_switch_fullscreen(void);
int hid_switch_windowed(void);

int hid_switch_normal_view(void);
int hid_switch_large_view(void);

void hid_set_callbacks(void);

int hid_screenshot(char *filename);

int hid_popup_menu(void);

#endif