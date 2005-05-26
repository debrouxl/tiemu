/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - a TI emulator
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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

#ifndef __TIEMU_PRINTL_H__
#define __TIEMU_PRINTL_H__

#define	PRINT_STD	0
#define PRINT_WRN	1
#define PRINT_ERR	2

/* Definition */
typedef int (*TIEMU_PRINTL) (int level, const char *format, ...);

/* Callback */
extern TIEMU_PRINTL printl;

/* Functions */
TIEMU_PRINTL tiemu_set_printl(TIEMU_PRINTL new_printl);

int ticables_printl(int level, const char *format, ...);
int tifiles_printl(int level, const char *format, ...);
int ticalcs_printl(int level, const char *format, ...);
int tiemu_printl(int level, const char *format, ...);

int close_console(void);

#endif
