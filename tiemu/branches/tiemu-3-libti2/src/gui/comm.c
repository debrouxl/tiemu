/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  tilp - Ti Linking Program
 *  Copyright (C) 1999-2004  Romain Lievin
 *
 *  This program is free software you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#undef GTK_DISABLE_DEPRECATED
#include <gtk/gtk.h>
#define GTK_DISABLE_DEPRECATED
#include <glade/glade.h>
#include <string.h>

#include "intl.h"
#include "comm.h"
#include "tilibs.h"
#include "support.h"
#include "struct.h"
#include "tie_error.h"

#include "ti68k_def.h"
#include "ti68k_int.h"

#if 0
static TicableLinkParam tmp_lp;
static GtkWidget *button = NULL;
static gint init = !0;
static GtkWidget *port = NULL;
static GtkWidget *tmo = NULL;
static GtkWidget *dly = NULL;

gint display_comm_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	int err;

	init = !0;

	xml = glade_xml_new
	    (tilp_paths_build_glade("comm-2.glade"), "comm_dbox", PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);

	dbox = glade_xml_get_widget(xml, "comm_dbox");

	// Cable  
	data = glade_xml_get_widget(xml, "optionmenu_comm_cable");
	switch (link_cable.link_type) 
	{
	case LINK_TGL:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
		break;
		
	case LINK_SER:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 1);
		break;

	case LINK_SLV:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 2);
	  break;

	case LINK_PAR:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 3);
	  break;

	case LINK_VTI:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 4);
	  break;

    case LINK_TIE:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 5);
	  break;
	
	case LINK_VTL:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 6);
	  break;

    case LINK_NUL:
      gtk_option_menu_set_history(GTK_OPTION_MENU(data), 7);
	  break;

	default:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 7);
	  break;
	}

	// Port
	port = data = glade_xml_get_widget(xml, "optionmenu_comm_port");
	switch (link_cable.port) {
	case PARALLEL_PORT_1:
	case SERIAL_PORT_1:
	case USB_PORT_1:
	case VIRTUAL_PORT_1:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 1);
	  break;
	 
	case PARALLEL_PORT_2:
	case SERIAL_PORT_2:
	case USB_PORT_2:
	case VIRTUAL_PORT_2:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 2);
	  break;

	case PARALLEL_PORT_3:
	case SERIAL_PORT_3:
	case USB_PORT_3:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 3);
	  break;

	case SERIAL_PORT_4:
	case USB_PORT_4:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 4);
	  break;

    case NULL_PORT:
      gtk_option_menu_set_history(GTK_OPTION_MENU(data), 5);
	  break;
	  
	case USER_PORT:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
	  break;

	default:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
	  break;
	}

	// Timeout
	tmo = data = glade_xml_get_widget(xml, "spinbutton_comm_timeout");
	if(link_cable.link_type != LINK_NUL)	
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), link_cable.timeout);
	else
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), params.timeout);
	
	// Delay
	dly = data = glade_xml_get_widget(xml, "spinbutton_comm_delay");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), link_cable.delay);

	// Avoid early callbacks
	memcpy(&tmp_lp, &link_cable, sizeof(TicableLinkParam));
	
	// Loop
	init = 0;
 
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) {
	case GTK_RESPONSE_OK:
		tmp_lp.timeout = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tmo));
		tmp_lp.delay   = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dly));

		switch(tihw.calc_type)
		{
    	case TI89:  tmp_lp.calc_type = CALC_TI89;  break;
		case TI89t: tmp_lp.calc_type = CALC_TI89T; break;
		case TI92:  tmp_lp.calc_type = CALC_TI92;  break;
		case TI92p: tmp_lp.calc_type = CALC_TI92P; break;
		case V200:  tmp_lp.calc_type = CALC_V200;  break;
		}

        memcpy(&link_cable, &tmp_lp, sizeof(TicableLinkParam));
		if(link_cable.link_type == LINK_NUL)
			params.timeout = tmp_lp.timeout;
        err = ti68k_linkport_reconfigure();
		handle_error();
		break;
	case GTK_RESPONSE_HELP:
		break;
	default:
		break;
	}

	gtk_widget_destroy(dbox);

	return 0;
}


