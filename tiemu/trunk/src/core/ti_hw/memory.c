/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001-2002, Romain Lievin
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

#include "uae.h"

#include "hardware.h"
#include "consts.h"
#include "images.h"
#include "memory.h"
#include "bkpts.h"
#include "params.h"

/* Memory blocks */

UBYTE *ti_rom;
UBYTE *ti_int_rom;
UBYTE *ti_ext_rom;
UBYTE *ti_ram;
UBYTE *ti_io;
UBYTE garbage_mem[0x10000];

UBYTE *mem_tab[8] = 
{
  0, // 000000-1FFFFF : RAM
  0, // 200000-3FFFFF : internal ROM (TI89)
  0, // 400000-5FFFFF : external ROM (TI92/TI92+)
  0, // 600000-6FFFFF : Memory mapped I/O (lower 6 bits)
  0, // 
  0,
  0,
  0
}; 
ULONG mem_mask[8] = {0,0,0,0,0,0,0,0};

int rom_changed[32]; // FLASH segments which have been (re)programmed

UBYTE* mem[256];

int memprotect;
int ram128;
int mem_and;
int ram_wrap;

int flash_protect;
int rom_ret_or;
int rom_write_ready;
int rom_write_phase;
int rom_erase;
int rom_erasePhase;

static IMG_INFO *cri = &current_img_info; // a shortcut

/* Mem init/exit */

static int mem_initialized = 0;

int hw_init_mem()
{
  int i;

  if(mem_initialized) 
    hw_exit_mem();

  /* Init vars */
  ram128 = (params.ram_size == 128);
  memprotect=0;
  ram_wrap=0; 
  mem_and=0x1ffff;
  
  rom_write_ready=0; 
  rom_write_phase=0; 
  rom_ret_or=0;
  rom_erase=0;
  rom_write_ready = 0;
  rom_write_phase = 0;
  rom_ret_or = 0;
  flash_protect = 0;

  mem_initialized = !0;

  /* Initialize bkpts */
  nBkptAsRB = nBkptAsRW = nBkptAsRL = 0;
  nBkptAsWB = nBkptAsWW = nBkptAsWL = 0;
  nBkptAsRgR = nBkptAsRgW = 0;
  listBkptAsRB = listBkptAsRW = listBkptAsRL = NULL;
  listBkptAsWB = listBkptAsWW = listBkptAsWL = NULL;
  listBkptAsRgW = listBkptAsRgR = NULL;

  /* Allocate memory */  
  ti_ram     = malloc(RAM_SIZE+4);
  ti_int_rom = malloc(ROM_SIZE+4);
  ti_ext_rom = malloc(ROM_SIZE+4);
  ti_io      = malloc(IO_SIZE+4);

  /* Clear RAM/ROM/IO */
  memset(ti_ram, 0x00, RAM_SIZE);
  memset(ti_io , 0x00, IO_SIZE);  
  for (i=0; i<2048*1024; i++)
    {
      if (i&1) { 
	ti_int_rom[i]=0x00; 
	ti_ext_rom[i]=0x00; 
      }      
      else { 
	ti_int_rom[i]=0x14; 
	ti_ext_rom[i]=0x14; 
      }
    }
  for (i=0;i<0x10000;i++) 
    garbage_mem[i]=0x14;

  /* Set all banks to RAM (with mask 0 per default) */
  for(i=0; i<8; i++) { 
    mem_tab[i] = ti_ram; 
  }

  /* Map RAM */
  mem_tab[0] = ti_ram;
  mem_mask[0] = RAM_SIZE-1;

  /* Map ROM in two places */
  mem_tab[1] = ti_int_rom;
  mem_mask[1] = ROM_SIZE-1;
  mem_tab[2] = ti_ext_rom;
  mem_mask[2] = ROM_SIZE-1;

  /* Map IO */
  mem_tab[3] = ti_io;
  mem_mask[3] = IO_SIZE-1;

  /* Map garbage mem */
  mem_tab[4] = garbage_mem;
  mem_mask[4] = 0x10000-1;

  if(cri->internal)
    ti_rom = ti_int_rom;
  else
    ti_rom = ti_ext_rom;
  
  // blit ROM
  memcpy(ti_rom, cri->content, cri->data_size);
  free(cri->content);

  return (ti_ram && ti_int_rom && ti_ext_rom && ti_io);
}

int hw_exit_mem()
{
  if(ti_ram) 
    free(ti_ram); 
  ti_ram=NULL;
  
  if(ti_int_rom) 
    free(ti_int_rom); 
  ti_int_rom=NULL;
  
  if(ti_ext_rom) 
    free(ti_ext_rom); 
  ti_ext_rom=NULL;
  
  if(ti_io)  
    free(ti_io);  
  ti_io=NULL;
  
  mem_initialized = 0;

  return 0;
}

