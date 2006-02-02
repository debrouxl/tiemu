/*  DCOP interface for TiEmu
 *
 *  Copyright (c) 2006 Kevin Kofler
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

#include <glib.h>
#include <gtk/gtk.h>
#include <dcopobject.h>
#include <dcopclient.h>
#include <kapp.h>

#include "dcop.h"
#include "images.h"
#include "fs_misc.h"

TiEmuDCOP::TiEmuDCOP() : DCOPObject( "TiEmuDCOP" )
{
  // Register with DCOP
  if (!kapp->dcopClient()->isRegistered()) 
    kapp->dcopClient()->attach();
  kapp->dcopClient()->setDefaultObject(objId());
}

TiEmuDCOP::~TiEmuDCOP()
{
}

bool TiEmuDCOP::image_loaded()
{
  return img_loaded;
}

int TiEmuDCOP::emulated_calc_type()
{
  if (img_loaded)
    return img_infos.calc_type;
  else
    return 0;
}

int TiEmuDCOP::emulated_hw_version()
{
  if (img_loaded)
    return img_infos.hw_type;
  else
    return 0;
}

QString TiEmuDCOP::emulated_os_version()
{
  if (img_loaded)
    return QString::fromUtf8(img_infos.version);
  else
    return QString::null;
}

void TiEmuDCOP::send_file(QString filename)
{
  if (img_loaded)
    ::send_file(filename.local8Bit());
}

void TiEmuDCOP::send_files(QStringList filenames)
{
  if (img_loaded)
    for (QStringList::Iterator it = filenames.begin(); it != filenames.end(); ++it)
      ::send_file((*it).local8Bit());
}

void TiEmuDCOP::debug_file(QString filename)
{
  if (img_loaded)
    send_file_and_debug_info(filename.local8Bit());
}
