/* Hey EMACS -*- linux-c -*- */
/* $Id: interface.c 514 2004-07-07 09:27:53Z roms $ */

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

//#define UAE_DISASM
#define MAME_DISASM

#ifdef UAE_DISASM

// some instructions use a weird naming scheme, remap !
static const char* instr[] = { 
	"ORSR.B", "ORSR.W",		/* ORI  #<data>,SR		*/
	"ANDSR.B", "ANDSR.W",	/* ANDI #<data>,SR		*/
	"EORSR.B", "EORSR.W",	/* EORI #<data>,SR		*/
	"MVSR2.W", "MVSR2.B",	/* MOVE SR,<ea>			*/
	"MV2SR.B", "MV2SR.W",	/* MOVE <ea>,SR			*/
	"MVR2USP.L",			/* MOVE An,USP			*/
	"MVUSP2R.L",			/* MOVE USP,An			*/
    "MVMEL.W", "MVMEL.L",   /* MOVEM <ea>,<list>  	*/
    "MVMLE.W", "MVMLE.L",   /* MOVEM <list>,<ea>	*/
	"TRAP",					/* TRAP	#<vector>		*/
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

uint32_t ti68k_debug_disassemble(uint32_t addr, char **line)
{
	uint32_t next;
	char *tok;
	gchar** split;
	int idx;
	char output[128];
	//uint16_t d;

	MC68000_disasm(addr, &next, 1, output);
	output[strlen(output)-1] = '\0'; // strip CR-LF

	// remove extra space as in 'BT .B' instead of BT.B
	tok = strstr(output, " .");
	if(tok)
		memcpy(tok, tok+1, strlen(tok));
	//printf("<%s>\n", output);
	
	// split string into address, opcode and operand
	split = g_strsplit(output, " ", 3);
	//printf("%s %s%*c %s\n", split[0], split[1], 8 - strlen(split[1]), ' ', split[2]);

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
			next += 2;
			break;
		case 13:
			g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
			next += 2;
			break;
        case 14:    /* MOVEM <list>,<ea>  */
        	g_free(split[1]);
			split[1] = g_strdup("MOVEM.W");			
			next += 2;
            break;
        case 15:    /* MOVEM <list>,<ea>  */
            // UAE does not fully disasm this instruction	
            g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
			//d = *((uint16_t *)get_real_address(next));
			next += 2;	
			break;
		case 16:	/* TRAP #<vector>	*/
			tmp = split[1] + strlen("TRAP");
			split[2] = g_strdup(tmp);
			*tmp = '\0';
			break;
		default:
			break;
		}
	}
	
	*line = g_strdup_printf("%s %s %s", 
			split[0] ? split[0] : "", 
			split[1] ? split[1] : "",
			split[2] ? split[2] : "");
	g_strfreev(split);

	return (next - addr);
}

#else

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

#endif