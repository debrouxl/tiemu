/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <stdint.h>

#include "defs.h"

struct RECT
{
  uint32_t left;
  uint32_t top;
  uint32_t right;
  uint32_t bottom;
};


struct skinInfos
{
  char *jpeg_path;
  char *skin_path;
  int type;
  int changed;
#ifndef __MACOSX__
  uint16_t *img_orig;
#endif
  unsigned int width;
  unsigned int height;
  unsigned char calc[9];
  unsigned int keymap;
  uint32_t colortype;
  uint32_t lcd_black;
  uint32_t lcd_white;
  unsigned char *name;
  unsigned char *author;
  struct RECT lcd_pos;
  struct RECT keys_pos[SKIN_KEYS];
};


#endif /* !__STRUCT_H__ */
