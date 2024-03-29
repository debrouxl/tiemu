/* Hey EMACS -*- linux-c -*- */
/* $Id: engine.h 1017 2005-03-26 10:18:28Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Li�vin
 *  Copyright (c) 2005, Romain Li�vin
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

#ifndef __TSOURCE_H__
#define __TSOURCE_H__

GSource *
g_timeout2_source_new (guint interval);

guint
g_timeout2_add_full (gint           priority,
					 guint          interval,
					 GSourceFunc    function,
					 gpointer       data,
					 GDestroyNotify notify);

guint 
g_timeout2_add (guint32        interval,
				GSourceFunc    function,
				gpointer       data);

void
g_timeout2_set_interval(guint source_id, 
						guint32 interval);

#endif
