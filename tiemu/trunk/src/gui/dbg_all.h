#ifndef __DBG_ALL_H__
#define __DBG_ALL_H__

int enter_gtk_debugger(int context);

typedef struct {
        GtkWidget *mem;
        GtkWidget *regs;
        GtkWidget *bkpts;
        GtkWidget *code;
        GtkWidget *pclog;
} DbgWidgets;

#endif
