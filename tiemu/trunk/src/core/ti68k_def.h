/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 225 2004-05-19 14:30:25Z roms $ */

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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef __TI68K_DEFS__
#define __TI68K_DEFS__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "tilibs.h"

/* Equivalences */

#ifdef __WIN32__
# define strcasecmp _stricmp
#endif

/* Constants */

#define MAXCHARS	256

#define TI92 		1
#define TI89 		2
#define MODULEPLUS 	4
#define TI92p 		(TI92 | MODULEPLUS)
#define V200		8
  
#define INTERNAL 	1
#define FLASH_ROM 	2

/* Structures */

typedef struct 
{
  char *rom_file;
  char *ram_file;
  char *tib_file;

  int background;
  int grayplanes;

  int tick_rate;
  int cycle_rate;
  int i_tick;
  int sync_one;
  int restrict;
} Ti68kParameters;

typedef struct
{
    // misc
    int     calc_type;

    int     ram_size;
    int     rom_size;
    int     rom_internal;
    int     rom_flash;
    char	rom_version[5];

    // ioports.c
    int     io0Bit7;
    int     io0Bit2;

    // keyboard.c
    int     on_key0;

    // lcd.c
    int     lcd_off;
    int     contrast;
    int     lcd_base_addr;
    char*   lcd_ptr;

    // linkport.c
    TicableLinkParam lc;

    // memory.c
    char    *int_rom;
    char    *ext_rom;
    char    *rom;
    char    *ram;
    char    *io;
    char    *garbage;

    // timer.c
    int     timer_value;
    int     timer_init;

} Ti68kHardware;

extern Ti68kParameters 	params;
extern TicableLinkParam link_cable;
extern Ti68kHardware 	tihw;

#endif
