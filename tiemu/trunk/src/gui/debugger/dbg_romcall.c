/* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_code.c 731 2004-11-28 16:05:10Z roms $ */

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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "intl.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "romcalls.h"
#include "dbg_code.h"
#include "dbg_mem.h"

enum {
		COL_ID, COL_NAME, COL_ADDR, COL_FULL
};
#define CLIST_NVCOLS	(4)		// 3 visible columns
#define CLIST_NCOLS		(4)		// 3 real columns

#define FONT_NAME	"courier"


GLADE_CB gboolean    
on_combo_entry1_match_selected             (GtkEntryCompletion *widget,
                                            GtkTreeModel *model,
                                            GtkTreeIter *iter,
                                            gpointer user_data);

void dbgromcall_create_window(GtkWidget *widget)
{
	GtkListStore *store;
	GtkTreeModel *model;

	GtkEntry *entry = GTK_ENTRY(GTK_BIN(GTK_COMBO_BOX(widget))->child);
	GtkEntryCompletion* completion;

	// create storage
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);

	// and set storage
	gtk_combo_box_set_model(GTK_COMBO_BOX(widget), model);
	gtk_combo_box_entry_set_text_column(GTK_COMBO_BOX_ENTRY(widget), COL_FULL);

	/* --- */

	// set auto-completion
	completion = gtk_entry_completion_new();
	gtk_entry_set_completion(entry, completion);
	gtk_entry_completion_set_model(completion, model);
	gtk_entry_completion_set_text_column (completion, COL_FULL);
	g_signal_connect(G_OBJECT(completion), "match-selected", 
		G_CALLBACK(on_combo_entry1_match_selected), NULL);
	//gtk_editable_select_region(GTK_EDITABLE(entry), 0, -1);
}

void dbgromcall_refresh_window(GtkWidget *widget)
{
	GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
	GtkListStore *store = GTK_LIST_STORE(model);
	GList *lst, *ptr;
	gchar *path;
	gint result;

	// (re)load symbols
	path = g_strconcat(inst_paths.base_dir, "romcalls.txt", NULL);
	result = ti68k_debug_load_symbols(path);
	g_free(path);

	if(result == -1)
		return;
	else if(result == -2)
		gtk_list_store_clear(store);

	// fill storage
	gtk_list_store_clear(store);
	lst = romcalls_sort_by_iname();
	if(lst == NULL)	return;

	for(ptr = lst; ptr != NULL; ptr = g_list_next(ptr))
	{
		uint32_t addr = ROMCALL_ADDR(ptr);
		const gchar *name = ROMCALL_NAME(ptr);
		int id = ROMCALL_ID(ptr);
		gchar** row_text = g_malloc0((CLIST_NVCOLS + 1) * sizeof(gchar *));
		GtkTreeIter iter;

		if(!strcmp(name, "unknown") || (name == NULL))
			continue;

		row_text[0] = g_strdup_printf("#%03x", id);
		row_text[1] = g_strdup(name);
		row_text[2] = g_strdup_printf("[$%x]", addr);
		row_text[3] = g_strdup_printf("%s [$%x] - #%03x", name, addr, id);

		gtk_list_store_append(store, &iter);
	    gtk_list_store_set(store, &iter, 
	    COL_ID, row_text[0], 
		COL_NAME, row_text[1],
        COL_ADDR, row_text[2],
		COL_FULL, row_text[3],
		-1);

		g_strfreev(row_text);
	}
}

void dbgromcall_erase_window(GtkWidget *widget)
{
	GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
	GtkListStore *store = GTK_LIST_STORE(model);
	
	gtk_list_store_clear(store);
}

static void goto_romcall(const char *str)
{
	gchar *ptr;
	uint32_t addr;
	int id;

	ptr = strchr(str, '[');
	if(ptr != NULL)
	{
		int ret = sscanf(ptr, "[$%x] - #%03x ", &addr, &id);
		if(ret == 2)
		{
			if(addr < 0x200000)
				dbgmem_add_tab(addr & 0xffffff);
			else
				dbgcode_disasm_at(addr & 0xffffff);
		}
	}
}

GLADE_CB void
on_combo_entry1_changed                    (GtkComboBox *combobox,
                                            gpointer user_data)
{
	GtkEntry *entry = GTK_ENTRY(GTK_BIN(combobox)->child);

	gchar *str = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
	goto_romcall(str);
	g_free(str);
}

GLADE_CB gboolean    
on_combo_entry1_match_selected             (GtkEntryCompletion *completion,
                                            GtkTreeModel *model,
                                            GtkTreeIter *iter,
                                            gpointer user_data)
{
	gchar *str;

	gtk_tree_model_get(model, iter, COL_FULL, &str, -1);
	goto_romcall(str);
	g_free(str);
	return FALSE;
}
