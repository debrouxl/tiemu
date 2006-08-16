/* Target machine definitions for a generic m68k monitor/emulator.
   Copyright 1986, 1987, 1989, 1993, 1994, 1995, 1996, 1998, 1999, 2003
   Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* The definitions here are appropriate for several embedded m68k-based
   targets, including IDP (rom68k), BCC (cpu32bug), and EST's emulator.  */

/* GCC is probably the only compiler used on this configuration.  So
   get this right even if the code which detects gcc2_compiled. is
   still broken.  */

#define BELIEVE_PCC_PROMOTION 1

/* The target system handles breakpoints.  */

#define DECR_PC_AFTER_BREAK 0

/* No float registers.  */

/*#define NUM_REGS 18 */

/* FIXME, should do GET_LONGJMP_TARGET for newlib.  */

/* (TiEmu 20050401 Kevin Kofler) Hardware (TiEmu) breakpoints and watchpoints */
#define TARGET_HAS_HARDWARE_WATCHPOINTS 1
#define TARGET_CAN_USE_HARDWARE_WATCHPOINT(type, count, other) 1
#define TARGET_REGION_OK_FOR_HW_WATCHPOINT(addr, len) 1
#define TARGET_REGION_SIZE_OK_FOR_HW_WATCHPOINT(size) 1
#include <stdint.h>
extern int ti68k_bkpt_add_address(uint32_t address);
extern int ti68k_bkpt_del_address(uint32_t address);
extern void dbgbkpts_refresh_window(void);
#define target_remove_hw_breakpoint(bp_tgt) (ti68k_bkpt_del_address((bp_tgt)->placed_address),dbgbkpts_refresh_window(),0)
#define target_insert_hw_breakpoint(bp_tgt) (ti68k_bkpt_add_address((bp_tgt)->placed_address),dbgbkpts_refresh_window(),0)
extern int ti68k_bkpt_add_watchpoint(uint32_t address, uint32_t len, int type);
extern int ti68k_bkpt_del_watchpoint(uint32_t address, uint32_t len, int type);
extern int ti68k_bkpt_stopped_data_address(uint32_t *address);
extern int ti68k_bkpt_stopped_by_watchpoint(void);
static inline int target_stopped_data_address (CORE_ADDR *addr_p)
{
  uint32_t address = 0;
  int retval = ti68k_bkpt_stopped_data_address(&address);
  *addr_p = address;
  return retval;
}
#define target_remove_watchpoint ti68k_bkpt_del_watchpoint
#define target_insert_watchpoint ti68k_bkpt_add_watchpoint
#define HAVE_CONTINUABLE_WATCHPOINT 1
#define STOPPED_BY_WATCHPOINT(wait_status) ti68k_bkpt_stopped_by_watchpoint()
#define target_remove_breakpoint target_remove_hw_breakpoint
#define target_insert_breakpoint target_insert_hw_breakpoint

/* (TiEmu 20050401 Kevin Kofler) BCD floats */
#define TARGET_FLOAT_FORMAT (&floatformat_smapbcd_big)
#define TARGET_DOUBLE_FORMAT (&floatformat_smapbcd_big)
#define TARGET_LONG_DOUBLE_FORMAT (&floatformat_smapbcd_big)
