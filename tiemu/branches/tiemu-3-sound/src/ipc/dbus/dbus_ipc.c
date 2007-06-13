/*  D-Bus interface for TiEmu
 *
 *  Copyright (c) 2006-2007 Kevin Kofler
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <dbus/dbus-protocol.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>

#include "dbus_ipc.h"
#include "ti68k_int.h"
#include "fs_misc.h"
#include "engine.h"
#include "dbg_all.h"
#include "m68k.h"

#define TIEMUDBUS_TYPE (tiemudbus_get_type())

typedef struct {
  GObject object;
} TiEmuDBus;

typedef struct {
  GObjectClass object_class;
} TiEmuDBusClass;

static gboolean tiemudbus_image_loaded(TiEmuDBus *this);
static gint tiemudbus_emulated_calc_type(TiEmuDBus *this);
static gint tiemudbus_emulated_hw_version(TiEmuDBus *this);
static const char *tiemudbus_emulated_os_version(TiEmuDBus *this);
static gboolean tiemudbus_ready_for_transfers(TiEmuDBus *this);
static gboolean tiemudbus_send_file(TiEmuDBus *this, const char *filename);
static gboolean tiemudbus_send_files(TiEmuDBus *this, const char **filenames);
static gboolean tiemudbus_debug_file(TiEmuDBus *this, const char *filename);
static gboolean tiemudbus_reset_calc(TiEmuDBus *this, gboolean clearram);
static gboolean tiemudbus_execute_command(TiEmuDBus *this, const char *command);
static gboolean tiemudbus_turn_calc_on(TiEmuDBus *this);
static gboolean tiemudbus_enter_debugger(TiEmuDBus *this);
#include "dbus_glue.h"

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED /*unused*/
/* M$VC could probably use a #pragma warning for this */
#endif

static void tiemudbus_class_init(TiEmuDBusClass *this)
{
  dbus_g_object_type_install_info(G_TYPE_FROM_CLASS(G_OBJECT_CLASS(this)),
                                  &dbus_glib_tiemudbus_object_info);
}

static void tiemudbus_init(TiEmuDBus *this)
{
  DBusGConnection *connection;
  DBusGProxy *proxy;
  GError *error;
  guint request_name_ret;

  // Connect to D-Bus
  error = NULL;
  connection = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
  if (!connection) {
    g_printerr("Failed to open connection to D-Bus session bus: %s\n",
               error->message);
    g_error_free(error);
    return;
  }

  // Register service with D-Bus
  proxy = dbus_g_proxy_new_for_name(connection, DBUS_SERVICE_DBUS,
                                    DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS);
  if (!org_freedesktop_DBus_request_name(proxy, "org.ticalc.lpg.tiemu.TiEmuDBus",
                                         0, &request_name_ret, &error)) {
    g_printerr("Failed to register service with D-Bus: %s\n",
               error->message);
    g_error_free(error);
    return;
  }

  // Register object with D-Bus
  dbus_g_connection_register_g_object(connection,
                                      "/org/ticalc/lpg/tiemu/TiEmuDBus",
                                      G_OBJECT(this));
}

G_DEFINE_TYPE(TiEmuDBus, tiemudbus, G_TYPE_OBJECT);

static gboolean tiemudbus_image_loaded(TiEmuDBus *this UNUSED)
{
  return img_loaded;
}

static gint tiemudbus_emulated_calc_type(TiEmuDBus *this UNUSED)
{
  if (img_loaded)
    return img_infos.calc_type;
  else
    return 0;
}

static gint tiemudbus_emulated_hw_version(TiEmuDBus *this UNUSED)
{
  if (img_loaded)
    return img_infos.hw_type;
  else
    return 0;
}

static const char *tiemudbus_emulated_os_version(TiEmuDBus *this UNUSED)
{
  if (img_loaded)
    return img_infos.version;
  else
    return NULL;
}

static gboolean tiemudbus_ready_for_transfers(TiEmuDBus *this UNUSED)
{
  return (img_loaded && !engine_is_stopped() && ti68k_linkport_ready());
}

static gboolean tiemudbus_send_file(TiEmuDBus *this UNUSED, const char *filename)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    fs_send_file(filename);
    engine_start();
    return TRUE;
  } else return FALSE;
}

static gboolean tiemudbus_send_files(TiEmuDBus *this UNUSED, const char **filenames)
{
  if (img_loaded && !engine_is_stopped()) {
    const char **it;
    engine_stop();
    for (it = filenames; *it; ++it)
      fs_send_file(*it);
    engine_start();
    return TRUE;
  } else return FALSE;
}

static gboolean tiemudbus_debug_file(TiEmuDBus *this UNUSED, const char *filename)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    fs_send_file_and_debug_info(filename);
    engine_start();
    return TRUE;
  } else return FALSE;
}

static gboolean tiemudbus_reset_calc(TiEmuDBus *this UNUSED, gboolean clearram)
{
  if (img_loaded) {
    engine_stop();

    if(clearram)
      memset(tihw.ram, 0, tihw.ram_size);

    ti68k_reset();
    if (dbg_on)
      close_debugger_async();
    else
      engine_start();

    return TRUE;
  } else return FALSE;
}

static gboolean tiemudbus_execute_command(TiEmuDBus *this UNUSED, const char *command)
{
  if (img_loaded) {
    gboolean result;
    char *utf8, *ti;
    unsigned short *utf16;
    utf8=g_strconcat("\f\r\r", command, "\n", NULL);
    utf16=ticonv_utf8_to_utf16(utf8);
    g_free(utf8);
    ti=ticonv_charset_utf16_to_ti(CALC_TI92, utf16);
    g_free(utf16);
    result=ti68k_kbd_push_chars(ti);
    g_free(ti);
    return result;
  } else return FALSE;
}

static gboolean tiemudbus_turn_calc_on(TiEmuDBus *this UNUSED)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    hw_m68k_irq(6);
    while (ti68k_debug_is_supervisor())
      hw_m68k_run(1,0);
    engine_start();
    return TRUE;
  } else return FALSE;
}

static gboolean tiemudbus_enter_debugger(TiEmuDBus *this UNUSED)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    ti68k_debug_break();
    engine_start();
    return TRUE;
  } else return FALSE;
}

static TiEmuDBus *dbus_interface;

void dbus_init(void)
{
  dbus_interface = g_object_new(TIEMUDBUS_TYPE, NULL);
}

void dbus_finish(void)
{
  g_object_unref(dbus_interface);
}
