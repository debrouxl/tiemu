#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "infos_cb.h"
#include "infos_dbox.h"
#include "support.h"

#include "struct.h"
#include "version.h"
#include "interface.h"

gint display_infos_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *w;
  gchar buffer[MAXCHARS];
  int i = 0;
  const char *p1, *p2, *p;

  dbox = create_infos_dbox();
 
  w = lookup_widget(dbox, "label1");
  sprintf(buffer, "GtkTiEmu, version %s", TIEMU_VERSION);
  gtk_label_set_text(GTK_LABEL(w), buffer);

  w = lookup_widget(dbox, "label5");
  sprintf(buffer, "ROM version: %s", ti68k_getRomVersion());
  gtk_label_set_text(GTK_LABEL(w), buffer);

  w = lookup_widget(dbox, "label6");
  sprintf(buffer, "Emulated RAM: %i Kb", ti68k_getRamSize());
  gtk_label_set_text(GTK_LABEL(w), buffer);
  
  w = lookup_widget(dbox, "label7");
  sprintf(buffer, "Main ROM size: %i Mb", ti68k_getRomSize() >> 20);
  gtk_label_set_text(GTK_LABEL(w), buffer);

  w = lookup_widget(dbox, "label8");
  i = ti68k_getRomType();
  if(i & INTERNAL) 
    p1 = _("internal"); 
  else 
    p1 = _("external");
  if(i & FLASH_ROM) 
    p2 = _("FLASH"); 
  else 
    p2 = _("PROM");
  sprintf(buffer, "ROM type: %s %s", p1, p2);
  gtk_label_set_text(GTK_LABEL(w), buffer);

  w = lookup_widget(dbox, "label9");
  switch(ti68k_getCalcType())
    {
    case TI92: p = "TI92";
      break;
    case TI89: p = "TI89";
      break;
    case TI92 | MODULEPLUS: p = "TI92+";
      break;
    default: p = "Unknown";
      break;
    }
  sprintf(buffer, "Calculator type: %s", p);
  gtk_label_set_text(GTK_LABEL(w), buffer);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "infos_dbox"));
}


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "infos_dbox"));
}


void
on_infos_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data)
{
  ti68k_unhalt();
}

/* */
