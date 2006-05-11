/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ti68k_int.h"

/* ti68k_debug_disassemble is a wrapper around the GDB or UAE disassembler, so
   we can use the GDB disassembler in GDB-enabled builds and the UAE one in
   non-GDB builds. The abstraction also allows plugging in any other
   disassembler instead at any moment. */

#ifndef NO_GDB

#include "../gdb/include/dis-asm.h"
struct ui_file;
struct disassemble_info gdb_disassemble_info (struct gdbarch *gdbarch, struct ui_file *file);
int print_insn_m68k (bfd_vma memaddr, disassemble_info *info);

/* GDB enabled: use the GDB disassembler */
uint32_t ti68k_debug_disassemble(uint32_t addr, char **line)
{
	static struct disassemble_info di;
	static char *output = NULL;
	uint32_t offset;
	gchar **split;
	gchar *p;

	if (!output)
	{
		di = gdb_disassemble_info (NULL, NULL);
		output = di.stream;
	}

	*output = 0;
	offset = print_insn_m68k(addr, &di);
	split = g_strsplit(output, " ", 2);

	if(split[1])
		for(p = split[1]; *p == ' '; p++);
	else
		p = "";

	*line = g_strdup_printf("%06x: %s %s", addr, split[0] ? split[0] : "", p);
	g_strfreev(split);

	return offset;
}

#else

int m68k_disasm (char *output, uint32_t addr);
int m68k_dasm(char **line, uint32_t addr);

/* GDB disabled: use the UAE disassembler */
uint32_t ti68k_debug_disassemble(uint32_t addr, char **line)
{
	uint32_t offset;

#if 1
	char output[256];
	offset = m68k_disasm(output, addr);
	*line = g_strdup(output);
#else
	offset = m68k_dasm(line, addr);
#endif

	return offset;
}
#endif
