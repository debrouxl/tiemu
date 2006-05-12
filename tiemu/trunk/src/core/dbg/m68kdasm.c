/* Hey EMACS -*- linux-c -*- */
/* $Id: bkpts.c 1693 2005-08-25 14:29:36Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2006, Romain Liévin, Kevin Kofler
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
    m68k VTi-style disassembler. Used ot replaced MAME's one.
	Code borrowed from UAE engine and adapted.	
*/

#ifdef NO_GDB

#include <stdio.h>
#include <string.h>
#include <glib.h>	// GUINT16_SWAP_LE_BE

#include "sysdeps.h"
#include "memory.h"
#include "newcpu.h"

#include "romcalls.h"
#include "handles.h"

#ifdef __WIN32__
#pragma warning( disable : 4244 )
#pragma warning( disable : 4761 )
#endif

#define get_ibyte_1(o) get_byte(regs.pc + (regs.pc_p - regs.pc_oldp) + (o) + 1)
#define get_iword_1(o) get_word(regs.pc + (regs.pc_p - regs.pc_oldp) + (o))
#define get_ilong_1(o) get_long(regs.pc + (regs.pc_p - regs.pc_oldp) + (o))

static char* ccnames[] =
{ "T","F","HI","LS","CC","CS","NE","EQ",
  "VC","VS","PL","MI","GE","LT","GT","LE" };

static long int m68kpc_offset;

static unsigned long REGPARAM2 op_illg_1 (uae_u32 opcode)
{
    op_illg (opcode);
    return 4;
}

