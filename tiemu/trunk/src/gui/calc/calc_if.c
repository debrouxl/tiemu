/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "calc_if.h"
#include "null"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_calc_wnd (void)
{
  GtkWidget *calc_wnd;
  GtkWidget *drawingarea1;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  calc_wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (calc_wnd, "calc_wnd");
  gtk_widget_set_events (calc_wnd, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_VISIBILITY_NOTIFY_MASK);
  gtk_window_set_title (GTK_WINDOW (calc_wnd), _("TiEmu II"));
  gtk_window_set_default_size (GTK_WINDOW (calc_wnd), 1, 1);
  gtk_window_set_resizable (GTK_WINDOW (calc_wnd), FALSE);

  drawingarea1 = gtk_drawing_area_new ();
  gtk_widget_set_name (drawingarea1, "drawingarea1");
  gtk_widget_show (drawingarea1);
  gtk_container_add (GTK_CONTAINER (calc_wnd), drawingarea1);
  GTK_WIDGET_SET_FLAGS (drawingarea1, GTK_CAN_FOCUS);
  GTK_WIDGET_SET_FLAGS (drawingarea1, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, drawingarea1, _("Use mouse or press a key."), NULL);

  g_signal_connect ((gpointer) calc_wnd, "delete_event",
                    G_CALLBACK (on_calc_wnd_delete_event),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "button_press_event",
                    G_CALLBACK (on_calc_wnd_button_press_event),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "button_release_event",
                    G_CALLBACK (on_calc_wnd_button_release_event),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "key_press_event",
                    G_CALLBACK (on_calc_wnd_key_press_event),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "key_release_event",
                    G_CALLBACK (on_calc_wnd_key_release_event),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "destroy",
                    G_CALLBACK (on_calc_wnd_destroy),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "expose_event",
                    G_CALLBACK (on_calc_wnd_expose_event),
                    NULL);
  g_signal_connect ((gpointer) calc_wnd, "window_state_event",
                    G_CALLBACK (on_calc_wnd_window_state_event),
                    NULL);
  g_signal_connect ((gpointer) drawingarea1, "configure_event",
                    G_CALLBACK (on_drawingarea1_configure_event),
                    NULL);
  g_signal_connect ((gpointer) drawingarea1, "expose_event",
                    G_CALLBACK (on_drawingarea1_expose_event),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (calc_wnd, calc_wnd, "calc_wnd");
  GLADE_HOOKUP_OBJECT (calc_wnd, drawingarea1, "drawingarea1");
  GLADE_HOOKUP_OBJECT_NO_REF (calc_wnd, tooltips, "tooltips");

  gtk_widget_grab_focus (drawingarea1);
  gtk_widget_grab_default (drawingarea1);
  return calc_wnd;
}

