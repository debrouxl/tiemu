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

/*
    TI's ASIC management: memory mapped I/O ports
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#include "uae.h"
#include "memory.h"
#include "keyboard.h"
#include "dbus.h"
#include "callbacks.h"
#include "ti68k_def.h"

int hw_io_init(void)
{
    tihw.io0Bit2=1; 
    tihw.io0Bit7=1;
}

int hw_io_reset(void)
{
    return 0;
}

int hw_io_exit(void)
{
    return 0;
}

#define bit_get(i,b)	(((b) & (1 << i)) >> i)
#define bit_set(i,b)	((b) &  (1 << i))
#define bit_clr(i,b)	((b) | ~(1 << i))

void io_put_byte(CPTR adr, UBYTE arg)
{
    switch(adr) 
    {
        case 0x00:	// rw <76...2..>
            tihw.contrast=(tihw.contrast&(~1))|((arg>>5)&1);
            if(tihw.calc_type != TI92)
	            cb_set_contrast(tihw.contrast); // avoid flickering with 92
            tihw.io0Bit7=(arg>>7)&1;
            tihw.io0Bit2=(arg>>2)&1;
        break;
        case 0x01:	// rw <.....2.0>
			// clr: interleave RAM (allows use of 256K of RAM)
			tihw.ram256 = bit_get(0, arg);
			//mem_mask[0] = tihw.ram256 ? 0x1fffff : 0x3fffff;

			// set: protected memory violation triggered when memory below [$000120] is written
			tihw.mem_prot = bit_get(2, arg);
	    break;
	    case 0x02:	// ??
	    break;
		case 0x03:	// -w <.654.210>
			break;
        case 0x04:
        break;
        case 0x05:	// -w <...43210>
        	// set: 000000..1FFFFF mapped to 200000..3FFFFF
			tihw.ram_wrap = bit_get(3, arg);
            //mem_tab[2] = mem_tab[0];
            //mem_tab[3] = mem_tab[1];
            
			if (!(arg&0x10)) specialflags |= SPCFLAG_STOP; 
            break;
        case 0x06: case 0x07: case 0x08: case 0x09: case 0x0a: case 0x0b:
        break;
        case 0x0c:	// rw <765.3210>
            lc_raw_access=(arg>>6)&1;
        break;
        case 0x0d:	// r- <76543210>
			break;
        case 0x0e:	// rw <....3210>
			// write link port w/ direct access
            lc_raw_access=(arg>>6)&1;
            if (lc_raw_access)
	        {
	            lc.set_red_wire(arg&1);
	            lc.set_white_wire((arg>>1)&1);
	        }
        break;
        case 0x0f: 	// rw <76543210>
        //tx buffer
            linkport_putbyte(arg);
            break;
        case 0x10: 	// -w <76543210> (hw1)
			// address of LCD memory divided by 8
            tihw.lcd_addr = (((tihw.lcd_addr >> 3) & 0xff) | 
						((arg & 0xff) << 8)) << 3;
        break;
        case 0x11: 	// -w <76543210> (hw1)
			// address of LCD memory divided by 8
            tihw.lcd_addr=(((tihw.lcd_addr >> 3) & 0xff00) |
						(arg & 0xff)) << 3;
        break;
        case 0x12:	// -w <76543210>
        break;
        case 0x13:	// -w <..543210>
        break;
        case 0x14:
        break;
        case 0x15:	// rw <7.6543210>
        break;
        case 0x16:
        break;
        case 0x17: 	// rw <76543210>
            tihw.timer_init = arg; 
            break;
        case 0x18:	// rw <76543210>
        break;    
        case 0x19:	// rw <......10>
        break;
        case 0x1a:	// rw <......10> <76543210>
        break;
        case 0x1b:	// r- <76543210>
        break;
        case 0x1c:	// -w <..5432..>
            tihw.lcd_off=(arg&0x80)>>7;
            if((arg&0x80)>>7)
	        {
	            specialflags |= SPCFLAG_STOP;
	            cb_screen_on_off(0);
	        }
            else
	            cb_screen_on_off(!0);
        break;
        case 0x1d:	// -w <7..43210>
            tihw.contrast=(tihw.contrast&1)|((arg&15)<<1);
            cb_set_contrast(tihw.contrast);
        break;
        case 0x1e:
        break;
        case 0x1f:
        break;
    }
  
    tihw.io[adr] = arg;
}

void io_put_word(CPTR adr, UWORD arg) 
{
    io_put_byte(adr,   arg>>8);
    io_put_byte(adr+1, arg&0xff);
}

void io_put_long(CPTR adr, ULONG arg) 
{
    io_put_word(adr,   arg>>16);
    io_put_word(adr+2, arg&0xffff);
}

UBYTE io_get_byte(CPTR adr) 
{
    int v;

    switch(adr) 
    {
        case 0x00:	// rw <76...2..>
            v=((tihw.contrast&1)<<5)|(tihw.io0Bit7<<7)|(tihw.io0Bit2<<2);
            tihw.io0Bit2=1;
            return v|0x4;
        case 0x01:	// rw <.....2.0>
			// interleave RAM (allows use of 256K of RAM)
			// protected memory violation triggered when memory below [$000120] is written
            return (tihw.mem_prot << 2) | tihw.ram256;
        case 0x02:
        	return 0x14;
        case 0x03:	// -w <.654.210>
        break;
        case 0x04:	// ??
        break;        
        case 0x05:	// -w <...43210>
        break;
        case 0x06:  // ??
        case 0x07: 
        case 0x08: 
        case 0x09: 
        case 0x0a: 
        case 0x0b:
        return 0x14;
        case 0x0c:	// rw <765.3210>
         //link status
            //link bug here !!!
            //return 5|((1-transflag)<<1)|(lc_raw_access?0x40:0);
        break;
        case 0x0d:	// r- <76543210>
            /* 0x40 -> always return tx buffer as empty */
            return (linkport_byteavail() ? 0x60 : 0x40);
        case 0x0e:	// rw <....3210>
            if (lc_raw_access)
	            return 0x50|(lc.get_white_wire()<<1)|lc.get_red_wire();
            break;
        case 0x0f: 	// rw <76543210>
            recvflag = 0;
            recvbyte = linkport_getbyte();
            return recvbyte;
        case 0x10: 	// -w <76543210> (hw1)
        break;
        case 0x11: 	// -w <76543210> (hw1) 
        return 0x14;
        case 0x12: 	// -w <76543210>
        return 0x14;
        case 0x13: 	// -w <..543210>
        return 1;
        case 0x14:	// ??
        return 0x14;
        case 0x15:	// rw <7.6543210> 
        return 0x1b;
        case 0x16:	// ??
        return 0x14;
        case 0x17: 	// rw <76543210>
        return tihw.timer_value;
        case 0x18: 	// rw <76543210>
        break;
        case 0x19:	// rw <......10>
        break;
        case 0x1a:	// rw <......10> <76543210> 
        return 0x14|((1-read_onkey())<<1); 
        case 0x1b:	// r- <76543210> 
        return hw_kbd_read_mask();
        case 0x1c:	// -w <..5432..> 
        break;
        case 0x1d:	// -w <7..43210>
        break;
        case 0x1e:	// ??
        return 0x14;
        case 0x1f:	// ??
        return 0x14;
		default:
        return 0x14;
    }
  
    return tihw.io[adr];
}

