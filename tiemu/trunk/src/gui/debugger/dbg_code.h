#include <gtk/gtk.h>

GtkWidget* dbgcode_create_window(void);
GtkWidget* dbgcode_display_window(void);
void dbgcode_refresh_window(void);

void dbgcode_disasm_at(uint32_t addr);