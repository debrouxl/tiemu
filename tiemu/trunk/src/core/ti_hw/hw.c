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
    TI's ASIC management: glue logic for screen, keyboard, linkport, timers.
*/


#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "libuae.h"
#include "mem.h"
#include "ports.h"
#include "dbus.h"
#include "kbd.h"
#include "hwprot.h"
#include "flash.h"
#include "m68k.h"
#include "images.h"
#include "ti68k_def.h"

/*
	Some explanations:
	
	OSC2 ---[$600015=/2^9]---> Fcnt +--[/4]--->		AI1
									+--[/timer]--->	AI5
									+--[/1024]--->	AI3

	HW1: OSC2 is incremented every 6250 OSC1 cycles
*/

#define HW1_RATE    625     // 10MHz / 1600 (timer rate)
#define HW2_RATE    1172    // 12MHz / 1024 (timer rate)

// Cycle rate for HW1/HW2 at 2^5, 2^9, 2^12, 2^18 (port $600015)
const int cycle_rate[2][4] = { { 391, 6250, 50000, 3200000 }, { 732, 11720, 93750, 6000000 } };

int cycle_instr = HW1_RATE;
int cycle_count = 0;

void set_cycle_rate(int i)
{
	cycle_instr = cycle_rate[(tihw.hw_type == HW1) ? 0 : 1][i] / 10;
}

/*
	Init hardware...
	A ROM image must have been loaded before calling this function.
*/
int hw_init(void)
{
    IMG_INFO *img = &img_infos;

    // Get infos from image
	tihw.calc_type = img_infos.calc_type;
	tihw.rom_base = img->rom_base;
	tihw.rom_flash = img->flash;
	strcpy(tihw.rom_version, img->version);
	tihw.hw_type = img->hw_type;
	
	tihw.ti92v1 = (tihw.calc_type == TI92) && (strcmp(tihw.rom_version, "2.0") < 0);
	tihw.ti92v2 = (tihw.calc_type == TI92) && (strcmp(tihw.rom_version, "2.0") >= 0);

    switch(tihw.calc_type)
    {
    case TI89:
    case TI89t:
		tihw.log_w = tihw.lcd_w = 160;
		tihw.log_h = tihw.lcd_h = 100;	
        break;
    case TI92:
    case TI92p:
    case V200:
		tihw.log_w = tihw.lcd_w = 240;
		tihw.log_h = tihw.lcd_h = 128;
        break;
    default:
        return -1;
        break;
    }

    // Do sub-initializations.
	TRY(hw_hwp_init());
	TRY(hw_mem_init());
	TRY(hw_flash_init());
	TRY(hw_io_init());
	TRY(hw_dbus_init());
	TRY(hw_kbd_init());
	TRY(hw_m68k_init());

    // Set hardware update rate (dependant from io[0x15])
    if(params.hw_rate != -1)
        cycle_instr = params.hw_rate;
    else if(tihw.hw_type == HW1)
        cycle_instr = HW1_RATE;
    else if(tihw.hw_type >= HW2)
        cycle_instr = HW2_RATE;

    return 0;
}

int hw_reset(void)
{
	TRY(hw_hwp_reset());
	TRY(hw_mem_reset());
	TRY(hw_flash_reset());
	TRY(hw_io_reset());
	TRY(hw_kbd_reset());
	TRY(hw_dbus_reset());
	TRY(hw_m68k_reset());

    return 0;
}

int hw_exit(void)
{
	TRY(hw_m68k_exit());
	TRY(hw_dbus_exit());
	TRY(hw_kbd_exit());
	TRY(hw_io_exit());
	TRY(hw_flash_exit());
	TRY(hw_mem_exit());
	TRY(hw_hwp_exit());

    return 0;
}

G_LOCK_DEFINE(lcd_flag);
volatile int lcd_flag = !0;

extern int lcd_hook_hw1(void);

