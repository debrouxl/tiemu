/* TiEmu OLE test client
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

#include <stdio.h>
#include <windows.h>
#include "tiemuole.h"

int main(void)
{
  if (OleInitialize(0)!=S_OK) {puts("OLE error. (#1)");return 1;}
  LCID lcid=GetUserDefaultLCID();
  ITypeLib *tlb;
  if (LoadRegTypeLib(&LIBID_TiEmuOLELib,1,0,lcid,&tlb)!=S_OK)
    {OleUninitialize();puts("OLE error. (#2)");return 2;}
  ITypeInfo *tinfo;
  MEMBERID mid;
  unsigned short n=1;
  if (tlb->lpVtbl->FindName(tlb,L"TiEmuOLE",0L,&tinfo,&mid,&n)!=S_OK || !n)
    {OleUninitialize();puts("OLE error. (#3)");return 3;}
  ITiEmuOLE *tiemuOLE;
  if (tinfo->lpVtbl->CreateInstance(tinfo,0,&IID_ITiEmuOLE,
      (void**)&tiemuOLE)!=S_OK)
    {OleUninitialize();puts("OLE error (#4).");return 4;}
  VARIANT_BOOL ready;
  do {
    if (tiemuOLE->lpVtbl->ready_for_transfers(tiemuOLE,&ready)!=S_OK)
      {tiemuOLE->lpVtbl->Release(tiemuOLE);OleUninitialize();puts("OLE error (#5).");return 5;}
  } while (!ready);
  Sleep(4000); // give the emulated calculator time to react
  if (tiemuOLE->lpVtbl->turn_calc_on(tiemuOLE,&ready)!=S_OK || !ready)
    {tiemuOLE->lpVtbl->Release(tiemuOLE);OleUninitialize();puts("OLE error (#6).");return 6;}
  Sleep(1000); // give the emulated calculator time to react
  BSTR command;
  command=SysAllocString(L"2+3");
  if (!command)
    {tiemuOLE->lpVtbl->Release(tiemuOLE);OleUninitialize();puts("OLE error (#7).");return 7;}
  if (tiemuOLE->lpVtbl->execute_command(tiemuOLE,command,&ready)!=S_OK || !ready)
    {SysFreeString(command);tiemuOLE->lpVtbl->Release(tiemuOLE);
     OleUninitialize();puts("OLE error (#8).");return 8;}
  SysFreeString(command);
  tiemuOLE->lpVtbl->Release(tiemuOLE);
  OleUninitialize();
  return 0;
}
