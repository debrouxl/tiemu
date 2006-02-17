/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
    Disasm: wrapper for the UAE disassembler. Needed because UAE is somewhat lazzy for
    some instructions and wrong for some others.
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include "libuae.h"
#include "m68k.h"

#include "ti68k_int.h"
#include "ti68k_err.h"

int Dasm68000 (unsigned char *pBase, char *buffer, int _pc);

uint32_t ti68k_debug_disassemble(uint32_t addr, char **line)
{
	uint8_t *mem;
	char output[256];
	uint32_t offset;
	gchar **split;
	gchar *p;

	mem = (uint8_t *)ti68k_get_real_address(addr);

	offset = Dasm68000(mem, output, addr);
	split = g_strsplit(output, " ", 2);
	//printf("<%06x: %s>\n", addr, output);

	if(split[1])
		for(p = split[1]; *p == ' '; p++);
	else
		p = "";

	*line = g_strdup_printf("%06x: %s %s", addr, split[0] ? split[0] : "", p);
	g_strfreev(split);

	return offset;
}

