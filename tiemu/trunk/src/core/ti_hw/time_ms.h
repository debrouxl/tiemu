/* Hey EMACS -*- linux-c -*- */
/* $Id: timeout.h 1493 2005-07-29 13:37:24Z roms $ */

/*  libCables - Ti Link Cable library, a part of the TiLP project
 *  Copyright (C) 1999-2005  Romain Lievin
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
  Some explanations about this unit: I use different functions for measuring
  time depending on the platform type. In fact, it appears that the Win98 clock() 
  function of <time.h> is 600 times  slower than the Win32 GetTickCount() 
  function of <windows.h>. WinNT clock() is running faster but still 
  worse than GetTickCount.
*/

#ifndef __TIME_MS__
#define __TIME_MS__

/*
  Platform independant time measurement (in milli-seconds) 
	and timeout management.
  - TMS_START:   retrieve time (starting point)
  - TMS_CURRENT: return the number of milli-seconds elapsed since TO_START()
  - TMS_ELAPSED: return TRUE if max tenth of seconds have elapsed
*/

// I don't want to include windows.h here.
//# include <windows.h>
//typedef DWORD tiTIME;
typedef unsigned long tiTIME;

#if defined(__WIN32__) && !defined(__MINGW32__)

# define  TMS_START(ref)          { (ref) = GetTickCount(); }
# define  TMS_CURRENT(ref)        ( GetTickCount() - (ref) )
# define  TMS_ELAPSED(ref, max)   ( TMS_CURRENT(ref) > (max) )

#else

# include <time.h>
# define  TMS_START(ref)         { (ref) = ((1000*clock()) / CLOCKS_PER_SEC); }
# define  TMS_CURRENT(ref)       ( (1000*clock()) / CLOCKS_PER_SEC - (ref) )
# define  TMS_ELAPSED(ref, max)  ( TMS_CURRENT(ref) > (max) )

#endif

#endif
