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
    Linkport management (D-bus serial input/output)
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "uae.h"

#include "tilibs.h"
#include "ti68k_err.h"
#include "ti68k_def.h"
#include "ti68k_int.h"

/*
	Linkport (lp) / directfile (df) mappers
*/
void  (*hw_dbus_putbyte)    (UBYTE arg);
UBYTE (*hw_dbus_getbyte)    (void);
int   (*hw_dbus_byteavail)	(void);
int   (*hw_dbus_checkread)  (void);

void  lp_putbyte(UBYTE arg);
UBYTE lp_getbyte(void);
int   lp_byteavail(void);
int   lp_checkread(void);

void  df_putbyte(UBYTE arg);
UBYTE df_getbyte(void);
int   df_byteavail(void);
int   df_checkread(void);

/* 
	Variables: External link port for direct file loading	
*/

TicableLinkCable *ilc = NULL;
TicalcFncts itc;
TicalcInfoUpdate iu = { 0 };

int lc_internal = 0;
int lc_speedy = 0;
int byte_t2f, byte_f2t;
int iput = 0;
int iget = 0;
int init_linkfile();

/* 
	Variables: Internal link port for D-BUS emulation
*/
TicableLinkCable lc;
int byteAvail = 0;
UBYTE lastByte;
int lc_timeout = 0;

int comError;
int transflag=0;
int transbyte;
int transnotready;
int recvflag=0;
int recvbyte;
int lc_raw_access;

int TO_VALUE = 1000;

void print_lc_error(int errnum)
{
  char msg[MAXCHARS] = "No error -> bug !\n";

  ticable_get_error(errnum, msg);
  DISPLAY("Link cable error: code = %i, msg = %s\n", errnum, msg);
  //iupdate_msgbox("Error", msg);
}

/*
  D-bus management (HW linkport)
*/

int hw_dbus_init(void)
{
	int err;

	transflag=0; recvflag=0;
	transnotready=0;
	lc_raw_access=0;

	// init linkport
	ticable_init();
	ticable_set_param(&link_cable);
	ticable_set_cable(link_cable.link_type, &lc);
	if( (err=lc.init()) )
    {
		print_lc_error(err);
		return;
    }
	if( (err=lc.open()) )
    {
		print_lc_error(err);
		return;
    }

	// init directfile
	init_linkfile();

	// set mappers to linkport
	hw_dbus_putbyte = lp_putbyte;
	hw_dbus_getbyte = lp_getbyte;
	hw_dbus_byteavail = lp_byteavail;
	hw_dbus_checkread = lp_checkread;
}

int hw_dbus_reset(void)
{
	return 0;
}

int hw_dbus_exit(void)
{
	int err;

	// exit linkport
	if( (err=lc.close()) )
    {
		print_lc_error(err);
		return;
    }
	if( (err=lc.exit()) )
    { 
		print_lc_error(err);
		return;
    }

	// exit directfile
}

/*
	Linkport access
*/

static int   lp_avail_byte;
static UBYTE lp_last_byte;

static void lp_putbyte(UBYTE arg)
{
	int err;
  
	lc_timeout = 0;

	if( (err=lc.put(arg)) )
	{
	  print_lc_error(err);
	  return;
	}
}

static UBYTE lp_getbyte(void)
{
	lc_timeout = 0;
	lp_avail_byte = 0;

	return lp_last_byte;
}

static int lp_byteavail(void)
{
	return lp_avail_byte;
}

static int lp_checkread(void)
{
	int err = 0;
	int status = 0;

	if(lp_avail_byte)
		return 0;

	if( (err=lc.check(&status)) )
    {
		print_lc_error(err);
		byteAvail = 0;
    }
  
	if(status & STATUS_RX)
    {
		if( (err=lc.get(&lp_last_byte)) )
        {
			print_lc_error(err);
        }
		lp_avail_byte = 1;
    }
	else
		lp_avail_byte = 0;
  
	return lp_avail_byte;
}


/*
	Directfile access
 */

void df_putbyte(UBYTE arg)
{
	lc_timeout = 0;

	byte_t2f = arg;
	iget = 1;
}

UBYTE df_getbyte(void)
{
	lc_timeout = 0;

	iput = 0;
    return byte_f2t;
}

int df_byteavail(void)
{
    return iput;
}

int df_checkread(void)
{
	int err = 0;
	int status = 0;

    return iput;
}

/***************/

void linkport_putbyte(UBYTE arg)
{
  int err;
  
  lc_timeout = 0;
  if(lc_internal)
    {
      byte_t2f = arg;
      iget = 1;
    }  
  else
    {
      if( (err=lc.put(arg)) )
	{
	  print_lc_error(err);
	  return;
	}
    }
}

UBYTE linkport_getbyte(void)
{
  lc_timeout = 0;
  if(lc_internal) 
    {
      iput = 0;
      return byte_f2t;
    }

  byteAvail = 0;
  return lastByte;
}

int linkport_byteavail(void)
{
  if(lc_internal) 
    return iput;

  return byteAvail;  
}

int linkport_checkread(void)
{
  int err = 0;
  int status = 0;

  if(lc_internal) 
    return iput;

  if(byteAvail)
    return 0;

  if( (err=lc.check(&status)) )
    {
      print_lc_error(err);
      byteAvail = 0;
    }
  if(status & STATUS_RX)
    {
      if( (err=lc.get(&lastByte)) )
        {
          print_lc_error(err);
        }
      byteAvail = 1;
    }
  else
    byteAvail = 0;
  
  return byteAvail;
}

