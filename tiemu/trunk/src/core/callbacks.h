/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
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

#ifndef __TI68K_CALLBACKS__
#define __TI68K_CALLBACKS__

#include <time.h>

/*
  Variables
*/

// Callbacks type
typedef int  (*callback_iv_t) (void);
typedef int  (*callback_ii_t) (int);
typedef void (*callback_vi_t) (int);
typedef void (*callback_vv_t) (void);

// Function pointers
extern callback_ii_t cb_launch_debugger;
extern callback_vi_t cb_update_progbar;

extern callback_iv_t cb_init_specific;
extern callback_iv_t cb_exit_specific;
extern callback_ii_t cb_set_contrast;
extern callback_iv_t cb_update_screen;
extern callback_iv_t cb_update_keys;
extern callback_vi_t cb_screen_on_off;

/* 
   Functions 
*/

void ti68k_gui_set_callbacks(
	callback_iv_t initSpecific,
	callback_iv_t exitSpecific,
	callback_iv_t updateScreen,
	callback_iv_t updateKeys,
	callback_vi_t screenOnOff,
	callback_ii_t setContrast
	);

callback_ii_t ti68k_debugger_define(callback_ii_t new_debugger);


#endif
