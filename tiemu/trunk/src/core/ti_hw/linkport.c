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
  TI linkport management (D-bus serial input/output)
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
#include "params.h"
#include "ti68k_def.h"
#include "ti68k_int.h"

/* Internal link port */
TicableLinkCable *ilc = NULL;
TicalcFncts itc;
TicalcInfoUpdate iu = { 0 };
int lc_internal = 0;
int lc_speedy = 0;
int byte_t2f, byte_f2t;
int iput = 0;
int iget = 0;
int init_linkfile();

/* External link port */
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
//int emuLink=1;
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

void hw_init_dbus()
{
  int err;

  transflag=0; recvflag=0;
  transnotready=0;
  lc_raw_access=0;

  ticable_init();
  ticable_set_param(&(params.link_cable));
  ticable_set_cable(params.link_cable.link_type, &lc);
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

  init_linkfile();
}

void hw_exit_dbus()
{
  int err;

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
}

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
      ti68k_doInstructions(1); 
    };
  
  return 0; 
}

int ilp_get(uint8_t *data)
{ 
  while(!iget) 
    { 
      ti68k_doInstructions(1); 
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

  switch(ti68k_getCalcType())
    {
    case TI92: ticalc_set_calc(CALC_TI92, &itc);
      break;
    case TI89: ticalc_set_calc(CALC_TI89, &itc);
      break;
    case TI92 | MODULEPLUS: ticalc_set_calc(CALC_TI92P, &itc);
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
  if(!(strstr(ext, "89") && (ti68k_getCalcType() == TI89)) ||
     (strstr(ext, "92") && (ti68k_getCalcType() == TI92))  ||
     (strstr(ext, "9x") && (ti68k_getCalcType() == TI92p)) ||
     (strstr(ext, "9X") && (ti68k_getCalcType() == TI92p)) ) */
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




