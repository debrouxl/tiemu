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
  Platform independant paths
*/

#ifdef __WIN32__
# include <windows.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "tilibs.h"

#include "struct.h"
#include "support.h"
#include "paths.h"


InstPaths inst_paths;      // installation paths


/* Returns the user's home directory */
int get_home_path(char **path)
{
#if defined(__LINUX__)
  uid_t uid;
  struct passwd *p;
  
  uid = getuid();
  
  if((p = getpwuid(uid)) == NULL)
    {
      *path=NULL;
      return 0;
    }
  else
    {
      *path = g_strdup(p->pw_dir);
      return 1;
    }
#endif

  return 0;
}

/* Return the path where the file is stored */
int build_home_path(char **path, char *filename)
{
#if defined(__LINUX__)
  char *home_dir;
  
  get_home_path(&home_dir);
  *path = g_strconcat(home_dir, DIR_SEPARATOR, CONF_DIR, 
		      DIR_SEPARATOR, filename, NULL);
    g_free(home_dir);
#elif defined(__WIN32__)
  *path = g_strconcat(inst_paths.base_dir, DIR_SEPARATOR, CONF_DIR,
		     DIR_SEPARATOR, filename, NULL);
#endif

  return 0;
}


/* Initialize TiEmu paths depending on platform type */
int initialize_paths(void)
{
  char *home_dir;
#ifdef __WIN32__
  HMODULE hModule;
  DWORD dWord;
  char sBuffer[4*MAXCHARS];
  char *dirname;
#endif
  /* 
     Init the path for the Windows version by getting the executable
     location.
  */
#if defined(__WIN32__)
  /* Retrieve the directory of the executable */
  hModule = GetModuleHandle("tilp.exe");
  dWord = GetModuleFileName(hModule, sBuffer, 4*MAXCHARS);
  dirname = g_dirname(sBuffer);
  DISPLAY("Current path: <%s>\n", dirname);
  inst_paths.base_dir = g_strdup(dirname);
  g_free(dirname);
#else
  inst_paths.base_dir = g_strdup("");
#endif
  //DISPLAY("inst_path.base_dir = <%s>\n", inst_paths.base_dir);

  /*
    Initialize share path
  */
  inst_paths.share_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				   G_DIR_SEPARATOR_S, NULL);
  //DISPLAY("inst_paths.share_dir = <%s>\n", inst_paths.share_dir);

  /*
    Initialize pixmaps path
  */
#ifndef __WIN32__
  inst_paths.pixmap_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				    "/pixmaps/", NULL);
#else
  inst_paths.pixmap_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				    "\\pixmaps\\", NULL);
#endif
  //DISPLAY("inst_paths.pixmap_dir = <%s>\n", inst_paths.pixmap_dir);
  add_pixmap_directory(inst_paths.pixmap_dir);

  /* 
     Init internationalization for Linux or Windows
     Some things about i18 paths:
     bintextdomain(PACKAGE, "/usr/share/locale"); ->
     '/usr/share/locale/fr/LC_MESSAGES/tilp.mo'
  */
#if defined(HAVE_LIBINTL_H) || defined(ENABLE_NLS)
#ifdef __LINUX__
  inst_paths.locale_dir = g_strjoin(inst_paths.base_dir, PACKAGE_LOCALE_DIR,
				    G_DIR_SEPARATOR_S, NULL);
#elif defined(__WIN32__)
  inst_paths.locale_dir = g_strjoin(inst_paths.base_dir, PACKAGE_LOCALE_DIR,
				    "\\locale\\", NULL);
#endif
  //DISPLAY("inst_paths.locale_dir = <%s>\n", inst_paths.locale_dir);
#endif
  
  /*
    Initialize help path
  */
#ifndef __WIN32__
  inst_paths.help_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				  "/help/", NULL);
#else
  inst_paths.help_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				  "\\help\\", NULL);
#endif
  //DISPLAY("inst_paths.help_dir = <%s>\n", inst_paths.help_dir);
  
  /*
    Initialize manpage path
  */
#ifndef __WIN32__
  inst_paths.manpage_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				     "/", NULL);
#else
  inst_paths.manpage_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				     "\\", NULL);
#endif
  //DISPLAY("inst_paths.manpage_dir = <%s>\n", inst_paths.manpage_dir);
  
  /*
    Initialize ROMs path
  */
#ifndef __WIN32__
  inst_paths.rom_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				    "/ROMs/", NULL);
#else
  inst_paths.rom_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				    "\\ROMs\\", NULL);
#endif
  //DISPLAY("inst_paths.rom_dir = <%s>\n", inst_paths.rom_dir);

  /*
    Initialize skin path
  */
#ifndef __WIN32__
  inst_paths.skin_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				    "/skins/", NULL);
#else
  inst_paths.skin_dir = g_strjoin(inst_paths.base_dir, SHARE_DIR,
				    "\\skins\\", NULL);
#endif
  //DISPLAY("inst_paths.skin_dir = <%s>\n", inst_paths.skin_dir);

  // on UNIX systems, HOME directory by default
#ifdef __LINUX__ 
  if(get_home_path(&home_dir))
    {
      chdir(home_dir);
      g_free(home_dir);
    }
  else
    {
      DISPLAY("Can not get HOME directory.\n");
      exit(-1);
    }
#endif

  return 0;
}
