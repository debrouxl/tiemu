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
    Memory management: RAM, PROM/FLASH, I/O ports and bkpts
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

static IMG_INFO *img = &img_infos;

uint8_t *mem_tab[16];		// 1MB per banks
uint32_t mem_mask[16];		// pseudo chip-select (allow wrapping / ghost space)

// 000000-0fffff : RAM (128 or 256 KB)
// 100000-1fffff : 
// 200000-2fffff : internal ROM (TI92, TI89, V200)
// 300000-3fffff : idem
// 400000-4fffff : external ROM (TI92, TI92-II, TI92+)
// 500000-5fffff : idem
// 600000-6fffff : memory mapped I/O
// 700000-7fffff : memory mapped I/O (HW2)
// 800000-8fffff : unused
// 900000-9fffff :   ...
// a00000-afffff : 
// b00000-bfffff : 
// c00000-cfffff : 
// d00000-dfffff : 
// e00000-efffff :   ...
// d00000-ffffff : unused


/* Mem init/exit */

int hw_mem_init(void)
{
	int i;

	if(tihw.ti92v2)
	{
		// TI92 II is same as TI92+ in memory size
		tihw.rom_size = ti68k_get_rom_size(TI92p);
		tihw.ram_size = ti68k_get_ram_size(TI92p);
		tihw.io_size = ti68k_get_io_size(TI92p);
	}
	else
	{
		tihw.rom_size = ti68k_get_rom_size(tihw.calc_type);
		tihw.ram_size = ti68k_get_ram_size(tihw.calc_type);
		tihw.io_size = ti68k_get_io_size(tihw.calc_type);
	}

	// init vars
    tihw.flash_prot = 1;
	memset(&wsm, 0, sizeof(FLASH_WSM));

    // clear breakpoints
	ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_range();

    // allocate mem
    tihw.ram = malloc(tihw.ram_size + 4);
    tihw.rom = malloc(tihw.rom_size + 4);
    tihw.io  = malloc(tihw.io_size + 4);
    tihw.io2 = malloc(tihw.io_size + 4);

    // clear RAM/ROM/IO
    memset(tihw.ram, 0x00, tihw.ram_size);
    memset(tihw.io , 0x00, tihw.io_size);  
	memset(tihw.io2, 0x00, tihw.io_size);
    for (i=0; i<tihw.rom_size; i++)
    {
        if (i & 1)
	        tihw.rom[i] = 0x00;
        else
	        tihw.rom[i] = 0x14;
    }

	// clear banks
	memset(&mem_tab, 0, sizeof(mem_tab));
	memset(&mem_mask, 0, sizeof(mem_mask));

    // set all banks to RAM (with mask 0 per default)
    for(i=0; i<16; i++)
        mem_tab[i] = tihw.ram; 

    // map RAM
    mem_tab[0] = tihw.ram;
    mem_mask[0] = tihw.ram_size-1;

    // map ROM (internal)
	if(tihw.rom_internal)
	{
        if(tihw.rom_size > 0*MB)
        {
		    mem_tab[2] = tihw.rom;
		    mem_mask[2] = MIN(tihw.rom_size, 1*MB)-1;
        }

        if(tihw.rom_size > 1*MB)
        {
		    mem_tab[3] = tihw.rom + 0x100000;
		    mem_mask[3] = MIN(tihw.rom_size - 1*MB, 1*MB)-1;
        }

        if(tihw.rom_size > 2*MB)
        {
            mem_tab[4] = tihw.rom + 0x200000;
		    mem_mask[4] = MIN(tihw.rom_size - 2*MB, 1*MB)-1;
        }

        if(tihw.rom_size > 3*MB)
        {
            mem_tab[5] = tihw.rom + 0x300000;
		    mem_mask[5] = MIN(tihw.rom_size - 3*MB, 1*MB)-1;
        }
	}

    // map ROM (external)
	if(!tihw.rom_internal)
	{
		mem_tab[4] = tihw.rom;
		mem_mask[4] = MIN(tihw.rom_size, 1*MB)-1;

		mem_tab[5] = tihw.rom + 0x100000;
		mem_mask[5] = MIN(tihw.rom_size - 1*MB, 1*MB)-1;
	}

    // map IO
    mem_tab[6] = tihw.io;
    mem_mask[6] = tihw.io_size-1;
	
	if(tihw.hw_type == HW2)
	{
		mem_tab[7] = tihw.io2;
		mem_mask[7] = tihw.io_size-1;
	}
  
    // blit ROM
    memcpy(tihw.rom, img->data, img->size);
    free(img->data);

    if(!tihw.ram || !tihw.rom || !tihw.io || !tihw.io2)
        return -1;

    tihw.initial_pc = find_pc();

    return 0;
}

