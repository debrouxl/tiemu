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

#ifndef __TI68K_MEMORY__
#define __TI68K_MEMORY__

#include "stdint.h"

/* Typedefs */

typedef uint8_t  (*GETBYTE_FUNC) (uint32_t);
typedef uint16_t (*GETWORD_FUNC) (uint32_t);
typedef uint32_t (*GETLONG_FUNC) (uint32_t);

typedef void	 (*PUTBYTE_FUNC) (uint32_t, uint8_t );
typedef void	 (*PUTWORD_FUNC) (uint32_t, uint16_t);
typedef void	 (*PUTLONG_FUNC) (uint32_t, uint32_t);

typedef int      (*HWPFETCH_FUNC)(uint32_t);

/* Functions */

int hw_mem_init(void);
int hw_mem_reset(void);
int hw_mem_exit(void);

// defs similar to UAE's memory.h (interface)
extern uint8_t  hw_get_byte(uint32_t addr);
extern uint16_t hw_get_word(uint32_t addr);
extern uint32_t hw_get_long(uint32_t addr);

extern void hw_put_byte(uint32_t addr, uint8_t  arg);
extern void hw_put_word(uint32_t addr, uint16_t arg);
extern void hw_put_long(uint32_t addr, uint32_t arg);

extern uint8_t* hw_get_real_address(uint32_t addr);

/* Variables */

extern uint8_t* mem_tab[];
extern uint32_t mem_msk[];

/* Defines */

#define rom_at_0() { mem_tab[0] = tihw.rom; mem_msk[0] = tihw.rom_size-1; }
#define ram_at_0() { mem_tab[0] = tihw.ram; mem_msk[0] = tihw.ram_size-1; }

/* Put/Get byte/word/longword */

#define bput(adr, arg) { mem_tab[(adr)>>20][(adr) & mem_msk[(adr)>>20]] = (arg); }
#define wput(adr, arg) { bput((adr), (arg)>> 8); bput((adr)+1, (arg)&0x00ff); }
#define lput(adr, arg) { wput((adr), (uint16_t)((arg)>>16)); wput((adr)+2, (uint16_t)((arg)&0xffff)); }

#define bget(adr) (mem_tab[(adr)>>20][(adr)&mem_msk[(adr)>>20]])
#define wget(adr) ((uint16_t)(((uint16_t)bget(adr))<< 8 | bget((adr)+1)))
#define lget(adr) ((uint32_t)(((uint32_t)wget(adr))<<16 | wget((adr)+2)))

#define IN_RANGE(a,v,b)	(((v) >= (a)) && ((v) <= (b)))

/* HW protection */
extern  HWPFETCH_FUNC	hwp_fetch_ptr;

#endif
