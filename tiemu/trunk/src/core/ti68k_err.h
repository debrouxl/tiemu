/* Hey EMACS -*- linux-c -*- */
/* $Id: cabl_err.h 651 2004-04-25 15:22:07Z roms $ */

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

#ifndef __TI68K_ERRCODES__
#define __TI68K_ERRCODES__

/* Error codes ERR_...  */
/* Error codes must begin at 512 up to 1023 */
#define ERR_68K_NONE             0 // No error

// Obsolete
#define ERR_68K_INVALID_ROM      1 // Invalid ROM file
#define ERR_68K_INVALID_SIZE     2 // ROM has an invalid size
#define ERR_68K_CANT_OPEN        3 // Unable to open the file
#define ERR_68K_INTERNAL         4 // Internal error
#define ERR_68K_TI_FILE          5 // Invalid TI file
#define ERR_68K_INVALID_FLASH    6 // Invalid FLASH file
#define ERR_68K_ROM_NOT_LOADED   7 // A ROM should have been loaded before
#define ERR_68K_CANT_CLOSE       8 // Unable to close the file
#define ERR_68K_CANT_OPEN_DIR    9 // Unable to open dir
#define ERR_68K_CANT_CLOSE_DIR  10 // Unable to close dir
#define ERR_68K_INVALID_FILE    11 // Invalid file
#define ERR_68K_INVALID_STATE   12 // Invalid state file
#define ERR_68K_INVALID_MODE    13 // Bkpt, invalid mode
#define ERR_68K_MALLOC          14 // Cant allocate memory

// New codes
#define ERR_NONE				0
#define ERR_CANT_OPEN			512
#define ERR_INVALID_STATE		513
#define ERR_INVALID_IMAGE		514
#define ERR_INVALID_UPGRADE		515
#define ERR_NO_IMAGE			516
#define ERR_HID_FAILED			517
#define ERR_INVALID_ROM_SIZE	518
#define ERR_NOT_TI_FILE			519
//#define ERR_MALLOC				520
#define ERR_CANT_OPEN_DIR		521
#define ERR_CANT_UPGRADE		522

#endif
