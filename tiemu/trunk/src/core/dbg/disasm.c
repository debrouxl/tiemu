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

/* GDB disabled: use this built-in disassembler (taken from UAE) */

#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "libuae.h"
#include "romcalls.h"
#include "handles.h"

//--- Disassembler starts here ---

#ifdef __WIN32__
# define snprintf	_snprintf
#endif

#define get_ibyte_1(o) get_byte(regs.pc + (regs.pc_p - regs.pc_oldp) + (o) + 1)
#define get_iword_1(o) get_word(regs.pc + (regs.pc_p - regs.pc_oldp) + (o))
#define get_ilong_1(o) get_long(regs.pc + (regs.pc_p - regs.pc_oldp) + (o))

extern long int m68kpc_offset;

char *sym_addr(uae_u32 addr)
{
	static char buf[256];
	int rcid = romcalls_is_addr(addr);

	if(rcid == -1)
		snprintf(buf, sizeof(buf), "$%06lX", (unsigned long)addr);
	else
		snprintf(buf, sizeof(buf), "$%06lX -> tios::%s", (unsigned long)addr, romcalls_get_name(rcid)); 

	return buf;
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
    char buffer[296];

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
	if (disp16 & 0x8000)
		sprintf (buffer,"(-$%X,A%d) [%s]", (-(uae_s16)(disp16)) & 0xffff, reg, sym_addr ((uae_u32)addr));
	else
		sprintf (buffer,"(+$%X,A%d) [%s]", disp16 & 0xffff, reg, sym_addr ((uae_u32)addr));
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
	    sprintf (buffer,"(%s%c%d.%c+%ld)+%ld [$%06lX]", name,
		    dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W',
		    disp,outer,
		    (unsigned long)addr);
	} else {
	  addr = m68k_areg(regs,reg) + (uae_s32)((uae_s8)disp8) + dispreg;
	  sprintf (buffer,"(A%d, %c%d.%c, $%02X) [$%06lX]", reg,
	       dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W',
	       disp8,
	       (unsigned long)addr);
	}
	break;
     case PC16:
	addr = m68k_getpc () + m68kpc_offset;
	disp16 = get_iword_1 (m68kpc_offset); m68kpc_offset += 2;
	addr += (uae_s16)disp16;
	if (disp16 & 0x8000)
		sprintf (buffer,"(-$%X,PC) [%s]", (-(uae_s16)(disp16)) & 0xffff, sym_addr ((uae_u32)addr));
	else
		sprintf (buffer,"(+$%X,PC) [%s]", disp16 & 0xffff, sym_addr ((uae_u32)addr));
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
	    sprintf (buffer,"(%s%c%d.%c+%ld)+%ld [%s]", name,
		    dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W',
		    disp,outer,
		    sym_addr ((uae_u32)addr));
	} else {
	  addr += (uae_s32)((uae_s8)disp8) + dispreg;
	  sprintf (buffer,"(PC, %c%d.%c, $%02X) [%s]", dp & 0x8000 ? 'A' : 'D',
		(int)r, dp & 0x800 ? 'L' : 'W',
		disp8, sym_addr ((uae_u32)addr));
	}
	break;
     case absw:
	sprintf (buffer,"$%lX", (unsigned long)(uae_s32)(uae_s16)get_iword_1 (m68kpc_offset));
	m68kpc_offset += 2;
	break;
     case absl:
	sprintf (buffer,"%s", sym_addr ((uae_u32)get_ilong_1 (m68kpc_offset)));
	m68kpc_offset += 4;
	break;
     case imm:
	switch (size){
	 case sz_byte:
	    sprintf (buffer,"#$%X", (unsigned int)(get_iword_1 (m68kpc_offset) & 0xff));
	    m68kpc_offset += 2;
	    break;
	 case sz_word:
	    sprintf (buffer,"#$%X", (unsigned int)(get_iword_1 (m68kpc_offset) & 0xffff));
	    m68kpc_offset += 2;
	    break;
	 case sz_long:
	    sprintf (buffer,"#$%lX", (unsigned long)(get_ilong_1 (m68kpc_offset)));
	    m68kpc_offset += 4;
	    break;
	 default:
	    break;
	}
	break;
     case imm0:
	offset = (uae_s32)(uae_s8)get_iword_1 (m68kpc_offset);
	m68kpc_offset += 2;
	sprintf (buffer,"#$%X", (unsigned int)(offset & 0xff));
	break;
     case imm1:
	offset = (uae_s32)(uae_s16)get_iword_1 (m68kpc_offset);
	m68kpc_offset += 2;
	sprintf (buffer,"#$%X", (unsigned int)(offset & 0xffff));
	break;
     case imm2:
	offset = (uae_s32)get_ilong_1 (m68kpc_offset);
	m68kpc_offset += 4;
	sprintf (buffer,"#$%lX", (unsigned long)offset);
	break;
     case immi:
	offset = (uae_s32)(uae_s8)(reg & 0xff);
	sprintf (buffer,"#$%lX", (unsigned long)offset);
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

