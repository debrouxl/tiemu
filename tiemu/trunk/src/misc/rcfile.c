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
  This unit manage configurable options and config file
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "intl.h"
#include "tilibs.h"
#include "paths.h"
#include "struct.h"
#include "version.h"
#include "paths.h"
#include "files.h"
#include "ti68k_int.h"
#include "screenshot.h"
#include "printl.h"

static char *rc_file; 

#define MAXCHARS 256

/*
  Return the path where the config file is stored
*/
int rcfile_get_path(char **path)
{
	*path = g_strconcat(inst_paths.home_dir, 
			    DIR_SEPARATOR_S, CONF_DIR, 
			    DIR_SEPARATOR, INI_FILE, NULL); 
	return 0;
}

/* 
   Print an error msg 
*/
static void stop (int line)
{
	printl(2, _("Configuration file error at line %i.\n"), line);
}

/*
  Find a token in a string and returns pointer after token
*/
static char *find_str(char *s, const char *t)
{
	char *p = strstr(s, t);

	if(p==NULL) 
		return NULL;
	else 
		return p+strlen(t);
}

/* Chech whether a RC file exists */
int rcfile_exist(void)
{
	rcfile_get_path(&rc_file);
  
	return !access(rc_file, F_OK);
}

/* Delete the RC file */
int rcfile_delete(void)
{
	rcfile_get_path(&rc_file);

	return unlink(rc_file);
}

/* Return TiLP version number */
int rcfile_get_version(char *version)
{
	FILE *txt;
	char buffer[256];
	int i = 0;
	char *p;

	strcpy(version, "");
	if (rcfile_exist() == 0) {
		return -1;
	}

	rcfile_get_path(&rc_file);

	txt = fopen(rc_file, "rt");
	if (txt == NULL)
		return -1;

	for (i = 0; i < 5; i++)
		fgets(buffer, 256, txt);

	p = strchr(buffer, '=');
	if (p == NULL)
		return -1;

	strcpy(version, ++p);

	p = strchr(version, '\r');
	if (p)
		*p = '\0';

	p = strchr(version, '\n');
	if (p)
		*p = '\0';

	return 0;
}

