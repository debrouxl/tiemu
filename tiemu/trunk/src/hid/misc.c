/*  GtkTiEmu - a TI89/92/92+ emulator
 *  Copyright (C) 2000-2001  Romain Lievin and Thomas Corvazier
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

//#include "platform.h"
#include "tilibs.h"
#include "struct.h"
#include "paths.h"
#include "version.h"

/*
  Display the program version
*/
int version(void)
{
  fprintf(stdout, "TiEmu II - Version %s\n", TIEMU_VERSION);
  fprintf(stdout, "  (C) Romain Lievin & Thomas Corvazier  2000-2001\n");
  fprintf(stdout, "  (C) Romain Lievin 2001-2003\n");
  fprintf(stdout, "  (C) Julien Blache 2003\n");
  fprintf(stdout, "THIS PROGRAM COMES WITH ABSOLUTELY NO WARRANTY\n");
  fprintf(stdout, "PLEASE READ THE DOCUMENTATION FOR DETAILS\n");

  return 0;
}

/*
  Display a short help
*/
int help(void)
{
  fprintf(stdout, "\n");

  version();

  fprintf(stdout, "usage: tiemu [-options] [filename]\n");
  fprintf(stdout, "\n");
  fprintf(stdout, "-h, --help    display this information page and exit\n");
  fprintf(stdout, "-v, --version display the version information and exit\n");
  fprintf(stdout, "-rom_file=    ROM to load at startup\n");
  fprintf(stdout, "-ram_file=    RAM image to load\n");
  fprintf(stdout, "-link=...     give the link cable type\n");
  fprintf(stdout, "-dev_port=... give the device port (TI Graph Link cable only)\n");
  fprintf(stdout, "-adr_port=... give the address of the port (parallel or serial link cable only)\n");
  fprintf(stdout, "-timeout=...  give the time out in seconds\n");
  fprintf(stdout, "-delay=...    give the delay in microseconds\n");
  fprintf(stdout, "-gray_planes= give the number of gray planes\n");
  fprintf(stdout, "-tick_rate=   give the TickRate value\n");
  fprintf(stdout, "-cycle_rate=  give the CycleRate value\n");
  fprintf(stdout, "-dark_col=    give the value of the dark color (0xrrggbb)\n");
  fprintf(stdout, "-light_col=   give the value of the dark color (0xrrggbb)\n");
  fprintf(stdout, "-sync_one=    enable(1) or disable(0) synch on IRQ1\n");
  fprintf(stdout, "-background=  enable(1) or disable(0) background\n");
  fprintf(stdout, "\n");
  fprintf(stdout, "filename      a filename which contains a ROM image\n");
  fprintf(stdout, "\n");

  exit(0);
  return 0;
}

static /*inline*/ int strexact(char *p1, char *p2)
{
  return (strstr(p1,p2) && strstr(p2,p1));
}

/*
  Scan the command line, extract arguments and init variables
*/
int scan_cmdline(int argc, char **argv)
{
  int cnt;
  char *p;
  char *q;
  char msg[80];
  
  for(cnt=1; cnt<argc; cnt++)
    {
      p=argv[cnt];
      if(*p=='-' ) 
	{
	  // an option
	  p++;
	}
      else
	{
	  // a ROM to load
	  g_free((options.params)->rom_file);
	  (options.params)->rom_file = g_strdup(p);
	}
      
      if(strstr  (msg, "rom_file="     ))
	{
	  q=msg+9;
	  g_free((options.params)->rom_file);
          (options.params)->rom_file = g_strdup(p);
	}
      if(strstr  (msg, "ram_file="     ))
	{
	  q=msg+9;
	  g_free((options.params)->ram_file);
          (options.params)->ram_file = g_strdup(p);
	}
      if(strstr  (msg, "gray_planes=")) 
	(options.params)->n_grayplanes = (int) atol(&msg[12]);
      if(strstr  (msg, "tick_rate="  )) 
	(options.params)->tick_rate = (int) atol(&msg[10]);
      if(strstr  (msg, "cycle_rate=" )) 
	(options.params)->cycle_rate = (int) atol(&msg[11]);
      if(strstr  (msg, "sync_one="   )) 
	(options.params)->sync_one = (int) atol(&msg[9]);
      if(strstr  (msg, "background="   )) 
	(options.params)->background = (int) atol(&msg[11]);
      
      if(strstr  (msg, "link="     )) 
	{
	  q=msg+5;
	  if(!strcmp(q, "par")) (options.link_cable)->link_type = LINK_PAR;
	  if(!strcmp(q, "ser")) (options.link_cable)->link_type = LINK_SER;
	  if(!strcmp(q, "tgl")) (options.link_cable)->link_type = LINK_TGL;
	  if(!strcmp(q, "avr")) (options.link_cable)->link_type = LINK_AVR;
	  if(!strcmp(q, "tie")) (options.link_cable)->link_type = LINK_TIE;
	  if(!strcmp(q, "vti")) (options.link_cable)->link_type = LINK_VTI;
	  if(!strcmp(q, "tpu")) (options.link_cable)->link_type = LINK_TPU;
	  if(!strcmp(q, "ugl")) (options.link_cable)->link_type = LINK_UGL;
	  if(!strcmp(q, "vtl")) (options.link_cable)->link_type = LINK_VTL;
	}
      if(strstr  (msg, "dev_port=" )) 
	strcpy((options.link_cable)->device, msg+9);
      if(strstr  (msg, "adr_port=" )) 
	(options.link_cable)->io_addr = (int) strtol(msg+9, &q, 16);
      if(strstr  (msg, "timeout="  )) 
	(options.link_cable)->timeout = (int) atol(&msg[8]);
      if(strstr  (msg, "delay="    )) 
	(options.link_cable)->delay = (int) atol(&msg[6]);


      if(strstr  (msg, "code_lines=")) 
	options.code_lines = (int) atol(&msg[11]);
      if(strstr  (msg, "stack_lines=")) 
	options.stack_lines = (int) atol(&msg[12]);
      if(strstr  (msg, "mem_lines=")) 
	options.mem_lines = (int) atol(&msg[10]);

      if(strexact(msg, "-help"       )) help();
      if(strexact(msg, "-version"    )) { version(); exit(0); }
    }

  return 0;
}
