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

#ifndef __TI68K_MEMORY__
#define __TI68K_MEMORY__

/* Functions */

int hw_mem_init(void);
int hw_mem_reset(void);
int hw_mem_exit(void);

extern UBYTE get_byte(CPTR adr);
extern UWORD get_word(CPTR adr);
extern ULONG get_long(CPTR adr);

extern void put_long(CPTR adr, ULONG arg);
extern void put_word(CPTR adr, UWORD arg);
extern void put_byte(CPTR adr, UBYTE arg);

extern UBYTE *get_real_address(CPTR adr);
extern int    valid_address(CPTR adr, ULONG size);

extern void extRomWriteByte(int adr, int v);
extern void intRomWriteByte(int adr, int v);

/* Banks sizes. Must be a power of two */

#define RAM_SIZE  (256 * 1024)
#define ROM_SIZE  (2 * 1024 * 1024)
#define IO_SIZE   (32)

/* Variables */

extern unsigned char /*UBYTE*/ *ti_ram;
extern UBYTE *ti_rom, *ti_int_rom, *ti_ext_rom;
extern unsigned char /*UBYTE*/ *ti_io;

extern UBYTE *mem_tab[8];
extern ULONG mem_mask[8];

extern int memprotect, ram128;
extern int mem_and, ram_wrap;

extern int flash_protect;
extern int rom_ret_or;
extern int rom_write_ready;
extern int rom_write_phase;
extern int rom_erase;
extern int rom_erasePhase;

/* Defines */

#define rom_at_0() { mem_tab[0] = ti_rom; mem_mask[0] = ROM_SIZE-1; }
#define ram_at_0() { mem_tab[0] = ti_ram; mem_mask[0] = RAM_SIZE-1; }

#define LM_BYTE 0
#define LM_WORD 1
#define LM_LONG 2

#define LM_READ 0
#define LM_WRITE 1

#endif
