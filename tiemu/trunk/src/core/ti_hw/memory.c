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
    TI memory space management: RAM, PROM/FLASH, I/O ports
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "uae.h"
#include "hardware.h"
#include "images.h"
#include "memory.h"
#include "bkpts.h"
#include "ti68k_def.h"

static IMG_INFO *img = &img_infos; // a shortcut

const int rom_sizes[] = { 1*MB, 2*MB, 2*MB, 4*MB };	// 92, 89, 92+, V200
const int ram_sizes[] = { 128*KB, 256*KB, 256*KB, 256*KB };
const int io_size = 32;

UBYTE *mem_tab[16];     // 1MB per banks
ULONG mem_mask[16];		// pseudo chip-select

// 000000-0fffff : RAM (128 or 256 KB)
// 100000-1fffff : 
// 200000-2fffff : internal ROM (TI89 or V200 ?)
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

int rom_changed[32]; // FLASH segments which have been (re)programmed
//int tihw.flash_prot;
int rom_ret_or;
int rom_write_ready;
int rom_write_phase;
int rom_erase;
int rom_erasePhase;

static int log2(int i);

/* Mem init/exit */

int hw_mem_init(void)
{
	int i;

	// get infos from image
	tihw.rom_internal = img->internal;
	tihw.rom_flash = img->flash;
	strcpy(tihw.rom_version, img->version);

	if((tihw.calc_type == TI92) && (strcmp(tihw.rom_version, "2.0") > 0))
	{
		// TI92 II is same as TI92+
		tihw.rom_size = rom_sizes[1];
		tihw.ram_size = ram_sizes[1];
	}
	else
	{
		tihw.rom_size = rom_sizes[log2(tihw.calc_type)];
		tihw.ram_size = ram_sizes[log2(tihw.calc_type)];
	}

	// init vars
	tihw.ram256 = (tihw.ram_size == 256*KB);
	//tihw.ram_wrap = 
	tihw.mem_prot = 1;
    tihw.flash_prot = 1;

  /* Init vars */
  rom_write_ready=0; 
  rom_write_phase=0; 
  rom_ret_or=0;
  rom_erase=0;
  rom_write_ready = 0;
  rom_write_phase = 0;
  rom_ret_or = 0;
  //tihw.flash_prot = 0;

    // clear breakpoints
	ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_access_range();
	ti68k_bkpt_clear_vector();
	ti68k_bkpt_clear_autoint();
	ti68k_bkpt_clear_trap();

    // allocate mem
    tihw.ram = malloc(tihw.ram_size + 4);
    tihw.rom = malloc(tihw.rom_size + 4);
    tihw.io  = malloc(32 + 4);
    tihw.io2 = malloc(32 + 4);

    // clear RAM/ROM/IO
    memset(tihw.ram, 0x00, tihw.ram_size);
    memset(tihw.io , 0x00, io_size);  
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
    mem_tab[1] = tihw.ram;
    mem_mask[1] = tihw.ram_size-1;

    // map ROM (internal)
    mem_tab[2] = tihw.rom;
    mem_mask[2] = tihw.rom_size/2-1;
    mem_tab[3] = tihw.rom + 0x100000;
    mem_mask[3] = tihw.rom_size/2-1;

    // map ROM (internal)
    mem_tab[4] = tihw.rom;
    mem_mask[4] = tihw.rom_size/2-1;
    mem_tab[5] = tihw.rom + 0x100000;
    mem_mask[5] = tihw.rom_size/2-1;

    // map IO
    mem_tab[6] = tihw.io;
    mem_mask[6] = io_size-1;
    mem_tab[7] = tihw.io2;
    mem_mask[7] = io_size-1;
  
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
	ti68k_bkpt_clear_access_range();
	ti68k_bkpt_clear_vector();
	ti68k_bkpt_clear_autoint();
	ti68k_bkpt_clear_trap();

    return 0;
}

/* Put/Get byte/word/longword */
#define bput(adr, arg) { mem_tab[(adr)>>20][(adr) & mem_mask[(adr)>>20]] = (arg); }
#define wput(adr, arg) { bput((adr), (arg)>> 8); bput((adr)+1, (arg)&0x00ff); }
#define lput(adr, arg) { wput((adr), (arg)>>16); wput((adr)+2, (arg)&0xffff); }

