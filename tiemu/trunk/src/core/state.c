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
  	State images
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include "libuae.h"
#include "dbus.h"
#include "timer.h"
#include "lcd.h"
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
	FILE *f;
  	IMG_INFO *img = &img_infos;
  	IMG_INFO sav;
  
  	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;
  
  	// Open file
  	DISPLAY("Loading state image (TiEmu v2.00 format): %s\n", filename);
  	f = fopen(filename, "rb");
  	if(f == NULL)
  		return ERR_CANT_OPEN;
  	
  	// Compare image infos
	fread(&sav, 1, sizeof(IMG_INFO), f);
	if(memcmp(&sav, img, sizeof(IMG_INFO)))
		return ERR_INVALID_STATE;
	
	// Load internal hardware (registers and special flags)
    fwrite(&regs, sizeof(regs), 1, f);
    fwrite(&specialflags, sizeof(specialflags), 1, f);

	MakeFromSR();
	m68k_setpc(m68k_getpc());
    
    // Load I/O ports state
    fread(tihw.io , IO_SIZE, 1, f);
    fread(tihw.io2, IO_SIZE, 1, f);
    
    // Load RAM content
    fread(tihw.ram, 256*KB, 1, f);
    
    fclose(f); 
  
  	return 0;
}


/*
  	This function saves the state of the calculator.
  	Can be called at any time.

  	Return an error code if an error occured, 0 otherwise
*/
int ti68k_state_save(char *filename)
{
  	FILE *f;
  	IMG_INFO *img = &img_infos;
  
  	if(!strlen(filename))
  		return ERR_CANT_OPEN;
  
  	// Open file
  	DISPLAY("Saving state image (TiEmu v2.00 format): %s\n", filename);
  	f = fopen(filename, "wb");
  	if(f == NULL)
  		return ERR_CANT_OPEN;
  	
  	// Save current image infos
	fwrite(img, 1, sizeof(IMG_INFO), f);
	
	// Save internal hardware
    m68k_setpc(m68k_getpc());
    MakeSR();
    
    // Save registers and special flags
    fwrite(&regs, sizeof(regs), 1, f);
    fwrite(&specialflags, sizeof(specialflags), 1, f);
    
    // Save I/O ports state
    fwrite(tihw.io , IO_SIZE, 1, f);
    fwrite(tihw.io2, IO_SIZE, 1, f);
    
    // Save RAM content
    fwrite(tihw.ram, 256*KB, 1, f);
    
    fclose(f);

  	return 0;
}
