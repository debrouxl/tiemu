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
    Handles/Heap access
*/

#include <stdio.h>

#include "handles.h"
#include "romcalls.h"
#include "ti68k_def.h"
#include "ti68k_int.h"

/*
	Retrieve address of heap (pointed by $5D42 on TI92).
*/
void heap_get_addr(uint32_t *base)
{
	uint32_t ptr;

	if(tihw.ti92v2)
	{
		ptr = 0x4720 + 0x1902;		//tios::main_lcd equ tios::globals+$0000
		*base = rd_long(ti68k_get_real_address(ptr));
	}
	else if(tihw.ti92v1)
	{
		ptr = 0x4440 + 0x1902;		//and tios::heap equ tios::globals+$1902
		*base = rd_long(ti68k_get_real_address(ptr));
	}
	else
	{
		uint32_t b, size, addr;

		romcalls_get_table_infos(&b, &size);
		if(size < 0x441)
		{
			// AMS1
			romcalls_get_symbol_address(0x96, &addr);		// tios::HeapDeref (#0x096)
			ptr = rd_word(ti68k_get_real_address(addr + 8));// MOVEA.W $7592,A0
			*base = rd_long(ti68k_get_real_address(ptr));
		} else
		{
			// AMS2
			romcalls_get_symbol_address(0x441, &addr);		// tios::HeapTable	(#0x441)
			*base  = addr;
		}
	}

	
}

/*
	Retrieve size of the heap (how many handles are allocated at the time when
	this function is called).
*/
void heap_get_size(uint16_t *size)
{
	uint32_t base, addr;
	int i;

	*size = 0;
	heap_get_addr(&base);
	if(base == -1)
		return;

	for(i = 0; ; i++)
	{
		addr = rd_long(ti68k_get_real_address(base + 4*i));
		if(addr == 0)
			break;
	}

	*size = i;
}

/*
	Given an handle, retrieve block size and block address
*/
void heap_get_block_size(int handle, uint32_t *addr, uint16_t *size)
{
	uint32_t base;

	heap_get_addr(&base);

	*addr = rd_long(ti68k_get_real_address(base + 4*handle));
	*size = rd_word(ti68k_get_real_address(*addr - 2));

	*size &= ~(1 << 16);	// remove lock
	*size <<= 1;			// size is twice
}