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

#include <glib.h>

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
#define CALC_MAX    V200
  
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
    char *tib_file;
    char *sav_file;

    int background;
    int grayplanes;

    int restrict;
    int cpu_rate;       // OSC1
    int hw_rate;        // OSC2
    int lcd_rate;       // synched OSC2 (hw1) or OSC3 (hw2)
    
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

	char	*lcd_ptr;		// direct pointer to LCD
	int		lc_file;		// direct file loading mode
	int		lc_speedy;		// speedy mode for direct file
    int     lc_timeout;     // timeout for internal link

	int		ti92v1;
	int		ti92v2;

    // ioports.c
	//...

    // keyboard.c
    int     on_key;

    // lcd.c
    int     lcd_off;
    int     contrast;
	int		lcd_power;

    // linkport.c
	//...

    // memory.c
    UBYTE	*rom;
    UBYTE	*ram;
    UBYTE	*io;
    UBYTE   *io2;
    int     initial_pc;

    int		flash_prot;

    // timer.c
    uint8_t     timer_value;
    uint8_t     timer_init;
    int         heartbeat;

} Ti68kHardware;

typedef struct
{
	// Memory
	GList *mem_rb;		// read byte
	GList *mem_rw;
	GList *mem_rl;

	GList *mem_wb;		// write byte	
	GList *mem_ww;	
	GList *mem_wl;

	GList *mem_rng_r;	// mem range
	GList *mem_rng_w;

	// Code
	GList *code;

	// Exceptions
	GList *exception;

	// Breakpoint cause
	int type;		// = cause
	int mode;		// = access type
	int id;			// = n°
} Ti68kBreakpoints;

extern Ti68kParameters 	params;
extern Ti68kHardware 	tihw;
extern TicableLinkParam link_cable;
extern Ti68kBreakpoints	bkpts;

#endif
