#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "link_cb.h"
#include "link_dbox.h"
#include "support.h"

#include "tilibs.h"
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "msg_cb.h"
#include "interface.h"
#include "main.h"

static gint  link_type;
static gchar device[MAXCHARS];
static guint io_addr;
static gint  port;
static gint  method;

static gint custom_toggled = 0;
static gint prev_link_type = 0;

gpointer wentry1 = NULL;
gpointer wentry2 = NULL;


static void s2v(void)
{
  port = (options.link_cable)->port;
  method = ticable_get_method();
  link_type = (options.link_cable)->link_type;
  io_addr = ticable_get_io_address();
  strcpy(device, ticable_get_io_device());
}

gint display_linktype_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_linktype_dbox();
  s2v();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton1");
  if(link_type == LINK_TGL)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton2");
  if(link_type == LINK_PAR)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton3");
  if(link_type == LINK_TIE)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton4");
  if(link_type == LINK_TPU)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton5");
  if(link_type == LINK_SER)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton6");
  if(link_type == LINK_AVR)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton7");
  if(link_type == LINK_VTI)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "radiobutton8");
  if(link_type == LINK_UGL)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (user_data), TRUE);

  s2v();

  gtk_widget_show_all(dbox);

  return 0;
}

/************************************/
/* Called when the box is displayed */
/************************************/


void
on_linktype_dbox_show20                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  //DISPLAY("show20\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Parallel port #1");
      if(port == PARALLEL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(GTK_WIDGET(GTK_BIN(user_data)->child));
      gtk_widget_show(user_data);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Serial port #1");
      if(port == SERIAL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TIE:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Virtual port #1 (emu)");
      if(port == VIRTUAL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_VTI:
#ifdef __WIN32__
      gtk_widget_hide(user_data);
      break;
#endif
    case LINK_VTL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Virtual port #1 (emu)");
      if(port == VIRTUAL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TPU:
      gtk_widget_hide(user_data);
      /*
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "'raw' mode");
      if(io_addr == TPU_RAW_MODE)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      */
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child),
                         "USB port #1");
      if(port == USB_PORT_1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(GTK_WIDGET(GTK_BIN(user_data)->child));
      gtk_widget_show(user_data);
      break;
    }
}


void
on_linktype_dbox_show21                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  //DISPLAY("show21\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Parallel port #2");
      if(port == PARALLEL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Serial port #2");
      if(port == SERIAL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TIE:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Virtual port #2 (tilp)");
      if(port == VIRTUAL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_VTI:
#ifdef __WIN32__
      gtk_widget_hide(user_data);
      break;
#endif
    case LINK_VTL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Virtual port #2 (tilp)");
      if(port == VIRTUAL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TPU:
      gtk_widget_hide(user_data);
      /*
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "'cooked' mode");
      if(io_addr == TPU_COOKED_MODE)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      */
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child),
                         "USB port #2");
      if(port == USB_PORT_2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(GTK_WIDGET(GTK_BIN(user_data)->child));
      gtk_widget_show(user_data);
      break;
    }
}


void
on_linktype_dbox_show22                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  //DISPLAY("show22\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Parallel port #3");
      if(port == PARALLEL_PORT_3)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Serial port #3");
      if(port == SERIAL_PORT_3)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TIE:
      gtk_widget_hide(user_data);
      break;
    case LINK_VTI:
      gtk_widget_hide(user_data);
      break;
    case LINK_VTL:
      gtk_widget_hide(user_data);
      break;
    case LINK_TPU:
      gtk_widget_hide(user_data);
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child),
                         "USB port #3");
      if(port == USB_PORT_3)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(GTK_WIDGET(GTK_BIN(user_data)->child));
      gtk_widget_show(user_data);
      break;
    }
}


void
on_linktype_dbox_show23                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  //DISPLAY("show23\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_widget_hide(user_data);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child), 
			 "Serial port #4");
      if(port == SERIAL_PORT_4)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TIE:
      gtk_widget_hide(user_data);
      break;
    case LINK_VTI:
      gtk_widget_hide(user_data);
      break;
    case LINK_VTL:
      gtk_widget_hide(user_data);
      break;
    case LINK_TPU:
      gtk_widget_hide(user_data);
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(user_data)->child),
                         "USB port #4");
      if(port == USB_PORT_4)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(GTK_WIDGET(GTK_BIN(user_data)->child));
      gtk_widget_show(user_data);
      break;
    }
}


