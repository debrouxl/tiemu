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

static void map_to_linkport(void)
{
    hw_dbus_putbyte = lp_putbyte;
	hw_dbus_getbyte = lp_getbyte;
	hw_dbus_byteavail = lp_byteavail;
	hw_dbus_checkread = lp_checkread;
}

static void map_to_directfile(void)
{
    hw_dbus_putbyte = df_putbyte;
	hw_dbus_getbyte = df_getbyte;
	hw_dbus_byteavail = df_byteavail;
	hw_dbus_checkread = df_checkread;
}

TicableLinkCable lc;

int TO_VALUE = 1000;

static void print_lc_error(int errnum)
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
	map_to_linkport();
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
    exit_linkfile();
}

/*
	Linkport access
*/

static int   lp_avail_byte;
static UBYTE lp_last_byte;

static void lp_putbyte(UBYTE arg)
{
	int err;
  
	tihw.lc_timeout = 0;

    err = lc.put(arg);
	if(err)
	{
	  print_lc_error(err);
	  return;
	}
}

static UBYTE lp_getbyte(void)
{
	tihw.lc_timeout = 0;
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

    err = lc.check(&status);
	if(err)
    {
		print_lc_error(err);
		lp_last_byte = 0;
    }
  
	if(status & STATUS_RX)
    {
        err = lc.get(&lp_last_byte);
		if(err)
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
 
int byte_t2f;
int byte_f2t;
int iput;
int iget;

void df_putbyte(UBYTE arg)
{
	tihw.lc_timeout = 0;

	byte_t2f = arg;
	iget = 1;
}

UBYTE df_getbyte(void)
{
	tihw.lc_timeout = 0;

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

	Wonderful, isn't it ?! Take a look at the 'TiLP framework' power ;-)
*/

TicableLinkCable* 	ilc = NULL;
TicalcFncts			itc;
TicalcInfoUpdate 	iu = { 0 };

/* libticables functions (link API) */
static int ilp_init_port()     { return 0; }
static int ilp_open_port()     { return 0; }

static int ilp_put(uint8_t data)
{ 
  	byte_f2t = data; 
  	iput = 1;
  	
  	while(iput) 
    { 
      	ti68k_debug_do_instructions(1); 
    };
  
  	return 0; 
}

static int ilp_get(uint8_t *data)
{ 
  	while(!iget) 
    { 
      	ti68k_debug_do_instructions(1);
    };
    
  	*data = byte_t2f;
  	iget = 0; 
  
  return 0; 
}

static int ilp_probe_port()    	{ return 0; }
static int ilp_close_port()    	{ return 0; }
static int ilp_term_port()     	{ return 0; }
static int ilp_check_port(int *st) { return 0; }

static void ilp_start()   { }
static void ilp_stop()    { }
static void ilp_refresh() { }
static void ilp_pbar()    { }
static void ilp_label()   { }

/* Initialize a pseudo link cable to be connected with HW */
static int init_linkfile()
{
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

static int exit_linkfile(void)
{
    if(ilc != NULL)
	    free(ilc);
    ilc = NULL;
}

int test_sendfile()
{
    map_to_directfile();
    tihw.lc_speedy = 1;
    tihw.lc_file = 1;
    //itc.send_var("/root/str.89s", 0, NULL);
    itc.send_var("C:\\str.9xs", 0, NULL);
    tihw.lc_file = 0;
    tihw.lc_speedy = 0;
    map_to_linkport();

    return 0;
}

int send_ti_file(const char *filename)
{
    gint ok = 0;
    gchar *ext;

    // Check for TI file
    if(!tifiles_is_a_ti_file(filename))
        return ERR_68K_TI_FILE;

    if(((tifiles_which_calc_type(filename) == CALC_TI89) && (tihw.calc_type == TI89)) ||
        ((tifiles_which_calc_type(filename) == CALC_TI92) && (tihw.calc_type == TI92)) ||
        ((tifiles_which_calc_type(filename) == CALC_TI92P) && (tihw.calc_type == TI92p)) ||
        ((tifiles_which_calc_type(filename) == CALC_V200) && (tihw.calc_type == V200)))
    {
        ok = 1;
    } else
        return ERR_68K_TI_FILE;

    // FLASH APP file ?
    if(tifiles_is_a_flash_file(filename) && !strcmp(tifiles_flash_app_file_ext(), tifiles_get_extension(filename)))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        tihw.lc_file = 1;  
        itc.send_flash(filename, MODE_APPS);
        tihw.lc_file = 0;
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

    // FLASH OS file ?
    if(tifiles_is_a_flash_file(filename) && !strcmp(tifiles_flash_os_file_ext(), tifiles_get_extension(filename)))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        tihw.lc_file = 1;  
        itc.send_flash(filename, MODE_AMS);
        tihw.lc_file = 0;
        tihw.lc_speedy = 0;
        map_to_linkport();
    }
  
    // Backup file ?
    else if(tifiles_is_a_backup_file(filename))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        tihw.lc_file = 1;  
        itc.send_backup(filename, MODE_NORMAL);
        tihw.lc_file = 0;
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

    // Group file ?
    else if(tifiles_is_a_group_file(filename))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        tihw.lc_file = 1;  
        itc.send_var(filename, MODE_NORMAL, NULL);
        tihw.lc_file = 0;
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

    // Single file
    else if(tifiles_is_a_single_file(filename))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        tihw.lc_file = 1;  
        itc.send_var(filename, MODE_NORMAL, NULL);
        tihw.lc_file = 0;
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

  return 0;
}




