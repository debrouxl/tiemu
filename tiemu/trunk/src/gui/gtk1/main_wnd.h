
#include <gtk/gtk.h>

#ifdef __WIN32__
# include "D:\devel\tiemu_project\SDL-1.2.0\include\SDL.h"
#else
# include <SDL/SDL.h>
#endif


gint gtk_main_window(void);

void hid_popup_menu(SDL_MouseButtonEvent event);
