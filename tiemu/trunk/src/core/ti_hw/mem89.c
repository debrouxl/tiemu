/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin, Kevin Kofler
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
    Memory management: TI89 FLASH without Hardware Protection
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
// 200000-2fffff : internal FLASH (TI89/V200)
// 300000-3fffff : 
// 400000-4fffff : internal FLASH (V200) or nothing (TI89)
// 500000-5fffff : 
// 600000-6fffff : memory mapped I/O (all HW)
// 700000-7fffff : memory mapped I/O (HW2, HW3)
// 800000-8fffff : unused
// 900000-9fffff :	 ... 
// a00000-afffff : 
// b00000-bfffff : 
// c00000-cfffff : 
// d00000-dfffff :
// e00000-efffff :   ...
// d00000-ffffff : unused

int ti89_mem_init(void)
{
    // map RAM
    mem_tab[0] = tihw.ram;
    mem_msk[0] = tihw.ram_size-1;
	mem_tab[1] = tihw.ram;
    mem_msk[1] = tihw.ram_size-1;

	// map FLASH
    mem_tab[2] = tihw.rom + 0x000000;
    mem_msk[2] = MIN(tihw.rom_size - 0*MB, 1*MB) - 1;

    mem_tab[3] = tihw.rom + 0x100000;
    mem_msk[3] = MIN(tihw.rom_size - 1*MB, 1*MB) - 1;

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

uint32_t ti89_get_long(uint32_t adr) 
{
	// RAM access
	if(IN_BOUNDS(0x000000, adr, 0x1fffff))
	{
		return getl(tihw.ram, adr, tihw.ram_size - 1);
	}

    // FLASH access
	else if(IN_BOUNDS(0x200000, adr, 0x5fffff))
	{
		return getl(tihw.rom, adr, tihw.rom_size - 1) | wsm.ret_or;
	}
	
	// memory-mapped I/O
    else if(IN_BOUNDS(0x600000, adr, 0x6fffff))
	{
       return io_get_long(adr);
	}

	// memory-mapped I/O (hw2)
	else if(IN_RANGE(adr, 0x700000, 32))
	{
		return io2_get_long(adr);
	}

    return 0x14141414;
}

uint16_t ti89_get_word(uint32_t adr) 
{
    // RAM access
	if(IN_BOUNDS(0x000000, adr, 0x1fffff))
	{
		return getw(tihw.ram, adr, tihw.ram_size - 1);
	}

    // FLASH access
	else if(IN_BOUNDS(0x200000, adr, 0x5fffff))
	{
		return getw(tihw.rom, adr, tihw.rom_size - 1) | wsm.ret_or;
	}
	
	// memory-mapped I/O
    else if(IN_BOUNDS(0x600000, adr, 0x6fffff))
	{
       return io_get_word(adr);
	}

	// memory-mapped I/O (hw2)
	else if(IN_RANGE(adr, 0x700000, 32))
	{
		return io2_get_word(adr);
	}

    return 0x1414;
}

uint8_t ti89_get_byte(uint32_t adr) 
{    
    // RAM access
	if(IN_BOUNDS(0x000000, adr, 0x1fffff))
	{
		return getb(tihw.ram, adr, tihw.ram_size - 1);
	}

    // FLASH access
	else if(IN_BOUNDS(0x200000, adr, 0x5fffff))
	{
		return getb(tihw.rom, adr, tihw.rom_size - 1) | wsm.ret_or;
	}
	
	// memory-mapped I/O
    else if(IN_BOUNDS(0x600000, adr, 0x6fffff))
	{
       return io_get_byte(adr);
	}

	// memory-mapped I/O (hw2)
	else if(IN_RANGE(adr, 0x700000, 32))
	{
		return io2_get_byte(adr);
	}

    return 0x14;
}

void ti89_put_long(uint32_t adr, uint32_t arg) 
{
    // RAM access
	if(IN_BOUNDS(0x000000, adr, 0x1fffff))
	{
		putl(tihw.ram, adr, tihw.ram_size - 1, arg);
	}

    // FLASH access
	else if(IN_BOUNDS(0x200000, adr, 0x5fffff))
	{
		FlashWriteLong(adr, arg);
	}

	// memory-mapped I/O
    else if(IN_BOUNDS(0x600000, adr, 0x6fffff))
	{
		io_put_long(adr, arg);
	}

	// memory-mapped I/O (hw2)
	else if(IN_RANGE(adr, 0x700000, 32))
	{
		io2_put_long(adr, arg);
	}

    return;
}

void ti89_put_word(uint32_t adr, uint16_t arg) 
{
    // RAM access
	if(IN_BOUNDS(0x000000, adr, 0x1fffff))
	{
		putw(tihw.ram, adr, tihw.ram_size - 1, arg);
	}

    // FLASH access
	else if(IN_BOUNDS(0x200000, adr, 0x5fffff))
	{
		FlashWriteWord(adr, arg);
	}

	// memory-mapped I/O
    else if(IN_BOUNDS(0x600000, adr, 0x6fffff))
	{
		io_put_word(adr, arg);
	}

	// memory-mapped I/O (hw2)
	else if(IN_RANGE(adr, 0x700000, 32))
	{
		io2_put_word(adr, arg);
	}

    return;
}

void ti89_put_byte(uint32_t adr, uint8_t arg) 
{
    // RAM access
	if(IN_BOUNDS(0x000000, adr, 0x1fffff))
	{
		putb(tihw.ram, adr, tihw.ram_size - 1, arg);
	}

    // FLASH access
	else if(IN_BOUNDS(0x200000, adr, 0x5fffff))
	{
		FlashWriteByte(adr, arg);
	}

	// memory-mapped I/O
    else if(IN_BOUNDS(0x600000, adr, 0x6fffff))
	{
		io_put_byte(adr, arg);
	}

	// memory-mapped I/O (hw2)
	else if(IN_RANGE(adr, 0x700000, 32))
	{
		io2_put_byte(adr, arg);
	}

    return;
}
