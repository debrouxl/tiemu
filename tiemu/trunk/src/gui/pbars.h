#include <gtk/gtk.h>
#include "support.h"

extern struct progress_window {
	GtkWidget *window;
	GtkWidget *pbar1;
	GtkWidget *pbar2;
	GtkWidget *label;
	GtkWidget *label_rate;
} p_win;

void create_pbar_type1(const gchar * title);
void create_pbar_type2(const gchar * title, gchar * text);
void create_pbar_type3(const gchar * title);
void create_pbar_type4(const gchar * title, gchar * text);
void create_pbar_type5(const gchar * title, gchar * text);
void destroy_pbar(void);

GLADE_CB void on_pbar_okbutton1_pressed(GtkButton * button,
					gpointer user_data);
