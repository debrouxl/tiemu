/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001-2002, Romain Lievin
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

#ifndef __TI68K_PARAMS__
#define __TI68K_PARAMS__

#include "tilibs.h"

struct global_param {
  char *rom_file;
  char *ram_file;
  char *tib_file;

  int ram_size;
  int rom_size;

  int background;
  int n_grayplanes;

  int tick_rate;
  int cycle_rate;
  int i_tick;
  int sync_one;
  int restrict;
};
typedef struct global_param Ti68kParameters;

extern Ti68kParameters params;

extern TicableLinkParam link_cable;

#endif
