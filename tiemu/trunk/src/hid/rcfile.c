/* Hey EMACS -*- linux-c -*- */
/* $Id: cabl_int.h 651 2004-04-25 15:22:07Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Originally written by Jonas Minsberg
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (C) 2001-2004, Romain Lievin
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tilibs.h"
#include "intl.h"
#include "paths.h"
#include "struct.h"
#include "version.h"
#include "paths.h"
#include "files.h"
#include "ticalc.h"
#include "interface.h"
#include "screenshot.h"

static char *rc_file; 

#define MAXCHARS 256

/*
  Return the path where the config file is stored
*/
int rcfile_get_path(char **path)
{
	*path = g_strconcat(inst_paths.home_dir, 
			    DIR_SEPARATOR_S, CONF_DIR, 
			    DIR_SEPARATOR, RC_FILE, NULL); 
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
			(options.link_cable)->calc_type = CALC_V200;

		else if (!strcmp(p, "ti92+"))
			(options.link_cable)->calc_type = CALC_TI92P;

		else if (!strcmp(p, "ti92"))
			(options.link_cable)->calc_type = CALC_TI92;

		else if (!strcmp(p, "ti89"))
			(options.link_cable)->calc_type = CALC_TI89;

		else if (!strcmp(p, "ti86"))
			(options.link_cable)->calc_type = CALC_TI86;

		else if (!strcmp(p, "ti85"))
			(options.link_cable)->calc_type = CALC_TI85;

		else if (!strcmp(p, "ti83+"))
			(options.link_cable)->calc_type = CALC_TI83P;

		else if (!strcmp(p, "ti83"))
			(options.link_cable)->calc_type = CALC_TI83;

		else if (!strcmp(p, "ti82"))
			(options.link_cable)->calc_type = CALC_TI82;

		else if (!strcmp(p, "ti73"))
			(options.link_cable)->calc_type = CALC_TI73;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "linktype="))) {
		if (!strcmp(p, "parallel"))
			(options.link_cable)->link_type = LINK_PAR;

		else if (!strcmp(p, "serial"))
			(options.link_cable)->link_type = LINK_SER;

		else if (!strcmp(p, "TIGraphLink"))
			(options.link_cable)->link_type = LINK_TGL;

		else if (!strcmp(p, "fastAVRlink"))
			(options.link_cable)->link_type = LINK_AVR;

		else if (!strcmp(p, "VTi"))
			(options.link_cable)->link_type = LINK_VTI;

		else if (!strcmp(p, "TiEmulator"))
			(options.link_cable)->link_type = LINK_TIE;

		else if (!strcmp(p, "virtual"))
			(options.link_cable)->link_type = LINK_VTL;

		else if (!strcmp(p, "UsbGraphLink"))
			(options.link_cable)->link_type = LINK_UGL;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "adr_port="))) {
		sscanf(p, "0x%03X", &((options.link_cable)->io_addr));

		//check_access();
		continue;
	}
	
	if ((p = find_str(buffer, "device="))) {
		strcpy((options.link_cable)->device, p);
	}
	
	if ((p = find_str(buffer, "timeout="))) {
		sscanf(p, "%i", &((options.link_cable)->timeout));
		continue;
	}
	
	if ((p = find_str(buffer, "baudrate="))) {
		sscanf(p, "%i", (int *) &((options.link_cable)->baud_rate));
		continue;
	}
	
	if ((p = find_str(buffer, "delay="))) {
		sscanf(p, "%i", &((options.link_cable)->delay));
		continue;
	}
	
	if ((p = find_str(buffer, "rts_cts="))) {
		if (!strcmp(p, "on"))
			(options.link_cable)->hfc = HFC_ON;

		else if (!strcmp(p, "off"))
			(options.link_cable)->hfc = HFC_OFF;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "port="))) {
		if (!strcmp(p, "user"))
			(options.link_cable)->port = USER_PORT;

		else if (!strcmp(p, "parallel port #1"))
			(options.link_cable)->port = PARALLEL_PORT_1;

		else if (!strcmp(p, "parallel port #2"))
			(options.link_cable)->port = PARALLEL_PORT_2;

		else if (!strcmp(p, "parallel port #3"))
			(options.link_cable)->port = PARALLEL_PORT_3;

		else if (!strcmp(p, "serial port #1"))
			(options.link_cable)->port = SERIAL_PORT_1;

		else if (!strcmp(p, "serial port #2"))
			(options.link_cable)->port = SERIAL_PORT_2;

		else if (!strcmp(p, "serial port #3"))
			(options.link_cable)->port = SERIAL_PORT_3;

		else if (!strcmp(p, "serial port #4"))
			(options.link_cable)->port = SERIAL_PORT_4;

		else if (!strcmp(p, "virtual port #1"))
			(options.link_cable)->port = VIRTUAL_PORT_1;

		else if (!strcmp(p, "virtual port #2"))
			(options.link_cable)->port = VIRTUAL_PORT_2;

		else if (!strcmp(p, "USB port #1"))
			(options.link_cable)->port = USB_PORT_1;

		else if (!strcmp(p, "USB port #2"))
			(options.link_cable)->port = USB_PORT_2;

		else if (!strcmp(p, "USB port #3"))
			(options.link_cable)->port = USB_PORT_3;

		else if (!strcmp(p, "USB port #4"))
			(options.link_cable)->port = USB_PORT_4;

		else
			stop(l);
		continue;
	}
	
	if ((p = find_str(buffer, "method="))) {
		if (!strcmp(p, "automatic"))
			(options.link_cable)->method = IOM_AUTO;

		else if (!strcmp(p, "asm"))
			(options.link_cable)->method = IOM_ASM;

		else if (!strcmp(p, "api"))
			(options.link_cable)->method = IOM_API;

		else if (!strcmp(p, "driver"))
			(options.link_cable)->method = IOM_DRV;

		else
			stop(l);
		continue;
	}

      /* GtkTiEmu specific part: emulator section */
	if( (p=find_str(buffer, "background=")) ) {
	  sscanf(p, "%i", &((options.params)->background));
	  continue;
	}

	if( (p=find_str(buffer, "rom_file=")) ) {
	  g_free((options.params)->rom_file);
	  (options.params)->rom_file = g_strdup(p);
	  continue;
	}

	if( (p=find_str(buffer, "ram_file=")) )
	{
	  g_free((options.params)->ram_file);
	  (options.params)->ram_file = g_strdup(p);
	  continue;
	}

	if( (p=find_str(buffer, "tib_file=")) )
	{
	  g_free((options.params)->tib_file);
	  (options.params)->tib_file = g_strdup(p);
	  continue;
	}

	if( (p=find_str(buffer, "gray_planes=")) )
	{
	  sscanf(p, "%i", &((options.params)->n_grayplanes));
	  continue;
	}

	if( (p=find_str(buffer, "tick_rate=")) )
	{
	  sscanf(p, "%u", &((options.params)->tick_rate));
	  continue;
	}

	if( (p=find_str(buffer, "cycle_rate=")) )
	{
	  sscanf(p, "%u", &((options.params)->cycle_rate));
	  continue;
	}

	if( (p=find_str(buffer, "itick=")) )
	{
	  sscanf(p, "%i", &((options.params)->i_tick));
	  continue;
	}
	
	if( (p=find_str(buffer, "code_lines=")) )
	{
	  sscanf(p, "%i", &(options.code_lines));
	  continue;
	}

	if( (p=find_str(buffer, "mem_lines=")) )
	{
	  sscanf(p, "%i", &(options.mem_lines));
	  continue;
	}

	if( (p=find_str(buffer, "stack_lines=")) )
	{
	  sscanf(p, "%i", &(options.stack_lines));
	  continue;
	}

	if( (p=find_str(buffer, "img_format=")) )
	{
	  if(!strcmp(p, "pcx")) options.img_format=IMG_PCX;
	  else if(!strcmp(p, "xpm")) options.img_format=IMG_XPM;
	  else if(!strcmp(p, "jpg")) options.img_format=IMG_JPG;
	  else stop(l);
	  continue;
	}
      
	if( (p=find_str(buffer, "img_type=")) )
	{
	  if(!strcmp(p, "bw")) options.img_type = IMG_BW;
	  else if(!strcmp(p, "color")) options.img_type = IMG_COL;
	  else stop(l);
	  continue;
	}
      
	if( (p=find_str(buffer, "img_size=")) )
	{
	  if(!strcmp(p, "lcd")) options.img_size = IMG_LCD;
	  else if(!strcmp(p, "skin")) options.img_size = IMG_SKIN;
	  else stop(l);
	  continue;
	}
      
	if( (p=find_str(buffer, "screen_file=")) )
	{
	  g_free(options.img_file);
	  options.img_file = g_strdup(p);
	  continue;
	}
      
	if( (p=find_str(buffer, "screen_counter=")) )
	{
	  sscanf(p, "%i", &(options.img_counter));
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
	mkdir(CONF_DIR, 0755);
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
	switch ((options.link_cable)->calc_type) {
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

	switch ((options.link_cable)->link_type) {
	case LINK_PAR:
		fprintf(txt, "parallel\n");
		break;
	case LINK_SER:
		fprintf(txt, "serial\n");
		break;
	case LINK_TGL:
		fprintf(txt, "TIGraphLink\n");
		break;
	case LINK_AVR:
		fprintf(txt, "fastAVRlink\n");
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
		fprintf(txt, "invalid\n");
		break;
	}

	fprintf(txt, "\n");
	fprintf(txt, "# Port to use (serial, parallel, ...\n");
	fprintf(txt, "port=");

	switch ((options.link_cable)->port) {
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
		fprintf(txt, "invalid\n");
		break;
	}

	fprintf(txt, "\n");
	fprintf(txt, "# Method to use for I/O accesses\n");
	fprintf(txt, "method=");
	if ((options.link_cable)->method & IOM_AUTO)
		fprintf(txt, "automatic\n");

	else if ((options.link_cable)->method & IOM_ASM)
		fprintf(txt, "asm\n");

	else if ((options.link_cable)->method & IOM_API)
		fprintf(txt, "api\n");

	else if ((options.link_cable)->method & IOM_DRV)
		fprintf(txt, "driver\n");

	else
		fprintf(txt, "automatic\n");
	fprintf(txt, "\n");
	fprintf(txt,
		"# Parallel/serial/virtual port address (0=automatic)\n");
	fprintf(txt, "adr_port=0x%03X\n", (options.link_cable)->io_addr);
	fprintf(txt, "\n");
	fprintf(txt,
		"# Serial device or character device (empty=automatic)\n");
	fprintf(txt, "serial_device=%s\n", (options.link_cable)->device);
	fprintf(txt, "\n");
	fprintf(txt, "# Baud rate for the fastAVRlink\n");
	fprintf(txt, "baudrate=%i\n", (options.link_cable)->baud_rate);
	fprintf(txt, "\n");
	fprintf(txt, "# Timeout value in 0.1 seconds\n");
	fprintf(txt, "timeout=%i\n", (options.link_cable)->timeout);
	fprintf(txt, "\n");
	fprintf(txt, "# Delay value\n");
	fprintf(txt, "delay=%i\n", (options.link_cable)->delay);
	fprintf(txt, "\n");
	fprintf(txt, "# Hardware flow control for fastAVRlink.\n");
	fprintf(txt, "rts_cts=%s\n",
		((options.link_cable)->hfc == HFC_ON) ? "on" : "off");
	fprintf(txt, "\n");
	fprintf(txt, "#\n");

  /* Specific part to GtkTiEmu */
	fprintf(txt, "#\n");
	fprintf(txt, "# EMULATOR SECTION\n");
	fprintf(txt, "#\n");
	fprintf(txt, "\n");
	fprintf(txt, "# Background (0 for LCD only, 1 with skin)\n");
	fprintf(txt, "background=%i\n", (options.params)->background);
	fprintf(txt, "\n");
	fprintf(txt, "# SKIN file\n");
	fprintf(txt, "skin_file=%s\n", options.skin_file);
	fprintf(txt, "\n");
	fprintf(txt, "# ROM file\n");
	fprintf(txt, "rom_file=%s\n", (options.params)->rom_file);
	fprintf(txt, "\n");
	fprintf(txt, "# RAM file\n");
	fprintf(txt, "ram_file=%s\n", (options.params)->ram_file);
  fprintf(txt, "\n");
  fprintf(txt, "# TIB file\n");
  fprintf(txt, "tib_file=%s\n", (options.params)->tib_file);
  fprintf(txt, "\n");
  fprintf(txt, "# Number of gray planes (0, 2, 5, 11)\n");
  fprintf(txt, "gray_planes=%i\n", (options.params)->n_grayplanes);
  fprintf(txt, "\n");
  fprintf(txt, "# Timer Tick rate\n");
  fprintf(txt, "tick_rate=%u\n", (options.params)->tick_rate);
  fprintf(txt, "\n");
  fprintf(txt, "# Cycle rate\n");
  fprintf(txt, "cycle_rate=%u\n", (options.params)->cycle_rate);
  fprintf(txt, "\n");
  fprintf(txt, "# Instructions per timertick\n");
  fprintf(txt, "itick=%i\n", (options.params)->i_tick);
  fprintf(txt, "\n");
  fprintf(txt, "# Synchronization of the refresh on IRQ1/timer (0 or 1)\n");
  fprintf(txt, "sync_one=%i\n", (options.params)->sync_one);
  fprintf(txt, "\n");
  fprintf(txt, "# Number of lines in the disassembly window\n");
  fprintf(txt, "code_lines=%i\n", options.code_lines);
  fprintf(txt, "\n");
  fprintf(txt, "# Number of lines in the stack window\n");
  fprintf(txt, "stack_lines=%i\n", options.stack_lines);
  fprintf(txt, "\n");
  fprintf(txt, "# Number of lines in the memory window\n");
  fprintf(txt, "mem_lines=%i\n", options.mem_lines);
  fprintf(txt, "\n");
  fprintf(txt, "\n");
  fprintf(txt, "#\n");
  fprintf(txt, "# SCREENSHOT SECTION\n");
  fprintf(txt, "#\n");
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot: image format (xpm, pcx, jpg, bmp)\n");
  fprintf(txt, "img_format=");
  switch(options.img_format)
    {
    case IMG_XPM: fprintf(txt, "xpm\n"); break;
    case IMG_PCX: fprintf(txt, "pcx\n"); break;
    case IMG_JPG: fprintf(txt, "jpg\n"); break;
	case IMG_BMP: fprintf(txt, "bmp\n"); break;
    }
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot: image type (bw, color)\n");
  fprintf(txt, "img_type=%s\n", (options.img_type == IMG_BW) ? "bw" : "color");
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot: image size (lcd, skin)\n");
  fprintf(txt, "img_size=%s\n", (options.img_size == IMG_LCD) ? "lcd" : "skin");
  fprintf(txt, "\n");

  fprintf(txt, "# Screenshot base file\n");
  fprintf(txt, "screen_file=%s\n", options.img_file);
  fprintf(txt, "\n");
  fprintf(txt, "# Screenshot counter\n");
  fprintf(txt, "screen_counter=%i\n", options.img_counter);
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
  fprintf(txt, "RC_END\n");
  fflush(txt);

  //setup_link_and_calc(options.server);

  fclose(txt);
}

/*
  Initialize the options structure with some default values
*/
int load_default_config()
{
	// share structures fields
	options.params = &params;
	options.link_cable = &(params.link_cable);

	ti68k_loadDefaultConfig();

	(options.params)->rom_file = g_strconcat(inst_paths.rom_dir, "", NULL);
	(options.params)->ram_file = g_strdup("");
	(options.params)->tib_file = g_strdup("");

	// other fields
	options.skin_file = g_strconcat(inst_paths.skin_dir, "ti92.skn", NULL);

	options.code_lines  = 20;
	options.stack_lines = 20;
	options.mem_lines   = 20;

	options.img_file = g_strdup("screenshot");
	options.img_counter = 0;
	options.img_type = IMG_COL;	//IMG_BW;
	options.img_format = IMG_PNG;
	options.img_size = IMG_SKIN;	//IMG_LCD;

	options.console=0;

	return 0;
}

