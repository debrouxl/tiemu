
#include <gtk/gtk.h>

#ifdef __WIN32__
# include "C:\SDL-1.2.7\include\SDL.h"
#else
# include <SDL/SDL.h>
#endif


gint gtk_main_window(void);

void hid_popup_menu(SDL_MouseButtonEvent event);
