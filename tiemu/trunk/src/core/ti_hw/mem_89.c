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
    Memory management: TI89/92+/V200 FLASH
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

/* Put/Get byte/word/longword */
#define bput(adr, arg) { mem_tab[(adr)>>20][(adr) & mem_mask[(adr)>>20]] = (arg); }
#define wput(adr, arg) { bput((adr), (arg)>> 8); bput((adr)+1, (arg)&0x00ff); }
#define lput(adr, arg) { wput((adr), (uint16_t)((arg)>>16)); wput((adr)+2, (uint16_t)((arg)&0xffff)); }

#define bget(adr) (mem_tab[(adr)>>20][(adr)&mem_mask[(adr)>>20]])
#define wget(adr) ((uint16_t)(((uint16_t)bget(adr))<< 8 | bget((adr)+1)))
#define lget(adr) ((uint32_t)(((uint32_t)wget(adr))<<16 | wget((adr)+2)))

uint32_t ti89_get_long(uint32_t adr) 
{
    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
    {
        tihw.protect = 1;
        return lget(adr);
    }

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.protect && adr>=0x210000 && adr<=0x211fff)
        return 0x14141414;
  
    // RAM access
    else if (adr<0x200000) 
        return lget(adr);
  
    // FLASH access
    else if (adr >= 0x200000 && adr<0x600000) 
        return (lget(adr) | wsm.ret_or);

    // memory-mapped I/O
    else if(adr >= 0x600000 && adr < 0x700000) 
        return io_get_long(adr & 0x1f);

	// memory-mapped I/O (hw2)
	else if(adr >= 0x700000 && adr < 0x800000)
		return io2_get_long(adr & 0x1f);

    else
        return 0;
}

uint16_t ti89_get_word(uint32_t adr) 
{
    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
    {
        tihw.protect = 1;
        return wget(adr);
    }

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.protect && adr>=0x210000 && adr<=0x211fff)
        return 0x1414;
  
    // RAM access
    else if (adr<0x200000) 
        return wget(adr);
  
    // FLASH access
    else if (adr >= 0x200000 && adr<0x600000) 
        return (wget(adr) | wsm.ret_or);

    // memory-mapped I/O
    else if(adr >= 0x600000 && adr < 0x700000) 
        return io_get_word(adr & 0x1f);

	// memory-mapped I/O (hw2)
	else if(adr >= 0x700000 && adr < 0x800000)
		return io2_get_word(adr & 0x1f);

    else
        return 0;
}

uint8_t ti89_get_byte(uint32_t adr) 
{
    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
    {
        tihw.protect = 1;
        return bget(adr);
    }

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.protect && adr>=0x210000 && adr<=0x211fff)
        return 0x14;
  
    // RAM access
    else if (adr<0x200000) 
        return bget(adr);
  
    // FLASH access
    else if (adr >= 0x200000 && adr<0x600000) 
        return (bget(adr) | wsm.ret_or);

    // memory-mapped I/O
    else if(adr >= 0x600000 && adr < 0x700000) 
        return io_get_byte(adr & 0x1f);

	// memory-mapped I/O (hw2)
	else if(adr >= 0x700000 && adr < 0x800000)
		return io2_get_byte(adr & 0x1f);

    else
        return 0;
}

void ti89_put_long(uint32_t adr, uint32_t arg) 
{
    // Write accesses to the boot installer sector ($200000-$20FFFF) are
    // filtered and never reach the flash ROM.
    if(adr >= 0x200000 && adr < 0x210000)
        return;

    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    else if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.protect = 0;
  
    // write to internal/external FLASH
    else if (adr >= 0x200000 && adr < 0x600000)
    {
        FlashWriteByte(adr,(arg>>24)&0xff);
        FlashWriteByte(adr+1,(arg>>16)&0xff);
        FlashWriteByte(adr+2,(arg>>8)&0xff);
        FlashWriteByte(adr+3,arg&0xff);
    }

    // memory-mapped I/O
    else if(adr >= 0x600000 && adr < 0x700000)
        io_put_long(adr & 0x1f, arg);

	// memory-mapped I/O (hw2)
	else if(adr >= 0x700000 && adr < 0x800000)
		io2_put_long(adr & 0x1f, arg);

    // standard access
    else
        lput(adr, arg);
}

void ti89_put_word(uint32_t adr, uint16_t arg) 
{
	// Write accesses to the boot installer sector ($200000-$20FFFF) are
    // filtered and never reach the flash ROM.
	if(adr >= 0x200000 && adr < 0x210000)
        return;

	//$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
	else if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.protect = 0;

	// write to internal/external FLASH
    else if (adr >= 0x200000 && adr < 0x600000) 
    {
	    FlashWriteByte(adr,(arg>>8)&0xff);
	    FlashWriteByte(adr+1,arg&0xff);
    }

	// memory-mapped I/O
    else if(adr >= 0x600000 && adr < 0x700000)
		io_put_word(adr & 0x1f, arg);

	// memory-mapped I/O (HW2)
	else if(adr >= 0x700000 && adr < 0x800000)
		io2_put_word(adr & 0x1f, arg);

	// standard access
	else
		wput(adr, arg);
}

void ti89_put_byte(uint32_t adr, uint8_t arg) 
{
    // Write accesses to the boot installer sector ($200000-$20FFFF) are
    // filtered and never reach the flash ROM.
    if(adr >= 0x200000 && adr < 0x210000)
        return;

    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    else if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.protect = 0;
  
    // write to internal/external FLASH
    else if (adr >= 0x200000 && adr < 0x600000)
    {
        FlashWriteByte(adr,arg&0xff);
    }

    // memory-mapped I/O
    else if(adr >= 0x600000 && adr < 0x700000)
        io_put_byte(adr & 0x1f, arg);

	// memory-mapped I/O (hw2)
	else if(adr >= 0x700000 && adr < 0x800000)
		io2_put_byte(adr & 0x1f, arg);

    // standard access
    else
        bput(adr, arg);
}
