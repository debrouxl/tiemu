/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001-2002, Romain Lievin
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
  TI core: exported & misc routines
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#include <sys/types.h>
#ifndef __WIN32__
#include <dirent.h>
#endif
#include <sys/stat.h>
//#include <unistd.h>
#include <string.h>

#include "uae.h"
#include "interface.h"

#include "packets.h"
#include "debugger.h"
#include "hardware.h"
#include "keyboard.h"
#include "params.h"
#include "callbacks.h"
#include "intlist.h"
#include "errcodes.h"
#include "images.h"
#include "tilibs.h"
#include "./ti_hw/memory.h"
#include "bkpts.h"
#include "lcd.h"
#include "linkport.h"
#include "ioports.h"
#include "m68k.h"

/* Used for checking the calling order of some functions */
int runlevel = 0;

void change_runlevel(int new, int min)
{
  if(new < min)
    {
      DISPLAY("Runlevel error: is %i, should be %i\n", runlevel, min);
      exit(-1);
    }
  runlevel = new;
}

/**********************/
/* Internal variables */
/**********************/

Ti68kRomInfo current_rom_info = { 0 };
Ti68kParameters params;


/***********************************/
/* Entry points for initialization */
/***********************************/

/*
  Initialization order (checked by the runlevel):
  - register callbacks
  - load the default config
  - load a ROM (init ROM)
  - init the lib (init HW, UAE, HID)
  - reset the lib
*/

/* 
   This should be the FIRST function to call for registering
   HID callbacks.
   The callbacks will not be used until ti68k_init().
*/
int ti68k_setGuiCallbacks(callback_iv_t initSpecific,
			  callback_iv_t exitSpecific,
			  callback_iv_t updateScreen,
			  callback_iv_t updateKeys,
			  callback_vi_t screenOnOff,
			  callback_ii_t setContrast
			  );

/*
  This should be the SECOND function to call (unless the 'params' 
  structure has been properly initialized.
 */
int ti68k_loadDefaultConfig(void)
{
  params.background = 1;
  params.n_grayplanes = 2;
  params.i_tick = 640;
  params.tick_rate = 40000;
  params.cycle_rate = 150;
  params.sync_one = 0;
  /*
  params.link_cable.link_type = LINK_TIE;
  params.link_cable.io_addr = VLINK0;
  params.link_cable.timeout = 10;
  params.link_cable.delay = 5;
  //params.link_cable.baudrate(BR9600);
  */
  ticable_get_default_param(&(params.link_cable));

  return 0;
}

/*
  This should be the THIRD function to call. 
  Load a ROM image (images.c).
*/
//int ti68k_loadImage(char *filename, char *file_loaded);

/*
  This is the FOURTH function to call for completely initializing the
  emulation engine.
*/
int ti68k_init(void)
{
  init_hardware();

  cb_init_specific();
  cb_screen_on_off(!0);

#if !defined(__WIN32__) && defined(PENT_COUNTER)
  calibrate_pcounter();	// crash under Win32
#endif

  /* I have noticed that TI92+ does not flicker when LCD is refresh on IRQ1 */
  if(ti68k_getCalcType() == (TI92|MODULEPLUS))
     params.sync_one = 0;

  return 0;
}

/*
  This should be the FIFTH function to call.
  It simply resets the hardware engine.
*/
int ti68k_reset(void)
{
  nBkptAddress = 0;
  listBkptAddress = 0;
  cycle_instr = params.i_tick;

  reset_hardware();

  return 0;
}

/*
  Close the library by exiting HID and exiting hardware the emulation engine
  (free ressources).
*/
int ti68k_exit(void)
{
  cb_exit_specific();
  exit_hardware();

  return 1;
}

/*
  Completely restart the library
*/
int ti68k_restart(void)
{
  ti68k_exit();
  ti68k_init();
  ti68k_reset();
  
  return 0;
}

/*****************/
/* ROM functions */
/*****************/

void* ti68k_getRomPtr(void)
{
  return ti_rom;
}

int ti68k_getRomSize(void)
{
  return params.rom_size;
}

const char *ti68k_getRomVersion(void)
{
  return current_rom_info.version;
}

int ti68k_isRomOk(void)
{
  return current_rom_info.loaded;
}

int ti68k_getRomInfo(ROM_INFO *ri)
{
  ri = &current_rom_info;
  return 0;
}

/*
int ti68k_getUpdateInfo(ROM_INFO *ri)
{
  ri = &current_tib_info;
  return 0;
}
*/

/*
  Write ROM to file
*/
int ti68k_dumpRom(const char *filename)
{
  FILE *f;
  int i;
  int j;
  int c;
  UBYTE *rom = ti_rom;

  f = fopen(filename, "wt");
  if(f == NULL)
    return ERR_CANT_OPEN;

  //iupdate->total = params.rom_size;
  for(i=0; i<params.rom_size; i+=16) // size in Bytes
    {
      //iupdate->count = i;
      //iupdate->percentage = (float)i/params.rom_size;
      //iupdate_pbar();
      
      fprintf(f, "%06X: ", i);
      for(j=0; j<16; j++)
	{
	  fprintf(f, "%02X ", rom[i+j]);
	}
      fprintf(f, "| ");
      for(j=0; j<16; j++)
        {
	  if( (rom[i+j] > 31) && (rom[i+j] < 128) )
	    c = rom[i+j];
	  else
	    c = ' ';
          fprintf(f, "%c", c);
        }
      fprintf(f, "\n");
    }

  fclose(f);

  return 0;
}


