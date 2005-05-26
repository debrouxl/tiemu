/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin, Kevin Kofler
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
  Logging system for the 4 domains:
  - ticables
  - tifiles
  - ticalcs 
  - tiemu
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifdef __WIN32__
#include <windows.h>
#endif

#include "intl.h"
#include "struct.h"
#include "printl.h"


/* 
   Print to stdout as default behaviour unless changed by tifiles_set_print 
   Level: such as "warning", "error", "information", etc. "" = nothing.
*/

#define DOMAIN_TICABLES         "ticables: "
#define DOMAIN_TIFILES          "tifiles : "
#define DOMAIN_TICALCS          "ticalcs : "
#define DOMAIN_TIEMU            "tiemu   : "


/**************** printl muxer ************************/

#ifdef __WIN32__
#include <windows.h>
static BOOL alloc_console_called = FALSE;
static HANDLE hConsole = (HANDLE)-1;
#endif

static FILE *flog = NULL;

static int printl_muxer(const char *domain, int level, const char *format, va_list ap)
{
	static int print_domain = !0;

#ifndef _MSC_VER
	va_list log_ap;
	va_copy(log_ap, ap);
#endif

#ifdef __WIN32__
        char buffer[128];
        int cnt;
        DWORD nWritten;

        // if console, do verbose
        if(options.console == FALSE)
                goto skip_console;

        // open console once
        if (alloc_console_called == FALSE) {
	        AllocConsole();
	        alloc_console_called = TRUE;
	        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	        //freopen("CONOUT$", "w", stdout);
        }
	
	// if carriage return, print domain, else bufferize
	if(print_domain) {
		cnt = sprintf(buffer, domain);
		WriteConsole(hConsole, buffer, cnt, &nWritten, NULL);
		//print_domain = 0;
	}	

	// print warning/error if needed
	switch(level) {
	case 1: cnt = sprintf(buffer, _("wrn: ")); break;
	case 2: cnt = sprintf(buffer, _("err: ")); break;
	default: cnt = 0; break;
	}
	WriteConsole(hConsole, buffer, cnt, &nWritten, NULL);

	// print format/args
#ifdef _MSC_VER
        cnt = _vsnprintf(buffer, 128, format, ap);
#else
        cnt = _vsnprintf(buffer, 128, format, log_ap);
#endif
        WriteConsole(hConsole, buffer, cnt, &nWritten, NULL);
#else
	//
	if(print_domain) {
		fprintf(stdout, domain);
		//print_domain = 0;
	}

	//
	switch(level) {
	case 1: fprintf(stdout, _("wrn: ")); break;
	case 2: fprintf(stdout, _("err: ")); break;
	}
	
	//
	vfprintf(stdout, format, log_ap);
#endif
#ifdef __WIN32__
skip_console:
#endif
	if (flog == NULL) {
    		flog = fopen(LOG_FILE, "wt");
                //flog = fopen(inst_paths.base_dir, "wt");
	}
	
	if(print_domain) {
			fprintf(flog, domain);
			print_domain = 0;
	}
		
	//
	switch(level) {
	case 1: fprintf(flog, _("wrn: ")); break;
	case 2: fprintf(flog, _("err: ")); break;
	}

	// print same stuffs in file
#ifndef _MSC_VER
	vfprintf(flog, format, log_ap);
#else
	vfprintf(flog, format, ap);
#endif

	// check for printing domain on next loop
	if(strchr(format, '\n') || strchr(format, '\r'))
		print_domain = !0;
	
	return 0;
}


int close_console(void)
{
#ifdef __WIN32__
	return FreeConsole();
#else
    return 0;
#endif
}

/**************** printl callbacks ********************/

int ticables_printl(int level, const char *format, ...)
{
        va_list ap;
	int ret = 0;

	va_start(ap, format);
        ret = printl_muxer(DOMAIN_TICABLES, level, format, ap);
        va_end(ap);
 
	return ret;
}

int tifiles_printl(int level, const char *format, ...)
{
        va_list ap;
	int ret = 0;

	va_start(ap, format);
        ret = printl_muxer(DOMAIN_TIFILES, level, format, ap);
        va_end(ap);
 
	return ret;
}

int ticalcs_printl(int level, const char *format, ...)
{
        va_list ap;
	int ret = 0;

	va_start(ap, format);
        ret = printl_muxer(DOMAIN_TICALCS, level, format, ap);
        va_end(ap);
 
	return ret;
}

int tiemu_printl(int level, const char *format, ...)
{
	va_list ap;
	int ret = 0;

	va_start(ap, format);
        ret = printl_muxer(DOMAIN_TIEMU, level, format, ap);
        va_end(ap);

	return ret;
}

static int tiemu_default(int level, const char *format, ...)
{
	va_list ap;
	int ret = 0;

	va_start(ap, format);
	vfprintf(stdout, format, ap);
    va_end(ap);

	return ret;
}

TIEMU_PRINTL printl = tiemu_default;

/*
	Change print behaviour (callback).
*/
TIEMU_PRINTL tiemu_set_printl(TIEMU_PRINTL new_printl)
{
  TIEMU_PRINTL old_printl = printl;

  printl = new_printl;

  return old_printl;
}
