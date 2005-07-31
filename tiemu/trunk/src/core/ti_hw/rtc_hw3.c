/* Hey EMACS -*- linux-c -*- */
/* $Id: hw.c 1601 2005-07-30 22:03:29Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
    TI's HW3 RTC helpers.
*/


#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __WIN32__
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#include "libuae.h"
#include "mem.h"
#include "ports.h"
#include "images.h"
#include "ti68k_def.h"

int rtc3_init(void)
{
	struct tm ref;
	time_t now;

	// Computes pseudo-constant (difference between PC ref and TI ref) because:
	// - TI counts seconds since January 1st, 1997 00:00:00
	// - PC counts seconds since January 1st, 1970 00:00:00
	time(&now);	
	memcpy(&ref, localtime(&now), sizeof(struct tm)); // get tm_isdst field (DST status)
	ref.tm_year  = 1997 - 1900;
	ref.tm_mon   = 0;
	ref.tm_yday  = 0;
	ref.tm_mday  = 1;
	ref.tm_wday  = 3;
	ref.tm_hour  = 0;
	ref.tm_min   = 0;
	ref.tm_sec   = 0;
	tihw.rtc3_beg = tihw.rtc3_cur = mktime(&ref);
	//printf("<<%s>>\n", asctime(&ref));

	return 0;
}

int rtc3_reset(void)
{
	return 0;
}

int rtc3_exit(void)
{
	return 0;
}

// call it before ti68k_state_save to update registers with current clock
int rtc3_state_save(void)
{
	struct _timeb tb;

	// 1/16th of seconds
	_ftime(&tb);
	tihw.io3[0x45] = (int)(tb.millitm / 0.0625);
	tihw.io3[0x45] &= 0x0f;

	// seconds since January 1st, 1997 00:00:00
	time(&tihw.rtc3_cur);
	tihw.io3[0x46] = MSB(MSW((time_t)difftime(tihw.rtc3_cur, tihw.rtc3_beg) + tihw.rtc3_load)); 
	tihw.io3[0x47] = LSB(MSW((time_t)difftime(tihw.rtc3_cur, tihw.rtc3_beg) + tihw.rtc3_load)); 
	tihw.io3[0x48] = MSB(LSW((time_t)difftime(tihw.rtc3_cur, tihw.rtc3_beg) + tihw.rtc3_load)); 
	tihw.io3[0x49] = LSB(LSW((time_t)difftime(tihw.rtc3_cur, tihw.rtc3_beg) + tihw.rtc3_load)); 

	return 0;
}

// call it after ti68k_state_load to fix clock
int rtc3_state_load(void)
{
	

	return 0;
}
