#ifndef EXTERN_H
#define EXTERN_H

#include <gtk/gtk.h>

#include "struct.h"

/* Some linked-list for breakpoints */
extern GList *bkpt_address_list;
extern GList *bkpt_access_list;
extern GList *bkpt_vector_list;
extern GList *bkpt_trap_list;

/* Used widget */
extern GtkWidget *debugger_dbox;
extern GtkWidget *code_clist;
extern GtkWidget *reg_text;
extern GtkWidget *data_bkpt_dbox;

extern gint selected_row;
extern gint data_bkpt_selected_row;
extern gint bkpt_encountered;

#endif
