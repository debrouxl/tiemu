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

#ifndef __PATHS_H__
#define __PATHS_H__

/* 
   Platform independant paths
*/

#if defined(__LINUX__) || defined(__MACOSX__)
# define CONF_DIR   	".tiemu/"
# define INI_FILE    	"tiemu.ini"
# define CACHE_FILE 	"ROMs.lst"
# define LOG_FILE 		"/tmp/tiemu.log"
#elif defined(__WIN32__)
# define CONF_DIR   	""
# define INI_FILE    	"tiemu.ini"
# define CACHE_FILE 	"ROMs.lst"
# define LOG_FILE 		"C:\\tiemu.log"
#else
# define CONF_DIR   	".tiemu/"
# define INI_FILE    	"init"
# define CACHE_FILE 	"ROMs.lst"
# define LOG_FILE 		"tiemu.log"
#endif

#ifdef __WIN32__
#define SHARE_DIR "" // local path
#endif

/*
  Portable installation paths
*/
typedef struct
{
  char *base_dir;		// Windows directory (determined at startup)
  char *locale_dir;		// locale
  char *manpage_dir;	// manpages
  char *help_dir;		// help files
  char *pixmap_dir;		// pixmaps
  char *img_dir;		// images
  char *skin_dir;		// skins
  char *glade_dir;		// GUI files
  char *home_dir;		// $HOME

  char current_dir[1024];	// current working directory
} TiemuInstPaths;

extern TiemuInstPaths inst_paths;

/*
  Functions
*/

int build_home_path(char **path, char *fileaname);

int initialize_paths(void);

const char *tilp_paths_build_glade(const char *name);

#endif

