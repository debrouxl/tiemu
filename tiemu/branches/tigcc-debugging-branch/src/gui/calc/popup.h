#include <gtk/gtk.h>

#include "support.h"

GtkWidget* display_popup_menu(void);

GLADE_CB void
on_popup_menu_cancel                   (GtkMenuShell    *menushell,
                                        gpointer         user_data);

GLADE_CB gboolean
on_popup_menu_button_press_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

GLADE_CB gboolean
on_popup_menu_key_press_event          (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

GLADE_CB void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_send_file_to_tiemu1_activate     (GtkMenuItem     *menuitem,
                                     gpointer         user_data);

GLADE_CB void
on_recv_file_from_tiemu1_activate     (GtkMenuItem     *menuitem,
                                       gpointer         user_data);

GLADE_CB void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_save_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_load_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_load_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_sync1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_number_of_colors1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_2_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_4_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_7_colors1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_blurry1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_normal_view__1x_1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_large_view__x2_1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_full_screen1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_none1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_set_skin1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_now__1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_rbm_options1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_help3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_manpage1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_changelog1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_infos1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_exit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

GLADE_CB void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
