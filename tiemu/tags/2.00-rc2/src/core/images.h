/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __TI68K_IMAGES__
#define __TI68K_IMAGES__


/*
  Definitions
*/

#define IMG_REV 2           // increase this number when changing the structure below

// dc = don't care for rom/tib
typedef struct
{
	char	signature[16];	// "TiEmu v2.00" (dc)
    long    revision;       // structure revision (compatibility)
	long	header_size;	// size of this structure and offset to pure data (dc)

	char	calc_type;		// calculator type
	char	version[5];		// firmware revision
	char	flash;			// EPROM or FLASH
	char	has_boot;		// FLASH upgrade does not have boot
	long	size;			// size of pure data
	char	hw_type;		// hw1 or hw2
    char    rom_base;       // ROM base address (MSB)

    char    fill[0x40-42];  // round up struct to 0x40 bytes
	char*	data;			// pure data (temporary use, 8 bytes)
} IMG_INFO;

extern int		img_loaded;
extern IMG_INFO img_infos;

/*
	Functions
*/

int ti68k_is_a_rom_file(const char *filename);
int ti68k_is_a_tib_file(const char *filename);
int ti68k_is_a_img_file(const char *filename);

void ti68k_display_rom_infos(IMG_INFO *rom);
void ti68k_display_tib_infos(IMG_INFO *tib);
void ti68k_display_img_infos(IMG_INFO *img);

int ti68k_get_rom_infos(const char *filename, IMG_INFO *rom, int preload);
int ti68k_get_tib_infos(const char *filename, IMG_INFO *tib, int preload);
int ti68k_get_img_infos(const char *filename, IMG_INFO *img);

int ti68k_convert_rom_to_image(const char *src, const char *dirname, char **dst);
int ti68k_convert_tib_to_image(const char *src, const char *dirname, char **dst);
int ti68k_merge_rom_and_tib_to_image(const char *srcname1, const char *srcname2, 
                                     const char *dirname, char **dstname);

int ti68k_load_image(const char *filename);
int ti68k_load_upgrade(const char *filename);
int ti68k_unload_image_or_upgrade(void);

int ti68k_scan_files(const char *src_dir, const char *dst_dir, int erase);
int ti68k_scan_images(const char *dirname, const char *filename);

int ti68k_find_image(const char *dirname, char **filename);

#endif