int hw_mem_reset(void)
{
    return 0;
}

int hw_mem_exit(void)
{
	// free memory
    if(tihw.ram)
        free(tihw.ram); 
    tihw.ram=NULL;
  
    if(tihw.rom)
	    free(tihw.rom);
    tihw.rom = NULL;
 
    if(tihw.io)  
        free(tihw.io);  
    tihw.io = NULL;

    if(tihw.io2)
        free(tihw.io2);
    tihw.io2 = NULL;

	// clear breakpoints
	ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_range();

    return 0;
}

GETBYTE_FUNC	*get_byte;
GETWORD_FUNC	*get_word;
GETLONG_FUNC	*get_long;
PUTBYTE_FUNC	*put_byte;
PUTWORD_FUNC	*put_word;
PUTLONG_FUNC	*put_long;

/* Put/Get byte/word/longword */
#define bput(adr, arg) { mem_tab[(adr)>>20][(adr) & mem_mask[(adr)>>20]] = (arg); }
#define wput(adr, arg) { bput((adr), (arg)>> 8); bput((adr)+1, (arg)&0x00ff); }
#define lput(adr, arg) { wput((adr), (uint16_t)((arg)>>16)); wput((adr)+2, (uint16_t)((arg)&0xffff)); }

#define bget(adr) (mem_tab[(adr)>>20][(adr)&mem_mask[(adr)>>20]])
#define wget(adr) ((uint16_t)(((uint16_t)bget(adr))<< 8 | bget((adr)+1)))
#define lget(adr) ((uint32_t)(((uint32_t)wget(adr))<<16 | wget((adr)+2)))

