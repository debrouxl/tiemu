/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - a TI emulator
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

#ifndef VERSION_H
#define VERSION_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/*
  This file contains version number
  and library requirements.
*/

#ifdef __WIN32__
# define TIEMU_VERSION "1.80-041120"		// For Win32
#else
# define TIEMU_VERSION VERSION
#endif
#define TIEMU_REQUIRES_LIBFILES_VERSION		"0.6.1"
#define TIEMU_REQUIRES_LIBCABLES_VERSION	"3.8.1"
#define TIEMU_REQUIRES_LIBCALCS_VERSION 	"4.5.4"

#endif


