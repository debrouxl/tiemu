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

/*
    To remove...
*/
typedef struct
{
  int	 val;
  int 	 val2;
} ADDR_RANGE;


/* Constants */

// Bkpts types (access)
#define BK_BYTE     1
#define BK_WORD     2
#define BK_LONG     4
#define BK_READ     16
#define BK_WRITE    32

#define BK_READ_BYTE (BK_READ | BK_BYTE)
#define BK_READ_WORD (BK_READ | BK_WORD)
#define BK_READ_LONG (BK_READ | BK_LONG)
#define BK_READ_LONG_WORD BK_READ_LONG
#define BK_WRITE_BYTE (BK_WRITE | BK_BYTE)
#define BK_WRITE_WORD (BK_WRITE | BK_WORD)
#define BK_WRITE_LONG (BK_WRITE | BK_LONG)
#define BK_WRITE_LONG_WORD BK_WRITE_LONG

// Constants for setBreakpointVector()
#define BK_NONE                -1
#define BK_BUS_ERROR           2
#define BK_ADDRESS_ERROR       3
#define BK_ILLEGAL_INSTRUCTION 4
#define BK_ZERO_DIVIDE         5
#define BK_CHK_INSTRUCTION     6
#define BK_TRAPV_INSTRUCTION   7
#define BK_PRIVILEGE_VIOLATION 8
#define BK_TRACE               9
#define BK_LINE_1010           10
#define BK_LINE_1111           11
#define BK_NONINIT_INTERRUPT   15

// Constants for setBreakpointAutoint()
#define BK_SPURIOUS  0
#define BK_AUTOINT_1 1
#define BK_AUTOINT_2 2
#define BK_AUTOINT_3 3
#define BK_AUTOINT_4 4
#define BK_AUTOINT_5 5
#define BK_AUTOINT_6 6
#define BK_AUTOINT_7 7

// Constants for setBreakpointTrap()
#define BK_TRAP_NONE -1
#define BK_TRAP_0 0
#define BK_TRAP_1 1
#define BK_TRAP_2 2
#define BK_TRAP_3 3
#define BK_TRAP_4 4
#define BK_TRAP_5 5
#define BK_TRAP_6 6
#define BK_TRAP_7 7
#define BK_TRAP_8 8
#define BK_TRAP_9 9
#define BK_TRAP_A 10
#define BK_TRAP_B 11
#define BK_TRAP_C 12
#define BK_TRAP_D 13
#define BK_TRAP_E 14
#define BK_TRAP_F 15

// Breakpoints cause (get with getBkptCause())
#define BK_CAUSE_ACCESS       1
#define BK_CAUSE_ACCESS_RANGE 2
#define BK_CAUSE_ADDRESS      3
#define BK_CAUSE_VECTOR       4
#define BK_CAUSE_TRAP         5
#define BK_CAUSE_AUTOINT      6

/*
  Variables
*/

// Memory
extern int breakMode;
extern int breakType;
extern int breakId;

typedef struct
{
	int	mode;
	int	type;
	int id;
} MEM_BKPT;

extern MEM_BKPT mem_bkpt;

extern GList *listBkptAsRB;
extern GList *listBkptAsWB;
extern GList *listBkptAsRW;
extern GList *listBkptAsWW;
extern GList *listBkptAsRL;
extern GList *listBkptAsWL;

extern GList *listBkptAsRgR;
extern GList *listBkptAsRgW;

// Code
extern GList *listBkptAddress;

// Vectors, AutoInts and Traps
extern int listBkptVector[16];
extern int listBkptAutoint[8];
extern int listBkptTrap[16];

extern int nBkptVector;
extern int nBkptAutoint;
extern int nBkptTrap;

/* Functions */

int ti68k_bkpt_set_address(int address);
int ti68k_bkpt_set_access(int address, int mode);
int ti68k_bkpt_set_access_range(int addressMin, int addressMax, int mode);
int ti68k_bkpt_set_vector(int vector);
int ti68k_bkpt_set_autointoint(int autoint);
int ti68k_bkpt_set_trapTrap(int trap);

void ti68k_bkpt_del_address(int i);
void ti68k_bkpt_del_access(int i, int mode);
void ti68k_bkpt_del_access_range(int i, int mode);
void ti68k_bkpt_del_vector(int i);
void ti68k_bkpt_del_autoint(int i);
void ti68k_bkpt_del_trap(int i);

void ti68k_bkpt_clear_address(void);
void ti68k_bkpt_clear_access(void);
void ti68k_bkpt_clear_access_range(void);
void ti68k_bkpt_clear_vector(void);
void ti68k_bkpt_clear_autoint(void);
void ti68k_bkpt_clear_trap(void);



#endif
