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

/*
  Platform independant paths
*/

#include <stdio.h>
#ifdef __WIN32__
#include <windows.h>
#include <direct.h> // _mkdir
#endif

#include "struct.h"
#include "paths.h"


TiemuInstPaths inst_paths;      // installation paths


/*
  Called by TiEmu at startup for initializing platform dependant paths.
*/
#if defined(__LINUX__) || defined(__BSD__)
static void init_linux_paths(void)
{
	inst_paths.base_dir =
	    g_strconcat(SHARE_DIR, G_DIR_SEPARATOR_S, NULL);
	inst_paths.pixmap_dir =
	    g_strconcat(inst_paths.base_dir, "pixmaps/", NULL);
	inst_paths.help_dir =
	    g_strconcat(inst_paths.base_dir, "help/", NULL);
	inst_paths.manpage_dir = 
        g_strconcat(inst_paths.base_dir, "", NULL);
	inst_paths.skin_dir =
	    g_strconcat(inst_paths.base_dir, "skins/", NULL);
	inst_paths.glade_dir =
	    g_strconcat(inst_paths.base_dir, "glade/", NULL);
	inst_paths.home_dir =
		g_strdup(g_get_home_dir());
	inst_paths.img_dir = 
            g_strconcat(inst_paths.home_dir, CONF_DIR, "images/", NULL);
	mkdir(inst_paths.img_dir);

	/* bintextdomain(PACKAGE, "/usr/share/locale"); ->
	   '/usr/share/locale/  fr/LC_MESSAGES/tilp.mo' */
#ifdef ENABLE_NLS
	inst_paths.locale_dir = g_strconcat(LOCALEDIR, "/", NULL);
#endif				/*  */

	// on LINUX systems, HOME directory by default for security reasons
	chdir(inst_paths.home_dir);
}
#endif				/*  */

#ifdef __WIN32__
static void init_win32_paths(void)
{
	HMODULE hModule;
	DWORD dWord;
	char *dirname;
	char *sBuffer;

	// Init the path for the Windows version by getting the 
	// executable location.
	hModule = GetModuleHandle("tiemu.exe");
	sBuffer = (char *) malloc(4096 * sizeof(char));
	dWord = GetModuleFileName(hModule, sBuffer, 4096);
	dirname = g_dirname(sBuffer);
	fprintf(stdout, "executable path: <%s>\n", dirname);
	inst_paths.base_dir = g_strconcat(dirname, "\\", NULL);
	g_free(dirname);
	free(sBuffer);  // malloc -> free

	inst_paths.pixmap_dir =
	    g_strconcat(inst_paths.base_dir, "pixmaps\\", NULL);
	inst_paths.help_dir =
	    g_strconcat(inst_paths.base_dir, "help\\", NULL);
	inst_paths.manpage_dir =
	    g_strconcat(inst_paths.base_dir, "", NULL);
	inst_paths.skin_dir =
	    g_strconcat(inst_paths.base_dir, "skins\\", NULL);
	inst_paths.glade_dir =
	    g_strconcat(inst_paths.base_dir, "glade\\", NULL);
	inst_paths.home_dir = 
	    g_strconcat(inst_paths.base_dir, "", NULL);
	inst_paths.img_dir = 
            g_strconcat(inst_paths.home_dir, CONF_DIR, "images\\", NULL);
#ifndef __WIN32__
    mkdir(inst_paths.img_dir, 0755);
#else
    mkdir(inst_paths.img_dir);
#endif

#ifdef ENABLE_NLS
	inst_paths.locale_dir =
	    g_strconcat(inst_paths.base_dir, "locale\\", NULL);
#endif				/*  */
	// on WIN32 systems, local by default
	_chdir(inst_paths.home_dir);
}
#endif				/*  */
int initialize_paths(void)
{

#if defined(__LINUX__) || defined(__BSD__)
	init_linux_paths();

#elif defined(__WIN32__)
	init_win32_paths();

#endif				/*  */
	return 0;
}

const char *tilp_paths_build_glade(const char *name)
{
	static char *path = NULL;
	
	g_free(path);
	path = g_strconcat(inst_paths.glade_dir, name, NULL);
	
	return path;
}
