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
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#include "uae.h"
#include "memory.h"
#include "keyboard.h"
#include "dbus.h"
#include "callbacks.h"
#include "ioports.h"
#include "ti68k_def.h"

int hw_io_init(void)
{
	memset(tihw.io, 0x14, sizeof(tihw.io));
	memset(tihw.io2, 0x14, sizeof(tihw.io2));

	tihw.io[0x00] = 0x00;
	tihw.io[0x01] = 0x04 | (tihw.ti92v1 ? 1 : 0);
	tihw.io[0x03] = 0xff;
	tihw.io[0x05] = 0x00;
	tihw.io[0x0c] = 0x8d;
	tihw.io[0x0d] = 0x40;
	tihw.io[0x0e] = 0xff;
	tihw.io[0x0f] = 0x00;

	tihw.io[0x12] = 0x31;
	tihw.io[0x13] = 0x80;
	tihw.io[0x15] = 0x1b;

	tihw.io[0x18] = 0x03;
	tihw.io[0x19] = 0x80;
	tihw.io[0x1a] = 0xff;
	tihw.io[0x1b] = 0xff;
	tihw.io[0x1c] = 0x21;
	tihw.io[0x1d] = 0x80;

	tihw.io2[0x11] = 0x40;
	tihw.io2[0x11] = 0x18;
	tihw.io2[0x17] = 0x00;
	tihw.io2[0x1d] = 0x06;
	tihw.io2[0x1f] = 0x07;

	return 0;
}

int hw_io_reset(void)
{
    return 0;
}

int hw_io_exit(void)
{
    return 0;
}


