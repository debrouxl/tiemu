#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

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

static GtkWidget *wentry2 = NULL;

static void
s2v(void)
{
  port = (options.link_cable)->port;
  method = ticable_get_method();
  link_type = (options.link_cable)->link_type;
  io_addr = ticable_get_io_address();
  strcpy(device, ticable_get_io_device());
}


gint display_linktype_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *w = NULL;

  dbox = create_linktype_dbox();
  s2v();
  
  gtk_widget_show_all(dbox);

  switch (link_type)
    {
      case LINK_TGL:
	w = lookup_widget(dbox, "radiobutton1");
	break;
      case LINK_PAR:
	w = lookup_widget(dbox, "radiobutton2");
	break;
      case LINK_TIE:
	w = lookup_widget(dbox, "radiobutton3");
	break;
      case LINK_TPU:
	w = lookup_widget(dbox, "radiobutton4");
	break;
      case LINK_SER:
	w = lookup_widget(dbox, "radiobutton5");
	break;
      case LINK_AVR:
	w = lookup_widget(dbox, "radiobutton6");
	break;
      case LINK_VTI:
	w = lookup_widget(dbox, "radiobutton7");
	break;
      case LINK_UGL:
	w = lookup_widget(dbox, "radiobutton8");
	break;
    }

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
  
  return 0;
}

/************************************/
/* Called when the box is displayed */
/************************************/


static void
update_port1 (GtkWidget *w)
{
  //DISPLAY("show20\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Parallel port #1");
      if(port == PARALLEL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Serial port #1");
      if(port == SERIAL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_TIE:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Virtual port #1 (emu)");
      if(port == VIRTUAL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_VTI:
#ifdef __WIN32__
      gtk_widget_hide(w);
      break;
#endif
    case LINK_VTL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Virtual port #1 (emu)");
      if(port == VIRTUAL_PORT_1)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_TPU:
      gtk_widget_hide(w);
      /*
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "'raw' mode");
      if(io_addr == TPU_RAW_MODE)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      */
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child),"USB port #1");
      if(port == USB_PORT_1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    }
}


static void
update_port2 (GtkWidget *w)
{
  //DISPLAY("show21\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Parallel port #2");
      if(port == PARALLEL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Serial port #2");
      if(port == SERIAL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_TIE:
    case LINK_VTL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Virtual port #2 (TiLP)");
      if(port == VIRTUAL_PORT_2)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_VTI:
#ifdef __WIN32__
      gtk_widget_hide(w);
      break;
#endif
    case LINK_TPU:
      gtk_widget_hide(w);
      /*
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "'cooked' mode");
      if(io_addr == TPU_COOKED_MODE)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      */
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "USB port #2");
      if(port == USB_PORT_2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    }
}


static void
update_port3 (GtkWidget *w)
{
  //DISPLAY("show22\n");
  switch(link_type)
    {
    case LINK_PAR:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Parallel port #3");
      if(port == PARALLEL_PORT_3)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Serial port #3");
      if(port == SERIAL_PORT_3)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_TIE:
    case LINK_VTI:
    case LINK_VTL:
    case LINK_TPU:
      gtk_widget_hide(w);
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "USB port #3");
      if(port == USB_PORT_3)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    }
}


static void
update_port4 (GtkWidget *w)
{
  //DISPLAY("show23\n");
  switch(link_type)
    {
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "Serial port #4");
      if(port == SERIAL_PORT_4)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_PAR:
    case LINK_TIE:
    case LINK_VTI:
    case LINK_VTL:
    case LINK_TPU:
      gtk_widget_hide(w);
      break;
    case LINK_UGL:
      gtk_label_set_text(GTK_LABEL(GTK_BIN(w)->child), "USB port #4");
      if(port == USB_PORT_4)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    }
}


static void
update_port_custom (GtkWidget *w)
{
  //DISPLAY("show24\n");
  switch(link_type)
    {
    case LINK_PAR:
    case LINK_AVR:
    case LINK_TGL:
    case LINK_SER:
    case LINK_UGL:
      if(port == USER_PORT)
	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
      gtk_widget_show(w);
      break;
    case LINK_TIE:
    case LINK_VTI:
    case LINK_VTL:
    case LINK_TPU:
      gtk_widget_hide(w);
      break;
    }
  //DISPLAY("port = %i\n", port);
}


