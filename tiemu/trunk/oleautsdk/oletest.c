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
	IUnknown *iunk;
	ITiEmuOLE *tiemuOLE;
	VARIANT_BOOL ready;
	BSTR command;

	fprintf(stdout, "Initializing OLE...\n");
	if (OleInitialize(0) != S_OK) {puts("OLE error. (#1)");return 1;}
  
	fprintf(stdout, "Getting object...");
	if (GetActiveObject(&CLSID_TiEmuOLE,NULL,&iunk) != S_OK || !iunk) 
	{ 
		// TiEmu not running
		// Figure out the path of TiEmu and run it
		char buffer[2049];
		HKEY hKey;
		DWORD type, size=2048;
		buffer[2048]=0;

		if (RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID\\{B2A17B13-9D6F-4DD4-A2A9-6FE06ADC1D33}\\LocalServer32",0, KEY_QUERY_VALUE,&hKey))
			{OleUninitialize();puts("OLE error (#2).");return 2;}
    
		if (RegQueryValueEx(hKey,NULL,NULL,&type,buffer,&size) || type != REG_SZ)
			{RegCloseKey(hKey);OleUninitialize();puts("OLE error (#3).");return 3;}
    
		
		if (RegCloseKey(hKey))
			{OleUninitialize();puts("OLE error (#4).");return 4;}

		if ((int)ShellExecute(NULL,NULL,buffer,NULL,NULL,SW_SHOW)<=32)
			{OleUninitialize();puts("OLE error (#5).");return 5;}
    
		while (GetActiveObject(&CLSID_TiEmuOLE,NULL,&iunk) != S_OK || !iunk); // Wait for the object
	}
	fprintf(stdout, "Done !\n");
  
	fprintf(stdout, "Querying interface...\n");
	if (iunk->lpVtbl->QueryInterface(iunk,&IID_ITiEmuOLE,(void**)&tiemuOLE) != S_OK || !tiemuOLE)
		{OleUninitialize();puts("OLE error (#6).");return 6;}
  
	fprintf(stdout, "Waiting for ready... ");
	do 
	{
		if (tiemuOLE->lpVtbl->ready_for_transfers(tiemuOLE,&ready) != S_OK)
			{tiemuOLE->lpVtbl->Release(tiemuOLE);OleUninitialize();puts("OLE error (#7).");return 7;}
	} while (!ready);
	Sleep(10000); // give the emulated calculator time to react
	fprintf(stdout, "Done !\n");
  
	fprintf(stdout, "Turning calc on... ");
	if (tiemuOLE->lpVtbl->turn_calc_on(tiemuOLE,&ready) != S_OK || !ready)
		{tiemuOLE->lpVtbl->Release(tiemuOLE);OleUninitialize();puts("OLE error (#8).");return 8;}
	Sleep(3000); // give the emulated calculator time to react
	fprintf(stdout, "Done !\n");

	fprintf(stdout, "Sending command... ");
    command=SysAllocString(L"2+3");
	if (!command)
		{tiemuOLE->lpVtbl->Release(tiemuOLE);OleUninitialize();puts("OLE error (#9).");return 9;}
  
	if (tiemuOLE->lpVtbl->execute_command(tiemuOLE,command,&ready) != S_OK || !ready)
		{SysFreeString(command);tiemuOLE->lpVtbl->Release(tiemuOLE);
	fprintf(stdout, "Done !\n");
     
	fprintf(stdout, "Releasing...");
	OleUninitialize();puts("OLE error (#10).");return 10;}
	SysFreeString(command);
	tiemuOLE->lpVtbl->Release(tiemuOLE);
	OleUninitialize();
	fprintf(stdout, "Done !\n");

	return 0;
}
