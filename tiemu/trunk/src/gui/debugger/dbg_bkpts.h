#include <gtk/gtk.h>

GtkWidget* dbgbkpts_create_window(void);
GtkWidget* dbgbkpts_display_window(void);
void dbgbkpts_refresh_window(void);

typedef struct 
{
	gint mode;
	gint type;
	gint min;
	gint max;
} DBG_DATA_INIT;
