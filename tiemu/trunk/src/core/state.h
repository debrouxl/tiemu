/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __TI68K_STATE__
#define __TI68K_STATE__

// Same as IMG_INFO
typedef struct
{
	char	signature[16];	// "TiEmu v2.00"
	long	header_size;	// size of this structure
	long	data_offset;	// offset to pure data

	char	calc_type;		// calculator type
	char	version[5];		// firmware revision
	char	internal;		// internal/external
	char	flash;			// EPROM or FLASH
	char	has_boot;		// FLASH upgrade does not have boot
	long	size;			// size of pure data
	char	hw_type;		// hw1 or hw2

	char*	data;			// pure data (temporary use)
} SAV_INFO;

int ti68k_state_load(char *filename);
int ti68k_state_save(char *filename);

#endif
