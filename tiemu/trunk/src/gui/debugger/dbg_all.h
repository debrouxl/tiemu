#ifndef __DBG_ALL_H__
#define __DBG_ALL_H__

#include "dbg_bkpts.h"
#include "dbg_code.h"
#include "dbg_cause.h"
#include "dbg_data.h"
#include "dbg_mem.h"
#include "dbg_regs.h"
#include "dbg_vectors.h"
#include "dbg_pclog.h"
#include "dbg_stack.h"
#include "dbg_heap.h"
#include "dbg_entry.h"

typedef struct {
    GtkWidget *mem;
    GtkWidget *regs;
    GtkWidget *bkpts;
    GtkWidget *code;
    GtkWidget *pclog;
    GtkWidget *stack;
	GtkWidget *heap;
} DbgWidgets;
extern DbgWidgets dbgw;

typedef struct {
    gint    mem;
    gint    regs;
    gint    bkpts;
    gint    code;
    gint    pclog;
    gint    stack;
	gint	heap;
} DbgStates;
extern DbgStates dbgs;

void gtk_debugger_preload(void);
int  gtk_debugger_enter(int context);
void gtk_debugger_refresh(void);

void set_other_windows_sensitivity(int state);

void gtk_debugger_minimize_all(int all);
void gtk_debugger_unminimize_all(int all);

void gtk_debugger_show_all(int all);
void gtk_debugger_hide_all(int all);

extern GtkWidget *main_wnd;

#endif
