#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <ctype.h>
#include <gtk/gtk.h>
#include <gmodule.h> // GModule

#include "platform.h"
#include "tilibs.h"
#include "interface.h"

#include "struct.h"
#include "ticalc.h"
#include "extern.h"
#include "rcfile.h"
#include "version.h"

#include "about_dbox.h"
#include "about_cb.h"
#include "adbkpt_dbox.h"
#include "adbkpt_cb.h"
#include "colors_dbox.h"
#include "colors_cb.h"
#include "dbkpt_dbox.h"
#include "dbkpt_cb.h"
#include "debug_dbox.h"
#include "debug_cb.h"
#include "itick_dbox.h"
#include "itick_cb.h"
#include "link_dbox.h"
#include "link_cb.h"
#include "logaddr_dbox.h"
#include "logaddr_cb.h"
#include "msg_dbox.h"
#include "msg_cb.h"
#include "romv_dbox.h"
#include "romv_cb.h"
#include "trap_dbox.h"
#include "trap_cb.h"
#include "viewlog_dbox.h"
#include "viewlog_cb.h"
#include "dbgopt_dbox.h"
#include "dbgopt_cb.h"
#include "infos_dbox.h"
#include "infos_cb.h"
#include "pgmentbkpt_dbox.h"
#include "pgmentbkpt_cb.h"
#include "state_dbox.h"
#include "state_cb.h"
#include "bkpt_cause.h"
#include "dump_dbox.h"
#include "dump_cb.h"
#include "scopt_dbox.h"
#include "scopt_cb.h"
#include "user_dbox.h"
#include "user_cb.h"
#include "fsel_dbox.h"
#include "fsel_cb.h"
#include "help_dbox.h"
#include "help_cb.h"
#include "about_dbox.h"
#include "about_cb.h"
#include "manpage_dbox.h"
#include "manpage_cb.h"
#include "startup_dbox.h"
#include "startup_cb.h"
#include "thanks_dbox.h"
#include "thanks_cb.h"
#include "tiemu_dbox.h"
#include "skin_cb.h"
#include "main.h"

/*
  Display the GTK popup menu and configure some items
*/
GtkWidget* display_popup_menu()
{
  GtkWidget *menu;
  gpointer user_data;
  gchar *s;
  GtkCheckMenuItem *i; 
  
  menu = create_popup_menu();

  user_data = gtk_object_get_data(GTK_OBJECT(menu), "popup_menu_header");
  s = g_strdup_printf("TiEmu, version %s", GTKTIEMU_VERSION);
  gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), s);
  g_free(s);

  user_data = gtk_object_get_data(GTK_OBJECT(menu), "sync1");
  i = (GtkCheckMenuItem *) user_data;
  gtk_check_menu_item_set_show_toggle(i, TRUE);
  gtk_check_menu_item_set_active(i, (options.params)->sync_one);

  user_data = gtk_object_get_data(GTK_OBJECT(menu), "restrict1");
  i = (GtkCheckMenuItem *) user_data;
  gtk_check_menu_item_set_show_toggle(i, TRUE);
  gtk_check_menu_item_set_active(i, !((options.params)->background));

  user_data = gtk_object_get_data(GTK_OBJECT(menu), "skin1");
  i = (GtkCheckMenuItem *) user_data;
  gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child),
  	     g_basename(options.skin_file));
  
  if(ti68k_getCalcType() & TI92)
    gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), ("Skin 92"));
  else
    gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), ("Skin 89"));

  user_data = gtk_object_get_data(GTK_OBJECT(menu), "none1");
  i = (GtkCheckMenuItem *) user_data;

  return menu;
}

/*************/
/* Callbacks */
/*************/

/* configure event */
gboolean
on_popup_menu_configure_event          (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  DISPLAY("configure\n");
  unhalt();
  return FALSE;
}


/* cancel */
void
on_popup_menu_cancel                   (GtkMenuShell    *menushell,
                                        gpointer         user_data)
{
  // call when you press ESC during popup menu
  DISPLAY("cancel\n");
  unhalt();
}

