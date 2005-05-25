/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <gtk/gtk.h>

#include "intl.h"
#include "dboxes.h"
#include "tilibs.h"
#include "refresh.h"

extern TicalcInfoUpdate info_update;
extern struct progress_window
{
	GtkWidget *window;
	GtkWidget *pbar1;
	GtkWidget *pbar2;
	GtkWidget *label;
	GtkWidget *label_rate;
} p_win;

TicableDataRate *dr;

static void gt_start(void)
{
  info_update.prev_percentage = info_update.percentage = 0.0;
  ticable_get_datarate(&dr);
}

static void gt_stop(void)
{
  info_update.prev_percentage = info_update.percentage = 0.0;
}

static void refresh_pbar1(void)
{
  gchar buffer[32];
  gfloat rate;

  info_update.percentage = (float) info_update.count / info_update.total;

  if(p_win.pbar1 != NULL)
    {
    info_update.percentage = (float) info_update.count / info_update.total;

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
      gtk_label_set_text(GTK_LABEL(p_win.label_rate), buffer);
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(p_win.pbar1), info_update.percentage);
      
      GTK_REFRESH();
    }
}

static void refresh_pbar2(void)
{
  if(p_win.pbar2 != NULL)
    {
      /* Refresh if necessary (for speeding up !) */
      if( (info_update.main_percentage - info_update.prev_main_percentage) 
	  < 0.05)
	return;
      else
	info_update.prev_main_percentage = info_update.main_percentage;
      
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(p_win.pbar2), info_update.main_percentage);
      
      GTK_REFRESH();
    }
}

static void gt_pbar(void)
{
  refresh_pbar1();
  refresh_pbar2();
}

static void gt_label(void)
{
  if(p_win.label == NULL) return;
  gtk_label_set_text(GTK_LABEL(p_win.label), info_update.label_text);
  
  GTK_REFRESH();
}

static void gt_refresh(void)
{
  while( gtk_events_pending() ) { gtk_main_iteration(); }
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
}







