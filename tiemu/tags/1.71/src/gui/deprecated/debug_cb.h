#include <gtk/gtk.h>

gint display_debugger_dbox(void);
gint display_code_popup_menu (void);
gint display_memory_popup_menu (void);
gint display_gotocode_dbox (void);
gint display_gotodata_dbox(void);

void
on_debugger_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data);

void
on_run1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_step1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_step_over1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_run_to_cursor1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_break1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_exit2_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_breakpoint_at_selection1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_breakpoint_on_program_entry1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_breakpoint_on_trap_exception1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_data_breakpoint1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_clear_all_breakpoints1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_log1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_view_log1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_edit_logged_addresses1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_log_link_port_send1_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_log_link_port_receive1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_clear_all1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_enable_logging1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_clist1_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_clist2_key_press_event              (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_clist2_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_go_to_address1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_go_to_pc1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_breakpoint1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_pc_to_selection1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_force_refresh1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_edit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry10_changed                     (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_options1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_clist2_event                        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_go_to1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry11_changed                     (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button46_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button47_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button99_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_gotocode_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data);

void
on_gotodata_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data);

void 
on_code_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data);
