#include <gtk/gtk.h>


GtkWidget* dbgmem_create_window(void);
GtkWidget* dbgmem_display_window(void);
void dbgmem_refresh_window(void);

void dbgmem_add_tab(uint32_t addr);

gint display_dbgmem_address(uint32_t *addr);
gint display_dbgmem_search(void);