/*
    This function is called by do_cycles to regularly updates the hardware.
    Rate is the same as the timer tick rate.
*/
void hw_update(void)
{
	static int timer;	// -> tihw.timer_value

    // OSC2 enable (bit clear means oscillator stopped!)
    if(!io_bit_tst(0x15,1))
	{
		hw_kbd_update();	// scan ON key
        return;
	}

    // Increment timer
    if(io_bit_tst(0x15,3))
    {
		if (tihw.timer_value == 0x00) 
			tihw.timer_value = tihw.io[0x17];
		else 
			tihw.timer_value++;
		timer++;
    }

	// Increment RTC timer every 8192 seconds
	if(io2_bit_tst(0x1f, 2) && io2_bit_tst(0x1f, 1))
	{
		static time_t old_clk;
		if(((clock() - old_clk) / CLOCKS_PER_SEC) > 3/*8191*/)
		{
			old_clk = clock();
			tihw.rtc_value++;
		}
	}

    // Toggles every FS (every time the LCD restarts at line 0) -> 90 Hz ~ timer/12
    // Don't use the actual LCD count (and use 12 rather than 11) to keep exposure
    // times consistent
    if(!(timer % 12) && tihw.hw_type >= HW2) 
    	tihw.io2[0x1d] ^= 0x80;

	/* Auto-int management */

	// Auto-int 1: 1/4 of timer rate
	// Triggered at a fixed rate: OSC2/2^11
    if(!(timer & 3)) 
    {		
    	if(!io_bit_tst(0x15,7))
			if((tihw.hw_type == HW1) || !(io2_bit_tst(0x1f, 2) && !io2_bit_tst(0x1f, 1)))
				hw_m68k_irq(1);
    }

	// Auto-int 2: keyboard scan
    // see keyboard.c

	// Auto-int 3: disabled by default by AMS
	// When enabled, it is triggered at a fixed rate: OSC2/2^19 = 1/1024 of timer rate = 1Hz
	if(io_bit_tst(0x15,1) && !(timer & 1023))
	{
        if(!io_bit_tst(0x15,7) && io_bit_tst(0x15,2))
			if((tihw.hw_type == HW1) || !(io2_bit_tst(0x1f, 2) && !io2_bit_tst(0x1f, 1)))
				hw_m68k_irq(3);
	}

	// DBus: External link activity ?
	/*
	if(!lc.get_red_wire() || !lc.get_white_wire())
	{
		io_bit_set(0x0d,3);
		io_bit_set(0x0d,2);
	}
	*/

	// DBUS enabled ?
	if(!io_bit_tst(0x0c,6))
	{
		// Check for data arrival (link cable)
		hw_dbus_checkread();

		// Trigger int4 on: error, link act, txbuf empty or rxbuf full
		if((io_bit_tst(0x0c,3) && io_bit_tst(0x0d,7))  ||
			(io_bit_tst(0x0c,2) && io_bit_tst(0x0d,3)) ||
			(io_bit_tst(0x0c,1) && io_bit_tst(0x0d,6)) ||
			(io_bit_tst(0x0c,0) && io_bit_tst(0x0d,5)))
		{
			hw_m68k_irq(4);
		}
	}

    // Auto-int 5: triggered by the programmable timer.
	// The default rate is OSC2/(K*2^9), where K=79 for HW1 and K=53 for HW2
    if(tihw.timer_value == 0)
    {
		if(!io_bit_tst(0x15,7))	
			if((tihw.hw_type == HW1) || !(io2_bit_tst(0x1f, 2) && !io2_bit_tst(0x1f, 1)))
			{
				hw_m68k_irq(5);
			}
			
    }

	// Auto-int 6: triggered when [ON] is pressed.
	// see keyboard.c

	// Auto-int 7: "vector table write protection" & "stack overflow"
	// see memory.c

	/* Hardware refresh */
  
  	// Update keyboard (~600Hz). Not related to timer but as a convenience
  	if(!(timer & 7))		// 1 and 3 don't work, 7 and 15 are ok
  		hw_kbd_update();
  		
  	// Update LCD (HW1: every 16th timer tick, HW2: unrelated)
  	if((tihw.hw_type == HW1) && !(timer & 15))
    {
        G_LOCK(lcd_flag);
        lcd_flag = !0;
        G_UNLOCK(lcd_flag);
		lcd_hook_hw1();
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

