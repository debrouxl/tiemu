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

#include "uae.h"
#include "hardware.h"
#include "ti68k_int.h"
#include "ti68k_err.h"


/**********************/
/* Internal variables */
/**********************/


Ti68kParameters     params = { 0 };
TicableLinkParam    link_cable = { 0 };
Ti68kHardware       tihw = { 0 };


/***********************************/
/* Entry points for initialization */
/***********************************/


/*
  Initialization order (checked by the runlevel):
  - register callbacks
  - load the default config
  - load a ROM (init ROM)
  - init the lib (init HW, UAE, HID)
  - reset the lib
*/

/* 
   This should be the FIRST function to call for registering
   HID callbacks.
   The callbacks will not be used until ti68k_init().
*/
void ti68k_gui_set_callbacks(
              callback_iv_t initSpecific,
			  callback_iv_t exitSpecific,
			  callback_iv_t updateScreen,
			  callback_iv_t updateKeys,
			  callback_vi_t screenOnOff,
			  callback_ii_t setContrast
			  );

/*
  This should be the SECOND function to call (unless the 'params' 
  structure has been properly initialized.
 */
int ti68k_config_load_default(void)
{
  params.background = 1;
  params.grayplanes = 2;
  params.i_tick = 640;
  params.tick_rate = 40000;
  params.cycle_rate = 150;
  params.sync_one = 0;
  params.restrict = 1;

  ticable_get_default_param(&link_cable);
  link_cable.link_type = LINK_NUL;
  link_cable.port = NULL_PORT;

  return 0;
}

/*
  This should be the THIRD function to call. 
  Load a ROM image (images.c).
*/
int ti68k_load_image(const char *filename);

/*
  This is the FOURTH function to call for completely initializing the
  emulation engine.
*/
int ti68k_init(void)
{
	// init libs
    ticable_init();
	tifiles_init();
	ticalc_init();

	// check image has been loaded
	if(img_loaded == 0)
		return ERR_68K_INVALID_ROM;

	// set calc type and init hardware
    tihw.calc_type = img_infos.calc_type;
	hw_init();

	// init hid
	cb_init_specific();
	cb_screen_on_off(!0);

	// note: TI92+ does not flicker when LCD is refresh on IRQ1 (??)
	if(tihw.calc_type == TI92p)
		params.sync_one = 0;

	return 0;
}

/*
  This should be the FIFTH function to call.
  It simply resets the hardware engine.
*/
int ti68k_reset(void)
{
	cycle_instr = params.i_tick;

	hw_reset();

	return 0;
}

/*
  Close the library by exiting HID and exiting the emulation engine
  (free ressources).
*/
int ti68k_exit(void)
{
    cb_exit_specific();
    hw_exit();

    ticable_exit();
	tifiles_exit();
	ticalc_exit();

	return 1;
}

/*
    Full restart of the library.
*/
int ti68k_restart(void)
{
  ti68k_exit();
  ti68k_init();
  ti68k_reset();
  
  return 0;
}


/***********************/
/* Debugging functions */
/***********************/


int ti68k_debug_disassemble(int addr, char *output)
{
  CPTR nextPc;

  MC68000_disasm(addr, &nextPc, 1, output);
  output[strlen(output)-1]='\0'; // strip CR-LF
  return (nextPc-addr);
}

int ti68k_debug_launch(void)
{
    return cb_launch_debugger();
}

int ti68k_debug_do_single_step(void)
{
    return specialflags |= SPCFLAG_DBTRACE;
}

/*
    Do 'n' instructions.
    Return 0 if successful, a negative value if an error occured,
    a positive value if a breakpoint has been encountered.

    Note: replace M68000_run() from UAE
*/
int ti68k_debug_do_instructions(int n)
{
    if(!img_loaded)
        return ERR_68K_ROM_NOT_LOADED;

    return hw_m68k_run(n);
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

extern int TO_VALUE;

int ti68k_linkport_set_timeout(int value)
{
    TO_VALUE = value;
    return value;
}

int ti68k_linkport_get_timeout(int value)
{
    return TO_VALUE;
}

int ti68k_linkport_reconfigure(void)
{
    hw_dbus_init();
    hw_dbus_exit();

    return 0;
}
