#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "scopt_cb.h"
#include "scopt_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "screenshot.h"
#include "main.h"

static gint i_image_format;
static gint i_image_type;
static gint i_image_size;
static gint i_screen_counter;
static gchar *i_screen_file;

static GtkWidget *label = NULL;
void refresh_label(void);

gint display_scopt_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_scopt_dbox();

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton18");
  i_image_format = options.img_format;
  if(options.img_format == IMG_PCX)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton19");
  i_image_format = options.img_format;
  if(options.img_format == IMG_XPM)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton20");
  i_image_format = options.img_format;
  if(options.img_format == IMG_JPG)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton21");
  i_image_format = options.img_format;
  if(options.img_format == IMG_BMP)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton22");
  i_image_type = options.img_type;
  if(options.img_type == IMG_BW )
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton23");
  i_image_type = options.img_type;
  if(options.img_type == IMG_COL )
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton24");
  i_image_size = options.img_size;
  if(options.img_size == IMG_LCD )
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton25");
  i_image_size = options.img_size;
  if(options.img_size == IMG_SKIN )
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "label3");
  label = GTK_WIDGET(user_data);
  refresh_label();

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "entry1");
  //gtk_entry_set_visibility(GTK_ENTRY(user_data), FALSE);
  //gtk_entry_set_editable(GTK_ENTRY(user_data), TRUE);
  i_screen_file = g_strdup(options.screen_file);
  i_screen_counter = options.screen_counter;
  gtk_entry_set_text(GTK_ENTRY(user_data), options.screen_file);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_scopt_radiobutton18_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_PCX;
  refresh_label();
}


void
on_scopt_radiobutton19_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_XPM;
  refresh_label();
}


void
on_scopt_radiobutton20_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_JPG;
  refresh_label();
}


void
on_scopt_radiobutton21_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_format = IMG_BMP;
  refresh_label();
}


void
on_scopt_radiobutton22_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_type = IMG_BW;
}


void
on_scopt_radiobutton23_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_type = IMG_COL;
}


void
on_scopt_radiobutton24_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_size = IMG_LCD;
}


void
on_scopt_radiobutton25_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active(togglebutton))
    i_image_size = IMG_SKIN;
}


void
on_scopt_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  options.img_format = i_image_format;
  options.img_type = i_image_type;
  options.img_size = i_image_size;
  options.screen_counter = i_screen_counter;
  g_free(options.screen_file);
  options.screen_file = g_strdup(i_screen_file);
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_scopt_cancel_button_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
on_scopt_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}

void refresh_label(void)
{
  gchar buffer[MAXCHARS];
  gchar *ext = "???";

  switch(i_image_format)
    {
    case IMG_XPM: ext = "xpm"; break;
    case IMG_PCX: ext = "pcx"; break;
    case IMG_JPG: ext = "jpg"; break;
    case IMG_BMP: ext = "bmp"; break;
    default: break;
    }
  
  sprintf(buffer, "%s%03i.%s", i_screen_file, i_screen_counter, ext);
  gtk_label_set_text(GTK_LABEL(label), buffer);
}

void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  g_free(options.screen_file);
  s = gtk_editable_get_chars(GTK_EDITABLE(user_data), 0, -1);
  i_screen_file = g_strdup(s);
  refresh_label();
}

/* */
