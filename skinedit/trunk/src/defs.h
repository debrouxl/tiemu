/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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


#ifndef __DEFS_H__
#define __DEFS_H__

#ifdef __MACOSX__
#define _(x) x
#endif /* __MACOSX__ */

#if defined(__MACOSX__) || defined(__WIN32__)
#define bswap_32(a) (a >> 24) | ((a & 0xff0000) >> 16) << 8 | ((a & 0xff00) >> 8) << 16 | (a & 0xff) << 8
#endif /* __MACOSX__ || __WIN32__ */

#define ACTION_NONE 0
#define ACTION_OPEN 1
#define ACTION_NEW 2
#define ACTION_EXIT 3

#define ACTION_LCD_WHITE 10
#define ACTION_LCD_BLACK 20

#define DEFINE_LCD 10
#define DEFINE_KEYS 20

#define ACTION_LCD_COORDS 100

#define EXPORT_VTI_2_1 21
#define EXPORT_VTI_2_5 25

#define LCD_COLORTYPE_HIGH 1
#define LCD_COLORTYPE_LOW 0
#define LCD_COLORTYPE_CUSTOM 2

#define LCD_HI_WHITE 0xb0ccae
#define LCD_HI_BLACK 0x8a6f53

#define LCD_LOW_WHITE 0xcfe0cc
#define LCD_LOW_BLACK 0x222e31

#define SKIN_KEYS 80

#define CALC_TI73	"TI-73"
#define CALC_TI82	"TI-82"
#define CALC_TI83	"TI-83"
#define CALC_TI83P	"TI-83+"
#define CALC_TI84P	"TI-84+"
#define CALC_TI85	"TI-85"
#define CALC_TI86	"TI-86"
#define CALC_TI89	"TI-89"
#define CALC_TI89T	"TI-89TM"
#define CALC_TI92	"TI-92"
#define CALC_TI92P	"TI-92+"
#define CALC_V200	"V200PLT"

#define SKIN_TYPE_TIEMU   10
#define SKIN_TYPE_VTI     2
#define SKIN_TYPE_OLD_VTI 1
#define SKIN_TYPE_NEW     0

#define ENDIANNESS_FLAG 0xfeedbabe
#define TIEMU_SKIN_ID "TiEmu v2.00"


#endif /* !__DEFS_H__ */
