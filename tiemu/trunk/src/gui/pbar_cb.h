#include <gtk/gtk.h>

void create_pbar_type1(const gchar *title);
void create_pbar_type2(const gchar *title, gchar *text);
void create_pbar_type3(const gchar *title);
void create_pbar_type4(const gchar *title, gchar *text);
void create_pbar_type5(const gchar *title, gchar *text);
void destroy_pbar(void);


void
pbar1_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);
