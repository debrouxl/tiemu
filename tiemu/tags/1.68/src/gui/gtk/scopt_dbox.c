/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "scopt_cb.h"
#include "scopt_dbox.h"
#include "support.h"

GtkWidget*
create_scopt_dbox (void)
{
  GtkWidget *scopt_dbox;
  GtkWidget *dialog_vbox1;
  GtkWidget *table1;
  GtkWidget *frame2;
  GtkWidget *hbox3;
  GtkWidget *vbox3;
  GSList *group2_group = NULL;
  GtkWidget *radiobutton22;
  GtkWidget *radiobutton23;
  GtkWidget *hseparator1;
  GSList *group3_group = NULL;
  GtkWidget *radiobutton24;
  GtkWidget *radiobutton25;
  GtkWidget *frame4;
  GtkWidget *entry1;
  GtkWidget *frame3;
  GtkWidget *vbox2;
  GSList *group1_group = NULL;
  GtkWidget *radiobutton18;
  GtkWidget *radiobutton19;
  GtkWidget *radiobutton20;
  GtkWidget *radiobutton21;
  GtkWidget *frame5;
  GtkWidget *label3;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbuttonbox1;
  GtkWidget *ok_button;
  GtkWidget *hbox2;
  GtkWidget *pixmap2;
  guint label2_key;
  GtkWidget *label2;
  GtkWidget *cancel_button;
  GtkWidget *hbox1;
  GtkWidget *pixmap1;
  guint label1_key;
  GtkWidget *label1;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  scopt_dbox = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (scopt_dbox), "scopt_dbox", scopt_dbox);
  gtk_window_set_title (GTK_WINDOW (scopt_dbox), _("Screen capture options"));
  gtk_window_set_policy (GTK_WINDOW (scopt_dbox), FALSE, FALSE, FALSE);

  dialog_vbox1 = GTK_DIALOG (scopt_dbox)->vbox;
  gtk_object_set_data (GTK_OBJECT (scopt_dbox), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_widget_ref (table1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "table1", table1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), table1, TRUE, TRUE, 0);

  frame2 = gtk_frame_new (_("Image type:"));
  gtk_widget_ref (frame2);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame2);
  gtk_table_attach (GTK_TABLE (table1), frame2, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame2), 5);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox3);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "hbox3", hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (frame2), hbox3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox3), 5);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "vbox3", vbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (hbox3), vbox3, TRUE, TRUE, 0);

  radiobutton22 = gtk_radio_button_new_with_label (group2_group, _("Black&White"));
  group2_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton22));
  gtk_widget_ref (radiobutton22);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton22", radiobutton22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton22);
  gtk_box_pack_start (GTK_BOX (vbox3), radiobutton22, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton22), TRUE);

  radiobutton23 = gtk_radio_button_new_with_label (group2_group, _("Color"));
  group2_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton23));
  gtk_widget_ref (radiobutton23);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton23", radiobutton23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton23);
  gtk_box_pack_start (GTK_BOX (vbox3), radiobutton23, FALSE, FALSE, 0);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "hseparator1", hseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox3), hseparator1, TRUE, TRUE, 0);

  radiobutton24 = gtk_radio_button_new_with_label (group3_group, _("LCD only"));
  group3_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton24));
  gtk_widget_ref (radiobutton24);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton24", radiobutton24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton24);
  gtk_box_pack_start (GTK_BOX (vbox3), radiobutton24, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton24), TRUE);

  radiobutton25 = gtk_radio_button_new_with_label (group3_group, _("Calculator"));
  group3_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton25));
  gtk_widget_ref (radiobutton25);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton25", radiobutton25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton25);
  gtk_box_pack_start (GTK_BOX (vbox3), radiobutton25, FALSE, FALSE, 0);

  frame4 = gtk_frame_new (_("Pattern:"));
  gtk_widget_ref (frame4);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "frame4", frame4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame4);
  gtk_table_attach (GTK_TABLE (table1), frame4, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame4), 5);

  entry1 = gtk_entry_new_with_max_length (255);
  gtk_widget_ref (entry1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "entry1", entry1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry1);
  gtk_container_add (GTK_CONTAINER (frame4), entry1);
  gtk_entry_set_text (GTK_ENTRY (entry1), _("screenshot"));

  frame3 = gtk_frame_new (_("Image format:"));
  gtk_widget_ref (frame3);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "frame3", frame3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame3);
  gtk_table_attach (GTK_TABLE (table1), frame3, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame3), 5);

  vbox2 = gtk_vbox_new (TRUE, 0);
  gtk_widget_ref (vbox2);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (frame3), vbox2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox2), 5);

  radiobutton18 = gtk_radio_button_new_with_label (group1_group, _("PCX (RLE compressed)"));
  group1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton18));
  gtk_widget_ref (radiobutton18);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton18", radiobutton18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton18);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton18, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton18), TRUE);

  radiobutton19 = gtk_radio_button_new_with_label (group1_group, _("XPM (Unix format)"));
  group1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton19));
  gtk_widget_ref (radiobutton19);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton19", radiobutton19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton19);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton19, FALSE, FALSE, 0);

  radiobutton20 = gtk_radio_button_new_with_label (group1_group, _("JPEG"));
  group1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton20));
  gtk_widget_ref (radiobutton20);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton20", radiobutton20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton20);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton20, FALSE, FALSE, 0);

  radiobutton21 = gtk_radio_button_new_with_label (group1_group, _("BMP (not compressed)"));
  group1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton21));
  gtk_widget_ref (radiobutton21);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "radiobutton21", radiobutton21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton21);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton21, FALSE, FALSE, 0);

  frame5 = gtk_frame_new (_("Resulting filename:"));
  gtk_widget_ref (frame5);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "frame5", frame5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame5);
  gtk_table_attach (GTK_TABLE (table1), frame5, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame5), 5);

  label3 = gtk_label_new (_("screenshot000.pcx"));
  gtk_widget_ref (label3);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "label3", label3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label3);
  gtk_container_add (GTK_CONTAINER (frame5), label3);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label3), 10, 0);

  dialog_action_area1 = GTK_DIALOG (scopt_dbox)->action_area;
  gtk_object_set_data (GTK_OBJECT (scopt_dbox), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "hbuttonbox1", hbuttonbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), hbuttonbox1, TRUE, TRUE, 0);

  ok_button = gtk_button_new ();
  gtk_widget_ref (ok_button);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "ok_button", ok_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ok_button);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), ok_button);
  GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (ok_button, "clicked", accel_group,
                              GDK_Return, 0,
                              GTK_ACCEL_VISIBLE);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (ok_button), hbox2);

  pixmap2 = create_pixmap (scopt_dbox, "yes.xpm");
  gtk_widget_ref (pixmap2);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "pixmap2", pixmap2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap2);
  gtk_box_pack_start (GTK_BOX (hbox2), pixmap2, TRUE, FALSE, 0);

  label2 = gtk_label_new ("");
  label2_key = gtk_label_parse_uline (GTK_LABEL (label2),
                                   _("_OK"));
  gtk_widget_ref (label2);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "label2", label2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (hbox2), label2, TRUE, TRUE, 0);
  gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label2), 5, 0);

  cancel_button = gtk_button_new ();
  gtk_widget_ref (cancel_button);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "cancel_button", cancel_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cancel_button);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), cancel_button);
  GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (cancel_button, "clicked", accel_group,
                              GDK_Escape, 0,
                              GTK_ACCEL_VISIBLE);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (cancel_button), hbox1);

  pixmap1 = create_pixmap (scopt_dbox, "no.xpm");
  gtk_widget_ref (pixmap1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "pixmap1", pixmap1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap1);
  gtk_box_pack_start (GTK_BOX (hbox1), pixmap1, TRUE, FALSE, 0);

  label1 = gtk_label_new ("");
  label1_key = gtk_label_parse_uline (GTK_LABEL (label1),
                                   _("_Cancel"));
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (scopt_dbox), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (hbox1), label1, TRUE, TRUE, 0);
  gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label1), 5, 0);

  gtk_signal_connect (GTK_OBJECT (scopt_dbox), "destroy",
                      GTK_SIGNAL_FUNC (on_scopt_dbox_destroy),
                      scopt_dbox);
  gtk_signal_connect (GTK_OBJECT (radiobutton22), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton22_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton23), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton23_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton24), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton24_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton25), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton25_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry1), "changed",
                      GTK_SIGNAL_FUNC (on_entry1_changed),
                      entry1);
  gtk_signal_connect (GTK_OBJECT (radiobutton18), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton18_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton19), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton19_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton20), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton20_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton21), "toggled",
                      GTK_SIGNAL_FUNC (on_scopt_radiobutton21_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (ok_button), "clicked",
                      GTK_SIGNAL_FUNC (on_scopt_ok_button_clicked),
                      scopt_dbox);
  gtk_signal_connect (GTK_OBJECT (cancel_button), "clicked",
                      GTK_SIGNAL_FUNC (on_scopt_cancel_button_clicked),
                      scopt_dbox);

  gtk_widget_add_accelerator (ok_button, "clicked", accel_group,
                              label2_key, GDK_MOD1_MASK, 0);
  gtk_widget_add_accelerator (cancel_button, "clicked", accel_group,
                              label1_key, GDK_MOD1_MASK, 0);

  gtk_window_add_accel_group (GTK_WINDOW (scopt_dbox), accel_group);

  return scopt_dbox;
}