/* 
   Read the RC file and configure the corresponding variables 
*/
void rcfile_read(void)
{
	FILE *txt;
	char buffer[256];
	char *p;
	int l=0;

	rcfile_get_path(&rc_file);

	txt=fopen(rc_file, "rt");
	if(txt == NULL) {
		DISPLAY(_("Configuration file not found, use default values. You can create one by the 'File|Save config' command menu.\n"));
		//gif->msg_box(_("Information"), _("Configuration file not found, use default values. You can create one by the 'File|Save config' command menu.\n"));
		return;
	}

	while(!feof(txt)) {
		fgets(buffer, 256, txt);
		l++;
		buffer[strlen(buffer)-1]='\0';

	if(!strcmp(buffer, "RC_END")) {
	  fclose(txt);
	  return;
	}

	if(buffer[0]=='#' || !strlen(buffer)) continue;
      
	/* Common part with TiLP: hardware section */
	if ((p = find_str(buffer, "calctype="))) {
		if (!strcmp(p, "v200"))
			link_cable.calc_type = CALC_V200;

		else if (!strcmp(p, "ti92+"))
			link_cable.calc_type = CALC_TI92P;

		else if (!strcmp(p, "ti92"))
			link_cable.calc_type = CALC_TI92;

		else if (!strcmp(p, "ti89"))
			link_cable.calc_type = CALC_TI89;

		else if (!strcmp(p, "ti86"))
			link_cable.calc_type = CALC_TI86;

		else if (!strcmp(p, "ti85"))
			link_cable.calc_type = CALC_TI85;

		else if (!strcmp(p, "ti83+"))
			link_cable.calc_type = CALC_TI83P;

		else if (!strcmp(p, "ti83"))
			link_cable.calc_type = CALC_TI83;

		else if (!strcmp(p, "ti82"))
			link_cable.calc_type = CALC_TI82;

		else if (!strcmp(p, "ti73"))
			link_cable.calc_type = CALC_TI73;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "linktype="))) {
		if (!strcmp(p, "parallel"))
			link_cable.link_type = LINK_PAR;

		else if (!strcmp(p, "serial"))
			link_cable.link_type = LINK_SER;

		else if (!strcmp(p, "TIGraphLink"))
			link_cable.link_type = LINK_TGL;

		else if (!strcmp(p, "VTi"))
			link_cable.link_type = LINK_VTI;

		else if (!strcmp(p, "TiEmulator"))
			link_cable.link_type = LINK_TIE;

		else if (!strcmp(p, "virtual"))
			link_cable.link_type = LINK_VTL;

		else if (!strcmp(p, "UsbGraphLink"))
			link_cable.link_type = LINK_UGL;

		else if (!strcmp(p, "none"))
			link_cable.link_type = LINK_NUL;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "adr_port="))) {
		sscanf(p, "0x%03X", &(link_cable.io_addr));

		//check_access();
		continue;
	}
	
	if ((p = find_str(buffer, "device="))) {
		strcpy(link_cable.device, p);
	}
	
	if ((p = find_str(buffer, "timeout="))) {
		sscanf(p, "%i", &(link_cable.timeout));
		continue;
	}
	
	if ((p = find_str(buffer, "delay="))) {
		sscanf(p, "%i", &(link_cable.delay));
		continue;
	}
	
	if ((p = find_str(buffer, "port="))) {
		if (!strcmp(p, "user"))
			link_cable.port = USER_PORT;

		else if (!strcmp(p, "parallel port #1"))
			link_cable.port = PARALLEL_PORT_1;

		else if (!strcmp(p, "parallel port #2"))
			link_cable.port = PARALLEL_PORT_2;

		else if (!strcmp(p, "parallel port #3"))
			link_cable.port = PARALLEL_PORT_3;

		else if (!strcmp(p, "serial port #1"))
			link_cable.port = SERIAL_PORT_1;

		else if (!strcmp(p, "serial port #2"))
			link_cable.port = SERIAL_PORT_2;

		else if (!strcmp(p, "serial port #3"))
			link_cable.port = SERIAL_PORT_3;

		else if (!strcmp(p, "serial port #4"))
			link_cable.port = SERIAL_PORT_4;

		else if (!strcmp(p, "virtual port #1"))
			link_cable.port = VIRTUAL_PORT_1;

		else if (!strcmp(p, "virtual port #2"))
			link_cable.port = VIRTUAL_PORT_2;

		else if (!strcmp(p, "USB port #1"))
			link_cable.port = USB_PORT_1;

		else if (!strcmp(p, "USB port #2"))
			link_cable.port = USB_PORT_2;

		else if (!strcmp(p, "USB port #3"))
			link_cable.port = USB_PORT_3;

		else if (!strcmp(p, "USB port #4"))
			link_cable.port = USB_PORT_4;

		else if (!strcmp(p, "null"))
			link_cable.port = NULL_PORT;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "method="))) {
		if (!strcmp(p, "automatic"))
			link_cable.method = IOM_AUTO;

		else if (!strcmp(p, "asm"))
			link_cable.method = IOM_ASM;

		else if (!strcmp(p, "api"))
			link_cable.method = IOM_API;

		else if (!strcmp(p, "driver"))
			link_cable.method = IOM_DRV;

		else if (!strcmp(p, "null"))
			link_cable.method = IOM_NULL;

		else
			stop(l);
		continue;
	}

      /* GtkTiEmu specific part: emulator section */
	if( (p=find_str(buffer, "background=")) ) {
	  sscanf(p, "%i", &(params.background));
	  continue;
	}

	if( (p=find_str(buffer, "rom_file=")) ) {
	  g_free(params.rom_file);
	  params.rom_file = g_strdup(p);
	  continue;
	}

	if( (p=find_str(buffer, "tib_file=")) )
	{
	  g_free(params.tib_file);
	  params.tib_file = g_strdup(p);
	  continue;
	}

    if( (p=find_str(buffer, "sav_file=")) )
	{
	  g_free(params.sav_file);
	  params.sav_file = g_strdup(p);
	  continue;
	}

	if( (p=find_str(buffer, "gray_planes=")) )
	{
	  sscanf(p, "%i", &(params.grayplanes));
	  continue;
	}

	if( (p=find_str(buffer, "cpu_rate=")) )
	{
	  sscanf(p, "%u", &(params.cpu_rate));
	  continue;
	}

	if( (p=find_str(buffer, "hw_rate=")) )
	{
	  sscanf(p, "%u", &(params.hw_rate));
	  continue;
	}

	if( (p=find_str(buffer, "lcd_rate=")) )
	{
	  sscanf(p, "%i", &(params.lcd_rate));
	  continue;
	}

	if( (p=find_str(buffer, "img_format=")) )
	{
	  if(!strcmp(p, "jpg")) options2.format=IMG_JPG;
	  else if(!strcmp(p, "png")) options2.format=IMG_PNG;
	  else if(!strcmp(p, "ico")) options2.format=IMG_ICO;
	  else stop(l);
	  continue;
	}
      
	if( (p=find_str(buffer, "img_type=")) )
	{
	  if(!strcmp(p, "bw")) options2.type = IMG_BW;
	  else if(!strcmp(p, "color")) options2.type = IMG_COL;
	  else stop(l);
	  continue;
	}
      
	if( (p=find_str(buffer, "img_size=")) )
	{
	  if(!strcmp(p, "lcd")) options2.size = IMG_LCD;
	  else if(!strcmp(p, "skin")) options2.size = IMG_SKIN;
	  else stop(l);
	  continue;
	}
      
	if( (p=find_str(buffer, "screen_file=")) )
	{
	  g_free(options2.file);
	  options2.file = g_strdup(p);
	  continue;
	}
      
	if( (p=find_str(buffer, "screen_counter=")) )
	{
	  sscanf(p, "%i", &(options2.counter));
	  continue;
	}
	
	if( (p=find_str(buffer, "console=")) )
	{
	  if(!strcmp(p, "no")) options.console = 0;
	  else if(!strcmp(p, "yes")) options.console = 1;
	  else if(!strcmp(p, "boot")) options.console = 2;
	  else stop(l);
	  continue;
	}
	
	if( (p=find_str(buffer, "skin_file=")) )
	{
	  g_free(options.skin_file);
	  options.skin_file = g_strdup(p);
	  continue;
	}

    if( (p=find_str(buffer, "keys_file=")) )
	{
	  g_free(options.keys_file);
	  options.keys_file = g_strdup(p);
	  continue;
	}

	if( (p=find_str(buffer, "bkpts_wnd=")) )
	{
	  sscanf(p, "(%i;%i;%i;%i)", &(options3.bkpts.x), &(options3.bkpts.y), 
				&(options3.bkpts.w), &(options3.bkpts.h));
	  continue;
	}
	if( (p=find_str(buffer, "code_wnd=")) )
	{
	  sscanf(p, "(%i;%i;%i;%i)", &(options3.code.x), &(options3.code.y), 
				&(options3.code.w), &(options3.code.h));
	  continue;
	}
	if( (p=find_str(buffer, "mem_wnd=")) )
	{
	  sscanf(p, "(%i;%i;%i;%i)", &(options3.mem.x), &(options3.mem.y), 
				&(options3.mem.w), &(options3.mem.h));
	  continue;
	}
	if( (p=find_str(buffer, "regs_wnd=")) )
	{
	  sscanf(p, "(%i;%i;%i;%i)", &(options3.regs.x), &(options3.regs.y), 
				&(options3.regs.w), &(options3.regs.h));
	  continue;
	}
	if( (p=find_str(buffer, "pclog_wnd=")) )
	{
	  sscanf(p, "(%i;%i;%i;%i)", &(options3.pclog.x), &(options3.pclog.y), 
				&(options3.pclog.w), &(options3.pclog.h));
	  continue;
	}

	
    }
  fclose(txt);

  return;
}

