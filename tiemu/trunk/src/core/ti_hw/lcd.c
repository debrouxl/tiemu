/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
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
    tihw.contrast = 13;
    tihw.lcd_addr = (tihw.rom_flash) ? 0x4c00 : 0x4440;
    tihw.lcd_ptr = &tihw.ram[tihw.lcd_addr];
    tihw.lcd_off = 0;
    
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
