/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 245 2004-05-23 20:45:43Z roms $ */

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

#ifndef __BITS__
#define __BITS__

#if 0   // linkport is broken

#define bit_get(v,b)	(((v) & (1 << (b))) >> (b))
#define bit_set(v,b)	v |=  (1 << (b));
#define bit_clr(v,b)	v &= ~(1 << (b));

#define bit_tst(v,b)    ((v) &  (1 << b))
#define bit_chg(v,b,s)	{                           \
                            if(s)                   \
                                { bit_set(v,b); }   \
                            else                    \
                                { bit_clr(v,b); }   \
                        }

#else

#define bit_get(v,b)	(((v) &  (1 << b)) >> b)
#define bit_set(v,b)	 ((v) |  (1 << b))
#define bit_clr(v,b)	 ((v) & ~(1 << b))

#define bit_tst(v,b)	 ((v) &  (1 << b))
#define bit_chg(v,b,s)	{ if(s) bit_set(v,b); else bit_clr(v,b); }

#endif

#endif
