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

#ifndef __TI68K_INTLIST__
#define __TI68K_INTLIST__

struct intlist
{
  struct intlist *next;
  int	 val;
  int 	 val2;
};
typedef struct intlist INTLIST;

extern int getSize (struct intlist*);
extern void addEnd (struct intlist**, int);
extern void add2End(struct intlist**, int, int);
extern void addAt  (struct intlist**, int, int);
extern void add2At (struct intlist**, int, int, int);
extern void delList(struct intlist**);
extern void delAt  (struct intlist**, int);

#endif