uae_s32 ShowEA (FILE *f, int reg, amodes mode, wordsizes size, char *buf)
{
    uae_u16 dp;
    uae_s8 disp8;
    uae_s16 disp16;
    int r;
    uae_u32 dispreg;
    uaecptr addr;
    uae_s32 offset = 0;
    char buffer[80];

    switch (mode){
     case Dreg:
	sprintf (buffer,"D%d", reg);
	break;
     case Areg:
	sprintf (buffer,"A%d", reg);
	break;
     case Aind:
	sprintf (buffer,"(A%d)", reg);
	break;
     case Aipi:
	sprintf (buffer,"(A%d)+", reg);
	break;
     case Apdi:
	sprintf (buffer,"-(A%d)", reg);
	break;
     case Ad16:
	disp16 = get_iword_1 (m68kpc_offset); m68kpc_offset += 2;
	addr = m68k_areg(regs,reg) + (uae_s16)disp16;
	sprintf (buffer,"(A%d,$%04x) == $%08lx", reg, disp16 & 0xffff,
					(unsigned long)addr);
	break;
     case Ad8r:
	dp = get_iword_1 (m68kpc_offset); m68kpc_offset += 2;
	disp8 = dp & 0xFF;
	r = (dp & 0x7000) >> 12;
	dispreg = dp & 0x8000 ? m68k_areg(regs,r) : m68k_dreg(regs,r);
	if (!(dp & 0x800)) dispreg = (uae_s32)(uae_s16)(dispreg);
	dispreg <<= (dp >> 9) & 3;

	if (dp & 0x100) {
	    uae_s32 outer = 0, disp = 0;
	    uae_s32 base = m68k_areg(regs,reg);
	    char name[10];
	    sprintf (name,"A%d, ",reg);
	    if (dp & 0x80) { base = 0; name[0] = 0; }
	    if (dp & 0x40) dispreg = 0;
	    if ((dp & 0x30) == 0x20) { disp = (uae_s32)(uae_s16)get_iword_1 (m68kpc_offset); m68kpc_offset += 2; }
	    if ((dp & 0x30) == 0x30) { disp = get_ilong_1 (m68kpc_offset); m68kpc_offset += 4; }
	    base += disp;

	    if ((dp & 0x3) == 0x2) { outer = (uae_s32)(uae_s16)get_iword_1 (m68kpc_offset); m68kpc_offset += 2; }
	    if ((dp & 0x3) == 0x3) { outer = get_ilong_1 (m68kpc_offset); m68kpc_offset += 4; }

	    if (!(dp & 4)) base += dispreg;
	    if (dp & 3) base = get_long (base);
	    if (dp & 4) base += dispreg;

	    addr = base + outer;
	    sprintf (buffer,"(%s%c%d.%c*%d+%ld)+%ld == $%08lx", name,
		    dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W',
		    1 << ((dp >> 9) & 3),
		    disp,outer,
		    (unsigned long)addr);
	} else {
	  addr = m68k_areg(regs,reg) + (uae_s32)((uae_s8)disp8) + dispreg;
	  sprintf (buffer,"(A%d, %c%d.%c*%d, $%02x) == $%08lx", reg,
	       dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W',
	       1 << ((dp >> 9) & 3), disp8,
	       (unsigned long)addr);
	}
	break;
     case PC16:
	addr = m68k_getpc () + m68kpc_offset;
	disp16 = get_iword_1 (m68kpc_offset); m68kpc_offset += 2;
	addr += (uae_s16)disp16;
	sprintf (buffer,"(PC,$%04x) == $%08lx", disp16 & 0xffff,(unsigned long)addr);
	break;
     case PC8r:
	addr = m68k_getpc () + m68kpc_offset;
	dp = get_iword_1 (m68kpc_offset); m68kpc_offset += 2;
	disp8 = dp & 0xFF;
	r = (dp & 0x7000) >> 12;
	dispreg = dp & 0x8000 ? m68k_areg(regs,r) : m68k_dreg(regs,r);
	if (!(dp & 0x800)) dispreg = (uae_s32)(uae_s16)(dispreg);
	dispreg <<= (dp >> 9) & 3;

	if (dp & 0x100) {
	    uae_s32 outer = 0,disp = 0;
	    uae_s32 base = addr;
	    char name[10];
	    sprintf (name,"PC, ");
	    if (dp & 0x80) { base = 0; name[0] = 0; }
	    if (dp & 0x40) dispreg = 0;
	    if ((dp & 0x30) == 0x20) { disp = (uae_s32)(uae_s16)get_iword_1 (m68kpc_offset); m68kpc_offset += 2; }
	    if ((dp & 0x30) == 0x30) { disp = get_ilong_1 (m68kpc_offset); m68kpc_offset += 4; }
	    base += disp;

	    if ((dp & 0x3) == 0x2) { outer = (uae_s32)(uae_s16)get_iword_1 (m68kpc_offset); m68kpc_offset += 2; }
	    if ((dp & 0x3) == 0x3) { outer = get_ilong_1 (m68kpc_offset); m68kpc_offset += 4; }

	    if (!(dp & 4)) base += dispreg;
	    if (dp & 3) base = get_long (base);
	    if (dp & 4) base += dispreg;

	    addr = base + outer;
	    sprintf (buffer,"(%s%c%d.%c*%d+%ld)+%ld == $%08lx", name,
		    dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W',
		    1 << ((dp >> 9) & 3),
		    disp,outer,
		    (unsigned long)addr);
	} else {
	  addr += (uae_s32)((uae_s8)disp8) + dispreg;
	  sprintf (buffer,"(PC, %c%d.%c*%d, $%02x) == $%08lx", dp & 0x8000 ? 'A' : 'D',
		(int)r, dp & 0x800 ? 'L' : 'W',  1 << ((dp >> 9) & 3),
		disp8, (unsigned long)addr);
	}
	break;
     case absw:
	sprintf (buffer,"$%08lx", (unsigned long)(uae_s32)(uae_s16)get_iword_1 (m68kpc_offset));
	m68kpc_offset += 2;
	break;
     case absl:
	sprintf (buffer,"$%08lx", (unsigned long)get_ilong_1 (m68kpc_offset));
	m68kpc_offset += 4;
	break;
     case imm:
	switch (size){
	 case sz_byte:
	    sprintf (buffer,"#$%02x", (unsigned int)(get_iword_1 (m68kpc_offset) & 0xff));
	    m68kpc_offset += 2;
	    break;
	 case sz_word:
	    sprintf (buffer,"#$%04x", (unsigned int)(get_iword_1 (m68kpc_offset) & 0xffff));
	    m68kpc_offset += 2;
	    break;
	 case sz_long:
	    sprintf (buffer,"#$%08lx", (unsigned long)(get_ilong_1 (m68kpc_offset)));
	    m68kpc_offset += 4;
	    break;
	 default:
	    break;
	}
	break;
     case imm0:
	offset = (uae_s32)(uae_s8)get_iword_1 (m68kpc_offset);
	m68kpc_offset += 2;
	sprintf (buffer,"#$%02x", (unsigned int)(offset & 0xff));
	break;
     case imm1:
	offset = (uae_s32)(uae_s16)get_iword_1 (m68kpc_offset);
	m68kpc_offset += 2;
	sprintf (buffer,"#$%04x", (unsigned int)(offset & 0xffff));
	break;
     case imm2:
	offset = (uae_s32)get_ilong_1 (m68kpc_offset);
	m68kpc_offset += 4;
	sprintf (buffer,"#$%08lx", (unsigned long)offset);
	break;
     case immi:
	offset = (uae_s32)(uae_s8)(reg & 0xff);
	sprintf (buffer,"#$%08lx", (unsigned long)offset);
	break;
     default:
	break;
    }
    if (buf == 0)
	fprintf (f, "%s", buffer);
    else
	strcat (buf, buffer);
    return offset;
}

