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

#ifndef __TI68K_M68K__
#define __TI68K_M68K__

/* Functions */

void hw_init_m68k(void);
void hw_reset_m68k(void);
void hw_exit_m68k(void);
int  hw_run_m68k(int);

/* Variables */


#endif
