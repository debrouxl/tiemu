/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Li�vin
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

/*
    Callbacks registering
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "libuae.h"

#include "callbacks.h"
#include "update.h"


/* Function pointers */

callback_iv_t cb_init_specific  = NULL;
callback_iv_t cb_exit_specific  = NULL;
callback_iv_t cb_update_screen  = NULL;
callback_iv_t cb_update_keys    = NULL;
callback_vi_t cb_screen_on_off  = NULL;
callback_ii_t cb_set_contrast   = NULL;

/* 
    Register HID callbacks (hid.c) 
*/
void ti68k_gui_set_callbacks(
              callback_iv_t initSpecific,
			  callback_iv_t exitSpecific,
			  callback_iv_t updateScreen,
			  callback_iv_t updateKeys,
			  callback_vi_t screenOnOff,
			  callback_ii_t setContrast
			  )
{
  cb_init_specific  = initSpecific;
  cb_exit_specific  = exitSpecific;
  cb_update_screen  = updateScreen;
  cb_update_keys    = updateKeys;
  cb_screen_on_off  = screenOnOff;
  cb_set_contrast   = setContrast;
}

