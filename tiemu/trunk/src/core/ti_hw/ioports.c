/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
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
  TI ASIC management: glue logic for screen, keyboard, linkport, timers.
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#include "uae.h"
#include "ti68k_def.h"
#include "images.h"
#include "memory.h"
#include "lcd.h"
#include "keyboard.h"
#include "linkport.h"
//#include "params.h"
#include "timer.h"
#include "callbacks.h"

//int init_int_tab_offset;
int io0Bit7;
int io0Bit2;

void io_put_byte(CPTR adr, UBYTE arg) 
{
  switch(adr) 
    {
    case 0x00: 
      contrast=(contrast&(~1))|((arg>>5)&1);
      if(tihw.calc_type != TI92)
	cb_set_contrast(contrast); // avoid flickering with 92
      io0Bit7=(arg>>7)&1;
      io0Bit2=(arg>>2)&1;
      break;
    case 0x01: 
      ram128=arg&1; memprotect=(arg>>2)&1;
      mem_and=(ram_wrap)?0x1ffff:(ram128?0x1ffff:0x3ffff);
      //mem_mask[0] = (mem_and==0x1fffff) ? (128*1024) : (256 * 1024);
	break;
    case 0x04: // bit 3 set: 000000..1FFFFF mapped to 200000..3FFFFF
      ram_wrap=(arg&8)>>3;
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
      lcd_base_addr=(((lcd_base_addr>>3)&0xff)|((arg&0xff)<<8))<<3;
      //cb_set_screen_ptr((int)(&ti_ram[lcd_base_addr]));
      break;
    case 0x11: 
      lcd_base_addr=(((lcd_base_addr>>3)&0xff00)|(arg&0xff))<<3;
      //cb_set_screen_ptr((int)(&ti_ram[lcd_base_addr]));
      break;
    case 0x17: 
      timer_init = arg; 
      break;
    case 0x1c:
      lcd_off=(arg&0x80)>>7;
      if((arg&0x80)>>7)
	{
	  specialflags |= SPCFLAG_STOP;
	  cb_screen_on_off(0);
	}
      else
	  cb_screen_on_off(!0);
      break;
    case 0x1d:
      contrast=(contrast&1)|((arg&15)<<1);
      cb_set_contrast(contrast);
      break;
    }
  ti_io[adr] = arg;
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
      v=((contrast&1)<<5)|(io0Bit7<<7)|(io0Bit2<<2);
      io0Bit2=1;
      return v|0x4;
    case 0x01:
      return (memprotect<<2)|ram128;
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
    case 0x17: return timer_value;
    case 0x18: break;
    case 0x19: break;
    case 0x1a: return 0x14|((1-read_onkey())<<1); 
    case 0x1b: return read_keyboard_mask();
    case 0x1c: case 0x1e: return 0x14;
    case 0x1d: case 0x1f: return 1;
    }
  return ti_io[adr];
}

UWORD io_get_word(CPTR adr) 
{
  return (((UWORD)io_get_byte(adr))<<8)|io_get_byte(adr+1);
}

ULONG io_get_long(CPTR adr) 
{
  return (((ULONG)io_get_word(adr))<<16)|io_get_word(adr+2);
}


void hw_init_io()
{
  io0Bit2=1; 
  io0Bit7=1;
}

void hw_exit_io()
{
}
