/*  TiEmu - a TI emulator
 *
 *  Originally written by Jonas Minsberg
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin
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

#include <SDL/SDL.h>

#include <gtk/gtk.h>
#include "../gtk/tiemu_cb.h"
#include "../gtk/refresh.h"


/* Display a popup menu: entry point */
void hid_popup_menu(SDL_MouseButtonEvent event)
{
  suspend();
  gtk_menu_popup(GTK_MENU(display_popup_menu()),
		 NULL, NULL, NULL, NULL,
		 event.button,
		 -897199374); // cannot pass time
}