static unsigned long REGPARAM2 op_illg_1 (uae_u32 opcode)
{
    op_illg (opcode);
    return 4;
}

static char* ccnames[] =
{ 
	"RA","F","HI","LS","CC","CS","NE","EQ",
	"VC","VS","PL","MI","GE","LT","GT","LE" 
};

int DasmFPU(uint16_t code, char *buf);

int m68k_disasm (char *output, uaecptr addr)
{
    char buf[273];
    uaecptr newpc = 0;

	char instrname[20],*ccpt;
	uae_u32 opcode;
	struct mnemolookup *lookup;
	struct instr *dp;
	uae_u32 orig_opcode;
	uaecptr nextpc;

	m68kpc_offset = addr - m68k_getpc ();
	output[0] = '\0';

	sprintf (buf, "%06lx: ", m68k_getpc () + m68kpc_offset);
	strcat (output, buf);
	opcode = get_iword_1 (m68kpc_offset);
	orig_opcode = opcode;
	m68kpc_offset += 2;
	if (cpufunctbl[opcode] == op_illg_1 || (orig_opcode & 0xf000) == 0xa000 || (orig_opcode & 0xf000) == 0xf000) {
	    opcode = 0x4AFC;
	}
	dp = table68k + opcode;
	for (lookup = lookuptab; lookup->mnemo != (int)dp->mnemo; lookup++)
	    ;

	strcpy (instrname, lookup->name);
	ccpt = strstr (instrname, "cc");
	if (ccpt != 0) {
	    strncpy (ccpt, ccnames[dp->cc], 2);
	}
	strcat (output, instrname);
	switch (dp->size)
	{
	 case sz_byte: strcat (output, ".B "); break;
	 case sz_word: strcat (output, ".W "); break;
	 case sz_long: strcat (output, ".L "); break;
	 default: strcat (output, "   "); break;
	}

	if (dp->suse) {
	    newpc = m68k_getpc () + m68kpc_offset;
	    newpc += ShowEA (0, dp->sreg, dp->smode, dp->size, output);
	}
	if (dp->suse && dp->duse)
	    strcat(output, ",");
	if (dp->duse) {
	    newpc = m68k_getpc () + m68kpc_offset;
	    newpc += ShowEA (0, dp->dreg, dp->dmode, dp->size, output);
	}
	if (ccpt != 0) {
	    if (cctrue(dp->cc)) {
		sprintf (buf, " [%06lX] (TRUE)", newpc);
		strcat (output, buf);
	    } else {
		sprintf (buf, " [%06lX] (FALSE)", newpc);
		strcat (output, buf);
	    }
	} else if ((opcode & 0xff00) == 0x6100) { /* BSR */
	    sprintf (buf, " [%s]", sym_addr (newpc));
	    strcat (output, buf);
	} else if((orig_opcode >= 0xf800) && (orig_opcode <= 0xfff2)) {
		char *buffer = &(output[8]);
		unsigned long pm;
		uint32_t pc = m68k_getpc();	/* addr */

		/* F-Line ROM calls (see KerNO doc and thanks to Lionel Debroux) */
		switch(orig_opcode)
		{
		case 0xfff0:	/* 6 byte bsr w/long word displacement */
			pm = get_ilong_1 (m68kpc_offset); m68kpc_offset += 6 - 2;
			if (pm & 0x8000)
				sprintf (buffer, "FLINE bsr.l *-$%lX [%lX]", (-(signed long)(int32_t)pm) - 2, pc + (signed long)(int32_t)pm + 2);
			else
				sprintf (buffer, "FLINE bsr.l *+$%lX [%lX]", pm + 2, pc + pm + 2);
			break;
		case 0xfff1:	/* 6 byte bra w/long word displacement */
			pm = get_ilong_1 (m68kpc_offset); m68kpc_offset += 6 - 2;
			if (pm & 0x8000)
				sprintf (buffer, "FLINE bra.l *-$%lX [%lX]", (-(signed long)(int32_t)pm) - 2, pc + (signed long)(int32_t)pm + 2);
			else
				sprintf (buffer, "FLINE bra.l *+$%lX [%lX]", pm + 2, pc + pm + 2);
			break;
		case 0xfff2:	/* 4 byte ROM CALL */
			pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 2;
			sprintf (buffer, "FLINE $%04x.l [%s]", pm/4, romcalls_get_name(pm / 4));
			break;
		case 0xffee:	/* jmp __ld_entry_point_plus_0x8000+word */
			pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 2;
			{
				int handle;
				uint32_t addr;
				
				heap_search_for_address(pc + 2, &handle);
				if (handle > 0) heap_get_block_addr(handle, &addr); else addr = 0;
				sprintf (buffer, "FLINE jmp.w *+$%lX [%lX]", (signed long)(signed short)pm + 0x8000, addr + (signed long)(signed short)pm + 0x8000);
			}
			break;
		case 0xffef:	/* jsr __ld_entry_point_plus_0x8000+word */
			pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 2;
			{
				int handle;
				uint32_t addr;
				
				heap_search_for_address(pc + 2, &handle);
				if (handle > 0) heap_get_block_addr(handle, &addr); else addr = 0;
				sprintf (buffer, "FLINE jsr.w *+$%lX [%lX]", (signed long)(signed short)pm + 0x8000, addr + (signed long)(signed short)pm + 0x8000);
			}
			break;
		case 0xf8b5:	/* 2 byte ROM call followed by an FPU opcode (special case: _bcd_math) */
			{
				char tmp[64];
				pm = get_iword_1 (m68kpc_offset); m68kpc_offset += 4 - 2;
				DasmFPU((uint16_t)pm, tmp);
				sprintf (buffer, "FLINE _bcd_math (FPU: %s)", tmp);
				break;
			}
		default:		/* 2 byte ROM CALL */
			sprintf (buffer, "FLINE $%03x.w [%s]", opcode & 0x7ff, romcalls_get_name(opcode & 0x7ff));
			break;
		}
	} else if ((orig_opcode & 0xf000) == 0xa000) { /* ER_throw */
		char *buffer = &(output[8]);
		sprintf (buffer, "ER_throw %d [%s]", opcode & 0xfff, ercodes_get_name(opcode & 0xfff));
	} else if (opcode == 0x4AFC && orig_opcode != 0x4AFC) { /* illegal instruction, but not ILLEGAL */
		sprintf (output, "%06lx: DC.W $%04X", addr, orig_opcode);
		
	}

	nextpc = m68k_getpc () + m68kpc_offset;	
	return (nextpc - addr);
}