int DasmFPU(uint16_t code, char *buf);

int m68k_disasm (char *output, uaecptr addr)
{
    char buf[64];
    uaecptr newpc = 0;
	char instrname[20],*ccpt;
	uae_u32 opcode;
	struct mnemolookup *lookup;
	struct instr *dp;
	uaecptr nextpc;

    m68kpc_offset = addr - m68k_getpc ();
    output[0] = '\0';
	
	// print address
	sprintf (buf, "%06lx: ", m68k_getpc () + m68kpc_offset);
	strcat (output, buf);

	// get opcode
	opcode = get_iword_1 (m68kpc_offset);
	m68kpc_offset += 2;
	if (cpufunctbl[opcode] == op_illg_1) 
	    opcode = 0x4AFC;

	// and search for instruction
	dp = table68k + opcode;
	for (lookup = lookuptab; lookup->mnemo != dp->mnemo; lookup++);

	// search for branches
	strcpy (instrname, lookup->name);
	ccpt = strstr (instrname, "cc");
	if (ccpt != 0)
	    strncpy (ccpt, ccnames[dp->cc], 2);
	strcat (output, instrname);
	//printf("<%04x><%s> %i\n", opcode, instrname, m68k_getpc () + m68kpc_offset - addr);

	// set transfer size
	switch (dp->size)
	{
	 case sz_byte: strcat (output, ".B "); break;
	 case sz_word: strcat (output, ".W "); break;
	 case sz_long: strcat (output, ".L "); break;
	 default: strcat (output, "   "); break;
	}

	// set argument
	if (dp->suse) 
	{
	    newpc = m68k_getpc () + m68kpc_offset;
	    newpc += ShowEA (0, dp->sreg, dp->smode, dp->size, output);
	}
	if (dp->suse && dp->duse)
	{
	    strcat(output, ",");
	}
	if (dp->duse) 
	{
	    newpc = m68k_getpc () + m68kpc_offset;
	    newpc += ShowEA (0, dp->dreg, dp->dmode, dp->size, output);
	}
	if (ccpt != 0) 
	{
	    if (cctrue(dp->cc)) 
		{
			sprintf (buf, " == %08lx (TRUE)", newpc);
			strcat (output, buf);
	    } 
		else 
		{
			sprintf (buf, " == %08lx (FALSE)", newpc);
			strcat (output, buf);
	    }
	} 

	if ((opcode & 0xff00) == 0x6100) 
	{
		/* BSR */
	    sprintf (buf, " == %08lx", newpc);
	    strcat (output, buf);
	}
	else if((opcode >= 0xf800) && (opcode <= 0xfff2))
	{
		char *buffer = &(output[8]);
		unsigned long pm;
		uint32_t pc = m68k_getpc();	// addr

		// F-Line ROM calls (see KerNO doc and thanks to Lionel Debroux)
		switch(opcode)
		{
		case 0xfff0:	// 6 byte bsr w/long word displacement
			pm = get_ilong_1 (m68kpc_offset); m68kpc_offset += 6 - 4;
			if (pm & 0x8000)
				sprintf (buffer, "FLINE  bsr.l *-$%lX [%lX]", (-(signed long)(int32_t)pm) - 2, pc + (signed long)(int32_t)pm + 2);
			else
				sprintf (buffer, "FLINE  bsr.l *+$%lX [%lX]", pm + 2, pc + pm + 2);
			break;
		case 0xfff1:	// 6 byte bra w/long word displacement
			pm = get_ilong_1 (m68kpc_offset); m68kpc_offset += 6 - 4;
            if (pm & 0x8000)
				sprintf (buffer, "FLINE  bra.l *-$%lX [%lX]", (-(signed long)(int32_t)pm) - 2, pc + (signed long)(int32_t)pm + 2);
			else
				sprintf (buffer, "FLINE  bra.l *+$%lX [%lX]", pm + 2, pc + pm + 2);
			break;
		case 0xfff2:	// 4 byte ROM CALL
			pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 4;
			sprintf (buffer, "FLINE  $%04x.l [%s]", pm/4, romcalls_get_name(pm / 4));
			break;
		case 0xffee:	// jmp __ld_entry_point_plus_0x8000+word (branchement avec offset signé de 2 octets rajouté à (début du programme)+0x8000)
			pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 4;
			{
				int handle;
				uint32_t addr;
				
				heap_search_for_address(pc + (signed short)pm + 2 + 0x8000, &handle);
				heap_get_block_addr(handle, &addr);				
				sprintf (buffer, "FLINE  jmp.w *+$%lX [%lX]", (signed long)(signed short)pm + 0x8000, addr + 2 + (signed long)(signed short)pm + 0x8000);
			}
			break;
		case 0xffef:	// jsr __ld_entry_point_plus_0x8000+word (appel de fonction avec offset signé de 2 octets rajouté à (début du programme)+0x8000)
			pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 4;
			{
				int handle;
				uint32_t addr;
				
				heap_search_for_address(pc + (signed short)pm + 2 + 0x8000, &handle);
				heap_get_block_addr(handle, &addr);
				sprintf (buffer, "FLINE  jsr.w *+$%lX [%lX]", (signed long)(signed short)pm + 0x8000, addr + 2 + (signed long)(signed short)pm + 0x8000);
			}
			break;
		case 0xf8b5:	// 2 byte ROM call followed by an FPU opcode (special case: _bcd_math)
			{
				char tmp[64];
				pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 4;
				DasmFPU(pm, tmp);
				sprintf (buffer, "JSR  _bcd_math (FPU: %s)", tmp);
				break;
			}
		default:		// 2 byte ROM CALL
			sprintf (buffer, "FLINE  $%03x.w [%s]", opcode & 0x7ff, romcalls_get_name(opcode & 0x7ff));
			m68kpc_offset += 2 - 4;
			break;
		}
	}
	// ER_throw
	else if ((opcode & 0xf000) == 0xa000)
	{
		char *buffer = &(output[8]);
		sprintf (buffer, "ER_throw %d [%s]", opcode & 0xfff, ercodes_get_name(opcode & 0xfff));
		m68kpc_offset += 2 - 4;
	}

	nextpc = m68k_getpc () + m68kpc_offset;	
    return (nextpc - addr);
}

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

// do the same work as m68k_disasm but some UAE instructions 
// use a weird naming scheme, remap them!
int m68k_dasm(char **line, uint32_t addr)
{
	char output[256];
	int offset;
	gchar** split;
	int idx;

	offset = m68k_disasm(output, addr);

	// split string into address, opcode and operand
	split = g_strsplit(output, " ", 3);
	/*printf("%s %s%*c %s\n", 
			split[0], 
			split[1], 8 - strlen(split[1]), ' ', 
			split[2]);*/

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
			offset += 2;
			break;
		case 13:
			g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
			offset += 2;
			break;
        case 14:    /* MOVEM <list>,<ea>  */
        	g_free(split[1]);
			split[1] = g_strdup("MOVEM.W");			
			offset += 2;
            break;
        case 15:    /* MOVEM <list>,<ea>  */
            // UAE does not fully disasm this instruction	
            g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
			offset += 2;	
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

	return offset;
}

#endif
