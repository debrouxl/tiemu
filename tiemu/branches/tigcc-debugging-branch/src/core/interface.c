/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
#include "printl.h"

#include "ti68k_int.h"
#include "ti68k_err.h"
#include "mem_size.h"


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
#ifdef __IPAQ__
    params.background = 0;
#else
    params.background = 1;
#endif
    params.grayplanes = 4;

    params.restricted = 1;
    params.cpu_rate = -1;
    params.hw_rate = -1;
    params.lcd_rate = -1;
	params.hw_protect = 0;
	params.recv_file = 1;

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

unsigned int ti68k_get_cycle_count(int reset, unsigned int *diff)
{
	static unsigned int old_cnt = 0;
	unsigned int new_cnt;

	new_cnt = hw_m68k_get_cycle_count(reset);

	if(diff != NULL)
		*diff = new_cnt - old_cnt;
	old_cnt = new_cnt;

	return new_cnt;
}

/******************/
/* Link functions */
/******************/

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

const int ti_rom_base[]  = { ROM_BASE_TI92_I, ROM_BASE_TI89, ROM_BASE_TI92P, ROM_BASE_V200, ROM_BASE_TI89T };
const int ti_rom_sizes[] = { ROM_SIZE_TI92_I, ROM_SIZE_TI89, ROM_SIZE_TI92P, ROM_SIZE_V200, ROM_SIZE_TI89T };
const int ti_ram_sizes[] = {RAM_SIZE_TI92_II, RAM_SIZE_TI89, RAM_SIZE_TI92P, RAM_SIZE_V200, RAM_SIZE_TI89T };

const int ti_io_sizes[]  = { IO1_SIZE_TI92_I, IO1_SIZE_TI89, IO1_SIZE_TI92P, IO1_SIZE_V200, IO1_SIZE_TI89T };
const int ti_io2_sizes[] = { IO2_SIZE_TI92_I, IO2_SIZE_TI89, IO2_SIZE_TI92P, IO2_SIZE_V200, IO2_SIZE_TI89T };
const int ti_io3_sizes[] = { IO3_SIZE_TI92_I, IO3_SIZE_TI89, IO3_SIZE_TI92P, IO3_SIZE_V200, IO3_SIZE_TI89T };

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
        printl(0, "Bad argument !\n");
        exit(0);
    }

    return ti_rom_sizes[log_b2(calc_type)];
}

int ti68k_get_ram_size(int calc_type)
{
    if(calc_type > CALC_MAX)
    {
        printl(0, "Bad argument !\n");
        exit(0);
    }

    return ti_ram_sizes[log_b2(calc_type)];
}

int ti68k_get_io_size(int calc_type)
{
	if(calc_type > CALC_MAX)
    {
        printl(0, "Bad argument !\n");
        exit(0);
    }

    return ti_io_sizes[log_b2(calc_type)];
}

int ti68k_get_io2_size(int calc_type)
{
	if(calc_type > CALC_MAX)
    {
        printl(0, "Bad argument !\n");
        exit(0);
    }

    return ti_io2_sizes[log_b2(calc_type)];
}

int ti68k_get_io3_size(int calc_type)
{
	if(calc_type > CALC_MAX)
    {
        printl(0, "Bad argument !\n");
        exit(0);
    }

    return ti_io3_sizes[log_b2(calc_type)];
}

void ti68k_unprotect_64KB_range(uint32_t addr)
{
    unsigned blockid = addr >> 12, i;

    for (i = blockid; i <= blockid + 16 && i < 64; i++)
        tihw.ram_exec[i] = 0;
}
