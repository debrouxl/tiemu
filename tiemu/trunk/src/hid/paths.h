/*  gtktilink - link program for TI calculators
 *  Copyright (C) 1999, 2000  Romain Lievin
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

#if defined(__LINUX__)
# define CONF_DIR   ".tiemu"
# define RC_FILE    "init"
# define ST_FILE    "version"
# define IO_FILE    "/etc/tiemu.conf"
# define CACHE_FILE "ROMs.lst"
#elif defined(__WIN32__)
# define CONF_DIR   ""
# define RC_FILE    "tiemu.ini"
# define ST_FILE    "tiemu.ver"
# define IO_FILE    "tiemu.cnf"
# define CACHE_FILE "ROMs.lst"
#else
# define CONF_DIR   ".tiemu"
# define RC_FILE    "init"
# define ST_FILE    "version"
# define IO_FILE    "/etc/tiemu.conf"
# define CACHE_FILE "ROMs.lst"
#endif

#ifdef __WIN32__
#define SHARE_DIR "" // local path
#endif

/*
  Portable installation paths
*/
struct InstPaths_
{
  char *base_dir;		// Windows directory (determined at startup)
  
  char *share_dir;		// shared directory
  char *locale_dir;		// locale
  char *manpage_dir;		// manpages
  char *help_dir;		// help files
  char *pixmap_dir;		// pixmaps
  char *rom_dir;		// ROMs directory
  char *skin_dir;		// Skins directory

  char current_dir[1024];	// current working directory
} inst_paths;
typedef struct InstPaths_ InstPaths;

extern InstPaths inst_paths;

/*
  Functions
*/

int get_home_path(char **path);
int build_home_path(char **path, char *fileaname);

int initialize_paths(void);

#endif