/*****************/
/* RAM functions */
/*****************/

void* ti68k_getRamPtr(void)
{
  return ti_ram;
}
	
int ti68k_getRamSize(void)
{
  return params.ram_size;
}

int ti68k_dumpRam(const char *filename)
{
  FILE *f;
  int i;
  int j;
  int c;
  UBYTE *ram = ti_ram;

  f = fopen(filename, "wt");
  if(f == NULL)
    return ERR_CANT_OPEN;

  for(i=0; i<1024*params.ram_size; i+=16) // size in KB
    {
      fprintf(f, "%06X: ", i);
      for(j=0; j<16; j++)
	{
	  fprintf(f, "%02X ", ram[i+j]);
	}
      fprintf(f, "| ");
      for(j=0; j<16; j++)
        {
	  if( (ram[i+j] > 31) && (ram[i+j] < 128) )
	    c = ram[i+j];
	  else
	    c = ' ';
          fprintf(f, "%c", c);
        }
      fprintf(f, "\n");
    }

  fclose(f);

  return 0;
}


/*****************/
/* HID functions */
/*****************/

void* ti68k_getLcdPtr(void)
{
  //UWORD arg = (((UWORD)ti_io[0x10])<<8)|ti_io[0x11];
  //return (&ti_ram[arg<<3]);
  return (&ti_ram[lcd_base_addr]);
}

char ti68k_getContrast(void)
{
  //return ( ((ti_io[0x1d] & 0xf) << 1) | ((ti_io[0] >> 5) & 1));
  //return (io_get_byte(0x1d)&0x0e);
  return contrast;
}

int ti68k_getCalcType(void)
{
  return current_rom_info.calc_type;
}

/***********************/
/* Debugging functions */
/***********************/

int ti68k_disasm(int addr, char *output)
{
  CPTR nextPc;

  MC68000_disasm(addr, &nextPc, 1, output);
  output[strlen(output)-1]='\0'; // strip CR-LF
  return (nextPc-addr);
}

int ti68k_launchDebugger(void)
{
  return cb_launch_debugger();
}

void ti68k_getBreakpointCause(int *type, int *id, int *mode) 
{
  *type = breakType;
  *mode = breakMode;
  *id = breakId;
}

int ti68k_doSingleStep(void)
{
  return specialflags |= SPCFLAG_DBTRACE;
}

/*
  Do 'n' instructions.
  Return 0 if successful, a negative value if an error occured,
  a positive value if a breakpoint has been encountered.

  Note: replace M68000_run() from UAE
*/
int ti68k_doInstructions(int n) //fait n instructions
{
  if (!ti68k_isRomOk())
    return ERR_ROM_NOT_LOADED;

  return hw_run_m68k(n);
}

/*****************************/
/* Registers access routines */
/*****************************/

void ti68k_setDataRegister(int n,int val)
{
  if (n>=0 && n<8) regs.d[n] = val;
}

void ti68k_setAddressRegister(int n,int val)
{
  if (n>=0 && n<8) regs.a[n] = val;
}

void ti68k_setSpRegister(int val)
{
  regs.usp=val;
}

void ti68k_setPcRegister(int val)
{
  regs.pc=val;
}

void ti68k_setSrRegister(int val)
{
  regs.sr=val;
}

void ti68k_setFlagRegister(unsigned char flag)
{
  //TODO
}

int ti68k_getDataRegister(int n)
{
  if (n>=0 && n<8) return regs.d[n];
  return 0;
}

int ti68k_getAddressRegister(int n)
{
  if (n>=0 && n<8) return regs.a[n];
  return 0;
}
	
int ti68k_getSpRegister()
{
  return regs.usp;
}

int ti68k_getPcRegister()
{
  return regs.pc;
}

int ti68k_getSrRegister()
{
  return regs.sr;
}

char *ti68k_getFlagRegister(void)
{
  static char str_SR[128];
  sprintf(str_SR, "T=%d S=%d X=%d N=%d\nZ=%d V=%d C=%d IMASK=%d\n",
	  regs.t, regs.s, regs.x, NFLG, ZFLG, VFLG, CFLG, regs.intmask);
  return str_SR;
}


/******************/
/* Link functions */
/******************/

extern int cmdState;
extern UBYTE *backBuf;
/*
  Remark: there is a bug here... If the buffer is allocated with malloc, GtkTiEmu will segfault (backBuf address out of bounds). If the buffer is allocated on the heap as an array, there will be no problem. Moreover, this problem does not appear when we send a file via the command mode, only in GUI mode.
Then, there may be a problem of shared memory or something else...
*/
int ti68k_sendFile(const char *filename)
{
  return send_ti_file(filename);
}

extern int TO_VALUE;

int ti68k_setInternalLinkTimeout(int value)
{
  TO_VALUE = value;
  return value;
}

int ti68k_getInternalLinkTimeout(int value)
{
  return TO_VALUE;
}

int ti68k_reconfigure_linkport()
{
  hw_exit_dbus();
  hw_init_dbus();

  return 0;
}
