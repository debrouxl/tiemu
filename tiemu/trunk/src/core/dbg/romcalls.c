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
#include <errno.h>

#include "romcalls.h"
#include "images.h"
#include "ti68k_def.h"

static ROM_CALL list[NMAX_ROMCALLS] = { 0 };
static int		list_size = 0;
static int		loaded;

static int old_ct = -1;		// previous calc type for reloading

static uint32_t rd_long(uint8_t *p)
{
	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

/* 
	Loading from file
*/

static void load_tigcc_file_type(FILE *f)    // os.h TIGCC file: ".set acos, 0xF5"
{
    char buf[256];
    char *name, *p;
    int n;
    int number;

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
		list[number].id = number;
    }
}

static void load_lionel_file_type(FILE *f)   // Lionel Debroux formatted file: 2E:ScrToHome
{
    char str[256];
    int number;

    while(!feof(f))
    {
        gchar **array;

        // get line
        fgets(str, sizeof(str), f);
		if(feof(f))
			break;
		for (number = strlen(str) - 1; str[number] == '\n' || str[number] == '\r'; number--) str[number] = '\0';

        if(!strchr(str, ':'))
            continue;

        // split
        array = g_strsplit(str, ":", 2);
        if(!array[0] || !array[1])
        {
            g_strfreev(array);
            continue;
        }

        // get values and store
        sscanf(array[0], "%x", &number);
		list[number].name = strdup(array[1]);
		list[number].id = number;

        g_strfreev(array);
    }
}

int romcalls_load_from_file(const char* filename)
{
	IMG_INFO *img = &img_infos;
    FILE *f;
	uint32_t addr;
	int i;
    char tmp[32];

	if(!img_loaded || (img->calc_type == TI92))
		return -1;

	if((old_ct == img->calc_type) && loaded)
		return 0;
	else
		old_ct = img->calc_type;

	printf("Loading symbols (ROM calls)... ");
    memset(list, 0, sizeof(list));
	list_size = 0;

    f = fopen(filename, "rt");
    if(f == NULL) {
			printf("Failed to open <%s> with error %s (%d)\n", 
			       filename, strerror(errno), errno);
        return -1;
		}

    fgets(tmp, sizeof(tmp), f);
    fgets(tmp, sizeof(tmp), f);
    if(!strncmp(tmp, ".set", strlen(".set")))
    {
        rewind(f);
        load_tigcc_file_type(f);
    }
    else
    {
        rewind(f);
        load_lionel_file_type(f);
    }

    fclose(f);
    loaded = !0;
	printf("Done !\n");

	// get function address
	//addr = rd_long(&tihw.ram[0xC8]);
	addr = rd_long(&tihw.rom[0x12000 + 0x88 + 0xC8]);
	list_size = rd_long(&tihw.rom[((addr-4) & 0x0fffff)]);

	printf("ROM calls: parsing %i entries at $%06x... ", list_size, addr);

	for(i = 0; i < list_size; i++)
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
	Parsing of ROM call address from FLASH (list is supposed to be sorted by id !)
*/

// cache last search
static int last_id = 0;	

// returns id or -1
int romcalls_is_address(uint32_t addr)
{
	int i;

	if(!loaded)	return -1;

	for(i = 0; i < list_size; i++)
	{
		if(addr == list[i].addr)
			return last_id = list[i].id;
	}

	return -1;
}

// returns id or -1
int romcalls_is_name(const char *name)
{
	int i;

	if(!loaded)	return -1;

	for(i = 0; i < list_size; i++)
	{
		if(!strcmp(name, list[i].name))
			return list[i].id;
	}

	return -1;
}

int romcalls_get_id(int i)
{
	return list[i].id;
}

const char* romcalls_get_name(int i)
{
	return list[i].name;
}

uint32_t romcalls_get_addr(int i)
{
	return list[i].addr;
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

/*
	Sorting functions
*/

static ROM_CALL* duplicate_list(ROM_CALL *src)
{
	ROM_CALL *dst = g_memdup(src, list_size * sizeof(ROM_CALL));

	return dst;
}

// negative value if a < b; zero if a = b; positive value if a > b
static compare_func_by_id(gconstpointer a, gconstpointer b, gpointer user_data)
{
	ROM_CALL *aa = (ROM_CALL *)a;
	ROM_CALL *bb = (ROM_CALL *)b;

	if(aa->id == bb->id)
		return 0;
	else if(aa->id > bb->id)
		return 1;
	else return -1;
}

ROM_CALL *romcalls_sort_by_id(void)
{
	ROM_CALL *ret = duplicate_list(list);
	g_qsort_with_data(ret, list_size, sizeof(ROM_CALL), compare_func_by_id, NULL);
	
	return ret;
}

// negative value if a < b; zero if a = b; positive value if a > b
static compare_func_by_addr(gconstpointer a, gconstpointer b, gpointer user_data)
{
	ROM_CALL *aa = (ROM_CALL *)a;
	ROM_CALL *bb = (ROM_CALL *)b;

	if(aa->addr == bb->addr)
		return 0;
	else if(aa->addr > bb->addr)
		return 1;
	else return -1;
}

ROM_CALL *romcalls_sort_by_addr(void)
{
	ROM_CALL *ret = duplicate_list(list);
	g_qsort_with_data(ret, list_size, sizeof(ROM_CALL), compare_func_by_addr, NULL);
	
	return ret;
}

// negative value if a < b; zero if a = b; positive value if a > b
static compare_func_by_name(gconstpointer a, gconstpointer b, gpointer user_data)
{
	ROM_CALL *aa = (ROM_CALL *)a;
	ROM_CALL *bb = (ROM_CALL *)b;

	return strcmp(aa->name, bb->name);
}

ROM_CALL *romcalls_sort_by_name(void)
{
	ROM_CALL *ret = duplicate_list(list);
	g_qsort_with_data(ret, list_size, sizeof(ROM_CALL), compare_func_by_name, NULL);
	
	return ret;
}

int romcalls_get_size(void)
{
	return list_size;
}