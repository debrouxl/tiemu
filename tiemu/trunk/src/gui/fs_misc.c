/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 197 2004-05-15 11:40:44Z roms $ */

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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "intl.h"
#include "filesel.h"
#include "skinops.h"
#include "refresh.h"
#include "paths.h"
#include "struct.h"
#include "ti68k_int.h"
#include "dboxes.h"
#include "calc.h"
#include "rcfile.h"
#include "pbars.h"
#include "tie_error.h"

gint display_skin_dbox()
{
	const gchar *filename;

	filename = (char *)create_fsel(inst_paths.skin_dir, "*.skn");
	if (!filename)
		return 0;

    // Load new skin
    g_free(options.skin_file);
    options.skin_file = g_strdup(filename);
    
    hid_change_skin(options.skin_file);
    ti68k_engine_start();

	return 0;
}

gint display_load_state_dbox()
{
	const gchar *filename;
	int err;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.sav");
	if (!filename)
		return 0;

    g_free(params.sav_file);
    params.sav_file = g_strdup(filename);
    
    err = ti68k_state_load(params.sav_file);
	handle_error();
    ti68k_engine_start();

	return 0;
}

gint display_save_state_dbox()
{
    const gchar *filename;
	int err;
	gchar *basename;
	gchar *dot;
	gchar *pattern;

    // get filename
	basename = g_path_get_basename(params.rom_file);
	dot = strrchr(basename, '.');
	if(dot != NULL)
		*dot = '\0';
	pattern = g_strconcat(basename, ".sav", NULL);
	g_free(basename);

	filename = create_fsel(inst_paths.base_dir, pattern);
	g_free(pattern);
	if (!filename)
		return 0;

    g_free(params.sav_file);
    params.sav_file = g_strdup(filename);
    err = ti68k_state_save(params.sav_file);
	handle_error();
    
    if(!rcfile_exist())
    {
        rcfile_write();

#if defined(__LINUX__)
        msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in ~/.gtktiemu)."));
#elif defined(__WIN32__)
        msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in gtktiemu.ini)."));
#endif
    }

    ti68k_engine_start();

	return 0;
}

gint display_tifile_dbox()
{
	const gchar *filename;
    const gchar *ext;
	int err;

    // set mask
    switch(tihw.calc_type) {
    case TI92:
        ext = "*.92?";
    case TI89:
        ext = "*.89?";
        break;
    case TI92p:
        ext = "*.9x?";
        break;
	case V200:
		ext = "*.v2?";
		break;
    default:
        ext = "*.???";
        break;
    }

    // get filename
	filename = (char *)create_fsel(inst_paths.base_dir, (char *)ext);
	if (!filename)
    {
        ti68k_engine_start();
		return 0;
    }

    // check extension
    if(!tifiles_is_a_ti_file(filename) || 
        !tifiles_is_ti9x(tifiles_which_calc_type(filename))) {
        msg_box(_("Error"), _("This file is not a valid TI file."));
        ti68k_engine_start();
        return -1;
    }

    // set pbar title
#if 1
    if(tifiles_is_a_tib_file(filename) || tifiles_is_a_flash_file(filename)) {
        create_pbar_type5(_("Flash"), "");
    } else if(tifiles_is_a_backup_file(filename)) {
        create_pbar_type3(_("Backup"));
    } else if(tifiles_is_a_group_file(filename)) {
        create_pbar_type5(_("Sending group file"), "");
    } else if(tifiles_is_a_single_file(filename)) {
        create_pbar_type4(_("Sending variable"), "");
    }
#endif

    // note that core is currently not bkpt-interruptible when
    // transferring file
    GTK_REFRESH();
    err = ti68k_linkport_send_file(filename);
    handle_error();
    destroy_pbar();	
    ti68k_engine_start();

	return 0;
}

gint display_set_rom_dbox(void)
{
    const gchar *filename;
    const gchar *src = NULL;
    gchar *dst;
    gchar *cmd;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.rom");
	if (!filename)
		return 0;

    dst = g_strconcat(inst_paths.img_dir, g_basename(src), NULL);
    cmd = g_strdup_printf("cp %s %s", src, dst);
    system(cmd); //copy_file(src, dst);    

    ti68k_engine_start();

    return 0;
}

gint display_set_tib_dbox(void)
{
    const gchar *filename;
    //const gchar *src;
    //gchar *dst;
    //gchar *cmd;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.89u;*.9xu;*.tib");
	if (!filename)
		return 0;
/*
    g_free(params.tib_file);
    params.tib_file = g_strdup(filename);
    if(ti68k_loadImage(params.tib_file))  {
        msg_box(_("Error"), _("Can not open the ROM/FLASH file."));
        ti68k_engine_start();
        return -1;
    }    
*/
    ti68k_engine_start();

    return 0;
}



gint display_import_romversion_dbox(void)
{
    const gchar *filename;
	char *dstname;
	char *basename;
	int ret;
	int err;
    
    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.rom;*.89u;*.9xu;*.tib");
	if (!filename)
		return 0;

    if(ti68k_is_a_rom_file(filename))
	{
		err = ti68k_convert_rom_to_image(filename, inst_paths.img_dir, &dstname);
		handle_error();
		g_free(dstname);
	}
	else if(ti68k_is_a_tib_file(filename))
	{
		ret = msg_box2("Question", 
			"Do you want to load it as a fake image or as a FLASH upgrade ? \n\n"	\
			"Click 'yes/ok' if you want to directly upgrade the calculator operating system (AMS). \n\n"	\
			"On the other hand, if you load it as a fake ROM image, TiEmu will convert the FLASH "	\
			"upgrade into an image but your image will suffer from some limitations "	\
			"(no boot block, no certificate, problems with fonts)");
		if(ret == 2)
		{	// fake rom
			err = ti68k_convert_tib_to_image(filename, inst_paths.img_dir, &dstname);
			handle_error();
			g_free(dstname);
		}
		else if(ret ==1)
		{
			//copy
			basename = g_path_get_basename(filename);
			dstname = g_strconcat(inst_paths.img_dir, basename, NULL);
			g_free(basename);

#ifdef __WIN32__
			CopyFile(filename, dstname, FALSE);
#else
#endif
			g_free(dstname);
		}
	}

    return 0;
}
