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

#ifndef __TI68K_CORE__
#define __TI68K_CORE__

#include "tilibs.h"
#include "keydefs.h"
#include "bkpts.h"
#include "state.h"
#include "consts.h"

#include "callbacks.h"
#include "images.h"

#ifdef __WIN32__
# define str_cmp(s1, s2) (stricmp((s1), (s2)))
#else
# define str_cmp(s1, s2) (strcasecmp((s1), (s2)))
#endif

/*************/
/* Constants */
/*************/  

#define MAXCHARS 256
  
/*************/
/* Functions */
/*************/

// Initialization
int ti68k_loadDefaultConfig(void);
int ti68k_init(void);
int ti68k_reset(void);
int ti68k_exit(void);

int ti68k_restart(void);

// ROM
void* ti68k_getRomPtr(void);
int ti68k_getRomSize(void);
const char *ti68k_getRomVersion(void);
int ti68k_isRomOk(void);
int ti68k_getRomInfo(ROM_INFO *ri);
int ti68k_dumpRom(const char *filename);

// RAM
void* ti68k_getRamPtr(void);
int ti68k_getRamSize(void);
int ti68k_dumpRam(const char *filename);

// LCD
void* ti68k_getLcdPtr(void);
char ti68k_getContrast(void);
int ti68k_getCalcType(void);

// Debugging
int ti68k_disasm(int addr, char *output);
int ti68k_launchDebugger(void);
void ti68k_getBreakpointCause(int *type, int *id, int *mode);
int ti68k_doSingleStep(void);
int ti68k_doInstructions(int n);

// Register access
void ti68k_setDataRegister(int n,int val);
void ti68k_setAddressRegister(int n,int val);
void ti68k_setSpRegister(int val);
void ti68k_setPcRegister(int val);
void ti68k_setSrRegister(int val);
void ti68k_setFlagRegister(unsigned char flag);
int  ti68k_getDataRegister(int n);
int  ti68k_getAddressRegister(int n);
int ti68k_getSpRegister();
int ti68k_getPcRegister();
int ti68k_getSrRegister();
char *ti68k_getFlagRegister(void);

// Link
int ti68k_sendFile(const char *filename);
int ti68k_setInternalLinkTimeout(int value);
int ti68k_getInternalLinkTimeout(int value);
int ti68k_reconfigure_linkport(void);
#define ti68k_reconfigureLinkport ti68k_reconfigure_linkport


/***********/
/* Externs */
/***********/

void ti68k_setActiveKey(int key, int active);
int ti68k_isKeyPressed(int key);



/**************************/
/* Compatibility wrappers */
/**************************/

#define ti68k_initLib68k  ti68k_init
#define ti68k_resetLib68k ti68k_reset
#define ti68k_exitLib68k  ti68k_exit

#endif
