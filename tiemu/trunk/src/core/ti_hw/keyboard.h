/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001, Romain Lievin
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

#ifndef __TI68K_KEYBOARD__
#define __TI68K_KEYBOARD__

#include "keydefs.h"

/* Functions */

int hw_kbd_init(void);
int hw_kbd_reset(void);
int hw_kbd_exit(void);

UBYTE read_keyboard_mask(void);
//UBYTE get_rowmask(UBYTE r);

// Turn into callback...
#define set_active_key ti68k_setActiveKey
#define is_key_pressed ti68k_isKeyPressed

void  ti68k_setActiveKey(int key, int active);
int   ti68k_isKeyPressed(int key);
#define read_onkey() ((UBYTE)on_key)

/* Variables */

extern int key_states[NB_MAX_KEYS];
extern int on_key;

#endif
