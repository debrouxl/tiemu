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
#include "sysdeps.h"	//UBYTE

/* Equivalences */

#ifdef __WIN32__
# define strcasecmp _stricmp
#endif

/* Constants */

#define MAXCHARS	256

#define TI92 		(1 << 0)
#define TI89 		(1 << 1)
#define TI92p	 	(1 << 2)
#define V200		(1 << 3)
//#define TI89TI		(1 << 4)
  
#define EXTERNAL	0
#define INTERNAL 	1

#define EPROM_ROM	0
#define FLASH_ROM 	2

#define KB			(1024)
#define MB			(1024*KB)

#define HW1			1
#define HW2			2

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
    // misc (non hardware)
    int     calc_type;

    int     ram_size;
    int     rom_size;
    int     rom_internal;
    int     rom_flash;
    char	rom_version[5];
	int		hw_type;

	char	*lcd_ptr;

    // ioports.c
    int     io0Bit7;
    int     io0Bit2;

    // keyboard.c
    int     on_key;

    // lcd.c
    int     lcd_off;
    int     contrast;
    int     lcd_addr;
	int		lcd_power;

    // linkport.c
    TicableLinkParam lc;

	int		tx_empty;
	int		rx_full;
	int		dbus_raw;
	int		dbus_auto;
	int		dbus_to;
	int		dbus_err;

    // memory.c
    UBYTE	*rom;
    UBYTE	*ram;
    UBYTE	*io;
    UBYTE   *io2;
    int     initial_pc;

	int		ram256;
	int		ram_wrap;
	int		mem_prot;
    int		flash_prot;

    // timer.c
    int     timer_value;
    int     timer_init;

} Ti68kHardware;

extern Ti68kParameters 	params;
extern TicableLinkParam link_cable;
extern Ti68kHardware 	tihw;

#endif
