#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "bkpt_cause.h"
#include "support.h"
//#include "platform.h"
#include "struct.h"
#include "interface.h"
#include "bkpts.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

/*static*/ void
bc_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


/*static*/ void
on_bc_dbox_destroy                    (GtkObject       *object,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void display_bkpt_cause()
{
  GtkWidget *bc_dbox;
  GtkWidget *dialog_vbox1;
  GtkWidget *label2;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbuttonbox1;
  GtkWidget *button1;
  GtkWidget *hbox1;
  GtkWidget *pixmap1;
  GtkWidget *label1;
  GtkAccelGroup *accel_group;

  gint type, id, mode;
  gchar buffer[256];
  gchar *sType = "Unknown type -> bug !";
  gchar *sMode = "Unknown type -> bug !";


  /* */
  // Get breakpoint cause
  ti68k_getBreakpointCause(&type, &id, &mode);
  sprintf(buffer, "Bkpt has been encountered at $%06x!\nType=%i, id=%i, mode=%i\n", ti68k_getPcRegister(), type, id, mode);
  fprintf(stderr, buffer);

  // Process type
  switch(type)
    {
    case 0:
      sType = "None";
      sMode = "None";
      break;
    case BK_CAUSE_ACCESS:
      sType = "Access";
      switch(mode)
	{
	case BK_READ_BYTE:
	  sMode = "Byte (read)";
	  break;
	case BK_READ_WORD:
	  sMode = "Word (read)";
	  break;
	case BK_READ_LONG:
	  sMode = "Long (read)";
	  break;
	case BK_WRITE_BYTE:
	  sMode = "Byte (write)";
	  break;
	case BK_WRITE_WORD:
	  sMode = "Word (write)";
	  break;
	case BK_WRITE_LONG:
	  sMode = "Long (write)";
	  break;
	default:
	  sMode = "Unknown -> bug !";
	  break;
	}
      break;
    case BK_CAUSE_ACCESS_RANGE:
      sType = "Access range";
      break;
    case BK_CAUSE_ADDRESS:
      sType = "Address";
      sMode = "None";
      break;
    case BK_CAUSE_VECTOR:
      sType = "Vector";
      switch(mode)
	{
	case BK_BUS_ERROR: sMode = "Bus error"; break;
	case BK_ADDRESS_ERROR: sMode = "Address error"; break;
	case BK_ILLEGAL_INSTRUCTION: sMode = "Illegal instruction"; break;
	case BK_ZERO_DIVIDE: sMode = "Division by zero"; break;
	case BK_CHK_INSTRUCTION: sMode = "CHK instruction"; break;
	case BK_TRAPV_INSTRUCTION: sMode = "TRAPV instruction"; break;
	case BK_PRIVILEGE_VIOLATION: sMode = "Privilege violation"; break;
	case BK_TRACE: sMode = "Trace"; break;
	case BK_LINE_1010: sMode = "Line 1010 emulator"; break;
	case BK_LINE_1111: sMode = "Line 1111 emulator"; break;
	case BK_NONINIT_INTERRUPT: sMode = "Non initialized interruption"; break;
	}
      break;
    case BK_CAUSE_TRAP:
      sType = "Trap";
      switch(mode)
        {
        case BK_TRAP_0: sMode = "Trap #0"; break;
	case BK_TRAP_1: sMode = "Trap #1 (int mask)"; break;
	case BK_TRAP_2: sMode = "Trap #2 reset)"; break;
	case BK_TRAP_3: sMode = "Trap #3"; break;
	case BK_TRAP_4: sMode = "Trap #4 (on/off)"; break;
	case BK_TRAP_5: sMode = "Trap #5"; break;
	case BK_TRAP_6: sMode = "Trap #6"; break;
	case BK_TRAP_7: sMode = "Trap #7"; break;
	case BK_TRAP_8: sMode = "Trap #8"; break;
	case BK_TRAP_9: sMode = "Trap #9"; break;
	case BK_TRAP_A: sMode = "Trap #10 (self test)"; break;
	case BK_TRAP_B: sMode = "Trap #11 (archive)"; break;
	case BK_TRAP_C: sMode = "Trap #12"; break;
	case BK_TRAP_D: sMode = "Trap #13"; break;
	case BK_TRAP_E: sMode = "Trap #14"; break;
	case BK_TRAP_F: sMode = "Trap #15 (ER_throw)"; break;
      break;
	}
      break;
    case BK_CAUSE_AUTOINT:
      sType = "Auto-int";
      switch(mode)
        {
        case BK_SPURIOUS: sMode = "Spurious interrupt"; break;
	case BK_AUTOINT_1: sMode = "Auto-int 1 (350Hz timer)"; break;
	case BK_AUTOINT_2: sMode = "Auto-int 2 (keyboard)"; break;
	case BK_AUTOINT_3: sMode = "Auto-int 3"; break;
	case BK_AUTOINT_4: sMode = "Auto-int 4 (link)"; break;
	case BK_AUTOINT_5: sMode = "Auto-int 5 (prog timer)"; break;
	case BK_AUTOINT_6: sMode = "Auto-int 6 (on key)"; break;
	case BK_AUTOINT_7: sMode = "Auto-int 7 (protected mem)"; break;  
	}
      break;
    default:
      sType = "Unknown type -> bug !";
      break;
    }

  sprintf(buffer, _("Bkpt has been encountered at $%06x\nType: %s\nId: #%i\nMode: %s"), ti68k_getPcRegister(), sType, id, sMode);

  /* */
  accel_group = gtk_accel_group_new ();

  bc_dbox = gtk_dialog_new ();
  GLADE_HOOKUP_OBJECT_NO_REF(bc_dbox, bc_dbox, "bc_dbox");
  gtk_window_set_title (GTK_WINDOW (bc_dbox), _("Information"));
  gtk_window_set_position (GTK_WINDOW (bc_dbox), GTK_WIN_POS_CENTER);
  gtk_window_set_modal (GTK_WINDOW (bc_dbox), TRUE);

  dialog_vbox1 = GTK_DIALOG (bc_dbox)->vbox;
  GLADE_HOOKUP_OBJECT_NO_REF(bc_dbox, dialog_vbox1, "dialog_vbox1");
  gtk_widget_show (dialog_vbox1);

  label2 = gtk_label_new (buffer);
  GLADE_HOOKUP_OBJECT(bc_dbox, label2, "label2");
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_FILL);
  gtk_label_set_line_wrap (GTK_LABEL (label2), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label2), 5, 5);

  dialog_action_area1 = GTK_DIALOG (bc_dbox)->action_area;
  GLADE_HOOKUP_OBJECT_NO_REF(bc_dbox, dialog_action_area1, "dialog_action_area1");
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  hbuttonbox1 = gtk_hbutton_box_new ();
  GLADE_HOOKUP_OBJECT(bc_dbox, hbuttonbox1, "hbuttonbox1");
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), hbuttonbox1, TRUE, TRUE, 0);

  button1 = gtk_button_new ();
  GLADE_HOOKUP_OBJECT(bc_dbox, button1, "button1");
  gtk_widget_show (button1);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button1);
  GTK_WIDGET_SET_FLAGS (button1, GTK_CAN_DEFAULT);

  hbox1 = gtk_hbox_new (FALSE, 0);
  GLADE_HOOKUP_OBJECT(bc_dbox, hbox1, "hbox1");
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (button1), hbox1);

  pixmap1 = create_pixmap (bc_dbox, "yes.xpm");
  GLADE_HOOKUP_OBJECT(bc_dbox, pixmap1, "pixmap1");
  gtk_widget_show (pixmap1);
  gtk_box_pack_start (GTK_BOX (hbox1), pixmap1, TRUE, FALSE, 0);

  label1 = gtk_label_new_with_mnemonic(_("_OK"));
  GLADE_HOOKUP_OBJECT(bc_dbox, label1, "label1");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (hbox1), label1, FALSE, TRUE, 0);
  gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label1), 5, 0);

  g_signal_connect ((gpointer)bc_dbox, "destroy",
		    G_CALLBACK(on_bc_dbox_destroy),
		    bc_dbox);
  g_signal_connect ((gpointer)button1, "clicked",
		    G_CALLBACK(bc_ok_button_clicked),
		    bc_dbox);

  gtk_window_add_accel_group (GTK_WINDOW (bc_dbox), accel_group);

  //return bc_dbox;
  gtk_widget_show_all(bc_dbox);
}

