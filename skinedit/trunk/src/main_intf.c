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

#include "main_intf.h"

#include "main_cb.h"


GtkWidget*
create_main_window (void)
{
  GtkWidget *main_window;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *file;
  GtkWidget *file_menu;
  GtkAccelGroup *file_menu_accels;
  GtkWidget *new;
  GtkWidget *open;
  GtkWidget *save;
  GtkWidget *save_as;
  GtkWidget *separator3;
  GtkWidget *export1;
  GtkWidget *export1_menu;
  GtkAccelGroup *export1_menu_accels;
  GtkWidget *vti_v21;
  GtkWidget *vti_v25;
  GtkWidget *separator1;
  GtkWidget *exit;
  GtkWidget *edit;
  GtkWidget *edit_menu;
  GtkAccelGroup *edit_menu_accels;
  GtkWidget *properties;
  GtkWidget *separator2;
  GtkWidget *lcd_position;
  GtkWidget *key_positions;
  GtkWidget *help;
  GtkWidget *help_menu;
  GtkAccelGroup *help_menu_accels;
  GtkWidget *about;
  GtkWidget *hbox1;
  GtkWidget *handlebox1;
  GtkWidget *toolbar1;
  GtkWidget *tmp_toolbar_icon;
  GtkWidget *tb_properties;
  GtkWidget *tb_lcd_pos;
  GtkWidget *tb_key_pos;
  GtkWidget *scrolledwindow1;
  GtkWidget *viewport1;

  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (main_window), "main_window", main_window);
  gtk_window_set_title (GTK_WINDOW (main_window), _("Skinedit"));
  gtk_window_set_default_size (GTK_WINDOW (main_window), 400, 300);
  gtk_window_set_policy (GTK_WINDOW (main_window), TRUE, TRUE, FALSE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (main_window), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_ref (menubar1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menubar1", menubar1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  file = gtk_menu_item_new_with_label (_("File"));
  gtk_widget_ref (file);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "file", file,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (file);
  gtk_container_add (GTK_CONTAINER (menubar1), file);

  file_menu = gtk_menu_new ();
  gtk_widget_ref (file_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "file_menu", file_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (file), file_menu);
  file_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (file_menu));

  new = gtk_menu_item_new_with_label (_("New"));
  gtk_widget_ref (new);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "new", new,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (new);
  gtk_container_add (GTK_CONTAINER (file_menu), new);

  open = gtk_menu_item_new_with_label (_("Open"));
  gtk_widget_ref (open);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "open", open,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (open);
  gtk_container_add (GTK_CONTAINER (file_menu), open);

  save = gtk_menu_item_new_with_label (_("Save"));
  gtk_widget_ref (save);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "save", save,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (save);
  gtk_container_add (GTK_CONTAINER (file_menu), save);

  save_as = gtk_menu_item_new_with_label (_("Save as"));
  gtk_widget_ref (save_as);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "save_as", save_as,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (save_as);
  gtk_container_add (GTK_CONTAINER (file_menu), save_as);

  separator3 = gtk_menu_item_new ();
  gtk_widget_ref (separator3);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "separator3", separator3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (separator3);
  gtk_container_add (GTK_CONTAINER (file_menu), separator3);
  gtk_widget_set_sensitive (separator3, FALSE);

  export1 = gtk_menu_item_new_with_label (_("Export"));
  gtk_widget_ref (export1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "export1", export1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export1);
  gtk_container_add (GTK_CONTAINER (file_menu), export1);

  export1_menu = gtk_menu_new ();
  gtk_widget_ref (export1_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "export1_menu", export1_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (export1), export1_menu);
  export1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (export1_menu));

  vti_v21 = gtk_menu_item_new_with_label (_("VTi v2.1"));
  gtk_widget_ref (vti_v21);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "vti_v21", vti_v21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vti_v21);
  gtk_container_add (GTK_CONTAINER (export1_menu), vti_v21);

  vti_v25 = gtk_menu_item_new_with_label (_("VTi v2.5"));
  gtk_widget_ref (vti_v25);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "vti_v25", vti_v25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vti_v25);
  gtk_container_add (GTK_CONTAINER (export1_menu), vti_v25);

  separator1 = gtk_menu_item_new ();
  gtk_widget_ref (separator1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "separator1", separator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (separator1);
  gtk_container_add (GTK_CONTAINER (file_menu), separator1);
  gtk_widget_set_sensitive (separator1, FALSE);

  exit = gtk_menu_item_new_with_label (_("Exit"));
  gtk_widget_ref (exit);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "exit", exit,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (exit);
  gtk_container_add (GTK_CONTAINER (file_menu), exit);

  edit = gtk_menu_item_new_with_label (_("Edit"));
  gtk_widget_ref (edit);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "edit", edit,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit);
  gtk_container_add (GTK_CONTAINER (menubar1), edit);

  edit_menu = gtk_menu_new ();
  gtk_widget_ref (edit_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "edit_menu", edit_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (edit), edit_menu);
  edit_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (edit_menu));

  properties = gtk_menu_item_new_with_label (_("Properties"));
  gtk_widget_ref (properties);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "properties", properties,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (properties);
  gtk_container_add (GTK_CONTAINER (edit_menu), properties);

  separator2 = gtk_menu_item_new ();
  gtk_widget_ref (separator2);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "separator2", separator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (separator2);
  gtk_container_add (GTK_CONTAINER (edit_menu), separator2);
  gtk_widget_set_sensitive (separator2, FALSE);

  lcd_position = gtk_menu_item_new_with_label (_("LCD position"));
  gtk_widget_ref (lcd_position);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "lcd_position", lcd_position,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (lcd_position);
  gtk_container_add (GTK_CONTAINER (edit_menu), lcd_position);

  key_positions = gtk_menu_item_new_with_label (_("Key positions"));
  gtk_widget_ref (key_positions);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "key_positions", key_positions,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (key_positions);
  gtk_container_add (GTK_CONTAINER (edit_menu), key_positions);

  help = gtk_menu_item_new_with_label (_("Help"));
  gtk_widget_ref (help);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "help", help,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (help);
  gtk_container_add (GTK_CONTAINER (menubar1), help);
  gtk_menu_item_right_justify (GTK_MENU_ITEM (help));

  help_menu = gtk_menu_new ();
  gtk_widget_ref (help_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "help_menu", help_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (help), help_menu);
  help_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (help_menu));

  about = gtk_menu_item_new_with_label (_("About"));
  gtk_widget_ref (about);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "about", about,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (about);
  gtk_container_add (GTK_CONTAINER (help_menu), about);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  handlebox1 = gtk_handle_box_new ();
  gtk_widget_ref (handlebox1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "handlebox1", handlebox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (handlebox1);
  gtk_box_pack_start (GTK_BOX (hbox1), handlebox1, FALSE, TRUE, 0);
  gtk_handle_box_set_handle_position (GTK_HANDLE_BOX (handlebox1), GTK_POS_TOP);

  toolbar1 = gtk_toolbar_new (GTK_ORIENTATION_VERTICAL, GTK_TOOLBAR_BOTH);
  gtk_widget_ref (toolbar1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "toolbar1", toolbar1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (toolbar1);
  gtk_container_add (GTK_CONTAINER (handlebox1), toolbar1);

  tmp_toolbar_icon = create_pixmap (main_window, "prop.xpm");
  tb_properties = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Properties"),
                                NULL, NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (tb_properties);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "tb_properties", tb_properties,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tb_properties);

  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar1));

  tmp_toolbar_icon = create_pixmap (main_window, "screen.xpm");
  tb_lcd_pos = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("LCD"),
                                NULL, NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (tb_lcd_pos);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "tb_lcd_pos", tb_lcd_pos,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tb_lcd_pos);

  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar1));

  tmp_toolbar_icon = create_pixmap (main_window, "keyboard.xpm");
  tb_key_pos = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Keys"),
                                NULL, NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (tb_key_pos);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "tb_key_pos", tb_key_pos,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tb_key_pos);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "scrolledwindow1", scrolledwindow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (hbox1), scrolledwindow1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_ref (viewport1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "viewport1", viewport1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), viewport1);

  sdl_eventbox = gtk_event_box_new ();
  gtk_widget_ref (sdl_eventbox);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "sdl_eventbox", sdl_eventbox,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (sdl_eventbox);
  gtk_container_add (GTK_CONTAINER (viewport1), sdl_eventbox);

  statusbar = gtk_statusbar_new ();
  gtk_widget_ref (statusbar);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "statusbar", statusbar,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (statusbar);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (main_window), "delete_event",
                      GTK_SIGNAL_FUNC (on_main_window_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (main_window), "destroy_event",
                      GTK_SIGNAL_FUNC (on_main_window_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (new), "activate",
                      GTK_SIGNAL_FUNC (on_new_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (open), "activate",
                      GTK_SIGNAL_FUNC (on_open_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (save), "activate",
                      GTK_SIGNAL_FUNC (on_save_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (save_as), "activate",
                      GTK_SIGNAL_FUNC (on_save_as_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (vti_v21), "activate",
                      GTK_SIGNAL_FUNC (on_vti_v21_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (vti_v25), "activate",
                      GTK_SIGNAL_FUNC (on_vti_v25_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (exit), "activate",
                      GTK_SIGNAL_FUNC (on_exit_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (properties), "activate",
                      GTK_SIGNAL_FUNC (on_properties_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (lcd_position), "activate",
                      GTK_SIGNAL_FUNC (on_lcd_position_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (key_positions), "activate",
                      GTK_SIGNAL_FUNC (on_key_positions_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (about), "activate",
                      GTK_SIGNAL_FUNC (on_about_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (tb_properties), "clicked",
                      GTK_SIGNAL_FUNC (on_tb_properties_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (tb_lcd_pos), "clicked",
                      GTK_SIGNAL_FUNC (on_tb_lcd_pos_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (tb_key_pos), "clicked",
                      GTK_SIGNAL_FUNC (on_tb_key_pos_clicked),
                      NULL);

  gtk_widget_add_events(GTK_WIDGET(sdl_eventbox),
			GDK_BUTTON_PRESS_MASK |  GDK_BUTTON_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);

  return main_window;
}


GtkWidget*
create_filesel (void)
{
  GtkWidget *filesel;
  GtkWidget *filesel_ok;
  GtkWidget *filesel_cancel;

  filesel = gtk_file_selection_new (_("Select File"));
  gtk_object_set_data (GTK_OBJECT (filesel), "filesel", filesel);
  gtk_container_set_border_width (GTK_CONTAINER (filesel), 10);
  gtk_window_set_modal (GTK_WINDOW (filesel), TRUE);

  filesel_ok = GTK_FILE_SELECTION (filesel)->ok_button;
  gtk_object_set_data (GTK_OBJECT (filesel), "filesel_ok", filesel_ok);
  gtk_widget_show (filesel_ok);
  GTK_WIDGET_SET_FLAGS (filesel_ok, GTK_CAN_DEFAULT);

  filesel_cancel = GTK_FILE_SELECTION (filesel)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (filesel), "filesel_cancel", filesel_cancel);
  gtk_widget_show (filesel_cancel);
  GTK_WIDGET_SET_FLAGS (filesel_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (filesel), "delete_event",
                      GTK_SIGNAL_FUNC (on_filesel_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (filesel), "destroy_event",
                      GTK_SIGNAL_FUNC (on_filesel_delete_destroy_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (filesel_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_filesel_cancel_clicked),
                      NULL);

  return filesel;
}

