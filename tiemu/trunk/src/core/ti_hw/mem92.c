/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 245 2004-05-23 20:45:43Z roms $ */

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
    Memory management: TI92 ROM v1.x & v2.x
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libuae.h"
#include "ports.h"
#include "hw.h"
#include "mem.h"
#include "flash.h"
#include "images.h"
#include "bkpts.h"
#include "m68k.h"
#include "ti68k_def.h"
#include "ti68k_int.h"

// 000000-0fffff : RAM (128 or 256 KB)
// 100000-1fffff : ghost
// 200000-2fffff : internal ROM (TI92) or unused
// 300000-3fffff : idem
// 400000-4fffff : external ROM (TI92,+) or unused
// 500000-5fffff : idem
// 600000-6fffff : memory mapped I/O
// 700000-7fffff : unused
// 800000-8fffff : unused
// 900000-9fffff :   ...
// a00000-afffff : 
// b00000-bfffff : 
// c00000-cfffff : 
// d00000-dfffff :
// e00000-efffff :   ...
// d00000-ffffff : unused

int ti92_mem_init(void)
{
    int i;

    // map RAM
    mem_tab[0] = tihw.ram;
    mem_msk[0] = tihw.ram_size-1;

	// map EPROM
    if(tihw.rom_base == 0x20)
    {
        // internal
        mem_tab[2] = tihw.rom;
        mem_msk[2] = 1*MB - 1;

        if(tihw.rom_size > 1*MB)
        {
            mem_tab[3] = tihw.rom + 0x100000;
            mem_msk[3] = 1*MB - 1;
        }
    }
    else
    {
        // external
        mem_tab[4] = tihw.rom;
        mem_msk[4] = 1*MB - 1;

        if(tihw.rom_size > 1*MB)
        {
            mem_tab[5] = tihw.rom + 0x100000;
            mem_msk[5] = 1*MB - 1;
        }
    }

    // map IO
    mem_tab[6] = tihw.io;
    mem_msk[6] = tihw.io_size-1;

    return 0;
}

/* Put/Get byte/word/longword */
#define bput(adr, arg) { mem_tab[(adr)>>20][(adr) & mem_msk[(adr)>>20]] = (arg); }
#define wput(adr, arg) { bput((adr), (arg)>> 8); bput((adr)+1, (arg)&0x00ff); }
#define lput(adr, arg) { wput((adr), (uint16_t)((arg)>>16)); wput((adr)+2, (uint16_t)((arg)&0xffff)); }

#define bget(adr) (mem_tab[(adr)>>20][(adr)&mem_msk[(adr)>>20]])
#define wget(adr) ((uint16_t)(((uint16_t)bget(adr))<< 8 | bget((adr)+1)))
#define lget(adr) ((uint32_t)(((uint32_t)wget(adr))<<16 | wget((adr)+2)))

#define IN_RANGE(a,v,b)	(((v) >= (a)) && ((v) <= (b)))

uint32_t ti92_get_long(uint32_t adr) 
{
    // RAM or PROM access
	if(IN_RANGE(0x000000, adr, 0x5fffff))
	{
        return lget(adr);
	}

    // memory-mapped I/O
	else if(IN_RANGE(0x600000, adr, 0x6fffff))
	{
        return io_get_long(adr & 0x1f);
	}

    return 0x14141414;
}

uint16_t ti92_get_word(uint32_t adr) 
{
	// RAM or PROM access
    if(IN_RANGE(0x000000, adr, 0x5fffff))
	{
        return wget(adr);
	}

    // memory-mapped I/O
    else if(IN_RANGE(0x600000, adr, 0x6fffff))
	{
        return io_get_word(adr & 0x1f);
	}

    return 0x1414;
}

uint8_t ti92_get_byte(uint32_t adr) 
{
	// RAM or PROM access
    if(IN_RANGE(0x000000, adr, 0x5fffff))
	{
        return bget(adr);
	}

    // memory-mapped I/O
    else if(IN_RANGE(0x600000, adr, 0x6fffff))
	{
        return io_get_byte(adr & 0x1f);
	}

    return 0x14;
}

void ti92_put_long(uint32_t adr, uint32_t arg) 
{
    // write to RAM
	if(IN_RANGE(0x000000, adr, 0x1fffff))
	{
		lput(adr, arg);
	}

    // memory-mapped I/O
    else if(IN_RANGE(0x600000, adr, 0x6fffff))
	{
        io_put_long(adr & 0x1f, arg);
	}
}

void ti92_put_word(uint32_t adr, uint16_t arg) 
{
    // write to RAM
	if(IN_RANGE(0x000000, adr, 0x1fffff))
	{
		wput(adr, arg);
	}

    // memory-mapped I/O
    else if(IN_RANGE(0x600000, adr, 0x6fffff))
	{
        io_put_word(adr & 0x1f, arg);
	}
}

void ti92_put_byte(uint32_t adr, uint8_t arg) 
{
	// write to RAM
	if(IN_RANGE(0x000000, adr, 0x1fffff))
	{
		bput(adr, arg);
	}

    // memory-mapped I/O
    else if(IN_RANGE(0x600000, adr, 0x6fffff))
	{
        io_put_byte(adr & 0x1f, arg);
	}
}
