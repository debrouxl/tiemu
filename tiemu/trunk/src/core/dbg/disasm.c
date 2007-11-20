/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - Tiemu Is an EMUlator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005-2006, Romain Liévin, Kevin Kofler
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

/* GDB enabled: use the GDB disassembler */

#include "../../gdb/include/dis-asm.h"
struct gdbarch;
struct ui_file;
struct disassemble_info gdb_disassemble_info (struct gdbarch *gdbarch, struct ui_file *file);
int print_insn_m68k (bfd_vma memaddr, disassemble_info *info);

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

extern int m68k_disasm (char *output, uint32_t addr);

static const char* instr[] = { 
	"ORSR.B",  "ORSR.W",	/* ORI  #<data>,SR		*/
	"ANDSR.B", "ANDSR.W",	/* ANDI #<data>,SR		*/
	"EORSR.B", "EORSR.W",	/* EORI #<data>,SR		*/
	"MVSR2.W", "MVSR2.B",	/* MOVE SR,<ea>			*/
	"MV2SR.B", "MV2SR.W",	/* MOVE <ea>,SR			*/
	"MVR2USP.L",			/* MOVE An,USP			*/
	"MVUSP2R.L",			/* MOVE USP,An			*/
	"MVMEL.W", "MVMEL.L",   /* MOVEM <ea>,<list>  	*/
	"MVMLE.W", "MVMLE.L",   /* MOVEM <list>,<ea>	*/
	"MVPMR.W", "MVPMR.L",   /* MOVEP <Dx>,<(d16,Ay)>*/
	"MVPRM.W", "MVPRM.L",   /* MOVEP <d16,Ay>,<Dx>	*/
	"TRAP.L",				/* TRAP	#<vector>		*/
	"RESET.L",
	"NOP.L",
	"STOP.L",
	"RTE.L",
	"RTS.L",
	"JMP.L",
	"LEA.L",
	"BT",

	NULL
};

static int match_opcode(const char *opcode)
{
	int i;

	if(opcode == NULL)
		return -1;

	for(i = 0; instr[i] != NULL; i++)
	{
		if(!strncmp(opcode, (char *)instr[i], strlen(instr[i])))
			return i;
	}

	return -1;
}

// do the same work as m68k_disasm but some instructions dis-assembled by the
// UAE engine use a weird/wrong naming scheme so we remap them here rather than
// touching the newcpu.c & table68k files because this file may be updated when 
// upgrading the UAE engine.
int m68k_dasm(char **line, uint32_t addr)
{
	char output[1024];
	int offset;
	gchar** split;
	int idx;

	offset = m68k_disasm(output, addr);

	// split string into address, opcode and operand
	split = g_strsplit(output, " ", 3);

	// search for opcode to rewrite
	idx = match_opcode(split[1]);
	if(idx != -1)
	{
		gchar *tmp;
		
		switch(idx)
		{
		case 0:		/* ORI to SR #<data>,SR		*/
			g_free(split[1]);
			split[1] = g_strdup("ORI.B");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 1:
			g_free(split[1]);
			split[1] = g_strdup("ORI.W");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 2:		/* ANDI to SR #<data>,SR	*/
			g_free(split[1]);
			split[1] = g_strdup("ANDI.B");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 3:
			g_free(split[1]);
			split[1] = g_strdup("ANDI.W");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 4:		/* EORI to SR #<data>,SR	*/
			g_free(split[1]);
			split[1] = g_strdup("EORI.B");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 5:
			g_free(split[1]);
			split[1] = g_strdup("EORI.W");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 6:		/* MOVE from SR SR,<ea>		*/
			g_free(split[1]);
			split[1] = g_strdup("MOVE.B");
			
			tmp = g_strconcat("SR,", split[2], NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 7:
			g_free(split[1]);
			split[1] = g_strdup("MOVE.W");
			
			tmp = g_strconcat("SR,", split[2], NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 8:		/* MOVE to SR <ea>,SR		*/
			g_free(split[1]);
			split[1] = g_strdup("MOVE.B");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 9:
			g_free(split[1]);
			split[1] = g_strdup("MOVE.W");
			
			tmp = g_strconcat(split[2], ",SR", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 10:	/* MOVE An,USP	*/
			g_free(split[1]);
			split[1] = g_strdup("MOVE");
			
			tmp = g_strconcat(split[2], ",USP", NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 11:	/* MOVE USP,An	*/
			g_free(split[1]);
			split[1] = g_strdup("MOVE");
			
			tmp = g_strconcat("USP,", split[2], NULL);
			g_free(split[2]);
			split[2] = tmp;
			break;
		case 12:    /* MOVEM <ea>,<list>  */
			g_free(split[1]);
			split[1] = g_strdup("MOVEM.W");
			break;
		case 13:
			g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
			break;
		case 14:    /* MOVEM <list>,<ea>  */
			g_free(split[1]);
			split[1] = g_strdup("MOVEM.W");
			break;
		case 15:    /* MOVEM <list>,<ea>  */
			g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
			break;
		case 16:    /* MOVEP <Dx>,<(d16,Ay)> */
			g_free(split[1]);
			split[1] = g_strdup("MOVEP.W");
			break;
		case 17:	/* MOVEP <Dx>,<(d16,Ay)> */
			g_free(split[1]);
			split[1] = g_strdup("MOVEP.L");
			break;
		case 18:    /* MOVEP MOVEP <d16,Ay>,<Dx> */
			g_free(split[1]);
			split[1] = g_strdup("MOVEP.W");
			break;
		case 19:    /* MOVEP <d16,Ay>,<Dx> */
			g_free(split[1]);
			split[1] = g_strdup("MOVEP.L");
			break;
		case 20:	/* TRAP #<vector>	*/
		case 21:	/* RESET.L			*/
		case 22:	/* NOP.L			*/
		case 23:	/* STOP.L			*/
		case 24:	/* RTE.L			*/
		case 25:	/* RTS.L			*/
		case 26:	/* JMP.L			*/
		case 27:	/* LEA.L			*/
			{
				char *p = strchr(split[1], '.');
				if(p) *p = '\0';
			}
			break;
		case 28:	/* BRA				*/
			g_free(split[1]);
			split[1] = g_strdup("BRA");
		break;

		default:
			break;
		}
	}

	// search for [value]
	if(strchr(split[2], '['))
	{
		char *p = strchr(split[2], '[');
		char *q = strrchr(split[2], ']');
		char tmp[256];

		p--;
		q++;
		strncpy(tmp, p, q - p);
		tmp[q-p] = '\0';
		strcpy(p, q);
		strcat(split[2], tmp);
	}
	
	*line = g_strdup_printf("%s %s %s", 
			split[0] ? split[0] : "", 
			split[1] ? split[1] : "",
			split[2] ? split[2] : "");
	g_strfreev(split);

	return offset;
}

uint32_t ti68k_debug_disassemble(uint32_t addr, char **line)
{
	uint32_t offset;

	offset = m68k_dasm(line, addr);

	return offset;
}
#endif
