/* Hey EMACS -*- linux-c -*- */
/* $Id: cabl_int.h 651 2004-04-25 15:22:07Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Originally written by Jonas Minsberg
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (C) 2001-2004, Romain Lievin
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


#include <stdio.h>
#include <string.h>

#ifndef __MACOSX__
#include "tilibs.h"
#else				/*  */
#include <libticalcs/calc_int.h>
#endif				/*  */

#include "intl.h"
#include "printl.h"
#include "user_cb.h"

/*
  This function can take 2 parameters:
  - an error to translate or 0
  - a pure message or NULL
 */
int tiemu_error(int err_code, char *err_str)
{
	int err = err_code;
	char s[512];

	if(!err_code && !err_str)
		return 0;

	if(err_code) {
		/* Retrieve the error message */
		err = ticable_get_error(err, s);
		if (err) {
			err = tifiles_get_error(err, s);
			if (err) {
				err = ticalc_get_error(err, s);
				if (err) {

					// next level: error for TiEmu
				}
			}
		}
	} else if(err_str) {
		strcpy(s, err_str);
	}
	
	printl(2, "%s", s);
	user1_box(_("Error"), s, _("OK"));

	return err_code;
}
