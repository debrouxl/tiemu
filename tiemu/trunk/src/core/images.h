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

#ifndef __TI68K_IMAGES__
#define __TI68K_IMAGES__


/*
  Definitions
*/

typedef struct
{
	char	signature[16];	// "TiEmu v2.00"
	long	header_size;	// size of this structure
	long	data_offset;	// offset to pure data
	char	calc_type;		// calculator type
	char	revision[4];	// firmware revision
	char	internal;		// internal/external
	char	flash;			// EPROM or FLASH
	char	img_type;		// FLASH upgrade or ROM image
	char	data_size;		// size of pure data
} IMG_HEADER;

typedef struct
{
    int calc_type;
    char version[20];
    int flash;
    int internal;
	int tib;
    int size;
    char *content;
	int loaded;
} ROM_INFO;

typedef ROM_INFO Ti68kRomInfo;

/*
  Variables
*/

extern int rom_loaded;
extern ROM_INFO current_rom_info;

/*
  Functions
*/

int ti68k_getImageInfo(const char *filename, ROM_INFO *ri);
int ti68k_loadImage(const char *filename);

int ti68k_getUpgradeInfo(const char *filename, ROM_INFO *ri);
int ti68k_loadUpgrade(const char *filename);

int ti68k_getFileInfo(const char *filename, ROM_INFO *ri);

int ti68k_convertTibToRom(const char *f_in, char *f_out);
int ti68k_scanFiles(const char *dirname, const char *filename);

int ti68k_getRomType(void);
int ti68k_dumpRom(const char *filename);


#endif