#define bput(adr, arg) {mem_tab[(adr)>>21][(adr) & mem_mask[(adr)>>21]] = (arg);}
#define wput(adr, arg) {bput((adr), (arg)>>8) ; bput((adr)+1, (arg)&0xff);}
#define lput(adr, arg) {wput((adr), (arg)>>16) ; wput((adr)+2, (arg)&0xffff);}

#define bget(adr) (mem_tab[(adr)>>21][(adr)&mem_mask[(adr)>>21]])
#define wget(adr) ((UWORD)(((UWORD)bget(adr))<<8|bget((adr)+1)))
#define lget(adr) ((ULONG)(((ULONG)wget(adr))<<16|wget((adr)+2)))


ULONG get_long(CPTR adr) 
{
  struct intlist* l;
  
  adr &= 0xFFFFFF;
  if (nBkptAsRL) 
    {
      l=listBkptAsRL;
      breakId=0;
      while (l) 
	{
	  if (l->val==adr) 
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
  if (nBkptAsRgR) 
    {
      l=listBkptAsRgR;
      breakId=0;
      while (l) 
	{
	  if (l->val>=adr && (adr+3)<=l->val2) 
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
  
  if(adr&1) 
    {
      specialflags |= SPCFLAG_ADRERR;
      return 0;
    }
  if (adr>=0x1c0000 && adr<0x200000)
    flash_protect = 1;
  if (flash_protect && adr>=0x210000 && adr<=0x211fff)
    return 0x14141414;
  if (adr<0x200000) 
    return lget(adr);
  if (adr<0x600000) 
    return (lget(adr)|rom_ret_or);
  else 
    return io_get_long(adr&0x1f);
}

UWORD get_word(CPTR adr) 
{
  struct intlist* l;
	
  adr &= 0xFFFFFF;
  if (nBkptAsRW) 
    {
      l=listBkptAsRW;
      breakId = 0;
      while (l) 
	{
	  if (l->val==adr) 
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
  if (nBkptAsRgR) 
    {
      l=listBkptAsRgR;
      breakId=0;
      while (l) 
	{
	  if (l->val>=adr && (adr+1)<=l->val2) 
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
  
  if(adr&1) 
    {
      specialflags |= SPCFLAG_ADRERR;
      return 0;
    }
  if (adr>=0x1c0000 && adr <0x200000)
    flash_protect = 1;
  if (flash_protect && adr>=0x210000 && adr<=0x211fff)
    return 0x1414;
  if (adr<0x200000) 
    return wget(adr);
  if (adr<0x600000) 
    return (wget(adr)|rom_ret_or);
  else 
    return io_get_word(adr&0x1f);
}

UBYTE get_byte(CPTR adr) 
{
  struct intlist* l;
  
  adr &= 0xFFFFFF;
  if (nBkptAsRB) 
    {
      l=listBkptAsRB;
      breakId = 0;
      while (l) {
	if (l->val==adr) 
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
  if (nBkptAsRgR) 
    {
      l=listBkptAsRgR;
      breakId=0;
      while (l) 
	{
	  if (l->val>=adr && adr<=l->val2) 
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
  
  if (adr>=0x1c0000 && adr <0x200000)
    flash_protect = 1;
  if (flash_protect && adr>=0x210000 && adr<=0x211fff)
    return 0x14;
  
  if (adr<0x200000) 
    return bget(adr);
  if (adr<0x600000) 
    return (bget(adr)|rom_ret_or);
  else 
    return io_get_byte(adr&0x1f);
}

void put_long(CPTR adr, ULONG arg) 
{
  struct intlist* l;
  adr &= 0xFFFFFF;
  if (nBkptAsWL) 
    {
      l=listBkptAsWL;
      breakId = 0;
      while (l) 
	{
	  if (l->val==adr) 
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
  if (nBkptAsRgW) 
    {
      l=listBkptAsRgW;
      breakId=0;
      while (l) 
	{
	  if (l->val>=adr && (adr+3)<=l->val2) 
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

  if(adr&1)
    specialflags |= SPCFLAG_ADRERR;
  else 
    {
      if (adr < 0x200000) 
	{
	  if (adr >=0x1c0000)
	    flash_protect=0;
	  lput(adr, arg);
	}
      else if (adr < 0x400000)
	{
	  intRomWriteByte(adr,(arg>>24)&0xff);
          intRomWriteByte(adr+1,(arg>>16)&0xff);
          intRomWriteByte(adr+2,(arg>>8)&0xff);
          intRomWriteByte(adr+3,arg&0xff);
	}
      else if (adr < 0x600000) 
	{
	  extRomWriteByte(adr,(arg>>24)&0xff);
	  extRomWriteByte(adr+1,(arg>>16)&0xff);
	  extRomWriteByte(adr+2,(arg>>8)&0xff);
	  extRomWriteByte(adr+3,arg&0xff);
	}
      else
	io_put_long(adr&0x1f, arg);
    }   
}

void put_word(CPTR adr, UWORD arg) 
{
  struct intlist* l;
	
  adr &= 0xFFFFFF;
  if (nBkptAsWW) 
    {
      l=listBkptAsWW;
      breakId = 0;
      while (l) 
	{
	  if (l->val==adr) 
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
  if (nBkptAsRgW) 
    {
      l=listBkptAsRgW;
      breakId=0;
      while (l) 
	{
	  if (l->val>=adr && (adr+1)<=l->val2) 
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
  
  if(adr&1)
    specialflags |= SPCFLAG_ADRERR;
  else {
    if (adr < 0x200000) 
      {
	if (adr >=0x1c0000)
	  flash_protect=0;
	wput(adr, arg);
      }
    else if (adr < 0x400000)
      {
	intRomWriteByte(adr,(arg>>8)&0xff);
        intRomWriteByte(adr+1,arg&0xff);
      }
    else if (adr < 0x600000) 
      {
	extRomWriteByte(adr,(arg>>8)&0xff);
	extRomWriteByte(adr+1,arg&0xff);
      }
    else
      io_put_word(adr&0x1f, arg);
  }  
}

void put_byte(CPTR adr, UBYTE arg) 
{
  struct intlist* l;
	
  adr &= 0xFFFFFF;
  
  if (nBkptAsWB) 
    {
      l=listBkptAsWB;
      breakId = 0;
      while (l) 
	{
	  if (l->val==adr) 
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
  if (nBkptAsRgW) 
    {
      l=listBkptAsRgW;
      breakId=0;
      while (l) 
	{
	  if (l->val>=adr && adr<=l->val2) 
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
  
  if (adr < 0x200000) 
    {
      if (adr >=0x1c0000)
	flash_protect=0;
      bput(adr, arg);
    }
  else if (adr < 0x400000)
    intRomWriteByte(adr,arg&0xff);
  else if (adr < 0x600000)
    extRomWriteByte(adr,arg&0xff);
  else
    io_put_byte(adr&0x1f, arg);
}

UBYTE *get_real_address(CPTR adr) 
{
  return &mem_tab[(adr>>21)&0x7][adr&mem_mask[(adr>>21)&0x7]];
}

int valid_address(CPTR adr, ULONG size) 
{
  return 1;
}

void intRomWriteByte(int addr,int v)
{
  UBYTE *intRom = mem_tab[2];
  int i;

  addr &= 0x1fffff;
  
  //if (getCalcType() != TI89) return;
  if(cri->calc_type != TI89)
    return;

  if (rom_write_ready)
    {
      if ((intRom[addr]==0xff)||(rom_write_ready==1))
        {
	  intRom[addr]=v;
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
	    intRom[(addr&0x1f0000)+i]=0xff;
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

void extRomWriteByte(int addr,int v)
{
  UBYTE *extRom = mem_tab[1];
  int i;
  
  addr &= 0x1fffff;
  
  if(flash_protect) 
    return;
  //if (getCalcType() != (TI92 | MODULEPLUS)) return;
  if(cri->calc_type != (TI92 | MODULEPLUS)) 
    return;

  if (rom_write_ready)
    {
      if ((extRom[addr]==0xff)||(rom_write_ready==1))
	{
	  extRom[addr]=v;
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
	    extRom[(addr&0x1f0000)+i]=0xff;
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
  Find the PC reset vector in ROM image/FLASH upgrade.
  If we have a FLASH upgrade, we copy the vector table into RAM.
  This is 
*/
int find_pc()
{
  int i;
  int vt = 0x000000; // vector table

  // wrap ROM
  /*
  if(cri->size < (2048*1024))
    {
      for (i=0;i<0x100000;i++)
	ti_int_rom[i+0x100000] = ti_int_rom[i];
	}*/

  // find PC reset vector
  if(cri->flash)
    { // TI89 or TI92+
      for (vt = 0x12000; vt<cri->data_size; vt++)
	{
	  if (*((int*)(ti_rom+vt)) == 0xcccccccc) {
	    vt += 4;
	    break;
	  }
	}
      vt += 4; // skip SP
   
      uae_initial_pc = ti_rom[vt+3] | (ti_rom[vt+2]<<8) |
        (ti_rom[vt+1]<<16) | (ti_rom[vt]<<24);
    }
  else
    { // TI92
      vt = 0;
      vt += 4; // skip SP
      
      uae_initial_pc = ti_rom[vt+3] | (ti_rom[vt+2]<<8) |
        (ti_rom[vt+1]<<16) | (ti_rom[vt]<<24);
    }

  // copy vector table into RAM for boot
  for (i=0; i<256; i++) {
    ti_ram[i] = ti_rom[vt + i];
  }  

  return 0;
}
