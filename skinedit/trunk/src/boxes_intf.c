/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "support.h"

#include "boxes_intf.h"

#include "boxes_cb.h"


GtkWidget*
create_list_keys_dialog (void)
{
  GtkWidget *scrolledwindow2;
  GtkWidget *list_keys;
  GtkWidget *label7;

  list_keys_dialog = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_object_set_data (GTK_OBJECT (list_keys_dialog), "list_keys_dialog", list_keys_dialog);
  gtk_window_set_title (GTK_WINDOW (list_keys_dialog), _("Keys"));
  gtk_window_set_default_size (GTK_WINDOW (list_keys_dialog), 100, 200);
  gtk_window_set_policy (GTK_WINDOW (list_keys_dialog), TRUE, TRUE, TRUE);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow2);
  gtk_object_set_data_full (GTK_OBJECT (list_keys_dialog), "scrolledwindow2", scrolledwindow2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow2);
  gtk_container_add (GTK_CONTAINER (list_keys_dialog), scrolledwindow2);

  list_keys = gtk_clist_new (1);
  gtk_widget_ref (list_keys);
  gtk_object_set_data_full (GTK_OBJECT (list_keys_dialog), "list_keys", list_keys,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (list_keys);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), list_keys);
  gtk_clist_set_column_width (GTK_CLIST (list_keys), 0, 80);
  gtk_clist_column_titles_hide (GTK_CLIST (list_keys));

  label7 = gtk_label_new (_("label7"));
  gtk_widget_ref (label7);
  gtk_object_set_data_full (GTK_OBJECT (list_keys_dialog), "label7", label7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label7);
  gtk_clist_set_column_widget (GTK_CLIST (list_keys), 0, label7);

  gtk_signal_connect (GTK_OBJECT (list_keys_dialog), "delete_event",
                      GTK_SIGNAL_FUNC (on_list_keys_dialog_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (list_keys_dialog), "destroy_event",
                      GTK_SIGNAL_FUNC (on_list_keys_dialog_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (list_keys), "select_row",
                      GTK_SIGNAL_FUNC (on_list_keys_select_row),
                      NULL);

  return list_keys_dialog;
}

GtkWidget*
create_skin_changed (gpointer action)
{
  GtkWidget *skin_changed;
  GtkWidget *vbox5;
  GtkWidget *label8;
  GtkWidget *hbox5;
  GtkWidget *hbuttonbox3;
  GtkWidget *skin_changed_save;
  GtkWidget *skin_changed_ignore;
  GtkWidget *skin_changed_cancel;

  skin_changed = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (skin_changed), "skin_changed", skin_changed);
  gtk_window_set_title (GTK_WINDOW (skin_changed), _("Save changes ?"));
  gtk_window_set_modal (GTK_WINDOW (skin_changed), TRUE);
  gtk_window_set_policy (GTK_WINDOW (skin_changed), FALSE, FALSE, TRUE);

  vbox5 = GTK_DIALOG (skin_changed)->vbox;
  gtk_object_set_data (GTK_OBJECT (skin_changed), "vbox5", vbox5);
  gtk_widget_show (vbox5);

  label8 = gtk_label_new (_("The current skin has changed.\nDo you want to save your modifications ?"));
  gtk_widget_ref (label8);
  gtk_object_set_data_full (GTK_OBJECT (skin_changed), "label8", label8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (vbox5), label8, TRUE, FALSE, 10);

  hbox5 = GTK_DIALOG (skin_changed)->action_area;
  gtk_object_set_data (GTK_OBJECT (skin_changed), "hbox5", hbox5);
  gtk_widget_show (hbox5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox5), 10);

  hbuttonbox3 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox3);
  gtk_object_set_data_full (GTK_OBJECT (skin_changed), "hbuttonbox3", hbuttonbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox3);
  gtk_box_pack_start (GTK_BOX (hbox5), hbuttonbox3, TRUE, TRUE, 0);

  skin_changed_save = gtk_button_new_with_label (_("Save"));
  gtk_widget_ref (skin_changed_save);
  gtk_object_set_data_full (GTK_OBJECT (skin_changed), "skin_changed_save", skin_changed_save,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (skin_changed_save);
  gtk_container_add (GTK_CONTAINER (hbuttonbox3), skin_changed_save);
  GTK_WIDGET_SET_FLAGS (skin_changed_save, GTK_CAN_DEFAULT);

  skin_changed_ignore = gtk_button_new_with_label (_("Ignore"));
  gtk_widget_ref (skin_changed_ignore);
  gtk_object_set_data_full (GTK_OBJECT (skin_changed), "skin_changed_ignore", skin_changed_ignore,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (skin_changed_ignore);
  gtk_container_add (GTK_CONTAINER (hbuttonbox3), skin_changed_ignore);
  GTK_WIDGET_SET_FLAGS (skin_changed_ignore, GTK_CAN_DEFAULT);

  skin_changed_cancel = gtk_button_new_with_label (_("Cancel"));
  gtk_widget_ref (skin_changed_cancel);
  gtk_object_set_data_full (GTK_OBJECT (skin_changed), "skin_changed_cancel", skin_changed_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (skin_changed_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox3), skin_changed_cancel);
  GTK_WIDGET_SET_FLAGS (skin_changed_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (skin_changed), "delete_event",
                      GTK_SIGNAL_FUNC (on_skin_changed_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (skin_changed), "destroy_event",
                      GTK_SIGNAL_FUNC (on_skin_changed_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (skin_changed_save), "clicked",
                      GTK_SIGNAL_FUNC (on_skin_changed_save_clicked),
                      action);
  gtk_signal_connect (GTK_OBJECT (skin_changed_ignore), "clicked",
                      GTK_SIGNAL_FUNC (on_skin_changed_ignore_clicked),
                      action);
  gtk_signal_connect (GTK_OBJECT (skin_changed_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_skin_changed_cancel_clicked),
                      action);

  return skin_changed;
}

GtkWidget*
create_about_box (void)
{
  GtkWidget *about_box;
  GtkWidget *dialog_vbox3;
  GtkWidget *frame3;
  GtkWidget *label9;
  GtkWidget *dialog_action_area3;
  GtkWidget *about_ok;

  about_box = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (about_box), "about_box", about_box);
  gtk_window_set_title (GTK_WINDOW (about_box), _("About... Skinedit"));
  gtk_window_set_policy (GTK_WINDOW (about_box), TRUE, TRUE, TRUE);

  dialog_vbox3 = GTK_DIALOG (about_box)->vbox;
  gtk_object_set_data (GTK_OBJECT (about_box), "dialog_vbox3", dialog_vbox3);
  gtk_widget_show (dialog_vbox3);

  frame3 = gtk_frame_new (_("Skinedit infos"));
  gtk_widget_ref (frame3);
  gtk_object_set_data_full (GTK_OBJECT (about_box), "frame3", frame3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (dialog_vbox3), frame3, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame3), 5);

  label9 = gtk_label_new (_("Skinedit was written by Julien BLACHE <jb@tilp.info>\n\nThis program is released under the terms of the \nGNU GENERAL PUBLIC LICENSE\nVERSION 2\n\nIt is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the\nimplied warranty of MERCHANTABILITY or FITNESS\nFOR A PARTICULAR PURPOSE. See the GNU\nGeneral Public License for more details."));
  gtk_widget_ref (label9);
  gtk_object_set_data_full (GTK_OBJECT (about_box), "label9", label9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label9);
  gtk_container_add (GTK_CONTAINER (frame3), label9);
  gtk_misc_set_padding (GTK_MISC (label9), 5, 5);

  dialog_action_area3 = GTK_DIALOG (about_box)->action_area;
  gtk_object_set_data (GTK_OBJECT (about_box), "dialog_action_area3", dialog_action_area3);
  gtk_widget_show (dialog_action_area3);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area3), 10);

  about_ok = gtk_button_new_with_label (_("OK !"));
  gtk_widget_ref (about_ok);
  gtk_object_set_data_full (GTK_OBJECT (about_box), "about_ok", about_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (about_ok);
  gtk_box_pack_start (GTK_BOX (dialog_action_area3), about_ok, TRUE, TRUE, 0);

  gtk_signal_connect (GTK_OBJECT (about_box), "delete_event",
                      GTK_SIGNAL_FUNC (on_about_box_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (about_box), "destroy_event",
                      GTK_SIGNAL_FUNC (on_about_box_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (about_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_about_ok_clicked),
                      NULL);

  return about_box;
}

