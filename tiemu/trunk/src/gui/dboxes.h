#ifndef __DBOXES_H__
#define __DBOXES_H__

#include <gtk/gtk.h>

#define BUTTON1  1
#define BUTTON2  2
#define BUTTON3  3

void msg_box(const gchar * title, gchar * message);
gint msg_box2(const char *title, char *message);
gint msg_box3(const char *title, char *message,
	      const char *button1, const char *button2,
	      const char *button3);
gint msg_box4(const char *title, char *message);
char *msg_entry(const char *title,
		const char *message, const char *content);


#endif
