/* Hey EMACS -*- linux-c -*- */
/* $Id: interface.c 238 2004-05-23 12:10:00Z roms $ */

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
    Registers access/manipulation
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "uae.h"
#include "ti68k_int.h"
#include "ti68k_def.h"


void ti68k_register_set_data(int n, uint32_t val)
{
    if (n>=0 && n<8) regs.d[n] = val;
}

void ti68k_register_set_addr(int n, uint32_t val)
{
    if (n>=0 && n<8) regs.a[n] = val;
}

void ti68k_register_set_usp(uint32_t val)
{
    regs.usp = val;
}

void ti68k_register_set_ssp(uint32_t val)
{
    regs.a[7] = val;
}

void ti68k_register_set_pc(uint32_t val)
{
    regs.pc = val;
}

void ti68k_register_set_sr(uint32_t val)
{
    regs.sr = (int)val;
}

void ti68k_register_set_flag(uint8_t flag)
{
  	//TODO
  	/* T  0  S  0  0  I2 I1 I0 0  0  0  X  N  Z  V  C */	  
  	/*
	sprintf(str, "%s - %s - - %s %s %s - - - %s %s %s %s %s",
		regs.t ? "T " : "0 ",
		regs.s ? "S " : "0 ",
		(regs.intmask & 4) ? "I2" : "0 ",
		(regs.intmask & 2) ? "I1" : "0 ",
		(regs.intmask & 1) ? "I0" : "0 ",
		regs.x ? "X " : "0 ",
		regflags.flags.n ? "N " : "0 ",
		regflags.flags.z ? "Z " : "0 ",
		regflags.flags.v ? "V " : "0 ",
		regflags.flags.c ? "C " : "0 "
		);	
	*/
}

int ti68k_register_get_data(int n)
{
    if (n>=0 && n<8) return regs.d[n];
    return 0;
}

int ti68k_register_get_addr(int n)
{
    if (n>=0 && n<8) return regs.a[n];
    return 0;
}
	
int ti68k_register_get_usp(void)
{
    return regs.usp;
}

int ti68k_register_get_ssp(void)
{
    return regs.a[7];
}

int ti68k_register_get_pc(void)
{
    return regs.pc;
}

int ti68k_register_get_sr(void)
{
    return regs.sr;
}

const char *ti68k_register_get_flag(void)
{
    static char str[64];

    printf("T=%d S=%d X=%d N=%d\nZ=%d V=%d C=%d IMASK=%d\n",
	  regs.t, regs.s, regs.x, NFLG, ZFLG, VFLG, CFLG, regs.intmask);
	  
	/* T  0  S  0  0  I2 I1 I0 0  0  0  X  N  Z  V  C */	  
	sprintf(str, "%s - %s - - %s %s %s - - - %s %s %s %s %s",
		regs.t ? "T " : "0 ",
		regs.s ? "S " : "0 ",
		(regs.intmask & 4) ? "I2" : "0 ",
		(regs.intmask & 2) ? "I1" : "0 ",
		(regs.intmask & 1) ? "I0" : "0 ",
		regs.x ? "X " : "0 ",
		regflags.flags.n ? "N " : "0 ",
		regflags.flags.z ? "Z " : "0 ",
		regflags.flags.v ? "V " : "0 ",
		regflags.flags.c ? "C " : "0 "
		);

    return str;
}
