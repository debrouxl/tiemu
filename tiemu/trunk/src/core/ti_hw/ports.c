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
 *  but WITHOUT ANY WARRAN7TY; without even the implied warranty of
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

#include "libuae.h"
#include "mem.h"
#include "kbd.h"
#include "dbus.h"
#include "ports.h"
#include "m68k.h"
#include "ti68k_def.h"

int hw_io_init(void)
{
	memset(tihw.io, 0x00, tihw.io_size);
	memset(tihw.io2, 0x00, tihw.io2_size);

	if(tihw.hw_type > HW1)
		tihw.lcd_adr = 0x4c00;

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

void set_cycle_rate(int);

void io_put_byte(uint32_t addr, uint8_t arg)
{
	addr &= tihw.io_size-1;

    switch(addr) 
    {
        case 0x00:	// rw <76...2..>
			// %5: bit 0 of contrast (TI92)
			if(tihw.calc_type == TI92)
                bit_chg(tihw.contrast,0,bit_get(arg,5));
        break;
        case 0x01:	// rw <.....2.0>
			// %0 clr: interleave RAM (allows use of 256K of RAM)
            if(tihw.hw_type == 1)
			    mem_msk[0] = bit_tst(arg,0) ? 0x1ffff : 0x3ffff;

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
			// turn off OSC1 (CPU), wake on int level 6 (ON key) and int level [5..1]
			m68k_setstopped(1);
        break;
        case 0x06: 
		case 0x07: 
		case 0x08: 
		case 0x09: 
		case 0x0a: 
		case 0x0b:
        break;
        case 0x0c:	// rw <765.3210>
        	// %[3:0]: Trigger interrupt level 4 on error, activity, tx empty, rx full
        	// see hardware.c
			// %6: link disable (usually reset link port or direct access to wires)
			if(bit_get(arg, 6))
				hw_dbus_reset();
        break;
        case 0x0d:	// r- <76543210>
			break;
        case 0x0e:	// rw <....3210>
			// set red/white wires (if direct access)			
			if(io_bit_tst(0x0c,6))
	        {
	            lc.set_red_wire(!bit_get(arg,0));
	            lc.set_white_wire(!bit_get(arg,1));
	        }
        break;
        case 0x0f: 	// rw <76543210>
			// write a byte to the transmit buffer (1 byte buffer)
			io_bit_clr(0x0d, 0);	// STX=0 (tx reg is full)
            hw_dbus_putbyte(arg);
            break;
        case 0x10: 	// -w <76543210> (hw1)
			// address of LCD memory divided by 8 (msb)
			if(tihw.hw_type == HW1)
				tihw.lcd_adr = ((arg << 8) | tihw.io[0x11]) << 3;
			//printf("$600010: lcd_adr = %04x\n", tihw.lcd_adr);
        break;
        case 0x11: 	// -w <76543210> (hw1)
			// address of LCD memory divided by 8 (lsb)
			if(tihw.hw_type == HW1)
				tihw.lcd_adr = ((tihw.io[0x10] << 8) | arg) << 3;
        break;
        case 0x12:	// -w <76543210>
			// LCD logical width = (64-n)*2 bytes = (64-n)*16 pixels <=> n = 64-w/16
			tihw.log_w = (64 - arg) * 16;
        break;
        case 0x13:	// -w <..543210>
			// LCD logical height = (256-n) <=> n = 256-h
			tihw.log_h = 0x100 - arg;
        break;
        case 0x14:
        break;
        case 0x15:	// rw <7.6543210>
        	// %7 set: Master disable timer interrupts (level 1, 3 and 5)
        	// see hardware.c
        	
        	// %[5-4]: Increment rate of $600017 
			set_cycle_rate((arg >> 4) & 3);
        	
        	// %3 set: Enable incrementing of $600017
        	// see hardware.c        	
        	
        	// %2 set: Trigger interrupt level 3 at OSC2/2^19  (~1 Hz on HW2)
        	// see hardware.c
        	
        	// %1 set: OSC2 (and OSC3?) enable (bit clear means oscillator stopped!)
        	// see hardware.c
        	
        	// %0 set: LCD controller DMA enable else LCD blank ("white")
        	// could be implemented but redundant with tihw.on_off
        break;
        case 0x16:
        break;
        case 0x17: 	// rw <76543210>
			// programmable rate generator
            break;
        case 0x18:	// rw <......10>
			// keyboard row mask (see keyboard.c)
        break;    
        case 0x19:	// rw <76543210>
			// keyboard row mask (see keyboard.c)
        break;
        case 0x1a:	// r- <......10>
        	// ON key status (see keyboard.c)
        	// Write any value to $60001A to acknowledge this interrupt (AutoInt6)
        break;
        case 0x1b:	// r- <76543210>
			// keyboard column status (see keyboard.c)
        	// Write any value to $60001B to acknowledge this interrupt (AutoInt2)
        break;
        case 0x1c:	// -w <..5432..>
        	// %[5-2] set: LCD RS (row sync) frequency, OSC2/((16-n)*8)
        	// %1111 turns off the RS completely (used when LCD is off)        	
			tihw.on_off = ((arg & 0x3c) == 0x3c) ? 0 : 1;
        break;
        case 0x1d:	// -w <7..43210>
			// %[3-0]: contrast
			if(tihw.calc_type == TI92)
			{
				// %[3-0]: bits <4321.> of contrast
				static int avg = 0;				

				avg = (avg + arg)/2;	// filter value
            	tihw.contrast = (tihw.contrast & 1) | ((avg & 15) << 1);
            }
            else
            {
            	// %[4/3-0]: LCD contrast bits 4/3-0 (bit 4/3 is msb on HW2/HW1)
				tihw.contrast = arg & (io2_bit_tst(0x1f,0) ? 0x1f : 0x0f);
				if(tihw.calc_type == TI89)
				{
					if(tihw.hw_type == HW1)
            			tihw.contrast = 31 - 2*tihw.contrast;
					else
						tihw.contrast = 31 - tihw.contrast;
				}
            }
        break;
        case 0x1e:
        break;
        case 0x1f:
        break;
    }
  
    tihw.io[addr] = arg;
}

void io_put_word(uint32_t addr, uint16_t arg) 
{
    io_put_byte(addr,   MSB(arg));
    io_put_byte(addr+1, LSB(arg));
}

void io_put_long(uint32_t addr, uint32_t arg) 
{
    io_put_word(addr,   MSW(arg));
    io_put_word(addr+2, LSW(arg));
}

uint8_t io_get_byte(uint32_t addr) 
{
    int v;
	
	addr &= tihw.io_size-1;
	v = tihw.io[addr];

    switch(addr) 
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
			// reading the DBus status register resets that register
			tihw.io[0x0d] = 0x40;
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
            v = hw_dbus_getbyte();
			io_bit_clr(0x0d, 5);	// SRX=0 (rx reg is empty)
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
			bit_chg(v,1,!tihw.on_key);
        break;
        case 0x1b:	// r- <76543210> 
			// keyboard column status
	        v = hw_kbd_read_cols();
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

uint16_t io_get_word(uint32_t addr) 
{
    return (((uint16_t)io_get_byte(addr))<<8) | io_get_byte(addr+1);
}

uint32_t io_get_long(uint32_t addr) 
{
    return (((uint32_t)io_get_word(addr))<<16) | io_get_word(addr+2);
}

/** HW2 **/

void io2_put_byte(uint32_t addr, uint8_t arg)
{
	int i;

	addr &= tihw.io2_size-1;

    switch(addr) 
    {
        case 0x00:	// rw <76543210>
		case 0x08:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)	// this is the fastest method (an easier method will use 64 bit integer)
				tihw.ram_exec[i] = arg & (1 << i);
		case 0x01:	// rw <76543210>
		case 0x09:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[8+i] = arg & (1 << i);
		case 0x02:	// rw <76543210>
		case 0x0a:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[16+i] = arg & (1 << i);
		case 0x03:	// rw <76543210>
		case 0x0b:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[24+i] = arg & (1 << i);
		case 0x04:	// rw <76543210>
		case 0x0c:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[32+i] = arg & (1 << i);
		case 0x05:	// rw <76543210>
		case 0x0d:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[40+i] = arg & (1 << i);
		case 0x06:	// rw <76543210>
		case 0x0e:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[48+i] = arg & (1 << i);
		case 0x07:	// rw <76543210>
		case 0x0f:
			if(tihw.protect)
				return;
			for(i = 0; i < 8; i++)
				tihw.ram_exec[56+i] = arg & (1 << i);
		case 0x11:	// -w <76543210>
			break;
		case 0x12:
			break;
		case 0x13:  // rw <..543210>
			break;
		case 0x14:	// rw <76543210>
			// RTC, incremented every 2^13 seconds. The whole word must be read: 
			// reading the port byte by byte can return wrong value
			tihw.rtc_value = (tihw.io2[0x14] << 8) | tihw.io2[0x15];
			break;
		case 0x15:	// rw <76543210>
			tihw.rtc_value = (tihw.io2[0x14] << 8) | tihw.io2[0x15];
			break;
		case 0x17:	// rw <......10>
			// Display memory snoop range
			tihw.lcd_adr = 0x4c00 + 0x1000*(arg&3);
			//printf("$700017: lcd_adr = %04x\n", tihw.lcd_adr);
		break;
		case 0x1d:	// rw <7...3210>
			// %1: Screen enable (clear this bit to shut down LCD)
			tihw.on_off = bit_tst(arg,1) ? 1 : 0;
			break;
		case 0x1f:	// rw <.....210>
			if(!tihw.protect) tihw.io2[addr] = arg; else return;
			// %0 set: use 5 contrast bits (default for AMS).

			// %[2:1]
			// %2 set: activates the incrementation of $700014.w
			break;
    }

    tihw.io2[addr] = arg;
}

void io2_put_word(uint32_t addr, uint16_t arg) 
{
    io2_put_byte(addr,   MSB(arg));
    io2_put_byte(addr+1, LSB(arg));
}

void io2_put_long(uint32_t addr, uint32_t arg) 
{
    io2_put_word(addr,   MSW(arg));
    io2_put_word(addr+2, LSW(arg));
}

uint8_t io2_get_byte(uint32_t addr) 
{
    int v;
	
	addr &= tihw.io2_size-1;
	v = tihw.io2[addr];

    switch(addr) 
    {
        case 0x00:
		case 0x08:
			break;
		case 0x01:
		case 0x09:
			break;
		case 0x02:
		case 0x0a:
			break;
		case 0x03:
		case 0x0b:
			break;
		case 0x04:
		case 0x0c:
			break;
		case 0x05:
		case 0x0d:
			break;
		case 0x06:
		case 0x0e:
			break;
		case 0x07:
		case 0x0f:
			break;
		case 0x11:
			break;
		case 0x12:
			break;
		case 0x13:
			break;
		case 0x14:	// rw <7...3210>	word
			// RTC, incremented every 2^13. The whole word must be read: 
			// reading the port byte per byte can return wrong value
			return MSB(tihw.rtc_value);
			break;
		case 0x15:
			return LSB(tihw.rtc_value);
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

uint16_t io2_get_word(uint32_t addr) 
{
    return (((uint16_t)io2_get_byte(addr))<<8) | io2_get_byte(addr+1);
}

uint32_t io2_get_long(uint32_t addr) 
{
    return (((uint32_t)io2_get_word(addr))<<16) | io2_get_word(addr+2);
}

