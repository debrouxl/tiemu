/* Hey EMACS -*- linux-c -*- */
/* $Id: interface.h 224 2004-05-19 14:04:04Z roms $ */

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

#ifndef __TI68K_INTF__
#define __TI68K_INTF__

#include "ti68k_def.h"
#include "tilibs.h"
 
/*************/
/* Functions */
/*************/

//Note: [ti68k]_[group]_[short_or_long_name]

// Initialization
int ti68k_config_load_default(void);

int ti68k_init(void);
int ti68k_reset(void);
int ti68k_exit(void);
int ti68k_restart(void);

// Debugging
int ti68k_debug_get_pc(void);
int ti68k_debug_disassemble(uint32_t addr, char **line);
int ti68k_debug_break(void);
int ti68k_debug_step(void);
int ti68k_debug_skip(uint32_t next_pc);
int ti68k_debug_do_instructions(int n);

// Link
int ti68k_linkport_send_file(const char *filename);
int ti68k_linkport_reconfigure(void);

// Keyboard
void ti68k_kbd_set_key(int key, int active);

// Misc
int ti68k_get_rom_size(int calc_type);
int ti68k_get_ram_size(int calc_type);

uint8_t* ti68k_get_real_address(uint32_t addr);

// Others
#include "bkpts.h"
#include "callbacks.h"
#include "engine.h"
#include "images.h"
#include "registers.h"
#include "state.h"
#include "type2str.h"

#endif