uint32_t hw_get_long(uint32_t adr) 
{
    GList* l;
  
    adr &= 0xFFFFFF;

    if ((l = bkpts.mem_rl) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        if ((uint32_t)GPOINTER_TO_INT(l->data) == adr) 
	        {
				bkpts.type = BK_TYPE_ACCESS;
	            bkpts.mode = BK_READ_LONG; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	        
            bkpts.id++;
	        l = l->next;
	    }
    }
  
    if ((l = bkpts.mem_rng_r) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if ((r->val1 >= adr) && ((adr+3) <= r->val2)) 
	        {
				bkpts.type = BK_TYPE_RANGE;
	            bkpts.mode = BK_READ_LONG; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }
  
    // Odd address: exception !
    if(adr & 1) 
    {
        specialflags |= SPCFLAG_ADRERR;
        return 0;
    }

    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
    {
        tihw.flash_prot = 1;
        return lget(adr);
    }

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.flash_prot && adr>=0x210000 && adr<=0x211fff)
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

uint16_t hw_get_word(uint32_t adr) 
{
    GList* l;
	
    adr &= 0xFFFFFF;

    if ((l = bkpts.mem_rw) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        if ((uint32_t)GPOINTER_TO_INT(l->data) == adr) 
	        {
				bkpts.type = BK_TYPE_ACCESS;
	            bkpts.mode = BK_READ_WORD;
	            specialflags |= SPCFLAG_BRK;
	            break;
	        }
	    
            bkpts.id++;
	        l = l->next;
	    }
    }
  
    if ((l = bkpts.mem_rng_r) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if ((r->val1 >= adr) && ((adr+1) <= r->val2)) 
	        {
				bkpts.type = BK_TYPE_RANGE;
	            bkpts.mode = BK_READ_WORD; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }
  
    // Odd address: exception !
    if(adr & 1) 
    {
        specialflags |= SPCFLAG_ADRERR;
        return 0;
    }

    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
    {
        tihw.flash_prot = 1;
        return wget(adr);
    }

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.flash_prot && adr>=0x210000 && adr<=0x211fff)
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

uint8_t hw_get_byte(uint32_t adr) 
{
    GList* l;
  
    adr &= 0xFFFFFF;

    if ((l = bkpts.mem_rb) != NULL) 
    {
        bkpts.id = 0;
        while (l) {
	        if ((uint32_t)GPOINTER_TO_INT(l->data) == adr) 
	        {
				bkpts.type = BK_TYPE_ACCESS;
	            bkpts.mode = BK_READ_BYTE;
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	    
            bkpts.id++;
	        l = l->next;
        }
    }

    if ((l = bkpts.mem_rng_r) != NULL)
    {
        bkpts.id = 0;
        while (l) 
	    {
        
            ADDR_RANGE *r = l->data;

	        if ((r->val1 >= adr) && (adr <= r->val2))
	        {
				bkpts.type = BK_TYPE_RANGE;
	            bkpts.mode = BK_READ_BYTE; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	  
            l = l->next;
	    }
    }
  
    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
    {
        tihw.flash_prot = 1;
        return bget(adr);
    }

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.flash_prot && adr>=0x210000 && adr<=0x211fff)
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

void hw_put_long(uint32_t adr, uint32_t arg) 
{
    GList* l;

    adr &= 0xFFFFFF;

    if ((l = bkpts.mem_wl) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        if ((uint32_t)GPOINTER_TO_INT(l->data) == adr) 
	        {
				bkpts.type = BK_TYPE_ACCESS;
	            bkpts.mode = BK_WRITE_LONG;
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }
  
    if ((l = bkpts.mem_rng_w) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        ADDR_RANGE *r = l->data;

            if ((r->val1 >= adr) && ((adr+3) <= r->val2))
	        {
				bkpts.type = BK_TYPE_RANGE;
	            bkpts.mode = BK_WRITE_LONG; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }

    // Odd address: exception !
    if(adr & 1)
    {
        specialflags |= SPCFLAG_ADRERR;
        return;
    }
    
    // Protected memory violation. Triggered when memory below [$000120] is
	// written while bit 2 of [$600001] is set
	if((adr < 0x120) && io_bit_tst(0x01,2))
		hw_m68k_irq(7);

    // Write accesses to the boot installer sector ($200000-$20FFFF) are
    // filtered and never reach the flash ROM.
    else if(adr >= 0x200000 && adr < 0x210000)
        return;

    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    else if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.flash_prot = 0;
  
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

void hw_put_word(uint32_t adr, uint16_t arg) 
{
    GList* l;
	
    adr &= 0xFFFFFF;

    if ((l = bkpts.mem_ww) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        if ((uint32_t)GPOINTER_TO_INT(l->data) == adr) 
	        {
				bkpts.type = BK_TYPE_ACCESS;
	            bkpts.mode = BK_WRITE_WORD;
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
            l = l->next;
	    }
    }
  
    if ((l = bkpts.mem_rng_w) != NULL)
    {
        bkpts.id = 0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if ((r->val1 >= adr) && ((adr+1) <= r->val2))
	        {
				bkpts.type = BK_TYPE_RANGE;
	            bkpts.mode = BK_WRITE_WORD; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }
  
    // Odd address: exception !
    if(adr & 1)
	{
        specialflags |= SPCFLAG_ADRERR;
		return;
	}

    // Protected memory violation. Triggered when memory below [$000120] is
	// written while bit 2 of [$600001] is set
    if((adr < 0x120) && io_bit_tst(0x01,2))
		hw_m68k_irq(7);

	else if(adr >= 0x200000 && adr < 0x210000)
        return;
	else if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.flash_prot = 0;

    else if (adr < 0x200000) // pb here
    {
	    if (adr >=0x1c0000)
	        tihw.flash_prot=0;
	    wput(adr, arg);
    }

    else if (adr >= 0x200000 && adr < 0x600000) 
    {
	    FlashWriteByte(adr,(arg>>8)&0xff);
	    FlashWriteByte(adr+1,arg&0xff);
    }
    else if(adr >= 0x600000 && adr < 0x700000)
		io_put_word(adr & 0x1f, arg);
	else if(adr >= 0x700000 && adr < 0x800000)
		io2_put_word(adr & 0x1f, arg);
	else
		wput(adr, arg);
}

void hw_put_byte(uint32_t adr, uint8_t arg) 
{
    GList* l;
	
    adr &= 0xFFFFFF;
  
    if ((l = bkpts.mem_wb) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        if ((uint32_t)GPOINTER_TO_INT(l->data) == adr) 
	        {
				bkpts.type = BK_TYPE_ACCESS;
	            bkpts.mode = BK_WRITE_BYTE;
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }

    if ((l = bkpts.mem_rng_w) != NULL) 
    {
        bkpts.id = 0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if ((r->val1 >= adr) && (adr <= r->val2)) 
	        {
				bkpts.type = BK_TYPE_RANGE;
	            bkpts.mode = BK_WRITE_BYTE; 
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	  
            bkpts.id++;
	        l = l->next;
	    }
    }

    // Protected memory violation. Triggered when memory below [$000120] is
	// written while bit 2 of [$600001] is set
    if((adr < 0x120) && io_bit_tst(0x01,2))
		hw_m68k_irq(7);

    // Write accesses to the boot installer sector ($200000-$20FFFF) are
    // filtered and never reach the flash ROM.
    else if(adr >= 0x200000 && adr < 0x210000)
        return;

    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    else if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.flash_prot = 0;
  
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

// Use: converts m68k address into PC-mapped address
uint8_t* hw_get_real_address(uint32_t adr) 
{
    return &mem_tab[(adr>>20)&0xf][adr&mem_mask[(adr>>20)&0xf]];
}
