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

static ROM_CALL list[2048];
static int loaded;

int romcalls_load_from_file(const char* filename)
{
    FILE *f;
    int n;
    char buf[256];
    char *name, *p;
    int number;

    if(loaded)
        return 0;

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
        if(p != NULL)
            *p = '\0';

        // get function number
        n = sscanf(buf, "%x", &number);
        if(n < 1)
        {
            free(name);
            continue;
        }

        printf("tios::%s (0x%03x)\n", name, number);

        // get function address
    }

    fclose(f);
    loaded = !0;

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