GLADE_CB void
comm_cable_changed                     (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
	gint nitem = gtk_option_menu_get_history(optionmenu);

	switch(nitem)
	{
	case 0: tmp_lp.link_type = LINK_TGL; break;
	case 1: tmp_lp.link_type = LINK_SER; break;
	case 2: tmp_lp.link_type = LINK_SLV; break;
	case 3:	tmp_lp.link_type = LINK_PAR; break;
	case 4: tmp_lp.link_type = LINK_VTI; break;
	case 5: tmp_lp.link_type = LINK_TIE; break;
	case 6: tmp_lp.link_type = LINK_VTL; break;
    case 7: tmp_lp.link_type = LINK_NUL; break;
	}
	
	// force port to avoid libticables bad argument 
	if(!init) 
	{
		switch(tmp_lp.link_type) 
		{
		case LINK_TGL:
		case LINK_SER:
		case LINK_AVR:
		case LINK_VTI:
		case LINK_TIE:
		case LINK_VTL:
			gtk_option_menu_set_history(GTK_OPTION_MENU(port), 5);
			gtk_option_menu_set_history(GTK_OPTION_MENU(port), 2);
			break;
		case LINK_PAR:
		case LINK_SLV:
			gtk_option_menu_set_history(GTK_OPTION_MENU(port), 5);
			gtk_option_menu_set_history(GTK_OPTION_MENU(port), 1);
                        break;
        case LINK_NUL:
            gtk_option_menu_set_history(GTK_OPTION_MENU(port), 5);
                        break;
		default:
			break;
		}
	}

	// change value
	/*
	if(tmo != NULL)
	{
		if(tmp_lp.link_type != LINK_NUL)	
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(tmo), link_cable.timeout);
		else
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(tmo), params.timeout);
	}
	*/
}

GLADE_CB void
comm_cable_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


GLADE_CB void
comm_port_changed                      (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
	GtkWidget *menu_item = optionmenu->menu_item;
	gchar *ed = menu_item->name;
	
	if(!strcmp(ed, "custom1"))
    		tmp_lp.port = USER_PORT;
  	else 
	{
    		switch(tmp_lp.link_type)
      		{
      		case LINK_TGL:
      		case LINK_SER:
      		case LINK_AVR:
			if(!strcmp(ed, "custom1"))
			  	tmp_lp.port = USER_PORT;
			else if(!strcmp(ed, "number1"))
			  	tmp_lp.port = SERIAL_PORT_1;
			else if(!strcmp(ed, "number2"))
			  	tmp_lp.port = SERIAL_PORT_2;
			else if(!strcmp(ed, "number3"))
			  	tmp_lp.port = SERIAL_PORT_3;
			else if(!strcmp(ed, "number4"))
			  	tmp_lp.port = SERIAL_PORT_4;
		break;

      		case LINK_SLV:
			if(!strcmp(ed, "custom1"))
			  tmp_lp.port = USER_PORT;
			else if(!strcmp(ed, "number1"))
			  tmp_lp.port = USB_PORT_1;
			else if(!strcmp(ed, "number2"))
			  tmp_lp.port = USB_PORT_2;
			else if(!strcmp(ed, "number3"))
			  tmp_lp.port = USB_PORT_3;
			else if(!strcmp(ed, "number4"))
			  tmp_lp.port = USB_PORT_4;
		break;

	      	case LINK_PAR:
			if(!strcmp(ed, "custom1"))
			  tmp_lp.port = USER_PORT;
			else if(!strcmp(ed, "number1"))
			  tmp_lp.port = PARALLEL_PORT_1;
			else if(!strcmp(ed, "number2"))
			  tmp_lp.port = PARALLEL_PORT_2;
			else if(!strcmp(ed, "number3"))
			  tmp_lp.port = PARALLEL_PORT_3;
		break;
	
	      	case LINK_VTL:
	      	case LINK_TIE:
	      	case LINK_VTI:
			if(!strcmp(ed, "number1"))
			  tmp_lp.port = VIRTUAL_PORT_1;
			else if(!strcmp(ed, "number2"))
			  tmp_lp.port = VIRTUAL_PORT_2;
		break;

            case LINK_NUL:
                tmp_lp.port = NULL_PORT;

      		default: 
		break;
      	}
  	}
}

GLADE_CB void
comm_port_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


GLADE_CB void
comm_spinbutton_delay_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  	tmp_lp.delay =
    		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
}


GLADE_CB void
comm_spinbutton_timeout_changed        (GtkEditable     *editable,
                                        gpointer         user_data)
{
  	tmp_lp.timeout =
    		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
}

#else

gint display_comm_dbox()
{
	return 0;
}

#endif