static void
update_entry1 (GtkWidget *w)
{
  gchar buffer[MAXCHARS];

  g_snprintf(buffer, MAXCHARS, "0x%03x", io_addr);

  gtk_entry_set_visibility(GTK_ENTRY(w), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(w), custom_toggled);
  gtk_entry_set_text(GTK_ENTRY(w), buffer);
}


static void
update_entry2 (GtkWidget *w)
{
  gchar buffer[MAXCHARS];

  wentry2 = w;

  g_snprintf(buffer, MAXCHARS, "%s", device);

  gtk_entry_set_visibility(GTK_ENTRY(w), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(w), custom_toggled);
  gtk_entry_set_text(GTK_ENTRY(w), buffer);
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

  s = gtk_editable_get_chars(editable, 0, -1);
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

  s = gtk_editable_get_chars(editable, 0, -1);
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
  if (!gtk_toggle_button_get_active(togglebutton))
    return;

  link_type = LINK_TGL;
  port = SERIAL_PORT_2;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton2_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;

  link_type = LINK_PAR;
  port = PARALLEL_PORT_1;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton3_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;

  link_type = LINK_TIE;
  port = VIRTUAL_PORT_2;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton4_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;

  link_type = LINK_TPU;
  /* port = ; */ /* dunno */

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton5_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;

  link_type = LINK_SER;
  port = SERIAL_PORT_2;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton6_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;
  
  link_type = LINK_AVR;
  port = SERIAL_PORT_2;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton7_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;
  
  link_type = LINK_VTI;
  port = VIRTUAL_PORT_2;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
}

void
linktype_radiobutton8_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (!gtk_toggle_button_get_active(togglebutton))
    return;
  
  link_type = LINK_UGL;
  port = USB_PORT_1;

  update_port1(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton10"));
  update_port2(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton11"));
  update_port3(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton12"));
  update_port4(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton13"));
  update_port_custom(lookup_widget(GTK_WIDGET(togglebutton), "radiobutton14"));
  update_entry1(lookup_widget(GTK_WIDGET(togglebutton), "entry1"));
  update_entry2(lookup_widget(GTK_WIDGET(togglebutton), "entry2"));
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
  GtkWidget *w;
  //DISPLAY("r14\n");
  custom_toggled = gtk_toggle_button_get_active(togglebutton);

  if(custom_toggled)
    port = USER_PORT;

  w = lookup_widget(GTK_WIDGET(togglebutton), "entry1");
  gtk_entry_set_visibility(GTK_ENTRY(w), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(w), custom_toggled);
  w = lookup_widget(GTK_WIDGET(togglebutton), "entry2");
  gtk_entry_set_visibility(GTK_ENTRY(w), custom_toggled);
  gtk_entry_set_editable(GTK_ENTRY(w), custom_toggled);
}


/*************/
/* Ok/Cancel */
/*************/

void
linktype_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "linktype_dbox"));

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
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "linktype_dbox"));
  unhalt();
}


void
on_linktype_button_fs_clicked            (GtkButton       *button,
					  gpointer         user_data)
{
  GtkWidget *dbox;
  gchar *s;

  dbox = create_fileselection_device();

  s = gtk_editable_get_chars(GTK_EDITABLE(lookup_widget(GTK_WIDGET(button), "entry2")), 0, -1);
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(dbox), s);

  gtk_widget_show(dbox);
}


void
on_seldev_ok_button1_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  const gchar *s;

  w = lookup_widget(GTK_WIDGET(button), "fileselection_device");

  s = gtk_file_selection_get_filename(GTK_FILE_SELECTION(w));
  gtk_entry_set_text(GTK_ENTRY(wentry2), s);

  gtk_widget_destroy(w);
}


void
on_seldev_cancel_button1_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "fileselection_device"));
}


void
on_button2_pressed                     (GtkButton       *button,
                                        gpointer         user_data)
{
  //display_probeio_dbox();
}

/* */


