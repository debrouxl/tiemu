/*  tilp - link program for TI calculators
 *  Copyright (C) 1999-2001  Romain Lievin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <gtk/gtk.h>

#include "platform.h"
#include "msg_cb.h" // msg_box()
#include "user_cb.h"
#include "tilibs.h"

extern TicalcInfoUpdate info_update;
extern struct progress_window
{
  GtkWidget *window;
  GtkWidget *pbar;
  GtkWidget *pbar2;
  GtkWidget *label;
  GtkWidget *label2;
  GtkWidget *label_rate;
} p_win;

TicableDataRate *dr;

static void gt_start()
{
  info_update.prev_percentage = info_update.percentage = 0.0;
  ticable_get_datarate(&dr);
}

static void gt_stop()
{
  info_update.prev_percentage = info_update.percentage = 0.0;
}

static void refresh_pbar1()
{
  gchar buffer[32];
  gfloat rate;

  if(p_win.pbar != NULL) 
    {
      /* Refresh if necessary (for speeding up !) */
      if( (info_update.percentage - info_update.prev_percentage) < 0.05)
	{
	  if( (info_update.percentage - info_update.prev_percentage) < 0)
	    info_update.prev_percentage = info_update.percentage;
	  else
	    return;
	}
      else
	info_update.prev_percentage = info_update.percentage;

      rate = dr->count / toCURRENT(dr->start);
      g_snprintf(buffer, 32, "Rate: %1.1f Kbytes/s", rate / 1000);
      gtk_label_set(GTK_LABEL (p_win.label_rate), buffer);
      gtk_progress_bar_update(GTK_PROGRESS_BAR (p_win.pbar), 
			      info_update.percentage);
      
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
}

static void refresh_pbar2()
{
  if(p_win.pbar2 != NULL)
    {
      /* Refresh if necessary (for speeding up !) */
      if( (info_update.main_percentage - info_update.prev_main_percentage) 
	  < 0.05)
	return;
      else
	info_update.prev_main_percentage = info_update.main_percentage;
      
      gtk_progress_bar_update(GTK_PROGRESS_BAR (p_win.pbar2), 
			      info_update.main_percentage);
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
}

static void gt_pbar()
{
  refresh_pbar1();
  refresh_pbar2();
}

static void gt_label()
{
  if(p_win.label == NULL) return;
  gtk_label_set(GTK_LABEL (p_win.label), info_update.label_text);
  while( gtk_events_pending() ) { gtk_main_iteration(); }
}

static void gt_refresh()
{
  while( gtk_events_pending() ) { gtk_main_iteration(); }
}

static int gt_choose(char *cur_varname, char *new_varname)
{
  /*
  int ret=0;
  int action = ACTION_NONE;
  gchar *s;
  
  strcpy(new_varname, "");
  ret = user3_box(_("Action"), 
		  _("The variable already exists..."), 
		  _("Skip"), _("Overwrite"), _("Rename"));
  switch(ret)
    {
    case 1:
      action = ACTION_SKIP;
      break;
    case 2:
      action = ACTION_OVERWRITE;
      break;
    case 3:
      action = ACTION_RENAME;
      s = gif->dlgbox_entry(_("Rename the variable"),
			    _("New name: "), cur_varname);
      if((s == NULL) || (strlen(s)>17)) { action=ACTION_SKIP; break; }
      strcpy(new_varname, s);
      g_free(s);
      break;
    default:
      break;
    }

    return action;
  */
  return 0;
}

void gt_init_refresh_functions(void)
{
  ticalc_set_update(&info_update, 
		    gt_start,
		    gt_stop,
		    gt_refresh,
		    gt_pbar, 
		    gt_label 
		    );

  fprintf(stdout, _("Initialized in GTK+ mode.\n"));

  return;
}







