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
    Gray-scale detection and management (many ideas from K. Kofler).
	Can manages 1 (b&w), 2 (4 colors) or 3 (7/8 colors) planes

	The HW1 part could be greatly optimized.

	The current algorithm are simple but fast (no time ratio detection).
	This will be improved later...
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

#define BUFSIZE			12	// store 12 plane addresses
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

/*
	HW1 grayscale management
*/
void lcd_hook_hw1(void)
{
	static uint32_t lcd_addrs[BUFSIZE];
	static int cnt;
	static int t;
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

#if 1
		printf("%06x-%06x-%06x\n", lcd_planes[0], lcd_planes[1], lcd_planes[2]);
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

static void process_address(uint32_t plane_addr)
{
	static uint32_t min=0;
	static uint32_t max=0;
	static uint32_t mid=0;
	uint32_t tmp;
	static int cnt;
	const int ngp2ngc[] = { 1, 1, 3, 7 }; 

	tmp = plane_addr;

	if(tmp < min)
		min = tmp;
	if((tmp > min) && (tmp < max))
		mid = tmp;
	if(tmp > max)
		max = tmp;

	if(!(cnt++ % 8))
	{
		int ngp = 1;

		lcd_planes[0] = min;
		lcd_planes[1] = mid;
		lcd_planes[2] = max;

		if(min != mid)
			ngp++;
		if(mid != max)
			ngp++;

		ngc = ngp2ngc[ngp];
		//printf("$%06x: %06x, %06x-%06x-%06x  %i\n", m68k_getpc(), tmp+0xa00, min+0xa00, mid+0xa00, max+0xa00, ngp);
		min = max = mid = tmp;
	}
}

/*
	HW2 grayscale management
*/
void lcd_hook_hw2(int refresh)
{
	static int dead_cnt = 0;
	static int fs_toggled = 0;

	// if refresh from GTK (calc.c), set 1 plane
	if(refresh)
	{
		fs_toggled = !0;

		if(++dead_cnt < 5)
			return;

		lcd_planes[0] = tihw.lcd_adr;
		ngc = 1;
		return;
	}	

	// if refresh from CPU loop (m68k.c), search for opcode signature:
	// TIGGL lib : 
	//		movem.l  (%a0)+,%d0-%d7/%a2-%a6 ; (%a1)==0x4c00, alors plan:=(%a0)
	// graphlib-titanik et graphlib-iceberg : 
	//		movem.l  (%a0)+,%d0-%d7/%a2-%a6 ; (%a1)==0x4c00+12, alors plan:=-12(%a0)
	if(fs_toggled)
	{
		UWORD opcode = curriword();

		if(opcode == 0x4cd8)
		{
			uint32_t a0 = regs.a[0]-0xa00;
			uint32_t a1 = regs.a[1]-0xa00;

			if(a1 == 0x4c0c)
				a0 -= 12;
			else if(a1 != 0x4c00)
				return;
			//printf("$%06x\n", m68k_getpc());
			//printf("%06x %06x\n", regs.a[0], regs.a[1]);

			process_address(a0);
			fs_toggled = 0;
			dead_cnt = 0;
		}
	}
}