/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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
    Interface: exported & misc routines
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "libuae.h"
#include "hw.h"
#include "m68k.h"
#include "dbus.h"

#include "ti68k_int.h"
#include "ti68k_err.h"


/**********************/
/* Internal variables */
/**********************/


Ti68kParameters     params = { 0 };
Ti68kHardware       tihw = { 0 };
TicableLinkParam    link_cable = { 0 };
Ti68kBreakpoints	bkpts = { 0 };


/***********************************/
/* Entry points for initialization */
/***********************************/


/*
  Initialization order (checked by the runlevel):
  - register callbacks
  - load the default config
  - load a ROM (init ROM)
  - init the lib (init HW, UAE, HID)
  - reset the lib
*/

/* 
   This should be the FIRST function to call for registering
   HID callbacks.
   The callbacks will not be used until ti68k_init().
*/
void ti68k_gui_set_callbacks(
              callback_iv_t initSpecific,
			  callback_iv_t exitSpecific,
			  callback_iv_t updateScreen,
			  callback_iv_t updateKeys,
			  callback_vi_t screenOnOff,
			  callback_ii_t setContrast
			  );

/*
  This should be the SECOND function to call (unless the 'params' 
  structure has been properly initialized.
 */
int ti68k_config_load_default(void)
{
    params.background = 1;
    params.grayplanes = 2;

    params.restricted = 1;
    params.cpu_rate = -1;
    params.hw_rate = -1;
    params.lcd_rate = -1;

    ticable_get_default_param(&link_cable);
    link_cable.link_type = LINK_NUL;
    link_cable.port = NULL_PORT;

    return 0;
}

/*
  This should be the THIRD function to call. 
  Load a ROM image (images.c).
*/
int ti68k_load_image(const char *filename);

/*
  This is the FOURTH function to call for completely initializing the
  emulation engine.
*/
int ti68k_init(void)
{
	// init libs
    ticable_init();
	tifiles_init();
	ticalc_init();

	// check if image has been loaded
	if(img_loaded == 0)
		return ERR_NO_IMAGE;

	// set calc type and init hardware
    tihw.calc_type = img_infos.calc_type;
	TRY(hw_init());

	// init hid
	if(cb_init_specific() != 0)
		return ERR_HID_FAILED;
	cb_screen_on_off(!0);

	return 0;
}

/*
  This should be the FIFTH function to call.
  It simply resets the hardware engine.
*/
int ti68k_reset(void)
{
	hw_reset();

	return 0;
}

/*
  Close the library by exiting HID and exiting the emulation engine
  (free ressources).
*/
int ti68k_exit(void)
{
    if(cb_exit_specific() != 0)
		return ERR_HID_FAILED;
    TRY(hw_exit());

    ticable_exit();
	tifiles_exit();
	ticalc_exit();

	return 0;
}

/*
    Full restart of the library (not tested).
*/
int ti68k_restart(void)
{
  TRY(ti68k_exit());
  TRY(ti68k_init());
  TRY(ti68k_reset());
  
  return 0;
}


/***********************/
/* Debugging functions */
/***********************/

int ti68k_debug_get_pc(void)
{
	return m68k_getpc();
}

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

	MC68000_disasm(addr, &next, 1, output);
	output[strlen(output)-1] = '\0'; // strip CR-LF

	// remove extra space as in 'BT .B' instead of BT.B
	tok = strstr(output, " .");
	if(tok)
		memcpy(tok, tok+1, strlen(tok));
	//printf("<%s>\n", output);
	
	// split string into address, opcode and operand
	split = g_strsplit(output, " ", 3);
	printf("%s %s%*c %s\n", 
			split[0], 
			split[1], 8 - strlen(split[1]), ' ', 
			split[2]);

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
        case 14:    /* MOVEM <list>,<ea>  */
        {	// UAE does not fully disasm this instruction
			/*
        	uint16_t d;
			
            g_free(split[1]);
			split[1] = g_strdup("MOVEM.W");
			
			d = *((uint16_t *)get_real_address(next));
			printf("%04X\n", d);
			//???
			next += 2;	
			*/
		}
            break;
        case 13:    /* MOVEM <ea>,<list>  */
        case 15:    /* MOVEM <list>,<ea>  */
            g_free(split[1]);
			split[1] = g_strdup("MOVEM.L");
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

int ti68k_debug_break(void)
{
    specialflags |= SPCFLAG_BRK;
    return 0;
}

int ti68k_debug_step(void)
{
    // Set up an internal trap (DBTRACE) which will 
    // launch/refresh the debugger when encountered
    specialflags |= SPCFLAG_DBTRACE;

    return 0;
}

int ti68k_debug_skip(uint32_t next_pc)
{
    broken_in = 0;
    //specialflags |= SPCFLAG_BRK;

    do 
    {
        specialflags |= SPCFLAG_DBTRACE;
		ti68k_engine_unhalt();

		// too far: stop
		if(m68k_getpc() > next_pc)
			break;
		// jump back: stop
		if(next_pc - m68k_getpc() > 0x80)
			break;
    } 
    while (next_pc != m68k_getpc() && !broken_in);

    return 0;
}

/*
    Do 'n' instructions.
    Return 0 if successful, a negative value if an error occured,
    a positive value if a breakpoint has been encountered.

    Note: replace M68000_run() from UAE
*/
int ti68k_debug_do_instructions(int n)
{
    if(!img_loaded)
        return ERR_NO_IMAGE;

    return hw_m68k_run(n);
}


/******************/
/* Link functions */
/******************/

/*
    Remark: there is a bug here... If the buffer is allocated with malloc, GtkTiEmu will 
    segfault (backBuf address out of bounds). If the buffer is allocated on the heap 
    as an array, there will be no problem. Moreover, this problem does not appear when we send 
    a file via the command mode, only in GUI mode.
    Then, there may be a problem of shared memory or something else...
*/
int ti68k_linkport_send_file(const char *filename)
{
    return send_ti_file(filename);
}

int ti68k_linkport_reconfigure(void)
{
    TRY(hw_dbus_init());
    TRY(hw_dbus_exit());

    return 0;
}

/******************/
/* Misc functions */
/******************/

const int ti_rom_sizes[] = { 1*MB, 2*MB, 2*MB, 4*MB, 4*MB };	// 92, 89, 92+, V200, TI89 Titanium
const int ti_ram_sizes[] = { 256*KB, 256*KB, 256*KB, 256*KB, 256*KB }; // should be 128

static int log_b2(int i)
{
    int j, v;

    for(j = 0, v = i; v != 0; v >>= 1, j++);

    return j-1;
}

int ti68k_get_rom_size(int calc_type)
{
    if(calc_type > CALC_MAX)
    {
        DISPLAY("Bad argument !\n");
        exit(0);
    }

    return ti_rom_sizes[log_b2(calc_type)];
}

int ti68k_get_ram_size(int calc_type)
{
    if(calc_type > CALC_MAX)
    {
        DISPLAY("Bad argument !\n");
        exit(0);
    }

    return ti_ram_sizes[log_b2(calc_type)];
}
