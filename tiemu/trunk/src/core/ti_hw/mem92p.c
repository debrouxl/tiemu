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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
    Memory management: TI92+ FLASH without Hardware Protection
	Except for ti92p_mem_init/ti89_mem_init, code is the same.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libuae.h"
#include "ports.h"
#include "hw.h"
#include "mem.h"
#include "mem89.h"
#include "images.h"
#include "bkpts.h"
#include "m68k.h"
#include "ti68k_def.h"
#include "ti68k_int.h"
#include "flash.h"

// 000000-0fffff : RAM (256 KB)
// 100000-1fffff : ghost of RAM
// 200000-2fffff : ghost of FLASH (HW2)
// 300000-3fffff : unused
// 400000-4fffff : external FLASH
// 500000-5fffff : idem
// 600000-6fffff : memory mapped I/O (all HW)
// 700000-7fffff : memory mapped I/O (HW2)
// 800000-8fffff : unused
// 900000-9fffff :	 ... 
// a00000-afffff : 
// b00000-bfffff : 
// c00000-cfffff : 
// d00000-dfffff :
// e00000-efffff :   ...
// d00000-ffffff : unused

int ti92p_mem_init(void)
{
    // map RAM
    mem_tab[0] = tihw.ram;
    mem_msk[0] = tihw.ram_size-1;
	mem_tab[1] = tihw.ram;
    mem_msk[1] = tihw.ram_size-1;

	// map FLASH
    mem_tab[4] = tihw.rom + 0x000000;;
    mem_msk[4] = MIN(tihw.rom_size - 0*MB, 1*MB) - 1;

    mem_tab[5] = tihw.rom + 0x100000;
    mem_msk[5] = MIN(tihw.rom_size - 1*MB, 1*MB) - 1;

    // ghosts
	if(tihw.hw_type == HW2)
	{
		mem_tab[2] = mem_tab[4];
		mem_msk[2] = mem_msk[4];
		mem_tab[3] = mem_tab[5];
		mem_msk[3] = mem_msk[5];
	}

    // map IO
    mem_tab[6] = tihw.io;
    mem_msk[6] = tihw.io_size-1;
	
	if(tihw.hw_type == HW2)
	{
		mem_tab[7] = tihw.io2;
		mem_msk[7] = tihw.io2_size-1;
	}

	// set mappers
	mem_get_byte_ptr = ti89_get_byte;
	mem_get_word_ptr = ti89_get_word;
	mem_get_long_ptr = ti89_get_long;
	mem_put_byte_ptr = ti89_put_byte;
	mem_put_word_ptr = ti89_put_word;
	mem_put_long_ptr = ti89_put_long;
  
    return 0;
}

// see mem89.c