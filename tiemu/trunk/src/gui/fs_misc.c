/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 197 2004-05-15 11:40:44Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Li�vin
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

#include <gtk/gtk.h>

#include "intl.h"
#include "filesel.h"
#include "skinops.h"
#include "refresh.h"
#include "paths.h"
#include "struct.h"
#include "interface.h"

gint display_skin_dbox()
{
	const gchar *filename;
    SKIN_INFOS si;
    gint ok;

	filename = create_fsel(inst_paths.skin_dir, "*.skn");
	if (!filename)
		return;

    // Read skin header
    if(skin_read_header(filename, &si) == -1) {

        msg_box(_("Error"), _("Unable to use this skin."));
        return -1;
    }

    // Check skin header
    switch(ti68k_getCalcType())
	{
	    case TI92:
            ok = !strcmp(si.calc, SKIN_TI92);
		break;
	    case TI89:
            ok = !strcmp(si.calc, SKIN_TI89);
		break;
	    case TI92 | MODULEPLUS:
            ok = !strcmp(si.calc, SKIN_TI92P);
		break;
	    default: 
            ok = 0;
		break;
	}

        
    if(!ok) {
        msg_box(_("Error"), _("Skin incompatible with the current calc model."));
        ti68k_unhalt();
        return -1;
    }

    // Load new skin
    g_free(options.skin_file);
    options.skin_file = g_strconcat(filename, NULL);
    
    hid_change_skin(options.skin_file);
    ti68k_unhalt();

	return 0;
}

gint display_load_state_dbox()
{
	const gchar *filename;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.sav");
	if (!filename)
		return;

    g_free((options.params)->ram_file);
    (options.params)->ram_file = g_strdup(filename);
    
    ti68k_loadState((options.params)->ram_file);
    ti68k_unhalt();

	return 0;
}

gint display_save_state_dbox()
{
    const gchar *filename;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.sav");
	if (!filename)
		return;

    g_free((options.params)->ram_file);
    (options.params)->ram_file = g_strdup(filename);
    ti68k_saveState((options.params)->ram_file);
    
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

    ti68k_unhalt();

	return 0;
}

gint display_tifile_dbox()
{
	const gchar *filename;
    const gchar *ext;

    // set mask
    switch(ti68k_getCalcType()) {
    case TI92:
        ext = "*.92?";
    case TI89:
        ext = "*.89?";
        break;
    case TI92 | MODULEPLUS:
        ext = "*.9x?";
        break;
    default:
        ext = "*.???";
        break;
    }

    // get filename
	filename = create_fsel(inst_paths.base_dir, ext);
	if (!filename)
		return;

    // check extension
    if(!tifiles_is_a_ti_file(filename) || 
        !tifiles_is_ti9x(tifiles_which_calc_type(filename))) {
        msg_box(_("Error"), _("This file is not a valid TI file."));
        ti68k_unhalt();
        return -1;
    }

    // set pbar title
    if(tifiles_is_a_tib_file(filename) || tifiles_is_a_flash_file(filename)) {
        create_pbar_type5(_("Flash"), "");
    } else if(tifiles_is_a_backup_file(filename)) {
        create_pbar_type3(_("Backup"));
    } else if(tifiles_is_a_group_file(filename)) {
        create_pbar_type5(_("Sending group file"), "");
    } else if(tifiles_is_a_single_file(filename)) {
        create_pbar_type4(_("Sending variable"), "");
    }

    ti68k_unhalt();
    ti68k_sendFile(filename);

	return 0;
}

gint display_set_rom_dbox(void)
{
    const gchar *filename;
    const gchar *src;
    gchar *dst;
    gchar *cmd;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.rom");
	if (!filename)
		return;

    dst = g_strconcat(inst_paths.img_dir, g_basename(src), NULL);
    cmd = g_strdup_printf("cp %s %s", src, dst);
    system(cmd); //copy_file(src, dst);    

    ti68k_unhalt();
}

gint display_set_tib_dbox(void)
{
    const gchar *filename;
    const gchar *src;
    gchar *dst;
    gchar *cmd;

    // get filename
	filename = create_fsel(inst_paths.base_dir, "*.89u;*.9xu;*.tib");
	if (!filename)
		return;
/*
    g_free((options.params)->tib_file);
    (options.params)->tib_file = g_strdup(filename);
    if(ti68k_loadImage((options.params)->tib_file))  {
        msg_box(_("Error"), _("Can not open the ROM/FLASH file."));
        ti68k_unhalt();
        return -1;
    }    
*/
    ti68k_unhalt();
}

