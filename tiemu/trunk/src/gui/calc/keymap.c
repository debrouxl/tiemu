/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - Tiemu Is an EMUlator
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "keydefs.h"
#include "ti68k_int.h"
#include "keymap.h"
#include "keynames.h"
#include "logging.h"

Pc2TiKey **keymap = NULL;
static int nkeys = 0;

static const char *kbd_layout(void)
{
	const gchar* const *array = g_get_language_names ();
	gchar **ptr;
	
	for(ptr = (gchar **)array; *ptr; ptr++)
	{
		if(strlen(*ptr) == 2)
			return *ptr;
	}

	return "en";	// English layout as default
}

// return calc type or -1
int keymap_read_header(const char *filename)
{
    FILE *f;
    char line[256] = { 0 };

    // open file
    f = fopen(filename, "rt");
    if(f == NULL)
    {
        fprintf(stderr, "unable to open keymap file <%s>.\n", filename);
        return -1;
    }

    // read first line
    if (!fgets(line, sizeof(line), f))
    {
        fprintf(stderr, "No calc model found !\n");
        fclose(f);
        return -1;
    }
    line[strlen(line) - 1] = '\0';
    if(strncmp(line, "Model", strlen("Model")))
    {
        fprintf(stderr, "No calc model found !\n");
        fclose(f);
        return -1;
    }

    // close file
    fclose(f);
	nkeys = 0;

    // and convert
    return ti68k_string_to_calctype(line + 7);
}

int keymap_load(const char *filename)
{
    FILE *f;
    char line[256];
    int pckey, tikey, mod;
    int idx = 0;

    // open file
    f = fopen(filename, "rt");
    if(f == NULL)
    {
        fprintf(stderr, "unable to open keymap file <%s>.\n", filename);
        return -1;
    }

    if (!fgets(line, sizeof(line), f))
    {
        fprintf(stderr, "No calc model found !\n");
        fclose(f);
        return -1;
    }
    line[strlen(line) - 1] = '\0';
    if(strncmp(line, "Model", strlen("Model")))
    {
        fprintf(stderr, "No calc model found !\n");
        fclose(f);
        return -1;
    }

    // parse file
    while(!feof(f))
    {
        gchar **split = NULL;
		gchar **split1 = NULL;	// PC key list
        gchar **split2 = NULL; // TI key list
		char *p;
		gchar **ptr;

        // remove cr/lf
        if (!fgets(line, sizeof(line), f))
            break;
        //line[strlen(line) - 2] = '\0';
	
		p = strrchr(line, '\r');
		if(p != NULL) *p = '\0';
		p = strrchr(line, '\n');
		if(p != NULL) *p = '\0';

        // skip comments
        if((line[0] == '#') || (line[0] == '/'))
            continue;

        // split string
        split = g_strsplit(line, ":", 2);
        if((split[0] == NULL) || (split[1] == NULL))
        {
            g_strfreev(split);
            continue;
        }

		// split PC sub-string
		split1 = g_strsplit(split[0], ",", -1);
		if(split1[0] == NULL)
        {
			g_strfreev(split);
            g_strfreev(split1);
            continue;
        }

        // split TI sub-string
        split2 = g_strsplit(split[1], ",", 2);
        if(split2[0] == NULL)
        {
			g_strfreev(split);
			g_strfreev(split1);
            g_strfreev(split2);
            continue;
        }

        // convert TI key name into value
        tikey = tikey_string_to_value(split2[0]);
        if(tikey == -1)
        {
            fprintf(stderr, "Warning: non-existent TI key: <%s>\n", split[1]);
            g_strfreev(split);
			g_strfreev(split1);
            g_strfreev(split2);
            continue;
        }

		// and modifier
        if(split2[1] == NULL)
            mod = -1;
        else
            mod = tikey_string_to_value(split2[1]);            

		// add an entry for each PC key
		for(ptr = split1; *ptr; ptr++)
		{
			Pc2TiKey *key;
			gchar* tok;
			gchar* lang = NULL;

			// check whether key is localized
			if(tok = strchr(*ptr, '/'))
			{
				lang = g_strdup(tok+1);
				*tok = '\0';
			}

			if(lang && strcmp(lang, kbd_layout()))
			{
				g_free(lang);
				continue;
			}
			g_free(lang);

			// convert key names into values
			pckey = pckey_string_to_value(*ptr);
			if(pckey == -1)
			{
				fprintf(stderr, "Warning: non-existent PC key: <%s>\n", split[0]);
				continue;
			}

			// and store value
			keymap = g_realloc(keymap, (idx+1) * sizeof(Pc2TiKey *));
			key = g_malloc0(sizeof(Pc2TiKey));

			key->pc_key = pckey;
			key->ti_key = tikey;
			key->modifier = mod;
			keymap[idx++] = key;
		}

        // free mem
        g_strfreev(split);
		g_strfreev(split1);
        g_strfreev(split2);
    }

    // close file
    tiemu_info(_("loading keymap: %s (%i keys)"), g_basename(filename), idx);
    fclose(f);
	nkeys = idx;

    return 0;
}

int keymap_unload(void)
{
	int i;

	for(i = 0; i < nkeys; i++)
		g_free(keymap[i]);
	g_free(keymap);

	return 0;
}

int keymap_num_keys(void)
{
	return nkeys;
}