/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 245 2004-05-23 20:45:43Z roms $ */

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
    TI's ASIC management: glue logic for screen, keyboard, linkport, timers.
*/


#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "uae.h"
#include "memory.h"
#include "ioports.h"
#include "dbus.h"
#include "keyboard.h"
#include "timer.h"
#include "lcd.h"
#include "m68k.h"
#include "callbacks.h"
#include "images.h"
#include "ti68k_def.h"

#define HW1_RATE    625     // 10MHz / 1600 (timer rate)
#define HW2_RATE    1172    // 12MHz / 1024 (timer rate)    732, 11720, 93750, 6E6

int cycle_instr = HW1_RATE;
int cycle_count = 0;

/*
	Init hardware...
	A ROM image must have been loaded before calling this function.
*/
int hw_init(void)
{
    IMG_INFO *img = &img_infos;

    // Get infos from image
	tihw.calc_type = img_infos.calc_type;
	tihw.rom_internal = img->internal;
	tihw.rom_flash = img->flash;
	strcpy(tihw.rom_version, img->version);
	tihw.hw_type = img->hw_type;
	
	tihw.ti92v1 = (tihw.calc_type == TI92) && (strcmp(tihw.rom_version, "2.0") < 0);
	tihw.ti92v2 = (tihw.calc_type == TI92) && (strcmp(tihw.rom_version, "2.0") >= 0);

	if(tihw.calc_type == TI89)
	{
		tihw.lcd_w = 160;
		tihw.lcd_h = 100;	
	}
	else
	{
		tihw.lcd_w = 240;
		tihw.lcd_h = 128;
	}

    // Do sub-initializations.
	TRY(hw_mem_init());
	TRY(hw_io_init());
	TRY(hw_dbus_init());
	TRY(hw_kbd_init());
	TRY(hw_timer_init());
	TRY(hw_lcd_init());
	TRY(hw_m68k_init());

    // Set hardware update rate (dependant from io[0x15])
    if(params.hw_rate != -1)
        cycle_instr = params.hw_rate;
    else if(tihw.hw_type == HW1)
        cycle_instr = HW1_RATE;
    else if(tihw.hw_type == HW2)
        cycle_instr = HW2_RATE;

    return 0;
}

int hw_reset(void)
{
	TRY(hw_mem_reset());
	TRY(hw_io_reset());
	TRY(hw_kbd_reset());
	TRY(hw_timer_reset());
	TRY(hw_lcd_reset());
	TRY(hw_dbus_reset());
	TRY(hw_m68k_reset());

    return 0;
}

int hw_exit(void)
{
	TRY(hw_m68k_exit());
	TRY(hw_dbus_exit());
	TRY(hw_lcd_exit());
	TRY(hw_timer_exit());
	TRY(hw_kbd_exit());
	TRY(hw_io_exit());
	TRY(hw_mem_exit());

    return 0;
}

G_LOCK_DEFINE(lcd_flag);
volatile int lcd_flag = !0;

/*
    This function is called by do_cycles to regularly updates the hardware.
    Rate is the same as the timer tick rate.
*/
void hw_update(void)
{
    // OSC2 enable (bit clear means oscillator stopped!)
    if(!io_bit_tst(0x15,1))
        return;

    // Increment timer
    if(io_bit_tst(0x15,3))
    {
    	tihw.timer_value++;
    	tihw.heartbeat--;
    }

	/* Auto-int management */

	// Auto-int 1: 1/4 of timer rate
	// Triggered at a fixed rate: OSC2/2^11
    if(!(tihw.timer_value & 3)) 
    {
    	if(!io_bit_tst(0x15,7))
    	{
        	specialflags |= SPCFLAG_INT;
        	currIntLev = 1;
        }
    }

	// Auto-int 2: keyboard scan
    // see keyboard.c

	// Auto-int 3: disabled by default by AMS
	// When enabled, it is triggered at a fixed rate: OSC2/2^19 = 1/1024 of timer rate = 1Hz
	if(io_bit_tst(0x15,1)&& !tihw.heartbeat)
	{
        tihw.heartbeat = 1024;
        if(!io_bit_tst(0x15,7) && io_bit_tst(0x15,2))
        {
			specialflags |= SPCFLAG_INT;
        	currIntLev = 3;
        }
	}

	// Triggered by the link hardware for various reasons.
	// External link activity ?
	if(!lc.get_red_wire() || !lc.get_white_wire())
		io_bit_set(0x0d,3);

	// DBUS enabled ?
	if(!io_bit_tst(0x0c,6))
	{
		// Check for data in buffer (and link activity)
		hw_dbus_checkread();

		// Trigger int4 on: error, link act, txbuf empty or rxbuf full
		if((io_bit_tst(0x0c,3) && io_bit_tst(0x0d,7)) ||
			(io_bit_tst(0x0c,2) && io_bit_tst(0x0d,3)) ||
			(io_bit_tst(0x0c,1) && io_bit_tst(0x0d,6)) ||
			(io_bit_tst(0x0c,0) && io_bit_tst(0x0d,5)))
		{
			io_bit_set(0x0d,4);					// interrupt pending
			specialflags |= SPCFLAG_INT;
			currIntLev = 4;
		}
	}

    // Auto-int 5: triggered by the programmable timer.
	// The default rate is OSC2/(K*2^9), where K=79 for HW1 and K=53 for HW2
    if(tihw.timer_value == 0)
    {
        tihw.timer_value = tihw.timer_init;
		if(!io_bit_tst(0x15,7))
		{
        	specialflags |= SPCFLAG_INT;
        	currIntLev = 5;
        }
    }

	// Auto-int 6: triggered when [ON] is pressed.
	// see keyboard.c

	// Auto-int 7: "vector table write protection" & "stack overflow"
	// see memory.c

	/* Hardware refresh */
  
  	// Update keyboard (~600Hz). Not related to timer but as a convenience
  	if(!(tihw.timer_value & 7))		// 1 and 3 don't work, 7 and 15 are ok
  		hw_kbd_update();
  		
  	// Update LCD (HW1: every 16Th timer tick, HW2: unrelated)
  	if((tihw.hw_type == HW1) && !(tihw.timer_value & 15))
    {
        G_LOCK(lcd_flag);
        lcd_flag = !0;
		//printf(">");
        G_UNLOCK(lcd_flag);
    }

}

/*
    This function is used to regularly update the hardware from CPU loop. 
    Note that CPU is running against OSC1 (HW1 @10Mhz, HW2 @12MHz)
    while hardware is synched against OSC2 (HW1 @700kHz,  HW2 @~520 kHz).
    OSC2 is the timing base for the timers, the link I/O hardware and 
    (HW1 only) the LCD controller.
    These 2 oscillators are independants.

    See hardware.h for inline definition.
*/
/*
static void INLINE do_cycles(void);
{
    if(cycle_count++ >= cycle_instr) 
    {
        hw_update();
        cycle_count = 0;
    }
}
*/

