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


// Previous state to detect change
static uint32_t old_d[8];
static uint32_t old_a[8];
static uint32_t old_sp;
static uint32_t old_pc;
static uint16_t old_sr;
static char old_sf[32];
static char old_uf[32];

void ti68k_register_set_data(int n, uint32_t val)
{
    if (n>=0 && n<8) regs.d[n] = val;
}

void ti68k_register_set_addr(int n, uint32_t val)
{
    if (n>=0 && n<8) regs.a[n] = val;
}

void ti68k_register_set_sp(uint32_t val)
{
    regs.usp = val;
}

void ti68k_register_set_pc(uint32_t val)
{
    m68k_setpc(val);
}

void ti68k_register_set_sr(uint32_t val)
{
    regs.sr = (int)val;
}

void ti68k_register_set_flag(uint8_t flag)
{
  	//TODO
  	/* T  0  S  0  0  I2 I1 I0 0  0  0  X  N  Z  V  C */	  
}

int ti68k_register_set_flags(const char *sf, const char *uf)
{
	/* SR: T 0 S 0 0 I2 I1 I0 0 0 0 X N Z V C */
	int t, s, i, x, n, z, v, c;
	int nargs;
	
	if(sf != NULL)
	{
		nargs = sscanf(sf, "T=%d S=%d I=%d", &t, &s, &i);
		
		if(nargs < 3)
			return 0;
		if((i < 0) || (i > 7))
			return 0;

		regs.t = t;
		regs.s = s;
		regs.intmask = i;
	}

	if(uf != NULL)
	{
		nargs = sscanf(uf, "X=%d N=%d \nZ=%d V=%d C=%d", &x, &n, &z, &v, &c);

		if(nargs < 5)
			return 0;

		regs.x = x;
		NFLG = n;
		ZFLG = z;
		VFLG = v;
		CFLG = c;
	}

	return !0;
}

int ti68k_register_get_data(int n, uint32_t *val)
{
	int c = 0;
	
    if (n>=0 && n<8)
    	*val = regs.d[n];
    	
    if(regs.d[n] != old_d[n])
    	c = !0;
    	
    old_d[n] = regs.d[n];
    return c;
}

int ti68k_register_get_addr(int n, uint32_t *val)
{
	int c = 0;

    if (n>=0 && n<8) 
    	*val = regs.a[n];
    
    if(regs.a[n] != old_a[n])
    	c = !0;
    	
    old_a[n] = regs.a[n];
    return c;
}
	
int ti68k_register_get_sp(uint32_t *val)
{
	int c = 0;
	
	*val = regs.usp;
	if(regs.usp != old_sp)
		c = !0;

	old_sp = regs.usp;
    return c;
}

int ti68k_register_get_pc(uint32_t *val)
{
	int c = 0;

    *val = m68k_getpc();
	if(*val != old_pc)
		c = !0;

	old_pc = regs.pc;
	return c;
}

int ti68k_register_get_sr(uint32_t *val)
{
	int c =0;

    *val = regs.sr;
	if(regs.sr != old_sr)
		c = !0;

	old_sr = regs.sr;
	return c;
}

const char *ti68k_register_get_flag(void)
{
    static char str[64];

	/* T  0  S  0  0  I2 I1 I0 0  0  0  X  N  Z  V  C */	  
    printf("T=%d S=%d I=%d | X=%d N=%d\nZ=%d V=%d C=%d\n",
	  regs.t, regs.s, regs.intmask,
	  regs.x, NFLG, ZFLG, VFLG, CFLG);

    return str;
}

int ti68k_register_get_flags(char *sf, char *uf)
{
	int c =0;

	/* SR: T 0 S 0 0 I2 I1 I0 0 0 0 X N Z V C */	  
    sprintf(sf, "T=%d S=%d I=%d", regs.t, regs.s, regs.intmask);
	sprintf(uf, "X=%d N=%d \nZ=%d V=%d C=%d", regs.x, NFLG, ZFLG, VFLG, CFLG);	 //%dSPC\n: SPC is important !

	if(strcmp(sf, old_sf) || strcmp(uf, old_uf))
		c = !0;

	strcpy(old_sf, sf);
	strcpy(old_uf, uf);
	return c;
}
