/* Hey EMACS -*- linux-c -*- */
/* $Id: hw.c 725 2004-11-24 15:59:33Z roms $ */

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
    Gray-scale detection and management (idea suggested by K. Kofler).
	Can manages 1 (b&w), 2 (4 colors) or 3 (7/8 colors) planes

			!!! Could be greatly optimized !!!
*/


#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>

#include "libuae.h"
#include "mem.h"
#include "ports.h"
#include "dbus.h"
#include "kbd.h"
#include "m68k.h"
#include "images.h"
#include "ti68k_def.h"

#define BUFSIZE	8			// store 12 plane addresses
#define UPDATE_PLANES	32	// update plane addresses every 16 LCD refresh

double round(double v)
{
	double f = v - (int)v;
	if(f > 0.5)
		return ceil(v);
	else
		return floor(v);
}

uint32_t lcd_planes[3];
int ngc = 1;

int gp_seq[9][8] = {			// gray plane sequences
	{ -1 },						// unused
	{ 0, -1 },					// b&w		(1 plane)
	{ -1 },						// unused
	{ 0, 0, 1, -1 },			// 4 colors (2 planes)
	{ -1 },						// unused
	{ -1 },						// unused
	{ -1 },						// unused
	{ 2, 0, 1, 0, 1, 0, -1 },	// 7 colors (3 planes)
	{ 1, 0, 2, 0, 0, 1, 0, -1 },// 8 colors (3 planes)
};


extern void lcd_hook(void)
{
	static uint32_t lcd_addrs[BUFSIZE];
	static int t;
	static int cnt;
	static double fir;
	uint32_t tmp;

	lcd_addrs[cnt++ % BUFSIZE] = tihw.lcd_adr;

	if(!(cnt % UPDATE_PLANES))
	{
		int np, i, ngp=1;
		static int old_ngp=1;

		// get address of plane #0
		np = 1;
		lcd_planes[0] = lcd_planes[1] = lcd_planes[2] = lcd_addrs[0];		

		// get address of plane #1
		for(i = 1; i < BUFSIZE; i++)
		{
			if(lcd_addrs[i] != lcd_planes[0])
			{
				lcd_planes[1] = lcd_addrs[i];
				np++;
				break;
			}
		}

		// get address of plane #2
		for(i = 1; i < BUFSIZE; i++)
		{
			if((lcd_addrs[i] != lcd_planes[0]) && (lcd_addrs[i] != lcd_planes[1]))
			{
				lcd_planes[2] = lcd_addrs[i];
				np++;
				break;
			}
		}

#ifdef FIR
		// FIR filter: get number of planes (don't need any more: 
		// I have implemented pending interrupts)
		fir = (np + 1.1*fir) / 2;
		ngp = (int)round(fir);
#else
		ngp = np;
#endif

		// keep plane address in the right order (to test ...)
		if(lcd_planes[0] > lcd_planes[1])
		{
			tmp = lcd_planes[0];
			lcd_planes[0] = lcd_planes[1];
			lcd_planes[1] = tmp;
		}

		if((ngp == 3) && (lcd_planes[1] > lcd_planes[2]))
		{
			tmp = lcd_planes[1];
			lcd_planes[1] = lcd_planes[2];
			lcd_planes[2] = tmp;
		}

#if 0
		//printf("%06x-%06x-%06x\n", lcd_planes[0], lcd_planes[1], lcd_planes[2]);
		//printf("%1.1f/%1.1f %i\n", round(fir), fir, c);				 
		//for(i = 0; i < 8; i++)	printf("%06x ", lcd_addrs[i]); printf("\n");
#endif

		if(old_ngp != ngp)
			printf("Detected %i planes !\n", ngp);
		old_ngp = ngp;

		if(ngp == 1)
			ngc = 1;
		else if(ngp == 2)
			ngc = 3;
		else if(ngp == 3)
			ngc = 7;
	}
}
