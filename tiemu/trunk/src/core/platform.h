/*  tilp - link program for TI calculators
 *  Copyright (C) 1999-2001  Romain Lievin
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
  This file include some dependant OS headers files.
*/

#ifndef __TI68K_PLATFORM__
#define __TI68K_PLATFORM__

#include "intl.h"		// GNU Internationalization library

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_DIRENT_H
# include <dirent.h>
# include <grp.h>
# include <pwd.h>
# include <unistd.h>
#else
# if defined(__WIN32__)
#  include "win32/pwd.h"
#  include "win32/grp.h"
#  include "win32/unistd.h"
#  include "win32/dirent.h"
# endif
#endif

#if defined(__WIN32__)	// Win32 (GTK or MFC/VCL)
# include <direct.h>
# include <process.h>
# include <io.h>
#endif

#if defined(__WIN32__)
# define snprintf	_snprintf
# define popen		_popen
# define pclose	        _pclose
#endif

#endif // DEP_H

/*
 * To get prototypes for the following POSIXish functions, you have to
 * include the indicated non-POSIX headers. The functions are defined
 * in OLDNAMES.LIB (MSVC) or -lmoldname-msvc (mingw32).
 *
 * getcwd: <direct.h> (MSVC), <io.h> (mingw32)
 * getpid: <process.h>
 * access: <io.h>
 * unlink: <stdio.h> or <io.h>
 * open, read, write, lseek, close: <io.h>
 * rmdir: <direct.h>
 * pipe: <direct.h>
 */
