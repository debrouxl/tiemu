/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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

	filename = (char *)create_fsel(inst_paths.skin_dir, NULL, "*.skn", FALSE);
	if (!filename)
	{
		return 0;
	}

    // Load new skin
    g_free(options.skin_file);
    options.skin_file = g_strdup(filename);
    
    hid_change_skin(options.skin_file);

	return 0;
}

gint display_load_state_dbox()
{
	const gchar *filename;
	int err;

    // get filename
	filename = create_fsel(inst_paths.home_dir, NULL, "*.sav", FALSE);
	if (!filename)
		return 0;

    g_free(params.sav_file);
    params.sav_file = g_strdup(filename);
    
    err = ti68k_state_load(params.sav_file);
	handle_error();

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

	filename = create_fsel(inst_paths.home_dir, pattern, "*.sav", TRUE);
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
	      _("You do not seem to have saved your settings. Configuration file saved (in ~/.tiemu)."));
#elif defined(__WIN32__)
        msg_box(_("Information"), 
	      _("You do not seem to have saved your settings. Configuration file saved (in tiemu.ini)."));
#endif
    }

	return 0;
}

gint display_tifile_dbox()
{
	const gchar *filename;
    const gchar *ext;
	int err;
	static gchar *folder = NULL;

    // set mask
    switch(tihw.calc_type) 
	{
    case TI92:
        ext = "*.92?";
		break;
	default:
        ext = "*.89?;*.92?;*.9x?;*.v2?";
        break;
    }

	// get filename
	if(folder == NULL)
		folder = g_strdup(inst_paths.base_dir);

	filename = (char *)create_fsel(folder, NULL, (char *)ext, FALSE);
	if (!filename)
    {
		return 0;
    }

	// keep folder
	g_free(folder);
	folder = g_path_get_dirname(filename);

    // check extension
    if(!tifiles_is_a_ti_file(filename) || 
        !tifiles_is_ti9x(tifiles_which_calc_type(filename))) 
	{
        msg_box(_("Error"), _("This file is not a valid TI file."));
        return -1;
    }

    // set pbar title
    if(tifiles_is_a_tib_file(filename) || tifiles_is_a_flash_file(filename)) 
	{
        create_pbar_type5(_("Flash"), "");
    } 
	else if(tifiles_is_a_backup_file(filename)) 
	{
        create_pbar_type3(_("Backup"));
    } 
	else if(tifiles_is_a_group_file(filename)) 
	{
        create_pbar_type5(_("Sending group file"), "");
    } 
	else if(tifiles_is_a_single_file(filename)) 
	{
        create_pbar_type4(_("Sending variable"), "");
    }

    // note that core is currently not bkpt-interruptible when
    // transferring file
    GTK_REFRESH();
    err = ti68k_linkport_send_file(filename);
    handle_error();
    destroy_pbar();	

	return 0;
}

gint display_tifiles_dbox()
{
	const gchar *ext;
	gchar **filenames, **ptr;
	int err;
	static gchar *folder = NULL;

    // set mask
    switch(tihw.calc_type) 
	{
    case TI92:
        ext = "*.92?";
		break;
	default:
        ext = "*.89?;*.92?;*.9x?;*.v2?";
        break;
    }

	// get filename
	if(folder == NULL)
		folder = g_strdup(inst_paths.base_dir);

	filenames = create_fsels(folder, NULL, (char *)ext, FALSE);
	if(!filenames)
		return 0;

	// keep folder
	g_free(folder);
	folder = g_path_get_dirname(filenames[0]);

    // check extension
	for(ptr = filenames; *ptr; ptr++)
	{
		if(!tifiles_is_a_ti_file(*ptr) || !tifiles_is_ti9x(tifiles_which_calc_type(*ptr))) 
		{
			msg_box(_("Error"), _("This file is not a valid TI file."));
			g_strfreev(filenames);
			return -1;
		}

		// set pbar title
		if(tifiles_is_a_tib_file(*ptr) || tifiles_is_a_flash_file(*ptr)) 
		{
			create_pbar_type5(_("Flash"), "");
		} 
		else if(tifiles_is_a_backup_file(*ptr)) 
		{
			create_pbar_type3(_("Backup"));
		} 
		else if(tifiles_is_a_group_file(*ptr)) 
		{
			create_pbar_type5(_("Sending group file"), "");
		} 
		else if(tifiles_is_a_single_file(*ptr)) 
		{
			create_pbar_type4(_("Sending variable"), "");
		}

		// note that core is currently not bkpt-interruptible when
		// transferring file
		GTK_REFRESH();
		err = ti68k_linkport_send_file(*ptr);
		handle_error();
		destroy_pbar();	
	}

	g_strfreev(filenames);
	return 0;
}

gint display_set_tib_dbox(void)
{
    const gchar *filename;
	gchar *path, *name;
	int err;

    // get filename
	filename = create_fsel(inst_paths.base_dir, NULL, "*.89u;*.9xu;*.v2u;*.tib", FALSE);
	if (!filename)
		return 0;

	if(!ti68k_is_a_tib_file(filename))
	{
		msg_box("Error", "Don't seem to be an upgrade.");
		return -1;
	}

	path = g_path_get_dirname(filename);
	name = g_path_get_basename(filename);

	// set tib file
	g_free(params.tib_file);
	params.tib_file = g_strconcat(path, G_DIR_SEPARATOR_S, name, NULL);
	g_free(path); g_free(name);

	err = ti68k_load_upgrade(params.tib_file);
	handle_error();
	if(err)
	{
		msg_box("Error", "Can not load the upgrade.");
		return -1;
	}
    
    //msg_box(_("Information"), _("Your configuration has been saved."));
    //rcfile_write();

    // simply reset, don't restart
    ti68k_reset();

    return 0;
}

gint display_import_romversion_dbox(void)
{
    const gchar *filename;
	char *dstname;
	int err;
    
    // get filename
	filename = create_fsel(inst_paths.base_dir, NULL, "*.rom;*.89u;*.9xu;*.v2u;*.tib", FALSE);
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
		// fake rom
		err = ti68k_convert_tib_to_image(filename, inst_paths.img_dir, &dstname);
		handle_error();
		g_free(dstname);
	}

    return 0;
}
