/*  GtkTiEmu - a TI emulator
 *  Originally written by Jonas Minsberg
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
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

#ifndef __SCREENSHOT__
#define __SCREENSHOT__


/*******************************/
/* Constants for do_screenshot */
/*******************************/

#define IMG_XPM 1
#define IMG_PCX 2
#define IMG_JPG 3
#define IMG_PNG 4
#define IMG_BMP 5

#define IMG_TYPE 64
#define IMG_COL  (IMG_TYPE+1)
#define IMG_BW   (IMG_TYPE+2)

#define IMG_SIZE 128
#define IMG_LCD  (IMG_SIZE+1)
#define IMG_SKIN (IMG_SIZE+2)

#endif