/*
  Internal link port emulation for direct sending/receiving files.
  The code below is not very obvious at first glance. This is the
  reason why I am explaining the idea and mechanisms.

  The idea is to use TiLP's libticalcs since libticalcs contains all the code
  needed for communicating with a TI (software protocol, file handling). 
  The libticalcs is built on libticables, a library which handles link cables
  (hardware protocol).
  In fact, I simply reimplement the basic functions of libticables usually 
  used by libticalcs for sending/receiving data. These functions exchange
  bytes with the linkport at HW level (io.c).
  The libticalcs provided the abstraction we need for this.
*/

/* libticables functions (link API) */
int ilp_init_port()     { return 0; }
int ilp_open_port()     { return 0; }

int ilp_put(uint8_t data)
{ 
  byte_f2t = data; 
  iput = 1;
  while(iput) 
    { 
      ti68k_debug_do_instructions(1); 
    };
  
  return 0; 
}

int ilp_get(uint8_t *data)
{ 
  while(!iget) 
    { 
      ti68k_debug_do_instructions(1); 
    };
  *data = byte_t2f;
  iget = 0; 
  
  return 0; 
}

int ilp_probe_port()    { return 0; }
int ilp_close_port()    { return 0; }
int ilp_term_port()     { return 0; }
int ilp_check_port(int *st) { return 0; }

void ilp_start()   { }
void ilp_stop()    { }
void ilp_refresh() { }
void ilp_pbar()    { }
void ilp_label()   { }

/* Initialize a pseudo link cable to be connected with HW */
int init_linkfile()
{
  if(ilc != NULL)
    free(ilc);
  ilc = (TicableLinkCable *)malloc(sizeof(TicableLinkCable));
  if(ilc == NULL)
    return ERR_68K_MALLOC;

  ilc->init  = ilp_init_port;
  ilc->open  = ilp_open_port;
  ilc->put   = ilp_put;
  ilc->get   = ilp_get;
  ilc->close = ilp_close_port;
  ilc->exit  = ilp_term_port;
  ilc->probe = ilp_probe_port;
  ilc->check = ilp_check_port;

  ticalc_set_cable(ilc);

  switch(tihw.calc_type)
    {
    case TI92: ticalc_set_calc(CALC_TI92, &itc);
      break;
    case TI89: ticalc_set_calc(CALC_TI89, &itc);
      break;
    case TI92p: ticalc_set_calc(CALC_TI92P, &itc);
      break;
	case V200: ticalc_set_calc(CALC_V200, &itc);
      break;
    default: return ERR_68K_INTERNAL;
      break;
    }

  ticalc_set_update(&iu, ilp_start, ilp_stop, ilp_refresh,
		    ilp_pbar, ilp_label);

  return 0;
}

int test_sendfile()
{
  lc_speedy = 1;
  lc_internal = 1;  
  itc.send_var("/root/str.89s", 0, NULL);
  lc_internal = 0;
  lc_speedy = 0;

  return 0;
}

int send_ti_file(const char *filename)
{
  char *ext;

  /* Get extension */
  ext = strrchr(filename, '.');
  if(ext == NULL)
    return ERR_68K_TI_FILE;
  else
    ext++;
  
  /* TI file ? */
  if(0) /*
  if(!(strstr(ext, "89") && (tihw.calc_type == TI89)) ||
     (strstr(ext, "92") && (tihw.calc_type == TI92))  ||
     (strstr(ext, "9x") && (tihw.calc_type == TI92p)) ||
     (strstr(ext, "9X") && (tihw.calc_type == TI92p)) ) */
    {
      return ERR_68K_TI_FILE;
    }

  /* FLASH APP file ? */
  else if( (ext[2] == 'k') || (ext[2] =='K'))
    {
      lc_speedy = 1;
      lc_internal = 1;  
      itc.send_flash(filename, MODE_APPS);
      lc_internal = 0;
      lc_speedy = 0;
    }

  /* FLASH OS file ? */
  else if( (ext[2] == 'u') || (ext[2] == 'U'))
    {
      lc_speedy = 1;
      lc_internal = 1;  
      itc.send_flash(filename, MODE_AMS);
      lc_internal = 0;
      lc_speedy = 0;
    }
  
  /* Backup file ? */
  else if( (ext[2] == 'b') || (ext[2] == 'B'))
    {
      lc_speedy = 1;
      lc_internal = 1;  
      itc.send_backup(filename, MODE_NORMAL);
      lc_internal = 0;
      lc_speedy = 0;
    }

  /* Group file ? */
  else if( (ext[2] == 'g') || (ext[2] == 'G'))
    {
      lc_speedy = 1;
      lc_internal = 1;  
      itc.send_var(filename, MODE_NORMAL, NULL);
      lc_internal = 0;
      lc_speedy = 0;
    }

  /* Single file */
  else
    {
      lc_speedy = 1;
      lc_internal = 1;  
      itc.send_var(filename, MODE_NORMAL, NULL);
      lc_internal = 0;
      lc_speedy = 0;
    }

  return 0;
}




