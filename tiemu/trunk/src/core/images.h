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

//#include "uae.h"

/*
  Definitions
*/

/* Store informations about TIB files or ROM images */
  struct ti68k_rom_infos_
  {
    int loaded;
    int calc_type;
    char version[20];
    int flash;
    int internal;
    int size;
    int tib;
    unsigned char *content;
  };
typedef struct ti68k_rom_infos_ ROM_INFO;
typedef struct ti68k_rom_infos_ Ti68kRomInfo;

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
