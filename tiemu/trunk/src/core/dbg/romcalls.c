/* Hey EMACS -*- linux-c -*- */
/* $Id: bkpts.c 517 2004-07-07 15:06:40Z roms $ */

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
    Symboles (ROM calls address and names)
*/

#include <stdio.h>
#include <string.h>

#include "romcalls.h"
#include "images.h"
#include "ti68k_def.h"

#define TBL_SIZE	0x800

static ROM_CALL list[TBL_SIZE];
static int		loaded;

static int old_ct = -1;		// previous calc type for reloading

static uint32_t rd_long(uint8_t *p)
{
	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

int romcalls_load_from_file(const char* filename)
{
	IMG_INFO *img = &img_infos;
    FILE *f;
    int n;
    char buf[256];
    char *name, *p;
    int number;
	uint32_t addr;
	int i;

    if(loaded)
        return 0;

	if(!img_loaded || (img->calc_type == TI92))
		return -1;

	if(old_ct == -1)
		old_ct = img->calc_type;
	else if(old_ct == img->calc_type)
		return 0;

	printf("Loading symbols (ROM calls)... ");
    memset(list, 0, sizeof(list));

    f = fopen(filename, "rt");
    if(f == NULL)
        return -1;

    while(!feof(f))
    {
        fgets(buf, sizeof(buf), f);

        // no '.set' ?
        if(*buf != '.')
            continue;

        // get function name
        name = strdup(buf+5);
        p = strchr(name, ',');
        if(p == NULL)
			continue;
        else
			*p++ = '\0';

        // get function number
        n = sscanf(p, "%x", &number);
        if((n < 1) || (number > 0x7ff))
        {
            free(name);
            continue;
        }

		// and store
		list[number].name = name;
    }

    fclose(f);
    loaded = !0;
	printf("Done !\n");

	// get function address
	addr = rd_long(&tihw.ram[0xC8]);
	printf("Parsing symbols (addresses) at $%06x... ", addr);

	for(i = 0; i < TBL_SIZE; i++)
	{
		if(list[i].name == NULL)
			list[i].name = strdup("unknown");

		list[i].addr = rd_long(&tihw.rom[(addr & 0x0fffff) + (i << 2)]); 
	}

	printf("Done !\n");

    return 0;
}

int romcalls_unload(void)
{
    loaded = 0;

    return 0;
}

int romcalls_is_loaded(void)
{
    return loaded;
}

  /*
memset(romFuncAddr,0,sizeof(int)*0x800);
    for (int i=0;(romFuncs[i].name)&&(i<(getmem_dword(getmem_dword(0xc8)-4))-1);i++)
        romFuncAddr[i]=getmem_dword(getmem_dword(0xc8)+(i<<2));
        */

// cache last search
static int last_id = 0;	

// returns id or -1
int romcalls_is_address(uint32_t addr)
{
	int i;

	if(!loaded)	return -1;

	for(i = 0; i < TBL_SIZE; i++)
	{
		if(addr == list[i].addr)
			return last_id = i;
	}

	return -1;
}

// returns id or -1
int romcalls_is_name(const char *name)
{
	int i;

	if(!loaded)	return -1;

	for(i = 0; i < TBL_SIZE; i++)
	{
		if(!strcmp(name, list[i].name))
			return i;
	}

	return -1;
}

const char* romcalls_get_name(int id)
{
	return list[id].name;
}

uint32_t romcalls_get_addr(int id)
{
	return list[id].addr;
}

const char* romcalls_get_addr_name(uint32_t addr)
{
	int id;

	if(!loaded)	return NULL;

	if(addr == list[last_id].addr)
		return list[last_id].name;

	id = romcalls_is_address(addr);
	if(id == -1)
		return NULL;
	else
		return romcalls_get_name(id);
	
	return NULL;
}