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

/*
    Memory management: TI89 Titanium without any HW protection
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libuae.h"
#include "ports.h"
#include "hw.h"
#include "mem.h"
#include "mem89tm.h"
#include "images.h"
#include "bkpts.h"
#include "m68k.h"
#include "ti68k_def.h"
#include "ti68k_int.h"
#include "flash.h"

// 000000-03ffff : RAM (256 KB), not mirrored
// 100000-1fffff : unused
// 200000-2fffff : image of 0x000000 (ghost, not mirrored)
// 300000-3fffff : unused
// 400000-4fffff : image of 0x000000 (ghost, not mirrored)
// 500000-5fffff : unused
// 600000-6fffff : memory mapped I/O (all HW)
// 700000-7fffff : memory mapped I/O (HW2, HW3), non ghost'ed
// 800000-8fffff : ROM (TI89 Titanium)
// 900000-9fffff : idem
// a00000-afffff : idem
// b00000-bfffff : idem
// c00000-cfffff : unused
// d00000-dfffff :	 ...
// e00000-efffff :   ...
// d00000-ffffff : unused

int ti89t_mem_init(void)
{
	int i;

    // map RAM
    mem_tab[0] = tihw.ram;
	mem_msk[0] = tihw.ram_size-1;
	
    // ghost of RAM
    for(i = 0; i < 4; i++)
    {
        mem_tab[2+i] = mem_tab[i]; 
        mem_msk[2+i] = mem_msk[i];
    }
   
	// map FLASH
    for(i = 0; i < 4; i++)
    {
        mem_tab[8+i] = tihw.rom + i*0x100000;
        mem_msk[8+i] = MIN(tihw.rom_size - i*MB, 1*MB) - 1;
    }

    // map IO
    mem_tab[6] = tihw.io;
    mem_msk[6] = tihw.io_size-1;
	
	if(tihw.hw_type >= HW2)
	{
		mem_tab[7] = tihw.io2;
		mem_msk[7] = tihw.io2_size;
	}

	// set mappers
	mem_get_byte_ptr = ti89t_get_byte;
	mem_get_word_ptr = ti89t_get_word;
	mem_get_long_ptr = ti89t_get_long;
	mem_put_byte_ptr = ti89t_put_byte;
	mem_put_word_ptr = ti89t_put_word;
	mem_put_long_ptr = ti89t_put_long;

    return 0;
}

uint32_t ti89t_get_long(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x5fffff))				// RAM access
	{
        return lget(adr);
	}
    else if(IN_RANGE(0x800000, adr, 0xbfffff))			// FLASH access
	{
		return FlashReadLong(adr);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
       return io_get_long(adr);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_long(adr);
	}

    return 0x14141414;
}

uint16_t ti89t_get_word(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x5fffff))				// RAM access
	{
        return wget(adr);
	}
    else if(IN_RANGE(0x800000, adr, 0xbfffff))			// FLASH access
	{
		return FlashReadWord(adr);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
       return io_get_word(adr);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_word(adr);
	}

    return 0x1414;
}

uint8_t ti89t_get_byte(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x5fffff))				// RAM access
	{
        return bget(adr);
	}
    else if(IN_RANGE(0x800000, adr, 0xbfffff))			// FLASH access
	{
		return FlashReadByte(adr);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
       return io_get_byte(adr);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_byte(adr);
	}

    return 0x14;
}

void ti89t_put_long(uint32_t adr, uint32_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x5fffff))				// RAM access
	{
        lput(adr, arg);
	}
    else if(IN_RANGE(0x800000, adr, 0xbfffff))			// FLASH access
	{
		FlashWriteLong(adr, arg);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_long(adr, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_long(adr, arg);
	}

    return;
}

void ti89t_put_word(uint32_t adr, uint16_t arg) 
{
	if(IN_RANGE(0x000000, adr, 0x5fffff))				// RAM access
	{
        wput(adr, arg);
	}
    else if(IN_RANGE(0x800000, adr, 0xbfffff))			// FLASH access
	{
		FlashWriteWord(adr, arg);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_word(adr, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_word(adr, arg);
	}

    return;
}

void ti89t_put_byte(uint32_t adr, uint8_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x5fffff))				// RAM access
	{
        bput(adr, arg);
	}
    else if(IN_RANGE(0x800000, adr, 0xbfffff))			// FLASH access
	{
        FlashWriteByte(adr,arg);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_byte(adr, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_byte(adr, arg);
	}

    return;
}