void
on_linktype_dbox_show24                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  //DISPLAY("show24\n");
  switch(link_type)
    {
    case LINK_PAR:
      if(port == USER_PORT)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      if(port == USER_PORT)
	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    case LINK_TIE:
      gtk_widget_hide(user_data);
      break;
    case LINK_VTI:
      gtk_widget_hide(user_data);
      break;
    case LINK_VTL:
      gtk_widget_hide(user_data);
      break;
    case LINK_TPU:
      gtk_widget_hide(user_data);
      break;
    case LINK_UGL:
      if(port == USER_PORT)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (user_data), TRUE);
      gtk_widget_show(user_data);
      break;
    }
  //DISPLAY("port = %i\n", port);
}


void
on_linktype_dbox_show30                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gchar buffer[MAXCHARS];

  wentry1 = user_data;
  g_snprintf(buffer, MAXCHARS, "0x%03x", io_addr);

  gtk_entry_set_visibility(GTK_ENTRY(user_data), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(user_data), custom_toggled ? TRUE : FALSE);
  gtk_entry_set_text(GTK_ENTRY(user_data), buffer);
}


void
on_linktype_dbox_show31                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gchar buffer[MAXCHARS];

  wentry2 = user_data;
  g_snprintf(buffer, MAXCHARS, "%s", device);

  gtk_entry_set_visibility(GTK_ENTRY(user_data), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(user_data), custom_toggled ? TRUE : FALSE);
  gtk_entry_set_text(GTK_ENTRY(user_data), buffer);
}


/***********************************/
/* Called whenever a char is typed */
/***********************************/

void
linktype_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;
  gint result;

  s = gtk_editable_get_chars(GTK_EDITABLE(user_data), 0, -1);
  result = sscanf(s, "0x%03x", &io_addr);
  if(result < 1)
    io_addr = AUTO_ADDR;
}


void
linktype_entry2_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;
  gint result;

  s = gtk_editable_get_chars(GTK_EDITABLE(user_data), 0, -1);
  result = sscanf(s, "%s", device);
  if(result < 1)
    strcpy(device, AUTO_NAME);
}


/*******************/
/* Link cable type */
/*******************/

void
linktype_radiobutton1_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r1\n");
  link_type = LINK_TGL;
  //if(link_type != prev_link_type)
  //port = SERIAL_PORT_2;
  
  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton2_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r2\n");
  link_type = LINK_PAR;
  //if(link_type != prev_link_type)
  //port = PARALLEL_PORT_1;

  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton3_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r3\n");
  link_type = LINK_TIE;
  //if(link_type != prev_link_type)
  //port = VIRTUAL_PORT_2;

  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton4_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r4\n");
  link_type = LINK_TPU;

  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton5_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r5\n");
  link_type = LINK_SER;
  //if(link_type != prev_link_type)
  //port = SERIAL_PORT_2;

  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton6_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r6\n");
  link_type = LINK_AVR;
  //if(link_type != prev_link_type)
  //port = SERIAL_PORT_2;
 
  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton7_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r7\n");
  link_type = LINK_VTI;
  //if(link_type != prev_link_type)  
  //port = VIRTUAL_PORT_2;
 
  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}

void
linktype_radiobutton8_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r8\n");
  link_type = LINK_UGL;
  //if(link_type != prev_link_type)
  //port = USB_PORT_1;
  
  if(gtk_toggle_button_get_active(togglebutton))
    gtk_signal_emit_by_name(GTK_OBJECT(user_data), "show");
}


/************/
/* I/O port */
/************/


