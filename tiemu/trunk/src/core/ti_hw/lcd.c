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
    LCD management
*/

#include "ti68k_def.h"

int hw_lcd_init(void)
{
	uint16_t lcd_addr;

	// Set contrast
	/*
	if(tihw.calc_type == TI92)
		tihw.contrast = 13;
	else if((tihw.calc_type == TI89) && (tihw.hw_type == 2))
		tihw.contrast = 13;
    tihw.lcd_off = 0;
	*/

	// TI92 ROM 1.x : $4440
	// TI92 ROM 2.x : $4720
	// TI89/92+/V200: $4c00;
	if(tihw.rom_flash)
		lcd_addr = 0x4c00;
	else if(tihw.ti92v2)
		lcd_addr = 0x4720;
	else if(tihw.ti92v1)
		lcd_addr = 0x4440;
	else
	    lcd_addr = 0x4c00;

	lcd_addr >>= 3;

	tihw.io[0x10] = MSB(lcd_addr);
	tihw.io[0x11] = LSB(lcd_addr);

    if(tihw.hw_type == HW2)
		tihw.io2[0x17] = 0x00;

	tihw.lcd_ptr = &tihw.ram[lcd_addr << 3];

    return 0;
}

int hw_lcd_reset(void)
{
    return 0;
}

int hw_lcd_exit(void)
{
    return 0;
}
