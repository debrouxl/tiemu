# ifndef __GTK_SDL_H__
# define __GTK_SDL_H__


# include <stdlib.h>

# include <gtk/gtk.h>
# include <gdk/gdk.h>
# include <gtk/gtkwidget.h>
#ifndef __WIN32__
# include <gdk/gdkx.h>
#endif

# include <SDL/SDL.h>

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */


# define GTK_TYPE_SDL			(gtk_sdl_get_type ())
# define GTK_SDL(obj)			(GTK_CHECK_CAST ((obj), GTK_TYPE_SDL, GtkSDL))
# define GTK_SDL_CLASS(klass)		(GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_SDL, GtkSDLClass))
# define GTK_IS_SDL(obj)		(GTK_CHECK_TYPE ((obj), GTK_TYPE_SDL))
# define GTK_IS_SDL_CLASS(klass)	(GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SDL))


typedef struct _GtkSDL		GtkSDL;
typedef struct _GtkSDLClass	GtkSDLClass;

struct _GtkSDL
{
  GtkWidget widget;

  SDL_Surface *surface;
  gint width, height, bpp;
  Uint16 flags;
};

struct _GtkSDLClass
{
  GtkWidgetClass parent_class;
};


GtkType    gtk_sdl_get_type	(void);
GtkWidget *gtk_sdl_new (gint width, gint height, gint bpp, Uint32 flags);
void       gtk_sdl_size		(GtkSDL      *sdl,
				gint                 width,
				gint                 height);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_SDL_H__ */
