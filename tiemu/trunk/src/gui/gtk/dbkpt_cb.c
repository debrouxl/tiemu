#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "dbkpt_cb.h"
#include "dbkpt_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "extern.h"
#include "ticalc.h"
#include "adbkpt_cb.h"
#include "adbkpt_dbox.h"

#include "interface.h"
#include "bkpts.h"

extern DATA_BKPT db;

gint display_data_bkpts_dbox(void)
{
#if 0 /* FUCKED */
  GtkWidget *dbox;
  GtkWidget *clist;
  GtkStyle *style;
  GdkFont *fixed_font;
  gchar *row_text[3];
  gint i;
  gchar buffer[MAXCHARS];
  gint cw;
  DATA_BKPT *s;

  dbox = create_data_bkpts_dbox(); 
  data_bkpt_dbox = dbox; /* ahem ? */

  clist = lookup_widget(dbox, "clist4");
  gtk_clist_clear((GtkCList *)clist);

  fixed_font = gdk_font_load("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1");
  style = gtk_style_new();
  style->font = fixed_font;
  cw = gdk_char_width(fixed_font, 'A');

  for(i=0; i<3; i++) row_text[i] = NULL;
  for(i=0; i<g_list_length(bkpt_access_list); i++)
    {
      s = (DATA_BKPT *)g_list_nth_data(bkpt_access_list, i);
      //DISPLAY("%i, s->id: %i\n", i, s->id);
      if(s->mode & BK_READ)
	{
	  sprintf(buffer, "Read of %06X-%06X\n", 
		  s->address, s->address);
	}
      if(s->mode & BK_WRITE)
	{
	  sprintf(buffer, "Write of %06X/%06X\n",
		  s->address, s->address);
	}
     
      if( (s->mode & BK_READ) && !(s->mode & BK_WRITE) )
	sprintf(buffer, "Read only");
      else if( (s->mode & BK_WRITE) && !(s->mode & BK_READ) )
	sprintf(buffer, "Write only");
      else
	sprintf(buffer, "Read & Write");
      row_text[0] = g_strdup(buffer);

      if(s->mode & BK_BYTE)
	sprintf(buffer, "Byte");
      else if(s->mode & BK_WORD)
	sprintf(buffer, "Word");
      else if(s->mode & BK_LONG)
	sprintf(buffer, "Long");
      else
	sprintf(buffer, "Byte");
      row_text[1] = g_strdup(buffer);
      row_text[2] = g_strdup_printf("%06X", s->address);

      gtk_clist_append((GtkCList *)clist, row_text);
      //DISPLAY("set row data: %i\n", s->id);
      gtk_clist_set_row_data((GtkCList *)clist, i, GINT_TO_POINTER(s->id));
      gtk_clist_set_row_style((GtkCList *)clist, i, style);
    }
  for(i=0; i<3; i++) 
    g_free(row_text[i]);
  
  gtk_widget_show_all(dbox);
#endif /* 0 */

  return 0;
}

#if 0 /* FUCKED */
/* A row of the Data Breakpoint DBox has been selected */
void
on_clist4_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  data_bkpt_selected_row = row;
}


/* A row of the Data Breakpoint DBox has been unselected */
void
on_clist4_unselect_row                 (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  data_bkpt_selected_row = -1;
}
#endif /* 0 */


/* Add button of the 'Data breakpoints dialog box' */
void
on_button_add_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_show(create_add_data_bkpt_dbox());
}


/* Delete button of the 'Data breakpoints dialog box' */
/* Remove the selected data breakpoint */
void
on_button_del_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
#if 0 /* FUCKED */
  gint id;
  GtkWidget *clist;
  int i;
  DATA_BKPT *s = NULL;

  clist = lookup_widget(GTK_WIDGET(button), "clist1");

  if(data_bkpt_selected_row != -1)
    {
      /* Remove data breakpoint */
      // get id associated with the row
      id = GPOINTER_TO_INT(gtk_clist_get_row_data((GtkCList *)clist, selected_row));
      //DISPLAY("Selected row: %i\n", data_bkpt_selected_row);
      //DISPLAY("id: %i\n", id);
      
      // retrieve structure associated with id
      for(i=0; i<g_list_length(bkpt_access_list); i++)
	{
	  s = (DATA_BKPT *)g_list_nth_data(bkpt_access_list, i);
	  if((s->id) == id)
	    {
	      //DISPLAY("id found\n");
	      break;
	    }
	}

      // delete the right id
      ti68k_delBreakpointAccess(s->id, s->mode);
      bkpt_access_list = g_list_remove(bkpt_access_list, s);
      gtk_clist_remove(GTK_CLIST(clist), selected_row);
    }
#endif /* 0 */
}

/* Ok button of the 'Data breakpoints dialog box' */
void
on_button_ok39_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "dbkpt_dbox"));
}


void
on_data_bkpts_dbox_destroy             (GtkWidget       *widget,
                                        gpointer         user_data)
{

}