#define bget(adr) (mem_tab[(adr)>>20][(adr)&mem_mask[(adr)>>20]])
#define wget(adr) ((UWORD)(((UWORD)bget(adr))<< 8 | bget((adr)+1)))
#define lget(adr) ((ULONG)(((ULONG)wget(adr))<<16 | wget((adr)+2)))

static void FlashWriteByte(int addr,int v);

ULONG get_long(CPTR adr) 
{
    GList* l;
  
    adr &= 0xFFFFFF;

    if (l=listBkptAsRL) 
    {
        breakId=0;
        while (l) 
	    {
	        if (l->data==adr) 
	        {
	            breakMode = BK_READ_LONG; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS;
	            break;
	        }
	        
            breakId++;
	        l=l->next;
	    }
    }
  
    if (l=listBkptAsRgR) 
    {
        breakId=0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if (r->val>=adr && (adr+3)<=r->val2) 
	        {
	            breakMode = BK_READ_LONG; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS_RANGE;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
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
        tihw.flash_prot = 1;

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.flash_prot && adr>=0x210000 && adr<=0x211fff)
        return 0x14141414;
  
    // RAM access
    else if (adr<0x200000) 
        return lget(adr);
  
    // FLASH access
    else if (adr >= 0x200000 && adr<0x600000) 
        return (lget(adr) | rom_ret_or);

    // memory-mapped I/O
    else if(adr >= 0x600000) 
        return io_get_long(adr&0x1f);
}

UWORD get_word(CPTR adr) 
{
    GList* l;
	
    adr &= 0xFFFFFF;

    if (l=listBkptAsRW) 
    {
        breakId = 0;
        while (l) 
	    {
	        if (l->data==adr) 
	        {
	            breakMode = BK_READ_WORD;
	            specialflags|=SPCFLAG_BRK;
	            breakType = BK_CAUSE_ACCESS;
	            break;
	        }
	    
            breakId++;
	        l=l->next;
	    }
    }
  
    if (l=listBkptAsRgR) 
    {
        breakId=0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if (r->val>=adr && (adr+1)<=r->val2) 
	        {
	            breakMode = BK_READ_WORD; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS_RANGE;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
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
        tihw.flash_prot = 1;

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.flash_prot && adr>=0x210000 && adr<=0x211fff)
        return 0x1414;
  
    // RAM access
    else if (adr<0x200000) 
        return wget(adr);
  
    // FLASH access
    else if (adr >= 0x200000 && adr<0x600000) 
        return (wget(adr) | rom_ret_or);

    // memory-mapped I/O
    else if(adr >= 0x600000) 
        return io_get_word(adr&0x1f);
}

UBYTE get_byte(CPTR adr) 
{
    GList* l;
  
    adr &= 0xFFFFFF;

    if (l=listBkptAsRB) 
    {
        breakId = 0;
        while (l) {
	        if (l->data==adr) 
	        {
	            breakMode = BK_READ_BYTE;
	            specialflags|=SPCFLAG_BRK;
	            breakType = BK_CAUSE_ACCESS;
	            break;
	        }
	    
            breakId++;
	        l=l->next;
        }
    }

    if (l=listBkptAsRgR) 
    {
        breakId=0;
        while (l) 
	    {
        
            ADDR_RANGE *r = l->data;

	        if (r->val>=adr && adr<=r->val2) 
	        {
	            breakMode = BK_READ_BYTE; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS_RANGE;
	            break;
	        }
	  
            breakId++;
	  
            l=l->next;
	    }
    }
  
    //$1C0000-$1FFFFF: "the Protection" enable/disable
    //Note: Four consecutive accesses to this range crashes a HW1 calc!
    //READ:  Enable the Protection
    //WRITE: Disable the Protection
    //Note: No access to this range will have any effect unless the access is
    //"authorized," see below.
    if(adr >= 0x1C0000 && adr < 0x200000 && tihw.hw_type == 2)
        tihw.flash_prot = 1;

    // The certificate memory ($210000-$211FFF) is read protected.
    else if (tihw.flash_prot && adr>=0x210000 && adr<=0x211fff)
        return 0x14;
  
    // RAM access
    else if (adr<0x200000) 
        return bget(adr);
  
    // FLASH access
    else if (adr >= 0x200000 && adr<0x600000) 
        return (bget(adr) | rom_ret_or);

    // memory-mapped I/O
    else if(adr >= 0x600000) 
        return io_get_byte(adr&0x1f);
}

void put_long(CPTR adr, ULONG arg) 
{
    GList* l;

    adr &= 0xFFFFFF;

    if (l=listBkptAsWL) 
    {
        breakId = 0;
        while (l) 
	    {
	        if (l->data==adr) 
	        {
	            breakMode = BK_WRITE_LONG;
	            specialflags|=SPCFLAG_BRK;
	            breakType = BK_CAUSE_ACCESS;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
	    }
    }
  
    if (l=listBkptAsRgW) 
    {
        breakId=0;
        while (l) 
	    {
	        ADDR_RANGE *r = l->data;

            if (r->val>=adr && (adr+3)<=r->val2) 
	        {
	            breakMode = BK_WRITE_LONG; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS_RANGE;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
	    }
    }

    // Odd address: exception !
    if(adr & 1)
    {
        specialflags |= SPCFLAG_ADRERR;
        return;
    }
    
	if((adr < 0x120) && tihw.mem_prot)
	{
		specialflags |= SPCFLAG_INT;
        currIntLev = 7;
	}

    if (adr < 0x200000) 
	{
	    if (adr >=0x1c0000)
	        tihw.flash_prot=0;
	    lput(adr, arg);
	}
    else if (adr < 0x400000)
	{
	    FlashWriteByte(adr,(arg>>24)&0xff);
        FlashWriteByte(adr+1,(arg>>16)&0xff);
        FlashWriteByte(adr+2,(arg>>8)&0xff);
        FlashWriteByte(adr+3,arg&0xff);
	}
    else if (adr < 0x600000) 
	{
	    FlashWriteByte(adr,(arg>>24)&0xff);
	    FlashWriteByte(adr+1,(arg>>16)&0xff);
	    FlashWriteByte(adr+2,(arg>>8)&0xff);
	    FlashWriteByte(adr+3,arg&0xff);
	}
    else
	    io_put_long(adr&0x1f, arg);

}

void put_word(CPTR adr, UWORD arg) 
{
    GList* l;
	
    adr &= 0xFFFFFF;

    if (l=listBkptAsWW) 
    {
        breakId = 0;
        while (l) 
	    {
	        if (l->data==adr) 
	        {
	            breakMode = BK_WRITE_WORD;
	            specialflags|=SPCFLAG_BRK;
	            breakType = BK_CAUSE_ACCESS;
	            break;
	        }
	  
            breakId++;
            l=l->next;
	    }
    }
  
    if (l=listBkptAsRgW)
    {
        breakId=0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if (r->val>=adr && (adr+1)<=r->val2) 
	        {
	            breakMode = BK_WRITE_WORD; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS_RANGE;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
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
    if((adr < 0x120) && tihw.mem_prot)
	{
		specialflags |= SPCFLAG_INT;
        currIntLev = 7;
	}

    if (adr < 0x200000) 
    {
	    if (adr >=0x1c0000)
	        tihw.flash_prot=0;
	    wput(adr, arg);
    }
    else if (adr < 0x400000)
    {
	    FlashWriteByte(adr,(arg>>8)&0xff);
        FlashWriteByte(adr+1,arg&0xff);
    }
    else if (adr < 0x600000) 
    {
	    FlashWriteByte(adr,(arg>>8)&0xff);
	    FlashWriteByte(adr+1,arg&0xff);
    }
    else
		io_put_word(adr&0x1f, arg);
}

void put_byte(CPTR adr, UBYTE arg) 
{
    GList* l;
	
    adr &= 0xFFFFFF;
  
    if (l=listBkptAsWB) 
    {
        breakId = 0;
        while (l) 
	    {
	        if (l->data==adr) 
	        {
	            breakMode = BK_WRITE_BYTE;
	            specialflags|=SPCFLAG_BRK;
	            breakType = BK_CAUSE_ACCESS;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
	    }
    }

    if (l=listBkptAsRgW) 
    {
        breakId=0;
        while (l) 
	    {
            ADDR_RANGE *r = l->data;

	        if (r->val>=adr && adr<=r->val2) 
	        {
	            breakMode = BK_WRITE_BYTE; 
	            specialflags|=SPCFLAG_BRK; 
	            breakType = BK_CAUSE_ACCESS_RANGE;
	            break;
	        }
	  
            breakId++;
	        l=l->next;
	    }
    }

    // Protected memory violation. Triggered when memory below [$000120] is
	// written while bit 2 of [$600001] is set
    if((adr < 0x120) && tihw.mem_prot)
	{
		specialflags |= SPCFLAG_INT;
        currIntLev = 7;
	}

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
  
    // write to internal FLASH
    else if (adr >= 0x200000 && adr < 0x400000)
        FlashWriteByte(adr,arg&0xff);

    // write to external FLASH
    else if (adr >= 0x400000 && adr < 0x600000)
        FlashWriteByte(adr,arg&0xff);

    // memory-mapped I/O
    else if(adr >= 0x600000)
        io_put_byte(adr&0x1f, arg);

    // standard access
    else
        bput(adr, arg);
}

// Use: ??
UBYTE *get_real_address(CPTR adr) 
{
    return &mem_tab[(adr>>20)&0xf][adr&mem_mask[(adr>>20)&0xf]];
}

// not reworked yet (from Corvazier)
static void FlashWriteByte(int addr, int v)
{
    UBYTE *rom = mem_tab[2];
    int i;

    // map ROM accesses
    if(tihw.rom_internal)
        rom = mem_tab[2];
    else
        rom = mem_tab[4];
  
    addr &= 0x1fffff;
  
    if(tihw.flash_prot) 
        return;

    // TI92 has EPROM
    if(tihw.calc_type == TI92)
        return;

    // Write State Machine (WSM, Sharp's data sheet)
    if (rom_write_ready)
    {
        if ((rom[addr]==0xff)||(rom_write_ready==1))
	    {
	        rom[addr]=v;
	        rom_changed[addr>>16]=1;
	    }
        else
	        rom_write_ready--;
            rom_write_ready--;
            rom_ret_or=0xffffffff;
    }
    else if (v==0x50)
        rom_write_phase=0x50;
    else if (v==0x10)
    {
        if (rom_write_phase==0x50)
	        rom_write_phase=0x51;
        else if (rom_write_phase==0x51)
        {
	        rom_write_ready=2;
	        rom_write_phase=0x50;
        }
    }
    else if (v==0x20)
    {
        if (rom_write_phase==0x50)
	        rom_write_phase=0x20;
    }
    else if (v==0xd0)
    {
        if (rom_write_phase==0x20)
        {
	        rom_write_phase=0xd0;
	        rom_ret_or=0xffffffff;
	        rom_erase=0xffffffff;
	        rom_erasePhase=0;
	        for (i=0;i<0x10000;i++)
	            rom[(addr&0x1f0000)+i]=0xff;
	        rom_changed[addr>>16]=1;
        } 
    }
    else if (v==0xff)
    {
        if (rom_write_phase==0x50)
        {
	        rom_write_ready=0;
	        rom_ret_or=0;
        }
    }
}

/*
    Find the PC reset vector in ROM dump or FLASH upgrade.
    If we have a FLASH upgrade, we copy the vector table into RAM.
    This allow to use FLASH upgrade as fake ROM dump.
*/
static int find_pc()
{
    int vt = 0x000000; // vector table
    int i;
    uint32_t pc;
  
    // find PC reset vector
    if(tihw.rom_flash)
    { 
        // TI89 or TI92+
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
      // TI92
      vt = 0;
      vt += 4; // skip SP
      
      pc = tihw.rom[vt+3] | (tihw.rom[vt+2]<<8) |
        (tihw.rom[vt+1]<<16) | (tihw.rom[vt]<<24);
    }

    // copy vector table into RAM for boot
    for (i = 0; i < 256; i++)
        tihw.ram[i] = tihw.rom[vt + i];

    return (pc);
}

static int log2(int i)
{
	return (int)(log10(i) / log10(2));
}