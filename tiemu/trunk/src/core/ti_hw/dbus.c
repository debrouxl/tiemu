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

#include "libuae.h"
#include "ports.h"
#include "dbus.h"
#include "m68k.h"

#include "tilibs.h"
#include "ti68k_err.h"
#include "ti68k_def.h"
#include "ti68k_int.h"

/*
	Linkport (lp) / directfile (df) mappers
*/

void  (*hw_dbus_putbyte)    (uint8_t arg);
uint8_t (*hw_dbus_getbyte)    (void);
int   (*hw_dbus_byteavail)	(void);
int   (*hw_dbus_checkread)  (void);

static void    lp_putbyte(uint8_t arg);
static uint8_t lp_getbyte(void);
static int     lp_byteavail(void);
static int     lp_checkread(void);

static void    df_putbyte(uint8_t arg);
static uint8_t df_getbyte(void);
static int     df_byteavail(void);
static int     df_checkread(void);

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

static int init_linkfile(void);
static int exit_linkfile(void);

int hw_dbus_init(void)
{
	int err;

	// init linkport
	ticable_init();
	ticable_set_param(&link_cable);
	ticable_set_cable(link_cable.link_type, &lc);
	if((err = lc.init()))
	{
		print_lc_error(err);
		return -1;
	}
	if((err = lc.open()))
	{
		print_lc_error(err);
		return -1;
	}

	// init directfile
	init_linkfile();

	// set mappers to linkport
	map_to_linkport();

    return 0;
}

int hw_dbus_reset(void)
{
	return 0;
}

int hw_dbus_exit(void)
{
	int err;

	// exit linkport
	if((err = lc.close()))
	{
		print_lc_error(err);
		return -1;
	}
	if((err = lc.exit()))
	{ 
		print_lc_error(err);
		return -1;
	}
	
	// exit directfile
	exit_linkfile();

    return 0;
}

/*
	Linkport access
*/

static int   lp_avail_byte;
static uint8_t lp_last_byte;

static void lp_putbyte(uint8_t arg)
{
	int err;
  
    err = lc.put(arg);
	if(err)
	{
		io_bit_set(0x0d,7);	// error
		print_lc_error(err);
		return;
	}
	io_bit_set(0x0d,6);		// tx buffer empty
	io_bit_set(0x0d,3);		// link activity
}

static uint8_t lp_getbyte(void)
{
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
	    io_bit_set(0x0d,7);		// error
	    print_lc_error(err);
	    lp_last_byte = 0;
	}
  
	if(status & STATUS_RX)
	{
        err = lc.get(&lp_last_byte);
		if(err)
        {
			io_bit_set(0x0d,7);	// error
			print_lc_error(err);
        }

		io_bit_set(0x0d,5);		// rx buf full
		io_bit_set(0x0d,3);		// link activity
		lp_avail_byte = 1;
    }
	else
		lp_avail_byte = 0;
  
	return lp_avail_byte;
}


/*
	Directfile access
*/
 
int t2f_data;   // ti => file data
int t2f_flag;   // data available

int f2t_data;   // file => ti data
int f2t_flag;   // data available

void df_putbyte(uint8_t arg)
{
	t2f_data = arg;
	t2f_flag = 1;
}

uint8_t df_getbyte(void)
{
	f2t_flag = 0;
    return f2t_data;
}

int df_byteavail(void)
{
    return f2t_flag;
}

