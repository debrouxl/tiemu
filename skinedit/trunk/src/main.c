/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <signal.h>

#include <gtk/gtk.h>

#include "support.h"

#include "main_intf.h"
#include "utils.h"


void
signal_handler(int sig)
{
  fprintf(stderr, _("Caught SIGINT, exiting ...\n"));

  clear_skin_infos();

  SDL_Quit();

  gtk_main_quit();

  exit(0);
}


int
main (int argc, char *argv[])
{

#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  textdomain (PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
  add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");

  signal(SIGINT, signal_handler);

  fprintf(stdout, _("Skinedit v%s, (C) 2002 Julien BLACHE <jb@tilp.info>\n"), VERSION);
  fprintf(stdout, _("Using GTKSDL v0.2.1 by <derethor@users.sourceforge.net>\n"));
  fprintf(stdout, "\n");
  fprintf(stdout, _("This program is free software; you can redistribute it and/or modify\n"));
  fprintf(stdout, _("it under the terms of the GNU General Public License as published by\n"));
  fprintf(stdout, _("the Free Software Foundation; version 2 of the License\n"));
  fprintf(stdout, "\n");
  fprintf(stdout, _("This program is distributed in the hope that it will be useful,\n"));
  fprintf(stdout, _("but WITHOUT ANY WARRANTY; without even the implied warranty of\n"));
  fprintf(stdout, _("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"));
  fprintf(stdout, _("GNU General Public License for more details.\n"));

  gtk_widget_show (create_main_window());

  sbar_print(_("Skinedit v%s -- Supported formats : TiEmu v2.00 (R/W), VTiv2.1 (R/W), VTiv2.5 (R/W)"), VERSION);

  gtk_main ();

  return 0;
}

