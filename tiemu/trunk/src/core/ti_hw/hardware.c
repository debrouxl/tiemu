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
  if(timer_value++ == 0)
    {
      timer_value = timer_init;
      specialflags |= SPCFLAG_INT;
      if(currIntLev < 5)
	currIntLev = 5;
    }
  else
    timer_value &= 0xff;

  /* Auto-int 1: 1/4 of timer rate */
  if(!(timer_value&3)) 
    {
      specialflags |= SPCFLAG_INT;
      currIntLev = 1;
    }

  /* Auto-int 2: keyboard scan */
  if(!(timer_value&2))
    {
    }

  /* Link status */
  if(linkport_checkread())
    ti_io[0xc] |= 0x2;

  /* Link interrupt */ 
  if(ti_io[0xc]&0x2) 
    {
      specialflags |= SPCFLAG_INT;
      currIntLev = 4;
    }
  
  /* LCD is refreshed every 16th time */
  if(!(timer_value&15))
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

void init_hardware() 
{
  hw_init_mem();
  hw_init_io();
  hw_init_kbd();
  hw_init_timer();
  hw_init_lcd();
  hw_init_dbus();
  hw_init_m68k();
}

void reset_hardware()
{
  hw_reset_m68k();
}

void exit_hardware(void) 
{
  hw_exit_m68k();
  hw_exit_dbus();
  hw_exit_lcd();
  hw_exit_timer();
  hw_exit_kbd();
  hw_exit_io();
  hw_exit_mem();
}