//--- End of disassembler ---

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
	"MOVE.L",				/* MOVEQ #i,Dr			*/
	"OR.", "AND.", "EOR.",	/* ORI/ANDI/EORI		*/
	"ADD.", "SUB.", "CMP.",	/* ADDI/SUBI/CMPI		*/

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
// UAE engine use a weird/wrong naming scheme wo we remap them here rather than
// touching the newcpu.c file because this file may be updated when upgrading
// the UAE engine.
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
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
			{
				char *p = strchr(split[1], '.');
				if(p) *p = '\0';
			}
			break;
		case 28:	/* MOVEQ */
			if(split[2][0] == '#')
			{
				g_free(split[1]);
				split[1] = g_strdup("MOVEQ");
			}
			break;

		case 29:	/* ORI/ANDI/EORI */
		case 30:
		case 31:
		case 32:	/* ADDI/SUBI/CMPI */
		case 33:
		case 34:
			if(split[2][0] == '#')
			{
				char *p = strchr(split[1], '.');
				char q = p[1];
				
				split[1] = g_realloc(split[1], strlen(split[1]) + 2);

				*p++ = 'I';
				*p++ = '.';
				*p++ = q;
				*p++ = '\0';
			}

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

uint32_t ti68k_debug_disassemble(uint32_t addr, char **line)
{
	uint32_t offset;

	offset = m68k_dasm(line, addr);

	return offset;
}
#endif
