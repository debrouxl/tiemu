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

#ifndef __WIN32__
# include <stdint.h>
#else
# include <windows.h>
typedef DWORD uint32_t;
#endif

#include "./defs.h"


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

#define MAX_COLORS 128		// we need to keep 16 colors for grayscales
#define SKIN_KEYS  80

typedef struct
{
  int type;

  unsigned char *img;
  unsigned char cmap[3][MAX_COLORS];
  int ncolors;

  unsigned int width;
  unsigned int height;

  unsigned char calc[8];
  uint32_t colortype;

  uint32_t lcd_black;
  uint32_t lcd_white;

  unsigned char *name;
  unsigned char *author;

  RECT lcd_pos;
  RECT keys_pos[SKIN_KEYS];

} skinInfos;


extern skinInfos skin;

/*************/
/* Functions */
/*************/

int skin_load(const char *filename);
int skin_unload(void);

int skin_read_header(const char*filename, skinInfos* infos);


#endif
