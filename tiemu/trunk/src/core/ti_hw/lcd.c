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
  TI screen (LCD) management
*/

#include "uae.h"
//#include "interface.h"
#include "images.h"

int lcd_off;
int contrast;
int lcd_base_addr;

void hw_init_lcd()
{
  contrast=13; //16
  lcd_base_addr=(img_infos.flash)?0x4c00:0x4440;
  lcd_off=0;
}

void hw_exit_lcd()
{
}
