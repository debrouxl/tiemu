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

#include "prop_intf.h"

#include "prop_cb.h"


GtkWidget*
create_prop_dialog (void)
{
  GtkWidget *prop_dialog;
  GtkWidget *dialog_vbox2;
  GtkWidget *vbox2;
  GtkWidget *hbox2;
  GtkWidget *hbox3;
  GtkWidget *table1;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *prop_entry_title;
  GtkWidget *prop_entry_author;
  GtkWidget *hseparator1;
  GtkWidget *hbox4;
  GtkWidget *frame1;
  GtkWidget *table2;
  GSList *calcs_group = NULL;
  GtkWidget *ti73;
  GtkWidget *ti82;
  GtkWidget *ti83;
  GtkWidget *ti83p;
  GtkWidget *ti85;
  GtkWidget *ti86;
  GtkWidget *ti89;
  GtkWidget *ti92;
  GtkWidget *ti92p;
  GtkWidget *v200;
  GtkWidget *vseparator1;
  GtkWidget *frame2;
  GtkWidget *table3;
  GSList *lcd_group = NULL;
  GtkWidget *lcd_high_contrast;
  GtkWidget *lcd_low_contrast;
  GtkWidget *lcd_custom;
  GtkWidget *lcd_low_white;
  GtkWidget *lcd_hi_white;
  GtkWidget *lcd_hi_black;
  GtkWidget *lcd_low_black;
  GtkWidget *lcd_custom_white;
  GtkWidget *lcd_custom_black;
  GtkWidget *dialog_action_area2;
  GtkWidget *hbuttonbox2;
  GtkWidget *prop_dlg_ok;
  GtkWidget *prop_dlg_cancel;

  prop_dialog = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (prop_dialog), "prop_dialog", prop_dialog);
  gtk_window_set_title (GTK_WINDOW (prop_dialog), _("Properties"));
  gtk_window_set_modal (GTK_WINDOW (prop_dialog), TRUE);
  gtk_window_set_policy (GTK_WINDOW (prop_dialog), FALSE, FALSE, TRUE);

  dialog_vbox2 = GTK_DIALOG (prop_dialog)->vbox;
  gtk_object_set_data (GTK_OBJECT (prop_dialog), "dialog_vbox2", dialog_vbox2);
  gtk_widget_show (dialog_vbox2);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox2);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox2), vbox2, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox2), 10);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, TRUE, TRUE, 0);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox3);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "hbox3", hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (hbox2), hbox3, TRUE, TRUE, 0);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_widget_ref (table1);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "table1", table1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (hbox3), table1, TRUE, TRUE, 0);

  label3 = gtk_label_new (_("Title : "));
  gtk_widget_ref (label3);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "label3", label3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label3);
  gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  label4 = gtk_label_new (_("Author : "));
  gtk_widget_ref (label4);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "label4", label4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label4);
  gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);

  prop_entry_title = gtk_entry_new ();
  gtk_widget_ref (prop_entry_title);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "prop_entry_title", prop_entry_title,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prop_entry_title);
  gtk_table_attach (GTK_TABLE (table1), prop_entry_title, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  prop_entry_author = gtk_entry_new ();
  gtk_widget_ref (prop_entry_author);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "prop_entry_author", prop_entry_author,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prop_entry_author);
  gtk_table_attach (GTK_TABLE (table1), prop_entry_author, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator1);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "hseparator1", hseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox2), hseparator1, FALSE, TRUE, 4);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox4);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "hbox4", hbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox4, FALSE, FALSE, 0);

  frame1 = gtk_frame_new (_("Calc type"));
  gtk_widget_ref (frame1);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (hbox4), frame1, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame1), 3);

  table2 = gtk_table_new (5, 2, TRUE);
  gtk_widget_ref (table2);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "table2", table2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table2);
  gtk_container_add (GTK_CONTAINER (frame1), table2);

  ti73 = gtk_radio_button_new_with_label (calcs_group, _("TI-73"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti73));
  gtk_widget_ref (ti73);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti73", ti73,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti73);
  gtk_table_attach (GTK_TABLE (table2), ti73, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti82 = gtk_radio_button_new_with_label (calcs_group, _("TI-82"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti82));
  gtk_widget_ref (ti82);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti82", ti82,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti82);
  gtk_table_attach (GTK_TABLE (table2), ti82, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti83 = gtk_radio_button_new_with_label (calcs_group, _("TI-83"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti83));
  gtk_widget_ref (ti83);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti83", ti83,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti83);
  gtk_table_attach (GTK_TABLE (table2), ti83, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti83p = gtk_radio_button_new_with_label (calcs_group, _("TI-83 Plus"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti83p));
  gtk_widget_ref (ti83p);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti83p", ti83p,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti83p);
  gtk_table_attach (GTK_TABLE (table2), ti83p, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti85 = gtk_radio_button_new_with_label (calcs_group, _("TI-85"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti85));
  gtk_widget_ref (ti85);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti85", ti85,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti85);
  gtk_table_attach (GTK_TABLE (table2), ti85, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti86 = gtk_radio_button_new_with_label (calcs_group, _("TI-86"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti86));
  gtk_widget_ref (ti86);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti86", ti86,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti86);
  gtk_table_attach (GTK_TABLE (table2), ti86, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti89 = gtk_radio_button_new_with_label (calcs_group, _("TI-89"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti89));
  gtk_widget_ref (ti89);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti89", ti89,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti89);
  gtk_table_attach (GTK_TABLE (table2), ti89, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ti89), TRUE);

  ti92 = gtk_radio_button_new_with_label (calcs_group, _("TI-92"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti92));
  gtk_widget_ref (ti92);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti92", ti92,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti92);
  gtk_table_attach (GTK_TABLE (table2), ti92, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  ti92p = gtk_radio_button_new_with_label (calcs_group, _("TI-92 Plus"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ti92p));
  gtk_widget_ref (ti92p);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "ti92p", ti92p,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ti92p);
  gtk_table_attach (GTK_TABLE (table2), ti92p, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  v200 = gtk_radio_button_new_with_label (calcs_group, _("Voyage 200"));
  calcs_group = gtk_radio_button_group (GTK_RADIO_BUTTON (v200));
  gtk_widget_ref (v200);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "v200", v200,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (v200);
  gtk_table_attach (GTK_TABLE (table2), v200, 1, 2, 4, 5,
		    (GtkAttachOptions) (GTK_FILL),
		    (GtkAttachOptions) (0), 0, 0);
  
  vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "vseparator1", vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vseparator1);
  gtk_box_pack_start (GTK_BOX (hbox4), vseparator1, FALSE, TRUE, 4);

  frame2 = gtk_frame_new (_("LCD color"));
  gtk_widget_ref (frame2);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (hbox4), frame2, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame2), 3);

  table3 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table3);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "table3", table3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table3);
  gtk_container_add (GTK_CONTAINER (frame2), table3);

  lcd_high_contrast = gtk_radio_button_new_with_label (lcd_group, _("High contrast"));
  lcd_group = gtk_radio_button_group (GTK_RADIO_BUTTON (lcd_high_contrast));
  gtk_widget_ref (lcd_high_contrast);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_high_contrast", lcd_high_contrast,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_high_contrast);
  gtk_table_attach (GTK_TABLE (table3), lcd_high_contrast, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (lcd_high_contrast), TRUE);

  lcd_low_contrast = gtk_radio_button_new_with_label (lcd_group, _("Low contrast"));
  lcd_group = gtk_radio_button_group (GTK_RADIO_BUTTON (lcd_low_contrast));
  gtk_widget_ref (lcd_low_contrast);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_low_contrast", lcd_low_contrast,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_low_contrast);
  gtk_table_attach (GTK_TABLE (table3), lcd_low_contrast, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  lcd_custom = gtk_radio_button_new_with_label (lcd_group, _("Custom"));
  lcd_group = gtk_radio_button_group (GTK_RADIO_BUTTON (lcd_custom));
  gtk_widget_ref (lcd_custom);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_custom", lcd_custom,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_custom);
  gtk_table_attach (GTK_TABLE (table3), lcd_custom, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  lcd_low_white = gtk_drawing_area_new ();
  gtk_widget_ref (lcd_low_white);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_low_white", lcd_low_white,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_low_white);
  gtk_table_attach (GTK_TABLE (table3), lcd_low_white, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 1, 1);
  gtk_widget_set_usize (lcd_low_white, 25, 20);

  lcd_hi_white = gtk_drawing_area_new ();
  gtk_widget_ref (lcd_hi_white);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_hi_white", lcd_hi_white,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_hi_white);
  gtk_table_attach (GTK_TABLE (table3), lcd_hi_white, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 1, 1);
  gtk_widget_set_usize (lcd_hi_white, 25, 20);

  lcd_hi_black = gtk_drawing_area_new ();
  gtk_widget_ref (lcd_hi_black);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_hi_black", lcd_hi_black,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_hi_black);
  gtk_table_attach (GTK_TABLE (table3), lcd_hi_black, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 1, 1);
  gtk_widget_set_usize (lcd_hi_black, 25, 20);

  lcd_low_black = gtk_drawing_area_new ();
  gtk_widget_ref (lcd_low_black);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_low_black", lcd_low_black,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_low_black);
  gtk_table_attach (GTK_TABLE (table3), lcd_low_black, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 1, 1);
  gtk_widget_set_usize (lcd_low_black, 25, 20);

  lcd_custom_white = gtk_drawing_area_new ();
  gtk_widget_ref (lcd_custom_white);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_custom_white", lcd_custom_white,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_custom_white);
  gtk_table_attach (GTK_TABLE (table3), lcd_custom_white, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 1, 1);
  gtk_widget_set_usize (lcd_custom_white, 25, 20);

  lcd_custom_black = gtk_drawing_area_new ();
  gtk_widget_ref (lcd_custom_black);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "lcd_custom_black", lcd_custom_black,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_custom_black);
  gtk_table_attach (GTK_TABLE (table3), lcd_custom_black, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 1, 1);
  gtk_widget_set_usize (lcd_custom_black, 25, 20);

  dialog_action_area2 = GTK_DIALOG (prop_dialog)->action_area;
  gtk_object_set_data (GTK_OBJECT (prop_dialog), "dialog_action_area2", dialog_action_area2);
  gtk_widget_show (dialog_action_area2);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area2), 10);

  hbuttonbox2 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox2);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "hbuttonbox2", hbuttonbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox2);
  gtk_box_pack_start (GTK_BOX (dialog_action_area2), hbuttonbox2, TRUE, TRUE, 0);

  prop_dlg_ok = gtk_button_new_with_label (_("OK"));
  gtk_widget_ref (prop_dlg_ok);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "prop_dlg_ok", prop_dlg_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prop_dlg_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), prop_dlg_ok);
  GTK_WIDGET_SET_FLAGS (prop_dlg_ok, GTK_CAN_DEFAULT);

  prop_dlg_cancel = gtk_button_new_with_label (_("Cancel"));
  gtk_widget_ref (prop_dlg_cancel);
  gtk_object_set_data_full (GTK_OBJECT (prop_dialog), "prop_dlg_cancel", prop_dlg_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prop_dlg_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), prop_dlg_cancel);
  GTK_WIDGET_SET_FLAGS (prop_dlg_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (prop_dialog), "delete_event",
                      GTK_SIGNAL_FUNC (on_prop_dialog_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (prop_dialog), "destroy_event",
                      GTK_SIGNAL_FUNC (on_prop_dialog_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (lcd_custom_white), "button_press_event",
                      GTK_SIGNAL_FUNC (on_lcd_custom_white_button_press_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (lcd_custom_black), "button_press_event",
                      GTK_SIGNAL_FUNC (on_lcd_custom_black_button_press_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (prop_dlg_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_prop_dlg_ok_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (prop_dlg_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_prop_dlg_cancel_clicked),
                      NULL);

  gtk_widget_add_events(GTK_WIDGET(lcd_custom_white),
			GDK_BUTTON_PRESS_MASK);
  
  gtk_widget_add_events(GTK_WIDGET(lcd_custom_black),
			GDK_BUTTON_PRESS_MASK);


  return prop_dialog;
}


GtkWidget*
create_lcd_color_select (gpointer action)
{
  GtkWidget *lcd_color_select;
  GtkWidget *lcd_color_unused;
  GtkWidget *lcd_color_ok;
  GtkWidget *lcd_color_cancel;

  lcd_color_select = gtk_color_selection_dialog_new (_("Select Color"));
  gtk_object_set_data (GTK_OBJECT (lcd_color_select), "lcd_color_select", lcd_color_select);
  gtk_container_set_border_width (GTK_CONTAINER (lcd_color_select), 10);
  gtk_window_set_modal (GTK_WINDOW (lcd_color_select), TRUE);

  lcd_color_unused = GTK_COLOR_SELECTION_DIALOG (lcd_color_select)->ok_button;
  gtk_widget_hide(lcd_color_unused);

  lcd_color_ok = GTK_COLOR_SELECTION_DIALOG (lcd_color_select)->cancel_button;
  gtk_label_set_text(GTK_LABEL(GTK_BUTTON(lcd_color_ok)->child), _("OK"));
  gtk_object_set_data (GTK_OBJECT (lcd_color_select), "lcd_color_ok", lcd_color_ok);
  gtk_widget_show (lcd_color_ok);
  GTK_WIDGET_SET_FLAGS (lcd_color_ok, GTK_CAN_DEFAULT | GTK_HAS_DEFAULT);

  lcd_color_cancel = GTK_COLOR_SELECTION_DIALOG (lcd_color_select)->help_button;
  gtk_label_set_text(GTK_LABEL(GTK_BUTTON(lcd_color_cancel)->child), _("Cancel"));
  gtk_object_set_data (GTK_OBJECT (lcd_color_select), "lcd_color_cancel", lcd_color_cancel);
  gtk_widget_show (lcd_color_cancel);
  GTK_WIDGET_SET_FLAGS (lcd_color_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (lcd_color_select), "delete_event",
                      GTK_SIGNAL_FUNC (on_lcd_color_select_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (lcd_color_select), "destroy_event",
                      GTK_SIGNAL_FUNC (on_lcd_color_select_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (lcd_color_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_lcd_color_ok_clicked),
                      action);
  gtk_signal_connect (GTK_OBJECT (lcd_color_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_lcd_color_cancel_clicked),
                      action);

  return lcd_color_select;
}
