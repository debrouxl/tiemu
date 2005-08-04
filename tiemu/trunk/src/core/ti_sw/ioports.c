/* Hey EMACS -*- linux-c -*- */
/* $Id: romcalls.c 1583 2005-07-18 14:38:36Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
    IO ports definitions loader/parser.
*/

#include <assert.h>
#include <stdio.h>

#include "ti68k_def.h"
#include "ioports.h"

static IOPORT **table;
static int		size;

const char* ioports_get_filename()
{
	tihw.calc_type = TI92;
	switch(tihw.calc_type)
	{
	case TI89:  return "ioports_89.txt";
	case TI89t: return "ioports_89t.txt";
	case TI92:  return "ioports_92.txt";
	case TI92p: return "ioports_92p.txt";
	case V200:  return "ioports_v200.txt";
	}

	return "";
}

// convert "ro", "wo", "rw" into value
static int get_type(const char* s)
{
	while(*s == ' ') s++;

	if(s[0] == 'r' && s[1] == 'o')
		return IO_RO;
	else if(s[0] == 'w' && s[1] == 'o')
		return IO_WO;
	else if(s[0] == 'r' && s[1] == 'w')
		return IO_RW;

	return 0;
}

// parse <..5.....> entry
static int get_bits(const char *s, int size, int *bits)
{
	char *b, *e;
	int i, nbits = 8 * size;
	int all;

	while(*s == ' ') s++;

	b = strchr(s, '<');
	e = strrchr(s, '>');

	if(!b ||!e)
	{
		fprintf(stdout, "Missing '<' or '>' token !\n");
		return -1;
	}

	b++;
	e--;
	all = !strncmp(b, "all", 3);

	if((e - b + 1) != nbits && !all)
	{
		fprintf(stdout, "Number of bits does not match size !\n");
		return -1;
	}

	for(i = 0; i < nbits; i++)
	{
		if(b[i] == '.')
			bits[nbits - i] = 0;
		else if(isdigit(b[i]) || all)
			bits[nbits-1 - i] = 1;
		else
		{
			fprintf(stdout, "Wrong character in bit sequence (digit or '.') !\n");
			return -1;
		}
	}

	return 0;
}

/*
	Load information on I/O ports.
	Return 0 if success, -1 otherwise.

	File naming scheme : "ioports_model.txt" => ioports_89.txt
*/
int ioports_load(const char* path)
{
	FILE *f;
	gchar *filename;
	int i, n ,nlines;
	char line[1024];
	
	filename = g_strconcat(path, ioports_get_filename(), NULL);
	f = fopen(filename, "rb");
	if(f == NULL)
	{
		g_free(filename);
		return -1;
	}

	for(nlines = 0; !feof(f); nlines++)
		fgets(line, sizeof(line), f);
	rewind(f);

	table = (IOPORT **)calloc(nlines + 1, sizeof(IOPORT *));

	for(n = 0, i = 0; n < nlines; n++)
	{
		gchar **split;
		IOPORT *s;

		fgets(line, sizeof(line), f);
		if(line[0] == ';')
			continue;
		if(line[0] != '$')
			continue;

		split = g_strsplit(line, "|", 5);
		if(!split[0] || !split[1] || !split[2] || !split[3] || !split[4] )
		{
			fprintf(stderr, "Error at line %i\n", n);
			return -1;
		}

		s = table[i] = (IOPORT*)calloc(1, sizeof(IOPORT));

		sscanf(split[0], "$%06x", &s->addr);
		sscanf(split[1], "%i", &s->size);
		s->type = get_type(split[2]);
		if(get_bits(split[3], s->size, s->bits) == -1)
			return -1;
		s->name = strdup(split[4]);

		i++;
	}	

	g_free(filename);
	fclose(f);

    return 0;
}

int ioports_unload(void)
{
	IOPORT** ptr;

	for(ptr = table; *ptr != NULL; ptr++)
	{
		free((*ptr)->name);
		free(*ptr);
	}
	free(table);

	return 0;
}

int ioports_get_size(void)
{
	return size;
}

IOPORT* ioports_get_infos(int idx)
{
	assert(idx < 0 || idx >= size);
	return table[idx];
}