/* 
   Write the contents of some variables to the RC file 
   in a plain text format.
*/
void rcfile_write(void)
{
	FILE *txt;

	rcfile_get_path(&rc_file);
#ifndef __WIN32__
	mkdir(CONF_DIR, 0755);
#else
	_mkdir(CONF_DIR);
#endif
	txt=fopen(rc_file, "wt");

	if(txt==NULL) {
		//gif->msg_box(_("Error"), _("Unable to write the config file (~/.tilp or tilp.ini).\n"));
		return;
	}

	fprintf(txt, "# Config file for TiEmu II\n");
	fprintf(txt, "  (C) Romain Lievin & Thomas Corvazier  2000-2001\n");
	fprintf(txt, "  (C) Romain Lievin 2001-2003\n");
	fprintf(txt, "  (C) Julien Blache 2003\n");
	fprintf(txt, "  (C) Romain Liévin 2004\n");
	fprintf(txt, "# Warning: any comments that you add to this file WILL be overwritten\n");
	fprintf(txt, "\n");

	/* Common part with TiLP */
	fprintf(txt, "version=%s\n", TIEMU_VERSION);
	fprintf(txt, "\n");
	fprintf(txt, "#\n");
	fprintf(txt, "# HARDWARE SECTION\n");
	fprintf(txt, "#\n");
	fprintf(txt, "\n");
	fprintf(txt, "# Calculator type\n");
	fprintf(txt, "calctype=");
	switch (link_cable.calc_type) {
	case CALC_V200:
		fprintf(txt, "v200\n");
		break;
	case CALC_TI92P:
		fprintf(txt, "ti92+\n");
		break;
	case CALC_TI92:
		fprintf(txt, "ti92\n");
		break;
	case CALC_TI89:
		fprintf(txt, "ti89\n");
		break;
	case CALC_TI86:
		fprintf(txt, "ti86\n");
		break;
	case CALC_TI85:
		fprintf(txt, "ti85\n");
		break;
	case CALC_TI83P:
		fprintf(txt, "ti83+\n");
		break;
	case CALC_TI83:
		fprintf(txt, "ti83\n");
		break;
	case CALC_TI82:
		fprintf(txt, "ti82\n");
		break;
	case CALC_TI73:
		fprintf(txt, "ti73\n");
		break;
	}

	fprintf(txt, "\n");
	fprintf(txt, "# Link cable type\n");
	fprintf(txt, "linktype=");

	switch (link_cable.link_type) {
	case LINK_PAR:
		fprintf(txt, "parallel\n");
		break;
	case LINK_SER:
		fprintf(txt, "serial\n");
		break;
	case LINK_TGL:
		fprintf(txt, "TIGraphLink\n");
		break;
	case LINK_VTL:
		fprintf(txt, "virtual\n");
		break;
	case LINK_TIE:
		fprintf(txt, "TiEmulator\n");
		break;
	case LINK_VTI:
		fprintf(txt, "VTi\n");
		break;
	case LINK_UGL:
		fprintf(txt, "UsbGraphLink\n");
		break;
	default:
		fprintf(txt, "none\n");
		break;
	}

	fprintf(txt, "\n");
	fprintf(txt, "# Port to use (serial, parallel, ...\n");
	fprintf(txt, "port=");

	switch (link_cable.port) {
	case USER_PORT:
		fprintf(txt, "user\n");
		break;
	case PARALLEL_PORT_1:
		fprintf(txt, "parallel port #1\n");
		break;
	case PARALLEL_PORT_2:
		fprintf(txt, "parallel port #2\n");
		break;
	case PARALLEL_PORT_3:
		fprintf(txt, "parallel port #3\n");
		break;
	case SERIAL_PORT_1:
		fprintf(txt, "serial port #1\n");
		break;
	case SERIAL_PORT_2:
		fprintf(txt, "serial port #2\n");
		break;
	case SERIAL_PORT_3:
		fprintf(txt, "serial port #3\n");
		break;
	case SERIAL_PORT_4:
		fprintf(txt, "serial port #4\n");
		break;
	case VIRTUAL_PORT_1:
		fprintf(txt, "virtual port #1\n");
		break;
	case VIRTUAL_PORT_2:
		fprintf(txt, "virtual port #2\n");
		break;
	case USB_PORT_1:
		fprintf(txt, "USB port #1\n");
		break;
	case USB_PORT_2:
		fprintf(txt, "USB port #2\n");
		break;
	case USB_PORT_3:
		fprintf(txt, "USB port #3\n");
		break;
	case USB_PORT_4:
		fprintf(txt, "USB port #4\n");
		break;
	default:
		fprintf(txt, "null\n");
		break;
	}

	fprintf(txt, "\n");
	fprintf(txt, "# Method to use for I/O accesses\n");
	fprintf(txt, "method=");
	if (link_cable.method & IOM_AUTO)
		fprintf(txt, "automatic\n");

	else if (link_cable.method & IOM_ASM)
		fprintf(txt, "asm\n");

	else if (link_cable.method & IOM_API)
		fprintf(txt, "api\n");

	else if (link_cable.method & IOM_DRV)
		fprintf(txt, "driver\n");
	else if (link_cable.method & IOM_NULL)
		fprintf(txt, "null\n");
	else
		fprintf(txt, "automatic\n");
	fprintf(txt, "\n");
	fprintf(txt,
		"# Parallel/serial/virtual port address (0=automatic)\n");
	fprintf(txt, "adr_port=0x%03X\n", link_cable.io_addr);
	fprintf(txt, "\n");
	fprintf(txt,
		"# Serial device or character device (empty=automatic)\n");
	fprintf(txt, "serial_device=%s\n", link_cable.device);
	fprintf(txt, "\n");
	fprintf(txt, "# Timeout value in 0.1 seconds\n");
	fprintf(txt, "timeout=%i\n", link_cable.timeout);
	fprintf(txt, "\n");
	fprintf(txt, "# Delay value\n");
	fprintf(txt, "delay=%i\n", link_cable.delay);
	fprintf(txt, "\n");
	fprintf(txt, "#\n");

  /* Specific part to TiEmu */
	fprintf(txt, "#\n");
	fprintf(txt, "# EMULATOR SECTION\n");
	fprintf(txt, "#\n");
	fprintf(txt, "\n");
	fprintf(txt, "# Background (0 for LCD only, 1 with skin)\n");
	fprintf(txt, "background=%i\n", params.background);
	fprintf(txt, "\n");
	fprintf(txt, "# SKIN file\n");
	fprintf(txt, "skin_file=%s\n", options.skin_file);
	fprintf(txt, "\n");
    fprintf(txt, "# KEYS file\n");
	fprintf(txt, "keys_file=%s\n", options.keys_file);
	fprintf(txt, "\n");
	fprintf(txt, "# ROM file\n");
	fprintf(txt, "rom_file=%s\n", params.rom_file);
	fprintf(txt, "\n");
    fprintf(txt, "# TIB file\n");
    fprintf(txt, "tib_file=%s\n", params.tib_file);
    fprintf(txt, "\n");
    fprintf(txt, "# SAV file\n");
	fprintf(txt, "sav_file=%s\n", params.sav_file);
    fprintf(txt, "\n");
  fprintf(txt, "# Number of gray planes (2, 4, 7, 11)\n");
  fprintf(txt, "gray_planes=%i\n", params.grayplanes);
  fprintf(txt, "\n");
  fprintf(txt, "# CPU instructions rate (50000 per 30ms) = OSC1\n");
  fprintf(txt, "cpu_rate=%i\n", params.cpu_rate);
  fprintf(txt, "\n");
  fprintf(txt, "# Hardware update rate (625 (hw1) or 1172 (hw2) = OSC2/OSC3\n");
  fprintf(txt, "hw_rate=%i\n", params.hw_rate);
  fprintf(txt, "\n");
  fprintf(txt, "# LCD update rate (100 fps max.) = OSC3\n");
  fprintf(txt, "lcd_rate=%i\n", params.lcd_rate);
  fprintf(txt, "\n");
  fprintf(txt, "#\n");
  fprintf(txt, "# SCREENSHOT SECTION\n");
  fprintf(txt, "#\n");
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot: image format (xpm, pcx, jpg, bmp)\n");
  fprintf(txt, "img_format=");
  switch(options2.format)
    {
    case IMG_JPG: fprintf(txt, "jpg\n"); break;
    case IMG_PNG: fprintf(txt, "png\n"); break;
    case IMG_ICO: fprintf(txt, "ico\n"); break;
    }
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot: image type (bw, color)\n");
  fprintf(txt, "img_type=%s\n", (options2.type == IMG_BW) ? "bw" : "color");
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot: image size (lcd, skin)\n");
  fprintf(txt, "img_size=%s\n", (options2.size == IMG_LCD) ? "lcd" : "skin");
  fprintf(txt, "\n");

  fprintf(txt, "# Screenshot base file\n");
  fprintf(txt, "screen_file=%s\n", options2.file);
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot counter\n");
  fprintf(txt, "screen_counter=%i\n", options2.counter);
  fprintf(txt, "\n");
  fprintf(txt, "\n");
  fprintf(txt, "#\n");
  fprintf(txt, "# MISC SECTION\n");
  fprintf(txt, "#\n");
  fprintf(txt, "\n");
  fprintf(txt, "# Console mode (no, yes, boot)\n");
  fprintf(txt, "console=");
  switch(options.console)
  {
  case 0: fprintf(txt, "no\n"); break;
  case 1: fprintf(txt, "yes\n"); break;
  case 2: fprintf(txt, "boot\n"); break;
  default: fprintf(txt, "no\n"); break;
  }
  fprintf(txt, "\n");
	fprintf(txt, "# Geometry hints of debugger windows (x,y,w,h)\n");
	fprintf(txt, "bkpts_wnd=(%i;%i;%i;%i)", options3.bkpts.x, options3.bkpts.y, 
											options3.bkpts.w, options3.bkpts.h);
	fprintf(txt, "\n");
	fprintf(txt, "code_wnd=(%i;%i;%i;%i)", options3.code.x, options3.code.y, 
											options3.code.w, options3.code.h);
	fprintf(txt, "\n");
	fprintf(txt, "mem_wnd=(%i;%i;%i;%i)", options3.mem.x, options3.mem.y, 
											options3.mem.w, options3.mem.h);
	fprintf(txt, "\n");
	fprintf(txt, "regs_wnd=(%i;%i;%i;%i)", options3.regs.x, options3.regs.y, 
											options3.regs.w, options3.regs.h);
	fprintf(txt, "\n");
	fprintf(txt, "pclog_wnd=(%i;%i;%i;%i)", options3.pclog.x, options3.pclog.y, 
											options3.pclog.w, options3.pclog.h);
	fprintf(txt, "\n");

	fprintf(txt, "\n");
  fprintf(txt, "RC_END\n");
  fflush(txt);

  //setup_link_and_calc(options.server);

  fclose(txt);
}

