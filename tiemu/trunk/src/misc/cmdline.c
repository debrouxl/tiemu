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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "version.h"
#include "ti68k_def.h"

/*
  Display the program version
*/
int version(void)
{
	fprintf(stdout, "TiEmu II - Version %s\n", TIEMU_VERSION);
	fprintf(stdout, "  (C) Romain Lievin & Thomas Corvazier  2000-2001\n");
	fprintf(stdout, "  (C) Romain Lievin 2001-2003\n");
	fprintf(stdout, "  (C) Julien Blache 2003\n");
	fprintf(stdout, "  (C) Romain Lievin 2004\n");
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
	fprintf(stdout, "-sav_file=    RAM image to load\n");
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
  
	for(cnt=1; cnt<argc; cnt++) {
		p=argv[cnt];
		if(*p=='-' ) {
			// an option
			p++;
		} else {
			// a ROM to load
			g_free(params.rom_file);
			params.rom_file = g_strdup(p);
		}
      
	      if(strstr  (msg, "rom_file="     )) {
		  q=msg+9;
		  g_free(params.rom_file);
		  params.rom_file = g_strdup(p);
		}
	      if(strstr  (msg, "sav_file="     )) {
		  q=msg+9;
		  g_free(params.sav_file);
		  params.sav_file = g_strdup(p);
		}

	      if(strexact(msg, "-help"       )) help();
	      if(strexact(msg, "-version"    )) { version(); exit(0); }
	}

	return 0;
}
