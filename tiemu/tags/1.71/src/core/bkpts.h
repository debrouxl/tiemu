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

// Breakpoints mode (ti68k_bkpt_set_[access|access_range])
#define BK_MODE_BYTE     1
#define BK_MODE_WORD     2
#define BK_MODE_LONG     4
#define BK_MODE_READ     16
#define BK_MODE_WRITE    32

// shortcuts and compat
#define BK_MODE_RW       (BK_READ | KB_WRITE)

#define BK_READ_BYTE	(BK_READ | BK_BYTE)
#define BK_READ_WORD 	(BK_READ | BK_WORD)
#define BK_READ_LONG 	(BK_READ | BK_LONG)

#define BK_WRITE_BYTE 	(BK_WRITE | BK_BYTE)
#define BK_WRITE_WORD 	(BK_WRITE | BK_WORD)
#define BK_WRITE_LONG 	(BK_WRITE | BK_LONG)

#define BK_BYTE     BK_MODE_BYTE
#define BK_WORD     BK_MODE_WORD
#define BK_LONG     BK_MODE_LONG
#define BK_READ     BK_MODE_READ
#define BK_WRITE    BK_MODE_WRITE

// Breakpoints type 
typedef enum {
    BK_TYPE_ACCESS=1, BK_TYPE_RANGE, 
    BK_TYPE_CODE, BK_TYPE_EXCEPTION,
} Ti68kBkptType;

// Breakpoints cause (ti68k_bkpt_get_cause())
typedef enum {
    BK_CAUSE_ACCESS=1, BK_CAUSE_RANGE, BK_CAUSE_ADDRESS,
    BK_CAUSE_EXCEPTION
} Ti68kBkptCause;

/* Functions */

void ti68k_bkpt_set_address(uint32_t address);
void ti68k_bkpt_set_access(uint32_t address, int mode);
void ti68k_bkpt_set_range(uint32_t min, uint32_t max, int mode);
void ti68k_bkpt_set_exception(int n);

void ti68k_bkpt_del_address(uint32_t address);
void ti68k_bkpt_del_access(uint32_t address, int mode);
void ti68k_bkpt_del_range(uint32_t min, uint32_t max, int mode);
void ti68k_bkpt_del_exception(int n);

void ti68k_bkpt_clear_address(void);
void ti68k_bkpt_clear_access(void);
void ti68k_bkpt_clear_range(void);
void ti68k_bkpt_clear_exception(void);

void ti68k_bkpt_set_cause(int type, int mode, int id);
void ti68k_bkpt_get_cause(int *type, int *id, int *mode);


#endif
