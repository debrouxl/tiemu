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
    Memory management: TI89/V200 FLASH without Hardware Protection
	Except for ti92p_mem_init/ti89_mem_init, code is the same.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libuae.h"
#include "ports.h"
#include "hw.h"
#include "mem.h"
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

    // map 4MB FLASH
    if(tihw.calc_type == V200)
	{
		mem_tab[4] = tihw.rom + 0x200000;
		mem_msk[4] = MIN(tihw.rom_size - 2*MB, 1*MB) - 1;
		mem_tab[5] = tihw.rom + 0x300000;
		mem_msk[5] = MIN(tihw.rom_size - 3*MB, 1*MB) - 1;
    }

    // map IO
    mem_tab[6] = tihw.io;
    mem_msk[6] = tihw.io_size-1;
	
	if(tihw.hw_type == HW2)
	{
		mem_tab[7] = tihw.io2;
		mem_msk[7] = tihw.io2_size-1;
	}

    return 0;
}

uint32_t ti89_get_long(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        return lget(adr);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        return (lget(adr) | wsm.ret_or);
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

uint16_t ti89_get_word(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        return wget(adr);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        return (wget(adr) | wsm.ret_or);
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

uint8_t ti89_get_byte(uint32_t adr) 
{    
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        return bget(adr);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        return (bget(adr) | wsm.ret_or);
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

void ti89_put_long(uint32_t adr, uint32_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        lput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
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

void ti89_put_word(uint32_t adr, uint16_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        wput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
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

void ti89_put_byte(uint32_t adr, uint8_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        bput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
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