/* deactivate */
void
on_popup_menu_deactivate               (GtkMenuShell    *menushell,
                                        gpointer         user_data)
{
  // bug if we click outside of the rbm menu: we need to unhalt the emulation
  // core although a menu item could have been clicked.
  //DISPLAY("deactivate !\n");
  //unhalt();
}


void
on_popup_menu_header                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_send_file_to_gtktiemu1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  //test_sendfile();
  display_tifile_fileselection();
}


void
on_link_cable1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_linktype_dbox();
}


void
on_save_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  write_rc_file();

#if defined(__LINUX__)
  msg_box(_("Information"), 
	  _("Configuration file saved (in ~/.tiemu)."));
#elif defined(__WIN32__)
  msg_box(_("Information"), 
	  _("Configuration file saved (in tiemu.ini)."));
#endif
}


void
on_load_config1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  read_rc_file();

  msg_box(_("Information"), 
	  _("Configuration file loaded."));
}


void
on_load_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_fs_load_state();
}


void
on_save_state_image1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_fs_save_state();
}


void
on_revert_to_saved_state1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  ti68k_loadState((options.params)->ram_file);
  unhalt();
}


/* Menu item has launched the debugger */
void
on_enter_debugger1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint i;
  CODE_BKPT *s;

  // Suspend emulation core
  halt();
/*
#ifdef __WIN32__
	msg_box("Information",
		"The debugger crashes in the Win32 version so it is has been temporarily disabled. It will be available in a next release.\n");
	unhalt();
	return ;
#endif
*/
  
  // Remove all breakpoints so that we can 'single step'
  // Bkpts are restored when the debugger is exited
  for(i=0; i<g_list_length(bkpt_address_list); i++)
    {
      s = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
      
      ti68k_delBreakpointAddress(s->id);
    }

  // Launch debugger
  display_debugger_dbox();
}

int enter_gtk_debugger(void)
{
  on_enter_debugger1_activate(NULL, NULL);  

  return 0;
}


void
on_reset_calc1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  ti68k_reset();
  unhalt();
}


void
on_restrict_to_actual_speed1_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /*
  msg_box(_("Information"), 
	  _("This function is not available yet."));
  */
}


void
on_change_speed1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_tickrate_dbox();
}


void
on_cycle_rate1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_cyclerate_dbox();
}


void
on_itick1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_itick_dbox();
}


void
on_sync1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    (options.params)->sync_one = 0;
  else
    (options.params)->sync_one = 1;

  unhalt();
}

void
on_number_of_colors1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_ncolors_dbox();
}


void
on_normal_view__1x_1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  hid_switch_normal_view();
  unhalt();
}


void
on_large_view__x2_1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  hid_switch_large_view();
  unhalt();
}


void
on_full_screen1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  hid_switch_fullscreen();
  unhalt();
}


void
on_skin1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  hid_switch_with_skin();
  unhalt();
}


void
on_none1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  hid_switch_without_skin();
  unhalt();
}

void
on_set_skin1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_skinlist_dbox();
}


void
on_now__1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  do_screenshot(options.img_format, options.img_type, 
		options.img_size, NULL);
  unhalt();
}


void
on_rbm_options1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_scopt_dbox();
}


void
on_dump_ram1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_ramdump_fileselection();
}


void
on_dump_rom1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_romdump_fileselection();
}


void
on_rom_file1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_rom_fileselection ();
}


void
on_set_rom1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_romversion_dbox ();
}


void
on_help3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_help_dbox();
}


void
on_manpage1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_manpage_dbox();
}


void
on_changelog1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_startup_dbox();
}


void
on_thanks1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_thanks_dbox();
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_about_dbox ();
}


void
on_infos1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_infos_dbox();
}


void
on_exit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  ti68k_saveState((options.params)->ram_file);
  if(!is_rcfile_exist())
    write_rc_file();

  ti68k_exit();
  exit(0);
}


void
on_exit_without_saving_state1_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  ti68k_exit();
  exit(0);
}


/**/
