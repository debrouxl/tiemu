/* Hey EMACS -*- linux-c -*- */
/* $Id: mem89.c 941 2005-03-12 09:37:10Z roms $ */

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

static int access1 = 0;		// protection access authorization (hw1)
static int access2 = 0;		// same (hw2)
static int crash = 0;
static unsigned int arch_mem_limit = 0;
static int arch_mem_crash = 0;

#define HWP
//#define HWP1				// HW1 protection if define set
//#define HWP2				// HW2 protection if define set

static void crash_calc(void)
{
	//hw_reset();
	access1 = access2 = 0;
	crash = 0;
	printf("Grr, you've crashed the calc !\n");
}

// note: "if(!(adr & 1))" is used to avoid multiple increment when reading/writing words
// this don't work for longs but this problem will not exist when this code will be
// splitted.

uint8_t ti89_hwp_get_byte(uint32_t adr) 
{
#ifdef HWP
	// stealth I/O
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0 (hw1)
	{
		if(!tihw.protect && (tihw.hw_type == HW1))
			bit_clr(arch_mem_limit, 0);
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1 (hw1)
	{
		if(!tihw.protect && (tihw.hw_type == HW1))
			bit_clr(arch_mem_limit, 1);
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2 (hw1)
	{
		if(!tihw.protect && (tihw.hw_type == HW1))
			bit_clr(arch_mem_limit, 2);
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		tihw.on_off = 0;
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection enable
	{
		if((access1 >= 3) || (++access2 == 8)) 
		{
			tihw.protect = !0;
			access1 = access2 = 0;
		}
		else if(tihw.hw_type == HW1)
		{
			// any four consecutive access to $1c0000-1fffff crashes a HW1 calc
			if(!(adr & 1)) crash++;
			if(crash >= 4)
			{
				crash_calc();
				printf("1");
			}
		}
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		if(tihw.hw_type == HW1)
			if(!(adr & 1)) 
				access1++;
		if(tihw.hw_type == HW2)
			if(!(adr & 1)) 
				access2++;
	}
	else if(IN_RANGE(0x210000, adr, 0x211fff))			// certificate (read protected)
	{
		if(tihw.protect)
			return 0x14;
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		if(tihw.hw_type == HW1)
			if(!(adr & 1)) 
				access1++;
		if(tihw.hw_type == HW2)
			if(!(adr & 1)) 
				access2++;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		if(tihw.protect)
			return 0x14;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		if(tihw.hw_type == HW1)
			if(!(adr & 1)) 
				access1++;
		if(tihw.hw_type == HW2)
			if(!(adr & 1)) 
				access2++;
	}
	else if(IN_RANGE(0x390000+arch_mem_limit*0x10000, 
								adr, 0x3fffff))			// archive memory limit (hw1)
	{
		// three consecutive access to any adress >=$390000+limit*$10000 and <$400000 crashes the calc
		if(!(adr & 1)) arch_mem_crash++;
		if((tihw.hw_type == HW1) && (arch_mem_crash >= 4))
		{
			// instruction fetch ??
			//crash_calc();
			printf("2");
		}
	}
	else
	{
		access1 = access2 = 0;
		crash = arch_mem_crash = 0;
	}

	// protections (hw2)
	if(tihw.hw_type == HW2)
	{
		if(IN_RANGE(0x000000, adr, 0x03ffff))				// RAM page execution protection
		{
			// instruction fetch
			//if(tihw.ram_exec[adr >> 24]) crash_calc();
		}
		else if(IN_RANGE(0x040000, adr, 0x1fffff))			// RAM page execution protection
		{
			// instruction fetch
			//if(io2_bit_tst(6, 7)) crash_calc();
		}
		else if(IN_RANGE(0x210000, adr, 0x3fffff))			// FLASH page execution protection
		{
			// instruction fetch
			//if(adr >= (uint32_t)(0x210000 + tihw.io2[0x12]*0x10000)) crash_calc();
		}
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
       return io_get_byte(adr);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_byte(adr);
	}

    return 0x14;
}

uint16_t ti89_hwp_get_word(uint32_t adr) 
{
	return (ti89_hwp_get_byte(adr+0) << 8) | ti89_hwp_get_byte(adr+1);
}

uint32_t ti89_hwp_get_long(uint32_t adr) 
{
	return (ti89_hwp_get_word(adr+0) << 16) | ti89_hwp_get_word(adr+2);
}

void ti89_hwp_put_byte(uint32_t adr, uint8_t arg) 
{
#ifdef HWP
    // stealth I/O
	if(IN_RANGE(0x040000, adr, 0x07ffff))				// archive memory limit bit 0 (hw1)
	{
		if(!tihw.protect && (tihw.hw_type == HW1))
			bit_set(arch_mem_limit, 0);
	}
	else if(IN_RANGE(0x080000, adr, 0x0bffff))			// archive memory limit bit 1 (hw1)
	{
		if(!tihw.protect && (tihw.hw_type == HW1))
			bit_set(arch_mem_limit, 1);
	}
	else if(IN_RANGE(0x0c0000, adr, 0x0fffff))			// archive memory limit bit 2 (hw1)
	{
		if(!tihw.protect && (tihw.hw_type == HW1))
			bit_set(arch_mem_limit, 2);
	}
	else if(IN_RANGE(0x180000, adr, 0x1bffff))			// screen power control
	{
		tihw.on_off = !0;
	}
	else if(IN_RANGE(0x1c0000, adr, 0x1fffff))			// protection disable
	{
		if((access1 >= 3) || (++access2 == 8)) 
		{
			tihw.protect = 0;
			access1 = access2 = 0;
		}
		else if(tihw.hw_type == HW1)
		{
			// any four consecutive accesses to $1c0000-1fffff crash an HW1 calc
			if(!(adr & 1)) crash++;
			if(crash >= 4)
			{
				crash_calc();
				printf("3");
			}
		}
	}
	else if(IN_RANGE(0x200000, adr, 0x20ffff))			// protection access authorization
	{
		if(tihw.hw_type == HW1)
			if(!(adr & 1)) 
				access1++;
		if(tihw.hw_type == HW2)
			if(!(adr & 1)) 
				access2++;

		// don't write on boot code
		return;		
	}
	else if(IN_RANGE(0x210000, adr, 0x211fff))			// certificate (read protected)
	{
	}
	else if(IN_RANGE(0x212000, adr, 0x217fff))			// protection access authorization
	{
		if(tihw.hw_type == HW1)
			if(!(adr & 1)) 
				access1++;
		if(tihw.hw_type == HW2)
			if(!(adr & 1)) 
				access2++;
	}
	else if(IN_RANGE(0x2180000, adr, 0x219fff))			// read protected
	{
		return;
	}
	else if(IN_RANGE(0x21a0000, adr, 0x21ffff))			// protection access authorization
	{
		if(tihw.hw_type == HW1)
			if(!(adr & 1)) 
				access1++;
		if(tihw.hw_type == HW2)
			if(!(adr & 1)) 
				access2++;
	}
	else if(IN_RANGE(0x390000+arch_mem_limit*0x10000, 
								adr, 0x3fffff))			// archive memory limit (hw1)
	{
		// three consecutive access to any adress >=$390000+limit*$10000 and <$400000 crashes the calc
		if(!(adr & 1)) arch_mem_crash++;
		if((tihw.hw_type == HW1) && (arch_mem_crash >= 4))
		{
			// instruction fetch ??
			//crash_calc();
			//printf("4");
		}
	}
	else
	{
		access1= access2 = 0;
		crash = arch_mem_crash = 0;
	}

	// protections (hw2)
	if(tihw.hw_type == HW2)
	{
		if(IN_RANGE(0x000000, adr, 0x03ffff))				// RAM page execution protection
		{
			// instruction fetch
			//if(tihw.ram_exec[adr >> 24]) crash_calc();
		}
		else if(IN_RANGE(0x040000, adr, 0x1fffff))			// RAM page execution protection
		{
			// instruction fetch
			//if(io2_bit_tst(6, 7)) crash_calc();
		}
		else if(IN_RANGE(0x210000, adr, 0x3fffff))			// FLASH page execution protection
		{
			// instruction fetch
			//if(adr >= (uint32_t)(0x210000 + tihw.io2[0x12]*0x10000)) crash_calc();
		}
	}
#endif

    // memory
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

void ti89_hwp_put_word(uint32_t adr, uint16_t arg) 
{
	ti89_hwp_put_byte(adr+0, MSB(arg));
	ti89_hwp_put_byte(adr+1, LSB(arg));
}

void ti89_hwp_put_long(uint32_t adr, uint32_t arg) 
{
	ti89_hwp_put_word(adr+0, MSW(arg));
	ti89_hwp_put_word(adr+2, LSW(arg));
}