void io_put_byte(CPTR adr, UBYTE arg)
{
    switch(adr) 
    {
        case 0x00:	// rw <76...2..>
			// %5: bit 0 of contrast (TI92)
			if(tihw.calc_type == TI92 || tihw.calc_type == TI92p)
			{
            	tihw.contrast = bit_clr(tihw.contrast,0) | bit_get(arg,5);
				cb_set_contrast(tihw.contrast);
			}
        break;
        case 0x01:	// rw <.....2.0>
			// %0 clr: interleave RAM (allows use of 256K of RAM)
            if(tihw.hw_type == 1)
			    mem_mask[0] = bit_tst(arg,0) ? 0x1ffff : 0x3ffff;

			// %2 set: protected memory violation triggered when memory below [$000120] is written
	    break;
	    case 0x02:	// ??
	    break;
		case 0x03:	// -w <.654.210>
			// Bus waitstates
			break;
        case 0x04:
        break;
        case 0x05:	// -w <...43210>
        	// %3 set: 000000..1FFFFF mapped to 200000..3FFFFF
			if(bit_tst(arg,3) && (tihw.calc_type == TI92))
			{

				//mem_tab[2] = mem_tab[0];
				//mem_mask[2] = mem_mask[0];
				//mem_tab[3] = mem_tab[1];
				//mem_mask[3] = mem_mask[1];
            } else
            {
				//mem_tab[2] = tihw.rom;
				//mem_mask[2] = 0x1fffff;
				//mem_tab[3] = mem_tab[1];
				//mem_mask[3] = mem_mask[1];
            }
            
			// turn off OSC1 (CPU), wake on int level 6 (ON key) and int level [5..1]
			if (!(arg & 0x1f) && (tihw.calc_type != TI92)) 
                specialflags |= SPCFLAG_STOP; 
            break;
        case 0x06: 
		case 0x07: 
		case 0x08: 
		case 0x09: 
		case 0x0a: 
		case 0x0b:
        break;
        case 0x0c:	// rw <765.3210>
        	// %[3:0]: Trigger interrupt level 4 on ...
        	// see hardware.c
        break;
        case 0x0d:	// r- <76543210>
			break;
        case 0x0e:	// rw <....3210>
			// set red/white wires (if direct access)			
			if(io_bit_tst(0x0c,6))
	        {
	            lc.set_red_wire(bit_get(arg,0));
	            lc.set_white_wire(bit_get(arg,1));
	        }
        break;
        case 0x0f: 	// rw <76543210>
			// write a byte to the transmit buffer (1 byte buffer)
            hw_dbus_putbyte(arg);
            break;
        case 0x10: 	// -w <76543210> (hw1)
			// address of LCD memory divided by 8
			if(tihw.hw_type == HW1)
				tihw.lcd_ptr = &tihw.ram[((tihw.io[0x10] << 8) | tihw.io[0x11]) << 3];
        break;
        case 0x11: 	// -w <76543210> (hw1)
			// address of LCD memory divided by 8
			if(tihw.hw_type == HW1)
				tihw.lcd_ptr = &tihw.ram[((tihw.io[0x10] << 8) | tihw.io[0x11]) << 3];
        break;
        case 0x12:	// -w <76543210>
			// LCD logical width =(64-n)*2 bytes =(64-n)*16 pixels
        break;
        case 0x13:	// -w <..543210>
			// LCD logical height =(256-n)
        break;
        case 0x14:
        break;
        case 0x15:	// rw <7.6543210>
        	// %7 set: Master disable timer interrupts (level 1, 3 and 5)
        	// see hardware.c
        	
        	// %[5-4]: Increment rate of $600017 
        	// (we should set 'cycle_instr')
        	
        	// %3 set: Enable incrementing of $600017
        	// see hardware.c        	
        	
        	// %2 set: Trigger interrupt level 3 at OSC2/2^19  (~1 Hz on HW2)
        	// see hardware.c
        	
        	// %1 set: OSC2 (and OSC3?) enable (bit clear means oscillator stopped!)
        	// see hardware.c
        	
        	// %0 set: LCD controller DMA enable else LCD blank ("white")
        	// to do...        	
        break;
        case 0x16:
        break;
        case 0x17: 	// rw <76543210>
			// programmable rate generator
            tihw.timer_init = arg; 
            break;
        case 0x18:	// rw <......10>
			// keyboard row mask (see keyboard.c)
        break;    
        case 0x19:	// rw <76543210>
			// keyboard row mask (see keyboard.c)
        break;
        case 0x1a:	// r- <......10>
        	// keyboard col mask (see keyboard.c)
        	// Write any value to $60001A to acknowledge this interrupt (AutoInt6)
        break;
        case 0x1b:	// r- <76543210>
        	// Write any value to $60001B to acknowledge this interrupt (AutoInt2)
        break;
        case 0x1c:	// -w <..5432..>
        	// %[5-2] set: LCD RS (row sync) frequency, OSC2/((16-n)*8)
        	// %1111 turns off the RS completely (used when LCD is off)        	
        	if((arg & 0x3c) == 0x3c)
	            cb_screen_on_off(0);
            else
	            cb_screen_on_off(!0);
        break;
        case 0x1d:	// -w <7..43210>
			// %[3-1]: contrast
			if(tihw.calc_type == TI92 || tihw.calc_type == TI92p)
			{
				// %[3-1]: bits <4321.> of contrast
            	tihw.contrast = (tihw.contrast & 1) | ((arg & 15) << 1);
            	cb_set_contrast(tihw.contrast);
            }
            else
            {
            	// %[3-1]: LCD contrast bits 3-0 (bit 3 is msb on HW1)
            	tihw.contrast = arg & 0x0f;
            	cb_set_contrast(tihw.contrast);
            	
            	// %4: HW1: Screen disable (power down), HW2: LCD contrast bit 4 (msb)
				if(io2_bit_tst(0x1f,0))
					tihw.contrast = bit_clr(tihw.contrast,5) | (bit_get(arg,4) << 1);
				else
					tihw.lcd_power = bit_get(arg,4);
            }
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
    io_put_byte(adr,   MSB(arg));
    io_put_byte(adr+1, LSB(arg));
}

void io_put_long(CPTR adr, ULONG arg) 
{
    io_put_word(adr,   MSW(arg));
    io_put_word(adr+2, LSW(arg));
}

UBYTE io_get_byte(CPTR adr) 
{
    int v = tihw.io[adr];

    switch(adr) 
    {
        case 0x00:	// rw <76...2..>
			// %0: bits <....0> of contrast
			if(tihw.calc_type == TI92)
				v = ((tihw.contrast & 1) << 5);

			// %2: Battery voltage level is *above* the trig level
            v |= 4;
            
            // %[7-6]: keep clear
        break;
        case 0x01:	// rw <.....2.0>
        break;
        case 0x02:
		break;
        break;
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
        	// linkport status
        	// see hardware.c or dbus.c
        break;
        case 0x0d:	// r- <76543210>
			// linkport status
			v |= (hw_dbus_byteavail() ? 0x60 : 0x40);	// needed, why ?
		break;
        case 0x0e:	// rw <....3210>
			// %[2-3]: read red/white wires if raw access
			if(io_bit_tst(0x0c,6))
			{
				v |= lc.get_white_wire() << 3;
				v |= lc.get_white_wire() << 2;
			}
            break;
        case 0x0f: 	// rw <76543210>
			// read one byte from receive (incoming) buffer
			io_bit_clr(0x0c,0);		// rx buffer full
            return hw_dbus_getbyte();
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
        // Programmable rate generator
        return tihw.timer_value;
        case 0x18: 	// rw <76543210>
        break;
        case 0x19:	// rw <......10>
        break;
        case 0x1a:	// rw <......10>
        // ON key status (0=down, 1=up)
        return v | ((1 - read_onkey()) << 1); 
        case 0x1b:	// r- <76543210> 
        // keyboard row mask
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
  
    return v;
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
			// Display memory snoop range
			tihw.lcd_ptr = &tihw.ram[0x4c000 + 0x1000*(arg&3)];
		break;
		case 0x1d:	// rw <7...3210>
			// %1: Screen enable (clear this bit to shut down LCD)
			if(bit_tst(arg,1))
	            cb_screen_on_off(!0);
            else
	            cb_screen_on_off(0);
			break;
		case 0x1f:	// rw <.....210>
			// %0 set: use 5 contrast bits (default for AMS).
			break;
    }

    tihw.io2[adr] = arg;
}

void io2_put_word(CPTR adr, UWORD arg) 
{
    io2_put_byte(adr,   MSB(arg));
    io2_put_byte(adr+1, LSB(arg));
}

void io2_put_long(CPTR adr, ULONG arg) 
{
    io2_put_word(adr,   MSW(arg));
    io2_put_word(adr+2, LSW(arg));
}

UBYTE io2_get_byte(CPTR adr) 
{
    int v = tihw.io2[adr];

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
  
    return v;
}

UWORD io2_get_word(CPTR adr) 
{
    return (((UWORD)io2_get_byte(adr))<<8) | io2_get_byte(adr+1);
}

ULONG io2_get_long(CPTR adr) 
{
    return (((ULONG)io2_get_word(adr))<<16) | io2_get_word(adr+2);
}