/*
  Initialize the options structure with some default values
*/
int rcfile_default()
{
	ti68k_config_load_default();

	params.rom_file = g_strconcat(inst_paths.img_dir, "", NULL);
	params.sav_file = g_strdup("");
	params.tib_file = g_strdup("");

	// other fields
	options.skin_file = g_strconcat(inst_paths.skin_dir, "", NULL);
    options.keys_file = g_strconcat(inst_paths.skin_dir, "", NULL);
#if defined(__WIN32__) && defined(_CONSOLE)
	options.console = !0;
#else
	options.console = 0;
#endif
    options.view = VIEW_NORMAL;

	options2.file = g_strdup("screenshot");
	options2.counter = 0;
	options2.type = IMG_COL;	//IMG_BW;
	options2.format = IMG_PNG;
	options2.size = IMG_SKIN;	//IMG_LCD;

	// Optimized for 1024x768
	options3.bkpts.x = 490;
	options3.bkpts.y = 380;
	options3.bkpts.w = 320;
	options3.bkpts.h = 120;

	options3.code.x = 490;
	options3.code.y = 30;
	options3.code.w = 320;
	options3.code.h = 240;

	options3.mem.x = 0;
	options3.mem.y = 380;
	options3.mem.w = 480;
	options3.mem.h = 200;

	options3.regs.x = 830;
	options3.regs.y = 0;
	options3.regs.w = 180;
	options3.regs.h = 500;

	options3.pclog.x = 0;
	options3.pclog.y = 0;
	options3.pclog.w = 160;
	options3.pclog.h = 240;

    options3.stack.x = 0;
    options3.stack.y = 0;
    options3.stack.w = 160;
    options3.stack.h = 240;

	return 0;
}

