/* Hey EMACS -*- linux-c -*- */
/* $Id: romcalls.h 1455 2005-05-31 18:38:03Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin, Kevin Kofler
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

/*
  Breakpoint definitions
*/

#ifndef __IOPORTS_H__
#define __IOPORTS_H__

#include <stdio.h>
#include <stdint.h>
#include <glib.h>

/* Types */

typedef enum
{
	IO_RO = 1, IO_WO = 2, IO_RW = 3,
} IOACCESS;

typedef struct
{
	uint32_t	addr;		// $600000
	int			size;		// 1, 2, 4 bytes
	int			type;		// ro, wo, rw
	int			bits[32];	// <..5.....> or <all>
	char*		name;		// "Constrast and battery status"
} IOPORT;

/* Functions */

int ioports_load(const char *path);
int ioports_unload(void);

#endif
