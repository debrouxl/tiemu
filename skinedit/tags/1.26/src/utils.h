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


#ifndef __UTILS_H__
#define __UTILS_H__


#include <stdint.h>

#if !defined(__WIN32__) && !defined(__MACOSX__)
#include <byteswap.h>
#endif

#ifndef __MACOSX__
#include <glib.h>
#endif


void
set_calc_keymap(void);


void
clear_skin_infos(void);


void
vti_calc_type_to_string(uint32_t calc);


#if (G_BYTE_ORDER == G_BIG_ENDIAN) || defined(__MACOSX__)
void
byteswap_vti_skin(uint32_t *calc);
#endif


#ifndef __MACOSX__
void
sbar_print(char *fmt, ...);


void
set_lcd_color(GtkWidget *area, uint32_t color);
#endif /* !__MACOSX___ */


#endif /* !__UTILS_H__ */