UWORD io_get_word(CPTR adr) 
{
    return (((UWORD)io_get_byte(adr))<<8) | io_get_byte(adr+1);
}

ULONG io_get_long(CPTR adr) 
{
    return (((ULONG)io_get_word(adr))<<16) | io_get_word(adr+2);
}

/** HW2 **/

void io2_put_byte(CPTR adr, UBYTE arg)
{
    switch(adr) 
    {
        case 0x00:	// rw <76543210>
			break;
		case 0x01:	// rw <76543210>
			break;
		case 0x02:	// rw <76543210>
			break;
		case 0x03:	// rw <76543210>
			break;
		case 0x04:	// rw <76543210>
			break;
		case 0x05:	// rw <76543210>
			break;
		case 0x06:	// rw <76543210>
			break;
		case 0x07:	// rw <76543210>
			break;
		case 0x11:	// -w <76543210>
			break;
		case 0x12:
			break;
		case 0x13:  // rw <..543210>
			break;
		case 0x14:	// rw <76543210>
			break;
		case 0x15:	// rw <76543210>
			break;
		case 0x17:	// rw <......10>
			break;
		case 0x1d:	// rw <7...3210>
			break;
		case 0x1f:	// rw <.....210>
			break;
    }

    tihw.io2[adr] = arg;
}

void io2_put_word(CPTR adr, UWORD arg) 
{
    io2_put_byte(adr,   arg>>8);
    io2_put_byte(adr+1, arg&0xff);
}

void io2_put_long(CPTR adr, ULONG arg) 
{
    io2_put_word(adr,   arg>>16);
    io2_put_word(adr+2, arg&0xffff);
}

UBYTE io2_get_byte(CPTR adr) 
{
    int v;

    switch(adr) 
    {
        case 0x00:
			break;
		case 0x01:
			break;
		case 0x02:
			break;
		case 0x03:
			break;
		case 0x04:
			break;
		case 0x05:
			break;
		case 0x06:
			break;
		case 0x07:
			break;
		case 0x11:
			break;
		case 0x12:
			break;
		case 0x13:
			break;
		case 0x14:
			break;
		case 0x15:
			break;
		case 0x17:
			break;
		case 0x1d:
			break;
		case 0x1f:
			break;
    }
  
    return tihw.io2[adr];
}

UWORD io2_get_word(CPTR adr) 
{
    return (((UWORD)io2_get_byte(adr))<<8) | io2_get_byte(adr+1);
}

ULONG io2_get_long(CPTR adr) 
{
    return (((ULONG)io2_get_word(adr))<<16) | io2_get_word(adr+2);
}

