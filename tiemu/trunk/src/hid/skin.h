/* Hey EMACS -*- linux-c -*- */
/* $Id: cabl_int.h 651 2004-04-25 15:22:07Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Originally written by Jonas Minsberg
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (C) 2001-2004, Romain Lievin
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
   Most of these definitions and code comes from the JB's SkinEdit
   which is based on TiEmu skin code. TiEmu skin code is also based on
   VTi's skin code.
*/

#ifndef __SKINLOADER_H__
#define __SKINLOADER_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>


/***************/
/* Definitions */
/***************/

#define LCD_COLORTYPE_LOW    0
#define LCD_COLORTYPE_HIGH   1
#define LCD_COLORTYPE_CUSTOM 2

#define LCD_HI_WHITE 0xb0ccae
#define LCD_HI_BLACK 0x8a6f53

#define LCD_LOW_WHITE 0xcfe0cc
#define LCD_LOW_BLACK 0x222e31

#define MAX_COLORS (256 - 16)		// we need to keep 16 colors for grayscales
#define SKIN_KEYS  80

#define SKIN_TI73  "TI-73"
#define SKIN_TI82  "TI-82"
#define SKIN_TI83  "TI-83"
#define SKIN_TI83P "TI-83+"
#define SKIN_TI85  "TI-85"
#define SKIN_TI86  "TI-86"
#define SKIN_TI89  "TI-89"
#define SKIN_TI92  "TI-92"
#define SKIN_TI92P "TI-92+"

#define SKIN_TYPE_TIEMU   10
#define SKIN_TYPE_VTI     2
#define SKIN_TYPE_OLD_VTI 1
#define SKIN_TYPE_NEW     0

#define ENDIANNESS_FLAG 0xfeedbabe
#define TIEMU_SKIN_ID "TiEmu v2.00"

/*********/
/* Types */
/*********/

#ifndef __WIN32__	// already defined by Windows
typedef struct
{
  uint32_t left;
  uint32_t top;
  uint32_t right;
  uint32_t bottom;
} RECT;
#endif

typedef struct
{
  int type;

  unsigned char *img;
  unsigned char cmap[3][MAX_COLORS];
  int ncolors;

  unsigned int width;
  unsigned int height;

  unsigned char calc[9];
  uint32_t colortype;

  uint32_t lcd_black;
  uint32_t lcd_white;

  unsigned char *name;
  unsigned char *author;

  RECT lcd_pos;
  RECT keys_pos[SKIN_KEYS];

} SKIN_INFOS;


extern SKIN_INFOS skin_infos;

/*************/
/* Functions */
/*************/

int skin_load(const char *filename);
int skin_unload(void);

int skin_read_header(const char* filename, SKIN_INFOS* infos);
int skin_read_imager(const char* filename, SKIN_INFOS* infos);


#endif
