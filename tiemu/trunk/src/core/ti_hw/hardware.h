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

#ifndef __TI68K_HARDWARE__
#define __TI68K_HARDWARE__

int hw_init(void);
int hw_reset(void);
int hw_exit(void);

int hw_update(void);

#define CYCLES_PER_INSTR    10
#define CYCLES_PER_TICK     6400

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
        //hw_update();
        cycle_count = 0;
    }
}

#endif
