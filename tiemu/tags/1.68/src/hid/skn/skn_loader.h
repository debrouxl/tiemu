/*  GtkTiEmu - a TI emulator
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
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

#ifndef __SKNLOADER_H__
#define __SKNLOADER_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>

typedef struct tagRECT {
    uint32_t left;		// unsigned long, *32* bits
    uint32_t top;
    uint32_t right;
    uint32_t bottom;
} RECT;

#endif

#ifdef WORDS_BIGENDIAN
uint32_t __inline__
swap_bytes(uint32_t a)
{
    return (a >> 24) | ((a & 0xff0000) >> 16) << 8 | ((a & 0xff00) >> 8) <<
	16 | (a & 0xff) << 8;
}
#endif

#define MAX_COLORS 128		// we need to keep 16 colors for grayscales

typedef struct tagSKIN {
  unsigned char *img;
  unsigned char cmap[3][MAX_COLORS];
  int ncolors;     
  int width;       // skin width
  int height;      // skin height

  longword black;  // True color value for Black pixel
  longword white;  // The same for White pixel
  RECT rcLcd;      // screen coordinates in the skin file
  RECT rcKeys[80]; // coordinates of TI keys
  const char *key_mapping;    // key mapping for skin
} SKIN;

extern SKIN skin;
int load_skin(char *filename);
int unload_skin(void);

#endif
