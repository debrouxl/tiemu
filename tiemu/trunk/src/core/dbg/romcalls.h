/* Hey EMACS -*- linux-c -*- */
/* $Id: bkpts.h 517 2004-07-07 15:06:40Z roms $ */

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
  Breakpoint definitions
*/

#ifndef __ROMCALLS__
#define __ROMCALLS__

#include <stdio.h>
#include <stdint.h>

#include "romcalls.h"

/* Types */

typedef struct
{
    uint32_t    addr;
    char*       name;
} ROM_CALL;

/* Functions */

int romcall_is_address(uint32_t addr);

#endif
