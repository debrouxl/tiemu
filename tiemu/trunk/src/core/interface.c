/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

/*
    Interface: exported & misc routines
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "libuae.h"
#include "hw.h"
#include "m68k.h"
#include "dbus.h"

#include "ti68k_int.h"
#include "ti68k_err.h"


/**********************/
/* Internal variables */
/**********************/


Ti68kParameters     params = { 0 };
Ti68kHardware       tihw = { 0 };
TicableLinkParam    link_cable = { 0 };
Ti68kBreakpoints	bkpts = { 0 };


/***********************************/
/* Entry points for initialization */
/***********************************/


/*
  Initialization order (checked by the runlevel):
  - load the default config
  - load a ROM (init ROM)
  - init the lib (init HW, UAE, HID)
  - reset the lib
*/

/*
  This should be the FIRST function to call (unless the 'params' 
  structure has been properly initialized.
 */
int ti68k_config_load_default(void)
{
    params.background = 1;
    params.grayplanes = 4;

    params.restricted = 1;
    params.cpu_rate = -1;
    params.hw_rate = -1;
    params.lcd_rate = -1;

    ticable_get_default_param(&link_cable);
    link_cable.link_type = LINK_NUL;
    link_cable.port = NULL_PORT;

    return 0;
}

/*
  This should be the SECOND function to call. 
  Load a ROM image (images.c).
*/
int ti68k_load_image(const char *filename);

/*
  This is the THIRD function to call for completely initializing the
  emulation engine.
*/
int ti68k_init(void)
{
	// init libs
    ticable_init();
	tifiles_init();
	ticalc_init();

	// check if image has been loaded
	if(img_loaded == 0)
		return ERR_NO_IMAGE;

	// set calc type and init hardware
	memset(&tihw, 0, sizeof(Ti68kHardware));
    tihw.calc_type = img_infos.calc_type;
	TRY(hw_init());

	return 0;
}

/*
  This should be the FOURTH function to call.
  It simply resets the hardware engine.
*/
int ti68k_reset(void)
{
	hw_reset();

	return 0;
}

/*
  Close the library by exiting the emulation engine
  (free ressources).
*/
int ti68k_exit(void)
{
    TRY(hw_exit());

    ticable_exit();
	tifiles_exit();
	ticalc_exit();

	return 0;
}

/******************/
/* Link functions */
/******************/

/*
    Remark: there is a bug here... If the buffer is allocated with malloc, GtkTiEmu will 
    segfault (backBuf address out of bounds). If the buffer is allocated on the heap 
    as an array, there will be no problem. Moreover, this problem does not appear when we send 
    a file via the command mode, only in GUI mode.
    Then, there may be a problem of shared memory or something else...
*/
int ti68k_linkport_send_file(const char *filename)
{
    return send_ti_file(filename);
}

int ti68k_linkport_reconfigure(void)
{
	TRY(hw_dbus_exit());
    TRY(hw_dbus_init());

    return 0;
}

/******************/
/* Misc functions */
/******************/

const int ti_rom_sizes[] = { 1*MB, 2*MB, 2*MB, 4*MB, 4*MB };	// 92, 89, 92+, V200, TI89 Titanium
const int ti_ram_sizes[] = { 256*KB, 256*KB, 256*KB, 256*KB, 256*KB }; // should be 128
const int ti_io_sizes[] = { 32, 32, 32, 32, 64 };
const int ti_rom_base[] = { 0, 0x200000, 0x400000, 0x200000, 0x800000 };

static int log_b2(int i)
{
    int j, v;

    for(j = 0, v = i; v != 0; v >>= 1, j++);

    return j-1;
}

int ti68k_get_rom_size(int calc_type)
{
    if(calc_type > CALC_MAX)
    {
        DISPLAY("Bad argument !\n");
        exit(0);
    }

    return ti_rom_sizes[log_b2(calc_type)];
}

int ti68k_get_ram_size(int calc_type)
{
    if(calc_type > CALC_MAX)
    {
        DISPLAY("Bad argument !\n");
        exit(0);
    }

    return ti_ram_sizes[log_b2(calc_type)];
}

int ti68k_get_io_size(int calc_type)
{
	if(calc_type > CALC_MAX)
    {
        DISPLAY("Bad argument !\n");
        exit(0);
    }

    return ti_io_sizes[log_b2(calc_type)];
}
