/* Hey EMACS -*- linux-c -*- */
/* $Id: interface.c 234 2004-05-21 15:49:39Z roms $ */

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
    Type conversion routines
*/

#include <stdio.h>
#include <string.h>

#include "intl.h"
#include "ti68k_def.h"
#include "bkpts.h"

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
		case 0:						return "external-EPROM";
		case INTERNAL:				return "internal-EPROM";
		case FLASH_ROM:				return "external-FLASH";
		case FLASH_ROM | INTERNAL: 	return "internal-FLASH";
	}
		
	return 0;
}

int         ti68k_string_to_romtype(const char *str)
{
	if(!strcmp(str, "external-EPROM"))
		return 0;
	else if(!strcmp(str, "internal-EPROM"))
		return INTERNAL;
	else if(!strcmp(str, "external-FLASH"))
		return FLASH_ROM;
	else if(!strcmp(str, "internal-FLASH"))
		return FLASH_ROM | INTERNAL;
		
	return 0;
}

const char *ti68k_hwtype_to_string(int type)
{
	switch(type)
	{
		case HW1:  return "hw1";
		case HW2:  return "hw2";
		default:   return "none";
	}
}

int ti68k_string_to_hwtype(const char *str)
{
	if(!strcmp(str, "hw1"))
		return HW1;
	else if(!strcmp(str, "hw2"))
		return HW2;

	return 0;
}

const char *ti68k_exception_to_string(int number)
{
	switch(number)
	{
		case 0: return _("Initial SSP");
		case 1: return _("Initial PC");
		case 2: return _("Bus error vector");
		case 3: return _("Address error vector");
		case 4: return _("Illegal instruction vector");
		case 5: return _("Zero divide vector");
		case 6: return _("CHK instruction vector");
		case 7: return _("TRAPV instruction vector");
		case 8: return _("Privilege violation vector");
		case 9: return _("Trace vector");
		case 10: return _("Line 1010 emulator vectors");
		case 11: return _("Line 1111 emulator vectors");
		case 12: return _("Unassigned, reserved");
		case 13: return _("Unassigned, reserved");
		case 14: return _("Unassigned, reserved");
		case 15: return _("Uninitialised interrupt vector");
		case 16: return _("Unassigned, reserved");
		case 17: return _("Unassigned, reserved");
		case 18: return _("Unassigned, reserved");
		case 19: return _("Unassigned, reserved");
		case 20: return _("Unassigned, reserved");
		case 21: return _("Unassigned, reserved");
		case 22: return _("Unassigned, reserved");
		case 23: return _("Unassigned, reserved");
		case 24: return _("Spurious interrupt vector");
		case 25: return _("Level 1 interrupt auto-vectors");
		case 26: return _("Level 2 interrupt auto-vectors");
		case 27: return _("Level 3 interrupt auto-vectors");
		case 28: return _("Level 4 interrupt auto-vectors");
		case 29: return _("Level 5 interrupt auto-vectors");
		case 30: return _("Level 6 interrupt auto-vectors");
		case 31: return _("Level 7 interrupt auto-vectors");
		case 32: return _("TRAP #0 instruction vectors");
		case 33: return _("TRAP #1 instruction vectors");
		case 34: return _("TRAP #2 instruction vectors");
		case 35: return _("TRAP #3 instruction vectors");
		case 36: return _("TRAP #4 instruction vectors");
		case 37: return _("TRAP #5 instruction vectors");
		case 38: return _("TRAP #6 instruction vectors");
		case 39: return _("TRAP #7 instruction vectors");
		case 40: return _("TRAP #8 instruction vectors");
		case 41: return _("TRAP #8 instruction vectors");
		case 42: return _("TRAP #8 instruction vectors");
		case 43: return _("TRAP #8 instruction vectors");
		case 44: return _("TRAP #8 instruction vectors");
		case 45: return _("TRAP #8 instruction vectors");
		case 46: return _("TRAP #8 instruction vectors");
		case 47: return _("TRAP #15 instruction vectors");	
		case 48: 
		case 49: 
		case 50: 
		case 51: 
		case 52: 
		case 53: 
		case 54: 
		case 55: 
		case 56: 
		case 57: 
		case 58: 
		case 59: 
		case 60: 
		case 61: 
		case 62: 
		case 63: return _("Unassigned, reserved");
		case 64: return _("User interrupt vectors");
		
		default: return _("User interrupt vectors");
	}
}

const char *ti68k_bkpt_cause_to_string(int type)
{
	switch(type)
	{
	case BK_CAUSE_ACCESS:	return "access";
	case BK_CAUSE_RANGE:	return "access range";
	case BK_CAUSE_ADDRESS:	return "address";
    case BK_CAUSE_EXCEPTION:return "exception";
	default:				return "unknown (bug)";
	}
}


const char *ti68k_bkpt_type_to_string(int type)
{
	switch(type)
	{
    case BK_TYPE_ACCESS:    return _("access");
    case BK_TYPE_RANGE:     return _("range");
    case BK_TYPE_CODE:      return _("code");
    case BK_TYPE_EXCEPTION: return _("exception");
	default:                return _("unknown");
	}
}

int ti68k_string_to_bkpt_type(const char *str)
{
	if(!strcmp(str, _("access")))
		return BK_TYPE_ACCESS;
	else if(!strcmp(str, _("range")))
		return BK_TYPE_RANGE;
	else if(!strcmp(str, _("code")))
		return BK_TYPE_CODE;
	else if(!strcmp(str, _("exception")))
		return BK_TYPE_EXCEPTION;

	return 0;
}

const char *ti68k_bkpt_mode_to_string(int type, int mode)
{
	switch(type)
	{
	case BK_CAUSE_ACCESS:
		switch(mode)
		{
		case BK_READ_BYTE: return "byte-read";
		case BK_READ_WORD: return "word-read";
		case BK_READ_LONG: return "long-read";
		case BK_WRITE_BYTE:return "byte-write";
		case BK_WRITE_WORD:return "word-write";
		case BK_WRITE_LONG:return "long-write";
		default: return "unknwon (bug)";
		}
		break;
	case BK_CAUSE_RANGE:
        if((mode & BK_MODE_READ) && (mode & BK_MODE_WRITE))
            return "any";
		else if(mode & BK_MODE_READ)
            return "read";
        else if(mode & BK_MODE_WRITE)
            return "write";
		break;
	case BK_CAUSE_ADDRESS:
		return "n/a";
		break;
	}

    return "unknown (bug)";
}

int ti68k_string_to_bkpt_mode(const char * str)
{
    if(!strcmp(str, "any"))
        return BK_MODE_READ | BK_MODE_WRITE;
    else if(!strcmp(str, "read"))
        return BK_MODE_READ;
    else if(!strcmp(str, "write"))
        return BK_MODE_WRITE;
    else if(!strcmp(str, "byte-read"))
        return BK_READ_BYTE;
    else if(!strcmp(str, "byte-word"))
        return BK_READ_WORD;
    else if(!strcmp(str, "byte-long"))
        return BK_READ_LONG;
    else if(!strcmp(str, "byte-write"))
        return BK_WRITE_BYTE;
    else if(!strcmp(str, "word-write"))
        return BK_WRITE_WORD;
    else if(!strcmp(str, "long-write"))
        return BK_WRITE_LONG;
    else
        return 0;
}
