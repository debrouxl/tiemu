/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - a TI emulator
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

#ifndef STRUCT_H
#define STRUCT_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <glib.h>

#define MAXCHARS 256

#include "paths.h"

/* General options */
typedef struct
{
	gchar* skin_file;	
	gint	console;

} TieOptions;

/* Screen capture options */
typedef struct {
	int		format;
	int		type;
	int		size;

	char*	file;
	int		counter;
} ScrOptions;

/* Debugger options for windows size and placement */
typedef struct {
	int x;
	int y;
	int w;
	int h;
} GdkRect;

typedef struct {
	GdkRect bkpts;
	GdkRect code;
	GdkRect mem;
	GdkRect regs;
} DbgOptions;

extern TieOptions options;
extern ScrOptions options2;
extern DbgOptions options3;


#endif
