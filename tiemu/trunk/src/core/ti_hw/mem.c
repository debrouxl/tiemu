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
    Memory management: RAM, PROM/FLASH, I/O ports and bkpts
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

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
#include "mem89.h"
#include "mem92.h"
#include "mem92p.h"
#include "mem89tm.h"

static IMG_INFO *img = &img_infos;

uint8_t *mem_tab[16];		// 1MB per banks
uint32_t mem_msk[16];		// pseudo chip-select (allow wrapping / ghost space)

// 000000-0fffff : RAM (128 or 256 KB)
// 100000-1fffff : 
// 200000-2fffff : internal ROM (TI92, TI89, V200) or unused
// 300000-3fffff : idem
// 400000-4fffff : external ROM (TI92, TI92-II, TI92+) or unused
// 500000-5fffff : idem
// 600000-6fffff : memory mapped I/O (all HW)
// 700000-7fffff : memory mapped I/O (HW2, HW3)
// 800000-8fffff : ROM (TI89 Titanium) or unused
// 900000-9fffff : idem
// a00000-afffff : idem
// b00000-bfffff : idem
// c00000-cfffff : unused
// d00000-dfffff :	 ...
// e00000-efffff :   ...
// d00000-ffffff : unused

static GETBYTE_FUNC	get_byte_ptr;
static GETWORD_FUNC	get_word_ptr;
static GETLONG_FUNC	get_long_ptr;
static PUTBYTE_FUNC	put_byte_ptr;
static PUTWORD_FUNC	put_word_ptr;
static PUTLONG_FUNC	put_long_ptr;

/* Mem init/exit */

int hw_mem_init(void)
{
	int i;

    // get memory sizes
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
		tihw.io2_size = ti68k_get_io2_size(tihw.calc_type);
	}

	if(tihw.calc_type == TI89t)
		tihw.ram_size = 2*MB;	// used to avoid mirroring of segments

	// init vars
    tihw.protect = 0;
	hw_flash_init();

    // clear breakpoints
	ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_range();

    // allocate mem
    tihw.ram = malloc(tihw.ram_size);
    tihw.rom = malloc(tihw.rom_size);
    tihw.io  = malloc(tihw.io_size);
    tihw.io2 = malloc(tihw.io2_size);
    tihw.unused = malloc(1*MB);

    // clear RAM/ROM/IO
    memset(tihw.ram, 0x00, tihw.ram_size);
    memset(tihw.io , 0x00, tihw.io_size);  
	memset(tihw.io2, 0x00, tihw.io2_size);
    memset(tihw.rom, 0xff, tihw.rom_size);
    memset(tihw.unused, 0x00, 1*MB);

	// clear banks
	memset(&mem_tab, 0, sizeof(mem_tab));
	memset(&mem_msk, 0, sizeof(mem_msk));

    // default: set all banks to UNUSED (with mask 0 per default)
    for(i=0; i<16; i++)
    {
        mem_tab[i] = tihw.unused; 
        mem_msk[i] = 0;
    }

    // set banks on per calc basis
    switch(tihw.calc_type)
    {
    case TI92:
        ti92_mem_init();
        break;

    case TI92p:
        ti92p_mem_init();
        break;

    case TI89:
    case V200:
        ti89_mem_init();
        break;

    case TI89t:
        ti89t_mem_init();
        break;

    default:
        break;
    }
  
    // blit ROM
    memcpy(tihw.rom, img->data, img->size);
    free(img->data);

    if(!tihw.ram || !tihw.rom || !tihw.io || !tihw.io2)
        return -1;

    tihw.initial_pc = find_pc();

	// set memory specific parts mappers
	switch(tihw.calc_type)
	{
	case TI92:
		get_byte_ptr = ti92_get_byte;
		get_word_ptr = ti92_get_word;
		get_long_ptr = ti92_get_long;
		put_byte_ptr = ti92_put_byte;
		put_word_ptr = ti92_put_word;
		put_long_ptr = ti92_put_long;
	break;
	
	case TI89:
    case V200:
		get_byte_ptr = ti89_get_byte;
		get_word_ptr = ti89_get_word;
		get_long_ptr = ti89_get_long;
		put_byte_ptr = ti89_put_byte;
		put_word_ptr = ti89_put_word;
		put_long_ptr = ti89_put_long;
	break;
	
	case TI92p:
		get_byte_ptr = ti92p_get_byte;
		get_word_ptr = ti92p_get_word;
		get_long_ptr = ti92p_get_long;
		put_byte_ptr = ti92p_put_byte;
		put_word_ptr = ti92p_put_word;
		put_long_ptr = ti92p_put_long;
	break;

	case TI89t:
		get_byte_ptr = ti89t_get_byte;
		get_word_ptr = ti89t_get_word;
		get_long_ptr = ti89t_get_long;
		put_byte_ptr = ti89t_put_byte;
		put_word_ptr = ti89t_put_word;
		put_long_ptr = ti89t_put_long;
	break;
	}

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
	ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_range();

	hw_flash_exit();

    return 0;
}

// Use: converts m68k address into PC-mapped address
uint8_t* hw_get_real_address(uint32_t adr) 
{
    return &mem_tab[(adr>>20)&0xf][adr&mem_msk[(adr>>20)&0xf]];
}

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

	return get_long_ptr(adr);
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

	return get_word_ptr(adr);
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
  
	return get_byte_ptr(adr);
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
	else
		put_long_ptr(adr, arg);
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
	else
		put_word_ptr(adr, arg);
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
	else
		put_byte_ptr(adr, arg);
}
