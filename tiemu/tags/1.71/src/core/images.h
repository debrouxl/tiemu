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

// dc = don't care for rom/tib
typedef struct
{
	char	signature[16];	// "TiEmu v2.00" (dc)
	long	header_size;	// size of this structure (dc)
	long	data_offset;	// offset to pure data (dc)

	char	calc_type;		// calculator type
	char	version[5];		// firmware revision
	char	internal;		// internal or external ROM
	char	flash;			// EPROM or FLASH
	char	has_boot;		// FLASH upgrade does not have boot
	long	size;			// size of pure data
	char	hw_type;		// hw1 or hw2

	char*	data;			// pure data (temporary use)
} IMG_INFO;

extern int		img_loaded;
extern IMG_INFO img_infos;

// Hardware parameter block from TIGCC documentation
// Exists only on FLASH calculators
typedef struct {
	unsigned short len;                 /* length of parameter block    */
    unsigned long  hardwareID;          /* 1 = TI-92 Plus, 3 = TI-89    */
    unsigned long  hardwareRevision;    /* hardware revision number     */
    unsigned long  bootMajor;           /* boot code version number     */
    unsigned long  bootRevision;        /* boot code revision number    */
    unsigned long  bootBuild;           /* boot code build number       */
    unsigned long  gateArray;           /* gate array version number    */
    unsigned long  physDisplayBitsWide; /* display width                */
    unsigned long  physDisplayBitsTall; /* display height               */
    unsigned long  LCDBitsWide;         /* visible display width        */
    unsigned long  LCDBitsTall;         /* visible display height       */
} HW_PARM_BLOCK;

/*
	Functions
*/

int ti68k_is_a_rom_file(const char *filename);
int ti68k_is_a_tib_file(const char *filename);
int ti68k_is_a_img_file(const char *filename);

void ti68k_display_rom_infos(IMG_INFO *rom);
void ti68k_display_tib_infos(IMG_INFO *tib);
void ti68k_display_img_infos(IMG_INFO *img);
void ti68k_display_hw_param_block(HW_PARM_BLOCK *block);

int ti68k_get_rom_infos(const char *filename, IMG_INFO *rom, int preload);
int ti68k_get_tib_infos(const char *filename, IMG_INFO *tib, int preload);
int ti68k_get_img_infos(const char *filename, IMG_INFO *img);
int ti68k_get_hw_param_block(IMG_INFO *img, HW_PARM_BLOCK *block);

int ti68k_convert_rom_to_image(const char *src, const char *dirname, char **dst);
int ti68k_convert_tib_to_image(const char *src, const char *dirname, char **dst);
int ti68k_merge_rom_and_tib_to_image(const char *srcname1, const char *srcname2, 
                                     const char *dirname, char **dstname);

int ti68k_load_image(const char *filename);
int ti68k_load_upgrade(const char *filename);

int ti68k_scan_files(const char *src_dir, const char *dst_dir);
int ti68k_scan_images(const char *dirname, const char *filename);

#endif
