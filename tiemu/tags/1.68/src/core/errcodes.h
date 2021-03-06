/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001, Romain Lievin
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


/* Error codes: ERR_...  */
#define ERR_NONE             0 // No error
#define ERR_INVALID_ROM      1 // Invalid ROM file
#define ERR_INVALID_SIZE     2 // ROM has an invalid size
#define ERR_CANT_OPEN        3 // Unable to open the file
#define ERR_INTERNAL         4 // Internal error
#define ERR_TI_FILE          5 // Invalid TI file
#define ERR_INVALID_FLASH    6 // Invalid FLASH file
#define ERR_ROM_NOT_LOADED   7 // A ROM should have been loaded before
#define ERR_CANT_CLOSE       8 // Unable to close the file
#define ERR_CANT_OPEN_DIR    9 // Unable to open dir
#define ERR_CANT_CLOSE_DIR  10 // Unable to close dir
#define ERR_INVALID_FILE    11 // Invalid file
#define ERR_INVALID_STATE   12 // Invalid state file
#define ERR_INVALID_MODE    13 // Bkpt, invalid mode
#define ERR_MALLOC          14 // Cant allocate memory

#endif



