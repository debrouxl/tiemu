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
#include "ti68k_def.h"

/*
	Retrieve address of heap (pointed by $5D42).
*/
void heap_get_base_address(uint32_t *base)
{
	uint32_t ptr;

	switch(tihw.calc_type)
	{
	case TI92:
		ptr = 0x5D42;
		*base = rd_long(&tihw.ram[ptr]);
		//printf("heap_get_base_address: $%06x\n", *base);
		break;
	default:
		*base = 0;
		break;
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

	heap_get_base_address(&base);
	*size = 0;

	for(i = 0; ; i++)
	{
		addr = rd_long(&tihw.ram[base + 4*i]);
		//printf("%i: $%06x\n", i, addr);
		if(addr == 0)
			break;
	}

	*size = i;
	//printf("heap_get_size: %i\n", *size);
}

/*
	Given an handle, retrieve block size and block address
*/
void heap_get_block_size(int handle, uint32_t *addr, uint16_t *size)
{
	uint32_t base;

	heap_get_base_address(&base);

	*addr = rd_long(&tihw.ram[base + 4*handle]);
	*size = rd_word(&tihw.ram[*addr - 2]);
}