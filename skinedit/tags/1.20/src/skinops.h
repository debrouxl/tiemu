/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __SKINOPS_H__
#define __SKINOPS_H__

#include <stdlib.h>


int
load_skin(char *path);

int
load_skin_old_vti(FILE *fp);

int
load_skin_vti(FILE *fp);

int
load_skin_tiemu(FILE *fp);

int
load_jpeg(FILE *fp);

int
write_skin(void);

int
write_skin_as(char *dest);

void
write_header(FILE *fp);

unsigned char *
read_jpeg(FILE *fp, unsigned int *length);


#endif /* !__SKINOPS_H__ */
