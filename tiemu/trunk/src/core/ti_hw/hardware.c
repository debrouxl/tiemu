/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
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
  TI hardware management (ASIC).
  -> screen, keyboard, linkport, timers.
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#include "uae.h"
#include "images.h"
#include "memory.h"
#include "lcd.h"
#include "keyboard.h"
#include "dbus.h"
#include "ti68k_def.h"
#include "timer.h"
#include "ioports.h"
#include "callbacks.h"
#include "m68k.h"

int init_int_tab_offset;

int cycle_instr = 640;
int cycle_count = 0;


/* This function should be called everytime the counter increases */
void update_hardware()
{
  /* Auto-int 5: timer */
  if(tihw.timer_value++ == 0)
    {
      tihw.timer_value = tihw.timer_init;
      specialflags |= SPCFLAG_INT;
      if(currIntLev < 5)
	currIntLev = 5;
    }
  else
    tihw.timer_value &= 0xff;

  /* Auto-int 1: 1/4 of timer rate */
  if(!(tihw.timer_value&3)) 
    {
      specialflags |= SPCFLAG_INT;
      currIntLev = 1;
    }

  /* Auto-int 2: keyboard scan */
  if(!(tihw.timer_value & 2))
    {
    }

  /* Link status */
  if(linkport_checkread())
    tihw.io[0xc] |= 0x2;

  /* Link interrupt */ 
  if(tihw.io[0xc]&0x2) 
    {
      specialflags |= SPCFLAG_INT;
      currIntLev = 4;
    }
  
  /* LCD is refreshed every 16th time */
  if(!(tihw.timer_value&15))
    {
      if(lc_internal) 
	{
	  if(lc_timeout++ >= TO_VALUE) 
	    {
	      DISPLAY("Warning: internal link timeout !!!\n");
	      lc_internal = 0;
	      lc_timeout = 0;
	    }
	}

      hw_update_kbd();

      if(!params.sync_one)
	cb_update_screen();
    }
}


/*
	Init hardware...
	A ROM image must have been loaded before calling this function.
*/

int hw_init(void)
{
	hw_mem_init();
	hw_io_init();
	hw_kbd_init();
	hw_timer_init();
	hw_lcd_init();
	hw_dbus_init();
	hw_m68k_init();
}

int hw_reset(void)
{
	hw_mem_reset();
	hw_io_reset();
	hw_kbd_reset();
	hw_timer_reset();
	hw_lcd_reset();
	hw_dbus_reset();
	hw_m68k_reset();
}

int hw_exit(void)
{
	hw_m68k_exit();
	hw_dbus_exit();
	hw_lcd_exit();
	hw_timer_exit();
	hw_kbd_exit();
	hw_io_exit();
	hw_mem_exit();
}
