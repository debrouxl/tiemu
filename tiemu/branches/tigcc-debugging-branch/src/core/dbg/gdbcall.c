/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - a TI emulator
 *
 *  This file Copyright (c) 2005, Kevin Kofler
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
    Gdbcall: GDB interfacing functions
*/

#include "gdbcall.h"

#include <stdio.h>

/* These come from GDB's defs.h, which I don't want to include, because it
   pulls in many other headers from directories not even in our include
   path. */
#define RETURN_MASK_ALL -1
typedef int return_mask;
typedef void (catch_command_errors_ftype) (const char *, int);
extern int catch_command_errors (catch_command_errors_ftype *func, const char *command, int from_tty, return_mask);
#include "../gdb/gdb/top.h"

static void gdbcall_exec_command(const char *command_str)
{
  catch_command_errors (execute_command, command_str, 0, RETURN_MASK_ALL); 
}

void gdbcall_continue(void)
{
  gdbcall_exec_command("c");
}
