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
  Manage state images
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include "uae.h"
#include "interface.h"
#include "tilibs.h"
#include "./ti_hw/memory.h"
#include "linkport.h"
#include "errcodes.h"
#include "timer.h"
#include "lcd.h"
#include "../ti_hw/ioports.h"

int loadState_old(char *filename)
{
  FILE *fp;
  int m;

  DISPLAY("Loading RAM image (TiEmu format): %s\n", filename);
  if(!strcmp(filename, ""))
	  return ERR_68K_NONE;
  if( (fp = fopen(filename, "rb")) == NULL)
    return ERR_68K_CANT_OPEN;

  fread(&m, 1, sizeof(int), fp);
  if(m != RAM_SIZE) 
    {
      fclose(fp);
      return -2;
    }
  else
    {
      fread(ti_ram, 1, RAM_SIZE, fp);
      fread(ti_io, 1, IO_SIZE, fp);
      fread(&regs, sizeof(regs), 1, fp);
      fread(&timer_init, sizeof(timer_init), 1, fp);
      fread(&specialflags, sizeof(specialflags), 1, fp);
      fclose(fp);
      MakeFromSR();
      m68k_setpc(regs.pc);
      //update_contrast();
    }
  
  return ERR_68K_NONE;
}

int ti68k_saveState_old(char *filename)
{
  FILE *fp;
  int m = RAM_SIZE;

  if(strlen(filename)) // name exist ?
    {
      if( (fp = fopen(filename, "wb")) == NULL)
	return ERR_68K_CANT_OPEN;
      
      m68k_setpc(m68k_getpc());
      MakeSR();
      fwrite(&m, sizeof(int), 1, fp);     // write RAM_SIZE
      fwrite(ti_ram, 1, RAM_SIZE, fp);    // write RAM mem
      fwrite(ti_io, 1, IO_SIZE, fp);      // write IO  mem
      fwrite(&regs, sizeof(regs), 1, fp); // dump registers
      fwrite(&timer_init, sizeof(timer_init), 1, fp);       // write timer
      fwrite(&specialflags, sizeof(specialflags), 1, fp); // and flags
      fclose(fp);
    }

  return ERR_68K_NONE;
}

int loadState_vti(char *filename)
{
  FILE *fp;
  int kbmask=0;
  int comError=0;
  int cpuCompleteStop=0;
  char str[9], romName[56];
  int i;
  UBYTE* mem[256];
  int int0Count = 0;

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
  fread(ti_ram,256,1024,fp);
  fread(&memprotect,4,1,fp); fread(&ram128,4,1,fp);
  fread(&timer_value,4,1,fp); fread(&timer_init,4,1,fp);
  fread(&int0Count,4,1,fp);
  fread(&io0Bit7,4,1,fp); fread(&io0Bit2,4,1,fp);
  fread(&kbmask,4,1,fp);
  fread(&ram_wrap,4,1,fp); fread(&(mem_and),4,1,fp);
  fread(&comError,4,1,fp);
  fread(&transflag,4,1,fp); fread(&transbyte,4,1,fp);
  fread(&transnotready,4,1,fp);
  fread(&recvflag,4,1,fp); fread(&recvbyte,4,1,fp);
  fread(&rom_ret_or,4,1,fp);
  fread(&lcd_base_addr,4,1,fp); fread(&lcd_off,4,1,fp);
  fread(&contrast,4,1,fp);
  fread(&cpuCompleteStop,4,1,fp);
  for (i=0;i<32;i++)
    mem[i]=&ti_ram[(i&(mem_and>>16))<<16];

  // GtkTiEmu specific
  fread(ti_io, 1, IO_SIZE, fp);      // write IO
  fread(&specialflags, sizeof(specialflags), 1, fp); // and flags
  fclose(fp);
  
  return ERR_68K_NONE;
}

/*
  This function saves emulator settings in the VTi format
*/
int saveState_vti(char *filename)
{
  FILE *fp;
  int kbmask=0;
  int comError=0;
  int cpuCompleteStop=0;
  int int0Count = 0;

  if(strlen(filename)) // name exist ?
    {
      if( (fp = fopen(filename, "wb")) == NULL)
	return ERR_68K_CANT_OPEN;

      m68k_setpc(m68k_getpc());
      fprintf(fp,"GTKTIEMU");
      fwrite(filename,56,1,fp);
      MakeSR();
      fwrite(&regs,sizeof(regs),1,fp); // struct format differs !
      fwrite(ti_ram,256,1024,fp);
      fwrite(&memprotect,4,1,fp); fwrite(&ram128,4,1,fp);
      fwrite(&timer_value,4,1,fp); fwrite(&timer_init,4,1,fp);
      fwrite(&int0Count,4,1,fp);
      fwrite(&io0Bit7,4,1,fp); fwrite(&io0Bit2,4,1,fp);
      fwrite(&kbmask,4,1,fp);
      fwrite(&ram_wrap,4,1,fp); fwrite(&(mem_and),4,1,fp);
      fwrite(&comError,4,1,fp);
      fwrite(&transflag,4,1,fp); fwrite(&transbyte,4,1,fp);
      fwrite(&transnotready,4,1,fp);
      fwrite(&recvflag,4,1,fp); fwrite(&recvbyte,4,1,fp);
      fwrite(&rom_ret_or,4,1,fp);
      fwrite(&lcd_base_addr,4,1,fp); fwrite(&lcd_off,4,1,fp);
      fwrite(&contrast,4,1,fp);
      fwrite(&cpuCompleteStop,4,1,fp);
      
      // GtkTiEmu specific
      fwrite(ti_io, 1, IO_SIZE, fp);      // write IO
      fwrite(&specialflags, sizeof(specialflags), 1, fp); // and flags
      fclose(fp);
    }

  return ERR_68K_NONE;
}

/*
  Must be done between init_hardware and M68000_run.
  Typically called after initLib68k.
  This function (re)load the state of the calculator.
  It automagically determine the state file format.

  Return an error code if an error occured, 0 otherwise
*/
int ti68k_loadState(char *filename)
{
  FILE *fp;
  char str[MAXCHARS];

  if(!strcmp(filename, ""))
    return ERR_68K_NONE;
  if( (fp = fopen(filename, "rb")) == NULL)
    return ERR_68K_CANT_OPEN;
  else
    {
      fgets(str, MAXCHARS, fp);
      if(strstr(str, "VTIv2.0 ") || strstr(str, "GTKTIEMU"))
	return loadState_vti(filename);
      else
	return loadState_old(filename);
    }

  return ERR_68K_NONE;
}

/*
  This function save the state of the calculator.
  Can be called at any time.

  Return an error code if an error occured, 0 otherwise
*/
int ti68k_saveState(char *filename)
{
  return saveState_vti(filename);
}
