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
    FLASH algorithm management
*/

#include <stdio.h>
#include <stdint.h>

#include "mem.h"
#include "ti68k_def.h"
#include "flash.h"

FLASH_WSM   wsm;

/*
	Write a byte intl a Sharp FLASH memory.
	Not reworked yet (from Corvazier)
*/
void FlashWriteByte(uint32_t addr, int v)
{
	//int i;
    uint8_t *rom = tihw.rom;
  
	if(tihw.calc_type == TI92)
        return;

    if(tihw.protect) 
        return;

	addr -= tihw.rom_base << 16;
	addr &= tihw.rom_size - 1;

    // Write State Machine (WSM, Sharp's data sheet)
    if (wsm.write_ready)
    {
        if ((rom[addr] == 0xff) || (wsm.write_ready == 1))
	    {
	        rom[addr] = v;
	        wsm.changed[addr >> 16] = 1;
	    }
        else
	        wsm.write_ready--;
            wsm.write_ready--;
            wsm.ret_or = 0xffffffff;
    }
    else if (v == 0x50)
        wsm.write_phase = 0x50;
    else if (v == 0x10)
    {
        if (wsm.write_phase == 0x50)
	        wsm.write_phase = 0x51;
        else if (wsm.write_phase == 0x51)
        {
	        wsm.write_ready = 2;
	        wsm.write_phase = 0x50;
        }
    }
    else if (v == 0x20)
    {
        if (wsm.write_phase == 0x50)
	        wsm.write_phase = 0x20;
    }
    else if (v == 0xd0)
    {
        if (wsm.write_phase == 0x20)
        {
	        wsm.write_phase = 0xd0;
	        wsm.ret_or = 0xffffffff;
	        wsm.erase = 0xffffffff;
	        wsm.erase_phase = 0;

			memset(&rom[addr & 0xff0000], 0xff, 64*KB);
			/*
	        for (i = 0; i < 0x10000; i++)
	            rom[(addr & 0x1f0000) + i] = 0xff;	// 64KB block
				*/
			
	        wsm.changed[addr >> 16] = 1;
        } 
    }
    else if (v == 0xff)
    {
        if (wsm.write_phase == 0x50)
        {
	        wsm.write_ready = 0;
	        wsm.ret_or = 0;
        }
    }
}

/*
    Find the PC reset vector in ROM dump or FLASH upgrade.
    If we have a FLASH upgrade, we copy the vector table into RAM.
    This allow to use FLASH upgrade as fake ROM dump.
*/
uint32_t find_pc(void)
{
    int vt = 0x000000; // vector table
    int i;
    uint32_t pc;

    // find PC reset vector
    if(tihw.rom_flash)
    { 
        // FLASH (TI89, TI92+, 200, ...)
        for (vt = 0x12000; vt < tihw.rom_size; vt++)
	    {
	        if (*((int*)(tihw.rom + vt)) == 0xcccccccc) 
            {
	            vt += 4;
	            break;
	        }
        }
    
        vt += 4; // skip SP
   
        pc = tihw.rom[vt+3] | (tihw.rom[vt+2]<<8) |
            (tihw.rom[vt+1]<<16) | (tihw.rom[vt]<<24);
    }
  else
    { 
      // EPROM (TI92)
      vt = 0;
      vt += 4; // skip SP
      
      pc = tihw.rom[vt+3] | (tihw.rom[vt+2]<<8) |
        (tihw.rom[vt+1]<<16) | (tihw.rom[vt]<<24);
    }

    // copy vector table into RAM for boot (72 vectors, 288 bytes)
    for (i = 0; i < 0x120; i++)
        tihw.ram[i] = tihw.rom[vt + i];

	printf("found PC ($%06x) at offset 0x%x\n", pc, vt - 0x12000);

    return (pc);
}
