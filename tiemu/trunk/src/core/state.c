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

/*
  	Manage state images
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include "uae.h"
#include "./ti_hw/memory.h"
#include "dbus.h"
#include "timer.h"
#include "lcd.h"
#include "../ti_hw/ioports.h"
#include "ti68k_int.h"
#include "ti68k_err.h"

#define IO_SIZE     32

/*
  Must be done between init_hardware and M68000_run.
  Typically called after initLib68k.
  This function (re)load the state of the calculator.
  It automagically determine the state file format.

  Return an error code if an error occured, 0 otherwise
*/
int ti68k_state_load(char *filename)
{
  FILE *fp;
  int kbmask=0;
  int comError=0;
  int cpuCompleteStop=0;
  char str[9], romName[56];
  int i;
  UBYTE* mem[256];
  int int0Count = 0;
  int mem_and;

  DISPLAY("Loading RAM image (VTi format): %s\n", filename);
  if(!strcmp(filename, ""))
    return ERR_68K_NONE;
  if( (fp = fopen(filename, "rb")) == NULL)
    return ERR_68K_CANT_OPEN;

  fread(str,8,1,fp);
  str[8]=0;
  if( strcmp(str,"VTIv2.0 ") && strcmp(str, "GTKTIEMU") )
    return ERR_68K_INVALID_STATE;
  fread(romName,56,1,fp);
  //  if (strcmp(romName,romImage[currentROM].name))
  //  return;
  fread(&regs,sizeof(regs),1,fp);
  MakeFromSR();
  fread(tihw.ram,256,1024,fp);
//  fread(&tihw.mem_prot,4,1,fp); fread(&tihw.ram256,4,1,fp);
  fread(&tihw.timer_value,4,1,fp); fread(&tihw.timer_init,4,1,fp);
  fread(&int0Count,4,1,fp);
//  fread(&tihw.io0Bit7,4,1,fp); fread(&tihw.io0Bit2,4,1,fp);
  fread(&kbmask,4,1,fp);
//  fread(&tihw.ram_wrap,4,1,fp); fread(&(mem_and),4,1,fp);
  fread(&comError,4,1,fp);
  //fread(&transflag,4,1,fp); fread(&transbyte,4,1,fp);
  //fread(&transnotready,4,1,fp);
  //fread(&recvflag,4,1,fp); fread(&recvbyte,4,1,fp);
  //fread(&rom_ret_or,4,1,fp);
//  fread(&tihw.lcd_addr,4,1,fp); fread(&tihw.lcd_off,4,1,fp);
  fread(&tihw.contrast,4,1,fp);
  fread(&cpuCompleteStop,4,1,fp);
  for (i=0;i<32;i++)
    mem[i]=&tihw.ram[(i&(mem_and>>16))<<16];

  // GtkTiEmu specific
  fread(tihw.io, 1, IO_SIZE, fp);      // write IO
  fread(&specialflags, sizeof(specialflags), 1, fp); // and flags
  fclose(fp);
  
  return ERR_68K_NONE;
}


/*
  This function save the state of the calculator.
  Can be called at any time.

  Return an error code if an error occured, 0 otherwise
*/
int ti68k_state_save(char *filename)
{
  FILE *fp;
  int kbmask=0;
  int comError=0;
  int cpuCompleteStop=0;
  int int0Count = 0;
  int mem_and;

  if(strlen(filename)) // name exist ?
    {
      if( (fp = fopen(filename, "wb")) == NULL)
	return ERR_68K_CANT_OPEN;

      m68k_setpc(m68k_getpc());
      fprintf(fp,"GTKTIEMU");
      fwrite(filename,56,1,fp);
      MakeSR();
      fwrite(&regs,sizeof(regs),1,fp); // struct format differs !
      fwrite(tihw.ram,256,1024,fp);
//      fwrite(&tihw.mem_prot,4,1,fp); fwrite(&tihw.ram256,4,1,fp);
      fwrite(&tihw.timer_value,4,1,fp); fwrite(&tihw.timer_init,4,1,fp);
      fwrite(&int0Count,4,1,fp);
//      fwrite(&tihw.io0Bit7,4,1,fp); fwrite(&tihw.io0Bit2,4,1,fp);
      fwrite(&kbmask,4,1,fp);
//      fwrite(&tihw.ram_wrap,4,1,fp); fwrite(&(mem_and),4,1,fp);
      fwrite(&comError,4,1,fp);
      //fwrite(&transflag,4,1,fp); fwrite(&transbyte,4,1,fp);
      //fwrite(&transnotready,4,1,fp);
      //fwrite(&recvflag,4,1,fp); fwrite(&recvbyte,4,1,fp);
      //fwrite(&rom_ret_or,4,1,fp);
//      fwrite(&tihw.lcd_addr,4,1,fp); fwrite(&tihw.lcd_off,4,1,fp);
      fwrite(&tihw.contrast,4,1,fp);
      fwrite(&cpuCompleteStop,4,1,fp);
      
      // GtkTiEmu specific
      fwrite(tihw.io, 1, IO_SIZE, fp);      // write IO
      fwrite(&specialflags, sizeof(specialflags), 1, fp); // and flags
      fclose(fp);
    }

  return ERR_68K_NONE;
}