void
linktype_radiobutton10_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r10\n");
  switch(link_type)
    {
    case LINK_PAR:
      port = PARALLEL_PORT_1;
      break;
    case LINK_SER:
    case LINK_AVR:
    case LINK_TGL:
      port = SERIAL_PORT_1;
      break;
    case LINK_VTL:
      port = VIRTUAL_PORT_1;
      break;
    case LINK_VTI:
      port = VIRTUAL_PORT_1;
      break;
    case LINK_TIE:
      port = VIRTUAL_PORT_1;
      break;
    case LINK_TPU:
      //io_addr = TPU_RAW_MODE;
      break;
    case LINK_UGL:
      port = USB_PORT_1;
      break;
    }
}


void
linktype_radiobutton11_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{ 
  //DISPLAY("r11\n");
  switch(link_type)
    {
    case LINK_PAR:
      port = PARALLEL_PORT_2;
      break;
    case LINK_SER:
    case LINK_AVR:
    case LINK_TGL:
      port = SERIAL_PORT_2;
      break;
    case LINK_VTL:
      port = VIRTUAL_PORT_2;
      break;
    case LINK_VTI:
      port = VIRTUAL_PORT_2;
      break;
    case LINK_TIE:
      port = VIRTUAL_PORT_2;
      break;
    case LINK_TPU:
      //io_addr = TPU_COOKED_MODE;
      break;
    case LINK_UGL:
      port = USB_PORT_2;
      break;
    }
}


void
linktype_radiobutton12_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{  
  //DISPLAY("r12\n");
  switch(link_type)
    {
    case LINK_PAR:
      port = PARALLEL_PORT_3;
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      port = SERIAL_PORT_3;
      break;
    case LINK_VTL:
      break;
    case LINK_VTI:
      break;
    case LINK_TIE:
      break;
    case LINK_TPU:
      break;
    case LINK_UGL:
      port = USB_PORT_3;
      break;
    }
}


void
linktype_radiobutton13_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r13\n");
  switch(link_type)
    {
    case LINK_PAR:
      break;
    case LINK_SER:
    case LINK_AVR:
    case LINK_TGL:
      port = SERIAL_PORT_4;
      break;
    case LINK_VTL:
      break;
    case LINK_VTI:
      break;
    case LINK_TIE:
      break;
    case LINK_TPU:
      break;
    case LINK_UGL:
      port = USB_PORT_4;
      break;
    }
}


void
linktype_radiobutton14_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  //DISPLAY("r14\n");
  custom_toggled = gtk_toggle_button_get_active(togglebutton);

  if(custom_toggled)
    port = USER_PORT;

  gtk_entry_set_visibility(GTK_ENTRY(wentry1), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(wentry1), custom_toggled ? TRUE : FALSE);
  gtk_entry_set_visibility(GTK_ENTRY(wentry2), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(wentry2), custom_toggled ? TRUE : FALSE);
}


/*************/
/* Ok/Cancel */
/*************/

void
linktype_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));

  (options.link_cable)->method = method;
  (options.link_cable)->port = port;
  (options.link_cable)->link_type = link_type;
  (options.link_cable)->io_addr = io_addr;
  strcpy((options.link_cable)->device, device);

  ti68k_reconfigure_linkport();

  unhalt();
}


void
linktype_cancel_button_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}

void
on_linktype_dbox_show                  (GtkWidget       *widget,
                                        gpointer         user_data)
{
  //DISPLAY("show\n");
  if(link_type != prev_link_type)
    {
      switch(link_type)
	{
	case LINK_SER:
	case LINK_AVR:
	case LINK_TGL: port = SERIAL_PORT_2;
	  break;
	case LINK_PAR: port = PARALLEL_PORT_1;
	  break;
	case LINK_TIE:
	case LINK_VTI: port = VIRTUAL_PORT_2;
	  break;
	case LINK_UGL: port = USB_PORT_1;
	  break;
	default:
	  break;
	}
      prev_link_type = link_type;
    }
}

void
on_linktype_button_fs_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_show(create_fileselection_device());
}


void
on_seldev_ok_button1_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data));
  gtk_entry_set_text(GTK_ENTRY(wentry2), s);

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_seldev_cancel_button1_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_fileselection_device_show           (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(GTK_EDITABLE(wentry2), 0, -1);
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(user_data), s);
}


void
on_button2_pressed                     (GtkButton       *button,
                                        gpointer         user_data)
{
  //display_probeio_dbox();
}

/* */


