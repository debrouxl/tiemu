/* Hey EMACS -*- linux-c -*- */
/* $Id: romcalls.h 864 2005-02-22 09:54:05Z roms $ */

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

#ifndef __TIMEM__
#define __TIMEM__

/*
	Functions
*/

uint16_t rd_word(uint8_t *p);
uint32_t rd_long(uint8_t *p);

void wr_word(uint8_t *p, uint16_t d);
void wr_long(uint8_t *p, uint32_t d);

uint8_t  mem_rd_byte(uint32_t a);
uint16_t mem_rd_word(uint32_t a);
uint32_t mem_rd_long(uint32_t a);

void mem_wr_byte(uint32_t a, uint8_t  d);
void mem_wr_word(uint32_t a, uint16_t d);
void mem_wr_long(uint32_t a, uint32_t d);

#endif
