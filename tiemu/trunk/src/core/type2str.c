/* Hey EMACS -*- linux-c -*- */
/* $Id: type2str.c 689 2004-05-09 20:29:44Z roms $ */

/*  libticables - Ti Link Cable library, a part of the TiLP project
 *  Copyright (C) 1999-2004  Romain Lievin
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

#include "intl.h"
#include "consts.h"

const char *ti68k_calctype_to_string(int type)
{
	switch(type)
	{
		case TI89:  return "TI89";
		case TI92:  return "TI92";
		case TI92p: return "TI92+";
		case V200:  return "V200PLT";
		default:    return "none";
	}
}

int ti68k_string_to_calctype(const char *str)
{
	if(!strcmp(str, "TI89"))
		return TI89;
	else if(!strcmp(str, "TI92"))
		return TI92;
	else if(!strcmp(str, "TI92+"))
		return TI92p;
	else if(!strcmp(str, "V200PLT"))
		return V200;

	return 0;
}

const char *ti68k_romtype_to_string(int type)
{
	switch(type)
	{
		case 0:						return "external EPROM";
		case INTERNAL:				return "internal EPROM";
		case FLASH_ROM:				return "external FLASH";
		case FLASH_ROM | INTERNAL: 	return "internal FLASH";
	}
		
	return 0;
}

int         ti68k_string_to_romtype(const char *str)
{
	if(!strcmp(str, "external EPROM"))
		return 0;
	else if(!strcmp(str, "internal EPROM"))
		return INTERNAL;
	else if(!strcmp(str, "external FLASH"))
		return FLASH_ROM;
	else if(!strcmp(str, "internal FLASH"))
		return FLASH_ROM | INTERNAL;
		
	return 0;
}
