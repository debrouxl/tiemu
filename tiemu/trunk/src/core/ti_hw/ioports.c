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

#define bit_get(i,b)	(((b) & (1 << i)) >> i)
#define bit_set(i,b)	((b) &  (1 << i))
#define bit_clr(i,b)	((b) | ~(1 << i))

void io_put_byte(CPTR adr, UBYTE arg) 
{
    switch(adr) 
    {
        case 0x00:
            tihw.contrast=(tihw.contrast&(~1))|((arg>>5)&1);
            if(tihw.calc_type != TI92)
	            cb_set_contrast(tihw.contrast); // avoid flickering with 92
            tihw.io0Bit7=(arg>>7)&1;
            tihw.io0Bit2=(arg>>2)&1;
        break;
        case 0x01:
			// clr: interleave RAM (allows use of 256K of RAM)
			tihw.ram256 = bit_get(0, arg);
			//mem_mask[0] = tihw.ram256 ? 0x1fffff : 0x3fffff;

			// set: protected memory violation triggered when memory below [$000120] is written
			tihw.prot_mem = bit_get(2, arg);

            mem_and=(tihw.ram_wrap)?0x1ffff:(!tihw.ram256?0x1ffff:0x3ffff);
            //mem_mask[0] = (mem_and==0x1fffff) ? (128*1024) : (256 * 1024);
	    break;
        case 0x04:
			// set: 000000..1FFFFF mapped to 200000..3FFFFF
			tihw.ram_wrap = bit_get(3, arg);
            //mem_tab[1] = mem_tab[0];
        break;
        case 0x05: if (!(arg&0x10)) specialflags |= SPCFLAG_STOP; 
            break;
        case 0x0c: //link status
            lc_raw_access=(arg>>6)&1;
        break;
        case 0x0d: break;
        case 0x0e: // write link port w/ direct access
            lc_raw_access=(arg>>6)&1;
            if (lc_raw_access)
	        {
	            lc.set_red_wire(arg&1);
	            lc.set_white_wire((arg>>1)&1);
	        }
        break;
        case 0x0f: //tx buffer
            linkport_putbyte(arg);
            break;
        case 0x10: 
			// address of LCD memory divided by 8
            tihw.lcd_addr = (((tihw.lcd_addr >> 3) & 0xff) | 
						((arg & 0xff) << 8)) << 3;
        break;
        case 0x11: 
			// address of LCD memory divided by 8
            tihw.lcd_addr=(((tihw.lcd_addr >> 3) & 0xff00) |
						(arg & 0xff)) << 3;
        break;
        case 0x17: 
            tihw.timer_init = arg; 
            break;
        case 0x1c:
            tihw.lcd_off=(arg&0x80)>>7;
            if((arg&0x80)>>7)
	        {
	            specialflags |= SPCFLAG_STOP;
	            cb_screen_on_off(0);
	        }
            else
	            cb_screen_on_off(!0);
        break;
        case 0x1d:
            tihw.contrast=(tihw.contrast&1)|((arg&15)<<1);
            cb_set_contrast(tihw.contrast);
        break;
    }
  
    tihw.io[adr] = arg;
}

void io_put_word(CPTR adr, UWORD arg) 
{
    io_put_byte(adr, arg>>8);
    io_put_byte(adr+1, arg&0xff);
}

void io_put_long(CPTR adr, ULONG arg) 
{
    io_put_word(adr, arg>>16);
    io_put_word(adr+2, arg&0xffff);
}

UBYTE io_get_byte(CPTR adr) 
{
    int v;

    switch(adr) 
    {
        case 0x00: 
            v=((tihw.contrast&1)<<5)|(tihw.io0Bit7<<7)|(tihw.io0Bit2<<2);
            tihw.io0Bit2=1;
            return v|0x4;
        case 0x01:
			// interleave RAM (allows use of 256K of RAM)
			// protected memory violation triggered when memory below [$000120] is written
            return (tihw.prot_mem << 2) | tihw.ram256;
        case 2: case 4: case 6: case 8: case 0xa: return 0x14;
        case 3: case 5: case 7: case 9: case 0xb: return 1;      
        case 0x0c: //link status
            //link bug here !!!
            //return 5|((1-transflag)<<1)|(lc_raw_access?0x40:0);
        break;
        case 0x0d :
            /* 0x40 -> always return tx buffer as empty */
            return (linkport_byteavail() ? 0x60 : 0x40);
        case 0x0e:
            if (lc_raw_access)
	            return 0x50|(lc.get_white_wire()<<1)|lc.get_red_wire();
            break;
        case 0x0f :
            recvflag = 0;
            recvbyte = linkport_getbyte();
            return recvbyte;
        case 0x10: case 0x12: return 0x14;
        case 0x11: case 0x13: return 1;
        case 0x14: return 0x14;
        case 0x15: return 0x1b;
        case 0x16: return 0x14;
        case 0x17: return tihw.timer_value;
        case 0x18: break;
        case 0x19: break;
        case 0x1a: return 0x14|((1-read_onkey())<<1); 
        case 0x1b: return hw_kbd_read_mask();
        case 0x1c: case 0x1e: return 0x14;
        case 0x1d: case 0x1f: return 1;
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
