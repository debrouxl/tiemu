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
    Memory management: TI89/V200 FLASH with Hardware Protection
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
// 100000-1fffff : ghost
// 200000-2fffff : internal ROM (TI89)
// 300000-3fffff : idem
// 400000-4fffff : ?
// 500000-5fffff : ?
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
    int i = 0;

    // map RAM
    mem_tab[0] = tihw.ram;
    mem_msk[0] = tihw.ram_size-1;

	// map FLASH
    mem_tab[2] = tihw.rom;
    mem_msk[2] = MIN(tihw.rom_size - 0*MB, 1*MB) - 1;

    mem_tab[5] = mem_tab[3] = tihw.rom + 0x100000;
    mem_msk[5] = mem_msk[3] = MIN(tihw.rom_size - 1*MB, 1*MB) - 1;

    // ghosts
    /*
    mem_tab[4] = mem_tab[2];
    mem_msk[4] = mem_msk[2];
    mem_tab[5] = mem_tab[3];
    mem_msk[5] = mem_msk[3];
    */

    // map IO
    mem_tab[6] = tihw.io;
    mem_msk[6] = tihw.io_size-1;
	
	if(tihw.hw_type == HW2)
	{
		mem_tab[7] = tihw.io2;
		mem_msk[7] = tihw.io_size-1;
	}

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

static int access = 0;
static int crash = 0;

//#define HWP			// HW1 protection if define set

uint32_t ti89_get_long(uint32_t adr) 
{
#ifdef HWP
	// stealth I/O
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		access = crash = 0;
		//cb_screen_on_off(0);
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable/disable
	{
		if(crash >= 4)
			hw_m68k_reset();
		if(access >= 3)
			tihw.protect = !0;
		access = 0;
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
	else if(IN_RANGE(0x210000, adr, 0x21ffff))			// certificate
	{
		access = crash = 0;
		if(tihw.protect) return 0x14141414;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		access = crash = 0;
		return 0x14141414;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
#endif	

    // memory
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
       return io_get_long(adr & 0x1f);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_long(adr & 0x1f);
	}

    return 0x14141414;
}

uint16_t ti89_get_word(uint32_t adr) 
{
#ifdef HWP
    // stealth I/O
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		access = crash = 0;
		//cb_screen_on_off(0);
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable/disable
	{
		if(crash >= 4)
			hw_m68k_reset();
		if(access >= 3)
			tihw.protect = !0;
		access = 0;
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
	else if(IN_RANGE(0x210000, adr, 0x21ffff))			// certificate
	{
		access = crash = 0;
		if(tihw.protect) return 0x1414;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		access = crash = 0;
		return 0x1414;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
#endif

    // memory
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
       return io_get_word(adr & 0x1f);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_word(adr & 0x1f);
	}

    return 0x1414;
}

uint8_t ti89_get_byte(uint32_t adr) 
{
    // stealth I/O
#ifdef HWP
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		access = crash = 0;
		//cb_screen_on_off(0);
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable/disable
	{
		if(crash >= 4)
			hw_m68k_reset();
		if(access >= 3)
			tihw.protect = !0;
		access = 0;
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
	else if(IN_RANGE(0x210000, adr, 0x21ffff))			// certificate
	{
		access = crash = 0;
		//if(tihw.protect) return 0x14;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		access = crash = 0;
		return 0x14;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		access++;
		crash = 0;
	}
#endif

    // memory
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
       return io_get_byte(adr & 0x1f);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_byte(adr & 0x1f);
	}

    return 0x14;
}

void ti89_put_long(uint32_t adr, uint32_t arg) 
{
	// stealth I/O
#ifdef HWP
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		access = crash = 0;
		//cb_screen_on_off(!0);
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable/disable
	{
		if(crash >= 4)
			hw_m68k_reset();
		if(access >= 3)
			tihw.protect = 0;
		access = 0;
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		access++;
		crash = 0;
		return;		// boot installer
	}
	else if(IN_RANGE(0x210000, adr, 0x21ffff))			// certificate
	{
		access = crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		access++;
		crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		access = crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		access++;
		crash = 0;
		if(tihw.protect) return;
	}
#endif

    // memory
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        lput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        FlashWriteByte(adr,(arg>>24)&0xff);
        FlashWriteByte(adr+1,(arg>>16)&0xff);
        FlashWriteByte(adr+2,(arg>>8)&0xff);
        FlashWriteByte(adr+3,arg&0xff);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_long(adr & 0x1f, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_long(adr & 0x1f, arg);
	}

    return;
}

void ti89_put_word(uint32_t adr, uint16_t arg) 
{
#ifdef HWP
	// stealth I/O
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		access = crash = 0;
		//cb_screen_on_off(!0);
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable/disable
	{
		if(crash >= 4)
			hw_m68k_reset();
		if(access >= 3)
			tihw.protect = 0;
		access = 0;
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		access++;
		crash = 0;
		return;		// boot installer
	}
	else if(IN_RANGE(0x210000, adr, 0x21ffff))			// certificate
	{
		access = crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		access++;
		crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		access = crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		access++;
		crash = 0;
		if(tihw.protect) return;
	}
#endif

    // memory
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        wput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        FlashWriteByte(adr,(arg>>8)&0xff);
	    FlashWriteByte(adr+1,arg&0xff);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_word(adr & 0x1f, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_word(adr & 0x1f, arg);
	}

    return;
}

void ti89_put_byte(uint32_t adr, uint8_t arg) 
{
#ifdef HWP
    // stealth I/O
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2
	{
		access = crash = 0;
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		access = crash = 0;
		//cb_screen_on_off(!0);
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable/disable
	{
		if(crash >= 4)
			hw_m68k_reset();
		if(access >= 3)
			tihw.protect = 0;
		access = 0;
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		access++;
		crash = 0;
		return;		// boot installer
	}
	else if(IN_RANGE(0x210000, adr, 0x21ffff))			// certificate
	{
		access = crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		access++;
		crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		access = crash = 0;
		if(tihw.protect) return;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		access++;
		crash = 0;
		if(tihw.protect) return;
	}
#endif

    // memory
    if(IN_RANGE(0x000000, adr, 0x1fffff))				// RAM access
	{
        bput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        FlashWriteByte(adr,arg&0xff);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_byte(adr & 0x1f, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_byte(adr & 0x1f, arg);
	}

    return;
}
