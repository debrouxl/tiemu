/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001, Romain Lievin
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

#ifndef __TI68K_HARDWARE__
#define __TI68K_HARDWARE__

extern int init_int_tab_offset;
extern int timer_init, timer_value;

void init_hardware(void);
void reset_hardware(void);
void update_hardware(void);
void exit_hardware(void);

#define CYCLES_PER_INSTR 10
#define CYCLES_PER_TICK 6400

extern int cycle_instr;
extern int cycle_count;
extern int fast_cycle;

#if defined(__LINUX__)
#define INLINE inline
#elif defined(__WIN32__)
#define INLINE __inline
#else
# define INLINE
#endif

static void INLINE do_cycles(void) 
{
  if(cycle_count++ >= cycle_instr) 
    {
      update_hardware();
      cycle_count = 0;
    }
}

#endif
