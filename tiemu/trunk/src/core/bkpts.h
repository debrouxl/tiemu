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
  Breakpoint definitions
*/

#ifndef __TI68K_BKPTS__
#define __TI68K_BKPTS__

#include <glib.h>
#include <stdint.h>

/* Types */

typedef struct
{
  uint32_t	 val1;
  uint32_t 	 val2;
} ADDR_RANGE;

/* Constants */

// Constants for ti68k_bkpt_set_[access|access_range]()
#define BK_BYTE     1
#define BK_WORD     2
#define BK_LONG     4
#define BK_READ     16
#define BK_WRITE    32

#define BK_READ_BYTE	(BK_READ | BK_BYTE)
#define BK_READ_WORD 	(BK_READ | BK_WORD)
#define BK_READ_LONG 	(BK_READ | BK_LONG)

#define BK_WRITE_BYTE 	(BK_WRITE | BK_BYTE)
#define BK_WRITE_WORD 	(BK_WRITE | BK_WORD)
#define BK_WRITE_LONG 	(BK_WRITE | BK_LONG)

/*
// Constants for ti68k_bkpt_set_vector()
typedef enum {
    BK_NONE=-1, BK_RESET_SSP=0, BK_RESET_PC, BK_BUS_ERROR, BK_ADDRESS_ERROR,
    BK_ILLEGAL_INSTRUCTION, BK_ZERO_DIVIDE, BK_CHK_INSTRUCTION, BK_TRAPV_INSTRUCTION,
    BK_PRIVILEGE_VIOLATION, BK_TRACE, BK_LINE_1010, BK_LINE_1111, 
    BK_UNUSED1, BK_UNUSED2, BK_UNUSED3, BK_NONINIT_INTERRUPT
} Ti68kBkptVector;

// Constants for ti68k_bkpt_set_autoint()
typedef enum {
    BK_SPURIOUS=0, BK_AUTOINT_1, BK_AUTOINT_2, BK_AUTOINT_3, 
    BK_AUTOINT_4, BK_AUTOINT_5, BK_AUTOINT_6, BK_AUTOINT_7
} Ti68kBkptAutoint;

// Constants for ti68k_bkpt_set_trap
typedef enum {
    BK_TRAP_NONE=1, BK_TRAP_0=0, BK_TRAP_1, BK_TRAP_2, BK_TRAP_3, 
    BK_TRAP_4, BK_TRAP_5, BK_TRAP_6, BK_TRAP_7, 
    BK_TRAP_8, BK_TRAP_9, BK_TRAP_A, BK_TRAP_B, 
    BK_TRAP_C, BK_TRAP_D, BK_TRAP_E, BK_TRAP_F,
} Ti68kBkptTrap;
*/
// Breakpoints cause (ti68k_bkpt_get_cause())
typedef enum {
    BK_CAUSE_ACCESS=1, BK_CAUSE_RANGE, BK_CAUSE_ADDRESS,
    BK_CAUSE_VECTOR, BK_CAUSE_TRAP, BK_CAUSE_AUTOINT,
    BK_CAUSE_EXCEPTION
} Ti68kBkptCause;

/* Functions */

int ti68k_bkpt_set_address(int address);
int ti68k_bkpt_set_access(int address, int mode);
int ti68k_bkpt_set_access_range(int min, int max, int mode);
int ti68k_bkpt_set_exception(int n);

void ti68k_bkpt_del_address(int i);
void ti68k_bkpt_del_access(int i, int mode);
void ti68k_bkpt_del_access_range(int i, int mode);
void ti68k_bkpt_del_exception(int n);

void ti68k_bkpt_clear_address(void);
void ti68k_bkpt_clear_access(void);
void ti68k_bkpt_clear_access_range(void);
void ti68k_bkpt_clear_exception(void);

void ti68k_bkpt_get_cause(int *type, int *id, int *mode);


#endif
