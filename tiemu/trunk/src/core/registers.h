/* Hey EMACS -*- linux-c -*- */
/* $Id: interface.h 224 2004-05-19 14:04:04Z roms $ */

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

#ifndef __TI68K_REGS__
#define __TI68K_REGS__

typedef enum {
    REG_D0, REG_D1, REG_D2, REG_D3,
    REG_D4, REG_D5, REG_D6, REG_D7,
} Ti68kDataRegister;

typedef enum {
    REG_A0, REG_A1, REG_A2, REG_A3,
    REG_A4, REG_A5, REG_A6, REG_A7,
} Ti68kAddrRegister;

void ti68k_register_set_data(int n,int val);
void ti68k_register_set_addr(int n,int val);
void ti68k_register_set_sp(int val);
void ti68k_register_set_pc(int val);
void ti68k_register_set_sr(int val);
void ti68k_register_set_flag(unsigned char flag);

int ti68k_register_get_data(int n);
int ti68k_register_get_addr(int n);
int ti68k_register_get_sp(void);
int ti68k_register_get_pc(void);
int ti68k_register_get_sr(void);
char *ti68k_register_get_flag(void);

#endif
