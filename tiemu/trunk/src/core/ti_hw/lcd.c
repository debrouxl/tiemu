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
	// Set contrast
	if(tihw.calc_type == TI92)
		tihw.contrast = 13;
	else if((tihw.calc_type == TI89) && (tihw.hw_type == 2))
		tihw.contrast = 3;
    tihw.lcd_off = 0;

	// On 92 with ROM 1.x, the memory used by the system is at address $4440 and
	// on ROM 2.x, this address is $4720.
	// On TI89/92+ , this address is 4c00;
	if(tihw.rom_flash)
		tihw.lcd_addr = 0x4c00;
	else if(strcmp(tihw.rom_version, "2.0") > 0)
		tihw.lcd_addr = 0x4720;
	else
		tihw.lcd_addr = 0x4440;

	// for use by HID
	tihw.lcd_ptr = &tihw.ram[tihw.lcd_addr];
    
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
