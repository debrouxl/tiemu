/* Hey EMACS -*- linux-c -*- */
/* $Id: images.h 680 2004-10-11 16:45:27Z roms $ */

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

#ifndef __TI68K_HWPM__
#define __TI68K_HWPM__

#include <stdint.h>

/*
  Definitions
*/

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

// Possible values if hardwareID field
#define HWID_TI92P  1
#define HWID_TI89   3
#define HWID_V200   8
#define HWID_TI89T  9

/*
	Functions
*/

void ti68k_display_hw_param_block(HW_PARM_BLOCK *s);
int ti68k_get_hw_param_block(uint8_t *rom_data, uint8_t rom_base, HW_PARM_BLOCK *block);
int ti68k_put_hw_param_block(uint8_t *rom_data, uint8_t rom_base, const HW_PARM_BLOCK *s);

#endif
