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

	Many ideas from K. Kofler (as exposure time and opcode signature).
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
#define UPDATE_PLANES	16	// update plane addresses every 16 LCD refresh

uint32_t lcd_planes[3];
int ngc = 1;

/*
	Grayscale management (common)
*/
static void process_address(uint32_t plane_addr)
{
	uint32_t tmp;
	static uint32_t lcd_addrs[BUFSIZE];
	static int		lcd_ticks[BUFSIZE];
	static int		lcd_exptime[3];
	static int cnt;

	lcd_addrs[cnt % BUFSIZE] = plane_addr;
	lcd_ticks[cnt++ % BUFSIZE] = tihw.lcd_tick;
	tihw.lcd_tick = 0;

	if(!(cnt % UPDATE_PLANES))
	{
		int ngp = 0;
		int i;

		// reset exposure times
		memset(lcd_exptime, 0, sizeof(lcd_exptime));

		// get address of plane #0
		lcd_planes[0] = lcd_planes[1] = lcd_planes[2] = lcd_addrs[0];		
		ngp++;

		// get address of plane #1
		for(i = 0; i < BUFSIZE; i++)
		{
			if(lcd_addrs[i] != lcd_planes[0])
			{
				lcd_planes[1] = lcd_addrs[i];
				ngp++;
				break;
			}
		}

		// get address of plane #2
		for(i = 0; i < BUFSIZE; i++)
		{
			if((lcd_addrs[i] != lcd_planes[0]) && (lcd_addrs[i] != lcd_planes[1]))
			{
				lcd_planes[2] = lcd_addrs[i];
				ngp++;
				break;
			}
		}

		// get exposure time of plane #0
		for(i = 0; i < BUFSIZE; i++)
		{
			if(lcd_addrs[i] == lcd_planes[0])
				lcd_exptime[0] += lcd_ticks[i];
		}

		// get exposure time of plane #1
		for(i = 0; i < BUFSIZE; i++)
		{
			if(lcd_addrs[i] == lcd_planes[1])
				lcd_exptime[1] += lcd_ticks[i];
		}

		// get exposure time of plane #2
		for(i = 0; (i < BUFSIZE) && (ngp > 2); i++)
		{
			if(lcd_addrs[i] == lcd_planes[2])
				lcd_exptime[2] += lcd_ticks[i];
		}

		// sort plane addresses by exposition times (0,1), (0,2), (1,2)
		// note: sorting does not works fine and I don't see where the error is...
		if(lcd_exptime[0] < lcd_exptime[1])
		{
			tmp = lcd_planes[1];
			lcd_planes[1] = lcd_planes[0];
			lcd_planes[0] = tmp;
		}
		if(lcd_exptime[0] < lcd_exptime[2])
		{
			tmp = lcd_planes[2];
			lcd_planes[2] = lcd_planes[0];
			lcd_planes[0] = tmp;
		}
		if(lcd_exptime[1] < lcd_exptime[2])
		{
			tmp = lcd_planes[2];
			lcd_planes[2] = lcd_planes[1];
			lcd_planes[1] = tmp;
		}

		// now, determine number of grayscales (kevin)
		if(ngp == 1)
			ngc = 1;
		else if(ngp == 2)
			ngc = 3;
		else if(ngp == 3)
		{
			// check using fast integer computation and no possible divisions by 0
			// whether lcd_exptime[0] / lcd_exptime[2] > 3.5 = 7/2
			if (lcd_exptime[0] * 2 > lcd_exptime[2] * 7)
				ngc = 8;
			else
				ngc = 7;
		}

#if 0
		printf("%i: %06x-%06x-%06x (%i-%i-%i)\n", ngp,
				lcd_planes[0], lcd_planes[1], lcd_planes[2],
				lcd_exptime[0]>>4, lcd_exptime[1]>>4, lcd_exptime[2]>>4
				);
#endif
	}
}

/*
	HW1 grayscale management
*/
void lcd_hook_hw1(void)
{
	process_address(tihw.lcd_adr);
}

/*
	HW2 grayscale management
*/
void lcd_hook_hw2(int refresh)
{
	static int dead_cnt = 0;
	static int fs_toggled = 0;
#pragma warning( push )
#pragma warning( disable : 4305 )
	static const char moveml_a0p_d0d7a2a6_moveml_d0d7a2a6_a1[8] = {0x4c,0xd8,0x7c,0xff,0x48,0xd1,0x7c,0xff};
#pragma warning( pop ) 

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
	if(fs_toggled)
	{
		UWORD opcode = curriword();

		if(!memcmp(regs.pc_p, moveml_a0p_d0d7a2a6_moveml_d0d7a2a6_a1, 8))
		//if(opcode == 0x4cd8)
		{
			uint32_t a0 = regs.a[0]-0xa00;
			uint32_t a1 = regs.a[1]-0xa00;

			if(a1 == 0x4c0c)
				a0 -= 12;
			else if(a1 != 0x4c00)
				return;

			//printf("$%06x\n", m68k_getpc());
			//printf("%06x %06x\n", regs.a[0], regs.a[1]);
			//process_address_2(a0);

			//printf("%i\n", tihw.lcd_tick);
			process_address(a0);
			
			fs_toggled = 0;
			dead_cnt = 0;
		}
	}
}

// opcode signature:
	// TIGGL lib : 
	//		movem.l  (%a0)+,%d0-%d7/%a2-%a6 ; (%a1)==0x4c00, alors plan:=(%a0)
	// graphlib-titanik et graphlib-iceberg : 
	//		movem.l  (%a0)+,%d0-%d7/%a2-%a6 ; (%a1)==0x4c00+12, alors plan:=-12(%a0)