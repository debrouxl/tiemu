#define __SP_KDE_CPP__

/*
 * KDE utilities for Sodipodi - adapted to TiEmu
 *
 * Authors:
 *   Lauris Kaplinski <lauris@kaplinski.com>
 *   Kevin Kofler <Kevin@tigcc.ticalc.org>
 *
 * Copyright (C) 2003-2005 Authors
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#include <config.h>

#include <qtimer.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <kapp.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kfiledialog.h>
#include <ktoolbar.h>
#include <gtk/gtkmain.h>

#include "kde.h"
#include "kde-private.h"

#define SP_FOREIGN_FREQ 32
#define SP_FOREIGN_MAX_ITER 4

void
SPKDEBridge::EventHook (void) {
	int cdown = 0;
	while ((cdown++ < SP_FOREIGN_MAX_ITER) && gdk_events_pending ()) {
		gtk_main_iteration_do (FALSE);
	}
	gtk_main_iteration_do (FALSE);
}

void
SPKDEBridge::TimerHook (void) {
	int cdown = 10;
	while ((cdown++ < SP_FOREIGN_MAX_ITER) && gdk_events_pending ()) {
		gtk_main_iteration_do (FALSE);
	}
	gtk_main_iteration_do (FALSE);
}

static KApplication *KDESodipodi = NULL;
static KAboutData *KDEAbout = NULL;
static SPKDEBridge *Bridge = NULL;
static bool SPKDEModal = FALSE;

static void
sp_kde_gdk_event_handler (GdkEvent *event)
{
	if (SPKDEModal) {
		// KDE widget is modal, filter events
		switch (event->type) {
		case GDK_NOTHING:
		case GDK_DELETE:
		case GDK_SCROLL:
		case GDK_BUTTON_PRESS:
		case GDK_2BUTTON_PRESS:
		case GDK_3BUTTON_PRESS:
		case GDK_BUTTON_RELEASE:
		case GDK_KEY_PRESS:
		case GDK_KEY_RELEASE:
		case GDK_DRAG_STATUS:
		case GDK_DRAG_ENTER:
		case GDK_DRAG_LEAVE:
		case GDK_DRAG_MOTION:
		case GDK_DROP_START:
		case GDK_DROP_FINISHED:
			return;
			break;
		default:
			break;
		}
	}
	gtk_main_do_event (event);
}

void
sp_kde_init (int argc, char **argv, const char *appName, const char *programName, const char *version, const char *shortDescription, const char *copyrightStatement, const char *homePageAddress, const char *bugsEmailAddress)
{
	KDEAbout = new KAboutData(appName, programName, version, shortDescription, KAboutData::License_GPL, copyrightStatement, NULL, homePageAddress, bugsEmailAddress);
	KCmdLineArgs::init(argc, argv, KDEAbout);
	KDESodipodi = new KApplication;
	Bridge = new SPKDEBridge ("KDE Bridge");

	QObject::connect (KDESodipodi, SIGNAL (guiThreadAwake ()), Bridge, SLOT (EventHook ()));

	gdk_event_handler_set ((GdkEventFunc) sp_kde_gdk_event_handler, NULL, NULL);
}

void
sp_kde_finish (void)
{
	delete Bridge;
	delete KDESodipodi;
	delete KDEAbout;
}

char *
sp_kde_get_open_filename (unsigned char *dir, unsigned char *filter, unsigned char *title)
{
	QString fileName;

	QTimer timer;
	QObject::connect (&timer, SIGNAL (timeout ()), Bridge, SLOT (TimerHook ()));
	timer.changeInterval (1000 / SP_FOREIGN_FREQ);
	SPKDEModal = TRUE;

	fileName = KFileDialog::getOpenFileName ((const char *) dir,
						 (const char *) filter,
						 NULL,
						 (const char *) title);

	SPKDEModal = FALSE;

        return g_strdup (fileName);
}

char **
sp_kde_get_open_filenames (unsigned char *dir, unsigned char *filter, unsigned char *title)
{
	QStringList fileNames;
	char **result, **p;

	QTimer timer;
	QObject::connect (&timer, SIGNAL (timeout ()), Bridge, SLOT (TimerHook ()));
	timer.changeInterval (1000 / SP_FOREIGN_FREQ);
	SPKDEModal = TRUE;

	fileNames = KFileDialog::getOpenFileNames ((const char *) dir,
						   (const char *) filter,
						   NULL,
						   (const char *) title);

	SPKDEModal = FALSE;

	if (fileNames.empty())
		return NULL;

	p = result = (char **) g_malloc ((fileNames.count() + 1) * sizeof(char *));
	for (QStringList::Iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
		*(p++) = g_strdup (*it);
	}
	*p = NULL;

	return result;
}

char *
sp_kde_get_write_filename (unsigned char *dir, unsigned char *filter, unsigned char *title)
{
	QString fileName;

	QTimer timer;
	QObject::connect (&timer, SIGNAL (timeout ()), Bridge, SLOT (TimerHook ()));
	timer.changeInterval (1000 / SP_FOREIGN_FREQ);
	SPKDEModal = TRUE;

	fileName = KFileDialog::getSaveFileName ((const char *) dir,
						 (const char *) filter,
						 NULL,
						 (const char *) title);

	SPKDEModal = FALSE;

        return g_strdup (fileName);
}