int df_checkread(void)
{
    return f2t_flag;    // useless
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
    bytes with the linkport at HW level (ports.c).
    The libticalcs provides the abstraction we need for this.

	Wonderful, isn't it ?! Take a look at the 'TiLP framework' power ;-)
*/

TicableLinkCable* 	ilc = NULL;
TicalcFncts			itc;
TicalcInfoUpdate 	iu = { 0 };

/* libticables functions (link API) */
static int ilp_init_port(void)     { return 0; }
static int ilp_open_port(void)     { return 0; }

static int ilp_put(uint8_t data)
{ 
	tiTIME clk;

  	f2t_data = data; 
  	f2t_flag = 1;

	//io_bit_set(0x0d,5);		// rx buffer full
	//io_bit_set(0x0d,3);		// link activity

	toSTART(clk);
  	while(f2t_flag) 
    { 
      	ti68k_debug_do_instructions(1); 
		if(toELAPSED(clk, 20))	// 2s
			return ERR_WRITE_TIMEOUT;
    };
  
  	return 0;
}

static int ilp_get(uint8_t *data)
{ 
	tiTIME clk;

	toSTART(clk);
  	while(!t2f_flag) 
    { 
      	ti68k_debug_do_instructions(1);
		if(toELAPSED(clk, 20))
			return ERR_WRITE_TIMEOUT;
    };
    
  	*data = t2f_data;
  	t2f_flag = 0;

	//io_bit_set(0x0d,6);		// tx buffer empty
	//io_bit_set(0x0d,3);		// link activity
  
	return 0;
}

static int ilp_probe_port(void)    	{ return 0; }
static int ilp_close_port(void)    	{ return 0; }
static int ilp_term_port(void)     	{ return 0; }
static int ilp_check_port(int *st) { return 0; }

/* libticalcs functions (GUI callbacks API) */
static void ilp_start(void)   { }
static void ilp_stop(void)    { }
static void ilp_refresh(void) { }
static void ilp_pbar(void)    { }
static void ilp_label(void)   { }

/* Initialize a pseudo link cable to be connected with HW */
static int init_linkfile(void)
{
  	ilc = (TicableLinkCable *)malloc(sizeof(TicableLinkCable));
  	if(ilc == NULL)
    	return ERR_MALLOC;

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
    	default: return ERR_NONE;
      	break;
    }

  	ticalc_set_update(&iu, ilp_start, ilp_stop, ilp_refresh,
		    ilp_pbar, ilp_label);

    t2f_flag = 0;
    f2t_flag = 0;

  	return 0;
}

static int exit_linkfile(void)
{
    if(ilc != NULL)
	    free(ilc);
    ilc = NULL;

    return 0;
}

int test_sendfile(void)
{
    map_to_directfile();
    tihw.lc_speedy = 1;
    //itc.send_var("/root/str.89s", 0, NULL);
    itc.send_var("C:\\str.9xs", 0, NULL);
    tihw.lc_speedy = 0;
    map_to_linkport();

    return 0;
}

int send_ti_file(const char *filename)
{
    gint ok = 0;
	int ret;

    // Check for TI file
    if(!tifiles_is_a_ti_file(filename))
        return ERR_NOT_TI_FILE;

    if(((tifiles_which_calc_type(filename) == CALC_TI89) && (tihw.calc_type == TI89)) ||
        ((tifiles_which_calc_type(filename) == CALC_TI92) && (tihw.calc_type == TI92)) ||
        ((tifiles_which_calc_type(filename) == CALC_TI92P) && (tihw.calc_type == TI92p)) ||
        ((tifiles_which_calc_type(filename) == CALC_V200) && (tihw.calc_type == V200)))
    {
        ok = 1;
    } else
        return ERR_NOT_TI_FILE;

    t2f_flag = 0;
    f2t_flag = 0;

    // FLASH APP file ?
    if(tifiles_is_a_flash_file(filename) && !strcasecmp(tifiles_flash_app_file_ext(), tifiles_get_extension(filename)))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        ret = itc.send_flash(filename, MODE_APPS);
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

    // FLASH OS file ?
    if(tifiles_is_a_flash_file(filename) && !strcasecmp(tifiles_flash_os_file_ext(), tifiles_get_extension(filename)))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        ret = itc.send_flash(filename, MODE_AMS);
        tihw.lc_speedy = 0;
        map_to_linkport();
    }
  
    // Backup file ?
    else if(tifiles_is_a_backup_file(filename))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        ret = itc.send_backup(filename, MODE_NORMAL);
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

    // Group file ?
    else if(tifiles_is_a_group_file(filename))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        ret = itc.send_var(filename, MODE_NORMAL, NULL);
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

    // Single file
    else if(tifiles_is_a_single_file(filename))
    {
        map_to_directfile();
        tihw.lc_speedy = 1;
        ret = itc.send_var(filename, MODE_NORMAL, NULL);
        tihw.lc_speedy = 0;
        map_to_linkport();
    }

	// Transfer aborted ? Set hw link error
	if(ret != 0)
	{
		print_lc_error(ret);
		io_bit_set(0x0d,7);
	}

  return 0;
}

/*
//vti
int SendByte(int c)
{
	int count=1000000;

    recvflag=1;
    recvbyte=c;
    hw_m68k_irq(4);
    
    while ((recvflag)&&(count--))
        ti68k_debug_do_instructions(1);		//Execute();

    if (recvflag)
        return 0;

    return 1;
}

int GetByte(int *c)
{
    int count=1000000;

    while ((!transflag)&&(count--))
        ti68k_debug_do_instructions(1);		//Execute();

    if (transflag)
    {
        *c=transbyte&0xff;
        transflag=0;
    }
    else
    {
        //getError=1;
		printf("error !\n");
        return 0;
    }

    return 1;
}
//vti
*/