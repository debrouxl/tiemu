/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - Tiemu Is an EMUlator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
 *  Copyright (c) 2006-2007 Kevin Kofler
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __ENGINE_H__
#define __ENGINE_H__

/* 
   The TI92/89 should approximately execute NB_CYCLES_PER_LOOP_HW[12] in 
   ENGINE_TIME_LIMIT milliseconds (10.000.000 or 12.000.000 cycles/s).
   If you think this values are a bit too big, you can slow down 
   the emulator by changing them 
*/
#define NB_CYCLES_PER_LOOP_HW1 300000	// 300000 cycles in 30ms
#define NB_CYCLES_PER_LOOP_HW2 360000	// 360000 cycles in 30ms
#define NB_CYCLES_PER_LOOP_HW4 480000	// 480000 cycles in 30ms
#define ENGINE_TIME_LIMIT      30	// 30 ms
#define MIN_INSTRUCTIONS_PER_CYCLE 4 	// instructions take at least 4 cycles

#ifdef __cplusplus
extern "C" {
#endif
void engine_calibrate(void);

void engine_start(void); 
void engine_stop(void);

int engine_is_stopped(void);
int engine_is_running(void);
#ifdef __cplusplus
}
#endif

#endif
