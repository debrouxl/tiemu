#include <gtk/gtk.h>

GtkWidget* display_popup_menu();

void refresh_breakpoints(GtkWidget *widget);
int enter_gtk_debugger(void);

gboolean
on_popup_menu_configure_event          (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

void
on_popup_menu_show                     (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_popup_menu_cancel                   (GtkMenuShell    *menushell,
                                        gpointer         user_data);

void
on_popup_menu_deactivate               (GtkMenuShell    *menushell,
                                        gpointer         user_data);

void
on_popup_menu_show_skin1               (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_popup_menu_show_none                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_popup_menu_show_sync1               (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_popup_menu_show_restrict1           (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_send_file_to_gtktiemu1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_command_shell1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_load_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_program_entry_breakpoint1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_load_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_emulator_options1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_change_speed1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_cycle_rate1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_itick1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_sync1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_number_of_colors1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_dark___light_colors1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_redefine_keys1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_xtiger_keys1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_vti_keys1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_none1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_now__1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_rbm_options1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_dump_ram1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_dump_rom1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_rom_file1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_update_file1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_convert_into_rom1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_help3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_manpage1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_changelog1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_thanks1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_infos1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_exit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_menu_show_restrict1           (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_single1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_group1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_normal_view__1x_1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_large_view__x2_1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_full_screen1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_change_skin1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_skin1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
