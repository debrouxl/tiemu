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

#ifndef TIEMU_DCOP_H
#define TIEMU_DCOP_H

#include <qobject.h>
#include <dcopobject.h>
#include <qstringlist.h>

class TiEmuDCOP: public QObject, virtual public DCOPObject
{
  Q_OBJECT
  K_DCOP

  public:
    TiEmuDCOP();
    ~TiEmuDCOP();

  k_dcop:
    bool image_loaded();
    int emulated_calc_type();
    int emulated_hw_version();
    QString emulated_os_version();
    bool ready_for_transfers();
    void send_file(QString);
    void send_files(QStringList);
    void debug_file(QString);
};

#endif
