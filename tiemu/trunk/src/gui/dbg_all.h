#ifndef __DBG_ALL_H__
#define __DBG_ALL_H__

void preload_gtk_debugger(void);
int enter_gtk_debugger(int context);

typedef struct {
        GtkWidget *mem;
        GtkWidget *regs;
        GtkWidget *bkpts;
        GtkWidget *code;
        GtkWidget *pclog;
} DbgWidgets;

#endif
