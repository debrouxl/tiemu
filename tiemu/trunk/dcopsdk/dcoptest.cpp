/* TiEmu DCOP test client
   Copyright (C) 2003-2006 Kevin Kofler <kevin.kofler@chello.at>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  */

#include "tiemu.h"
#include <qregexp.h>
#include <kapp.h>
#include <kcmdlineargs.h>
#include <dcopclient.h>
#include <krun.h>
#include <unistd.h>
#include <cstdio>
using namespace std;

int main(int argc, char **argv)
{
  KCmdLineArgs::init(argc,argv,"dcoptest","dcoptest","","");
  KApplication app;
  DCOPClient &dcopclient=*(app.dcopClient());
  TiEmuDCOP_stub *tiemuDCOP;
  if (!dcopclient.attach())
    {puts("DCOP error (#1).");return 1;}
  QCStringList applist=dcopclient.registeredApplications();
  QCString appname;
  QCStringList::iterator it;
  for (it = applist.begin(); it != applist.end(); ++it) {
    if ((*it).contains(QRegExp("^tiemu-"))) {
      appname = (*it);
      break;
    }
  }
  if (appname.isNull()) { // TiEmu not running
    KRun::runCommand("tiemu");
    do {
      applist=dcopclient.registeredApplications();
      for (it = applist.begin(); it != applist.end(); ++it) {
        if ((*it).contains(QRegExp("^tiemu-"))) {
          appname = (*it);
          break;
        }
      }
    } while (appname.isNull());
  }
  tiemuDCOP = new TiEmuDCOP_stub(appname,"TiEmuDCOP");
  bool ready;
  do {
    ready=tiemuDCOP->ready_for_transfers();
    if (!tiemuDCOP->ok())
      {delete tiemuDCOP;dcopclient.detach();puts("DCOP error (#2).");return 2;}
  } while (!ready);
  sleep(10); // give the emulated calculator time to react
  if (!tiemuDCOP->turn_calc_on() || !tiemuDCOP->ok())
    {delete tiemuDCOP;dcopclient.detach();puts("DCOP error (#3).");return 3;}
  sleep(3); // give the emulated calculator time to react
  if (!tiemuDCOP->execute_command(QString("2+3")) || !tiemuDCOP->ok())
    {delete tiemuDCOP;dcopclient.detach();puts("DCOP error (#4).");return 4;}
  delete tiemuDCOP;
  if (!dcopclient.detach())
    {puts("DCOP error (#5).");return 5;}
  return 0;
}
