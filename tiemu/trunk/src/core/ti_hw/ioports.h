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

#ifndef __TI68K_IO__
#define __TI68K_IO__

int hw_io_init(void);
int hw_io_reset(void);
int hw_io_exit(void);

#define bit_get(v,b)	(((v) &  (1 << b)) >> b)
#define bit_set(v,b)	 ((v) |  (1 << b))
#define bit_clr(v,b)	 ((v) & ~(1 << b))

#define bit_tst(v,b)	 ((v) &  (1 << b))
#define bit_chg(v,b,s)	{ if(s) bit_set(v,b); else bit_clr(v,b); }

extern UBYTE io_get_byte(CPTR adr);
extern UWORD io_get_word(CPTR adr);
extern ULONG io_get_long(CPTR adr);

extern void io_put_long(CPTR adr, ULONG arg);
extern void io_put_word(CPTR adr, UWORD arg);
extern void io_put_byte(CPTR adr, UBYTE arg);

#define io_bit_get(a,b)		bit_get(tihw.io[a],b)
#define io_bit_set(a,b)		bit_set(tihw.io[a],b)
#define io_bit_clr(a,b)		bit_clr(tihw.io[a],b)

#define io_bit_tst(a,b)		bit_tst(tihw.io[a],b)
#define io_bit_chg(a,b,s)	bit_chg(tihw.io[a],b,s)	

extern UBYTE io2_get_byte(CPTR adr);
extern UWORD io2_get_word(CPTR adr);
extern ULONG io2_get_long(CPTR adr);

extern void io2_put_long(CPTR adr, ULONG arg);
extern void io2_put_word(CPTR adr, UWORD arg);
extern void io2_put_byte(CPTR adr, UBYTE arg);

#define io2_bit_get(a,b)		bit_get(tihw.io2[a],b)
#define io2_bit_set(a,b)		bit_set(tihw.io2[a],b)
#define io2_bit_clr(a,b)		bit_clr(tihw.io2[a],b)

#define io2_bit_tst(a,b)		bit_tst(tihw.io2[a],b)
#define io2_bit_chg(a,b,s)		bit_chg(tihw.io2[a],b,s)	


#endif
