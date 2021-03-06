/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Li�vin
 *  Copyright (c) 2005, Romain Li�vin
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
#include "ti68k_int.h"
#include "tie_error.h"

/*
  Display the program version
*/
int version(void)
{
	fprintf(stdout, "TiEmu II - Version %s\n", TIEMU_VERSION);
	fprintf(stdout, "  (C) Romain Lievin & Thomas Corvazier  2000-2001\n");
	fprintf(stdout, "  (C) Romain Lievin 2001-2003\n");
	fprintf(stdout, "  (C) Julien Blache 2003\n");
	fprintf(stdout, "  (C) Romain Lievin 2004-2005\n");
	fprintf(stdout, "  (C) Romain Lievin & Kevin Kofler 2005\n");
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

	fprintf(stdout, "usage: tiemu [-options] [image]\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "-h, --help     display this information page and exit\n");
	fprintf(stdout, "-v, --version  display the version information and exit\n");
	fprintf(stdout, "--import       import ROM or TIB into repository without loading\n");
	fprintf(stdout, "-rom=          ROM dump to convert and load\n");
	fprintf(stdout, "-tib=          TIB or upgrade to convert and load\n");
	fprintf(stdout, "-sav=          state image to load\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "filename       a filename which contains a TiEmu image\n");
	fprintf(stdout, "\n");

	exit(0);
	return 0;
}

static int strexact(char *p1, char *p2)
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
	gchar *dstname;
	int import = 0;
  
	for(cnt=1; cnt<argc; cnt++) 
	{
		p = argv[cnt];

		if(*p == '-' ) 
		{
			// a long option (like --help)
			p++;
		} else 
		{
			// an image to load
			g_free(params.rom_file);
			params.rom_file = g_strdup(p);
		}

		strcpy(msg, p);

		if(strexact(msg, "-import")) 
			import = !0;
      
	    if(strstr(msg, "rom=") || strstr(msg, "tib="))
		{	
			q=msg+4;

			if(ti68k_is_a_rom_file(q))
			{
				int err = ti68k_convert_rom_to_image(q, inst_paths.img_dir, &dstname);
				if(err) 
				{
					tiemu_error(err, NULL);
					exit(-1);
				}

				if(import)
					exit(0);

				g_free(params.rom_file);
				params.rom_file = dstname;
				g_free(params.sav_file);
				params.sav_file = g_strdup("");
			}
			else if(ti68k_is_a_tib_file(q))
			{
				int err = ti68k_convert_tib_to_image(q, inst_paths.img_dir, &dstname);
				if(err) 
				{
					tiemu_error(err, NULL);
					exit(-1);
				}

				if(import)
					exit(0);

				g_free(params.rom_file);
				params.rom_file = dstname;
				g_free(params.sav_file);
				params.sav_file = g_strdup("");
			}
			else
				exit(-1);
		}

		if(strstr(msg, "sav=")) 
		{
			q=msg+4;
			g_free(params.sav_file);
			params.sav_file = g_strdup(p);
		}
	      
		if(strexact(msg, "-help") || strexact(msg, "h")) 
			help();

	    if(strexact(msg, "-version") || strexact(msg, "v")) 
		{ 
			//version(); 
			exit(0); 
		}
	}

	return 0;
}
