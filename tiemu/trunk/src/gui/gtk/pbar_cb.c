#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "pbar_cb.h"
#include "pbar_dbox.h"
#include "support.h"

#include "tilibs.h"
#include "struct.h"
#include "pbar_cb.h"

#define PROGRESS_TYPE GTK_PROGRESS_CONTINUOUS

#define UNLOCK 0
#define LOCK   1

/*******************/
/* Local variables */
/*******************/

static GtkWidget *pbar_window;
static gint is_active;
TicalcInfoUpdate info_update;
struct progress_window
{
  GtkWidget *window;
  GtkWidget *pbar;
  GtkWidget *pbar2;
  GtkWidget *label;
  GtkWidget *label2;
  GtkWidget *label_rate;
} p_win;

/**************************/
/* General pbar functions */
/**************************/

/* Create a window with one progress bar */
void create_pbar_type1(const gchar *title)
{
  is_active = LOCK;
  info_update.prev_percentage = 0.0;
  info_update.percentage = 0.0;
  info_update.cancel = 0;

  pbar_window = create_pbar1_dbox();

  gtk_window_set_title (GTK_WINDOW(pbar_window), title);

  p_win.pbar = lookup_widget(pbar_window, "progressbar1");
  gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (p_win.pbar),
				  PROGRESS_TYPE);

  p_win.label_rate = lookup_widget(pbar_window, "label20");

  gtk_widget_show_all(pbar_window);
}

/* Create a window with one label */
void create_pbar_type2(const gchar *title, gchar *text)
{
  is_active = LOCK;
  info_update.prev_percentage = 0.0;
  info_update.percentage = 0.0;
  info_update.cancel = 0;

  pbar_window = create_pbar2_dbox();

  gtk_window_set_title (GTK_WINDOW(pbar_window), title);

  p_win.label = lookup_widget(pbar_window, "label3");
  gtk_label_set(GTK_LABEL(p_win.label), text);

  gtk_widget_show_all(pbar_window);
}

/* Create a window with two progress bars */
void create_pbar_type3(const gchar *title)
{
  is_active = LOCK;
  info_update.prev_main_percentage = 0.0;
  info_update.main_percentage = 0.0;
  info_update.prev_percentage = 0.0;
  info_update.percentage = 0.0;
  info_update.cancel = 0;

  pbar_window = create_pbar3_dbox();

  gtk_window_set_title (GTK_WINDOW(pbar_window), title);

  p_win.pbar = lookup_widget(pbar_window, "progressbar2");
  gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (p_win.pbar),
				  PROGRESS_TYPE);

  p_win.pbar2 = lookup_widget(pbar_window, "progressbar3");
  gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (p_win.pbar2),
				  PROGRESS_TYPE);

  p_win.label_rate = lookup_widget(pbar_window, "label21");

  gtk_widget_show_all(pbar_window);
}

/* Create a window with a one progress bar and one label */
void create_pbar_type4(const gchar *title, gchar *text)
{
  is_active = LOCK;
  info_update.percentage=0.0;
  info_update.cancel=0;

  pbar_window = create_pbar4_dbox();

  gtk_window_set_title (GTK_WINDOW(pbar_window), title);

  p_win.label = lookup_widget(pbar_window, "label15");
  gtk_label_set(GTK_LABEL(p_win.label), text);

  p_win.pbar = lookup_widget(pbar_window, "progressbar4");
  gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (p_win.pbar),
				  PROGRESS_TYPE);

  p_win.label_rate = lookup_widget(pbar_window, "label22");

  gtk_widget_show_all(pbar_window);
}

/* Create a window with two progress bars and one label */
void create_pbar_type5(const gchar *title, gchar *text)
{
  is_active = LOCK;
  info_update.prev_main_percentage = 0.0;
  info_update.main_percentage = 0.0;
  info_update.prev_percentage = 0.0;
  info_update.percentage = 0.0;
  info_update.cancel = 0;

  pbar_window = create_pbar5_dbox();

  gtk_window_set_title (GTK_WINDOW(pbar_window), title);

  p_win.label = lookup_widget(pbar_window, "label19");
  gtk_label_set(GTK_LABEL(p_win.label), text);

  p_win.pbar = lookup_widget(pbar_window, "progressbar5");
  gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (p_win.pbar),
				  PROGRESS_TYPE);

  p_win.pbar2 = lookup_widget(pbar_window, "progressbar6");
  gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (p_win.pbar2),
				  PROGRESS_TYPE);

  p_win.label_rate = lookup_widget(pbar_window, "label23");

  gtk_widget_show_all(pbar_window);
}

/* 
   Destroy a pbar window
*/
void destroy_pbar(void)
{
  p_win.pbar=NULL;
  p_win.pbar2=NULL;
  p_win.label=NULL;

  gtk_widget_destroy(pbar_window);
  is_active = UNLOCK;
}

/*************/
/* Callbacks */
/*************/


void
pbar1_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  info_update.cancel = 1;

  return;
}


