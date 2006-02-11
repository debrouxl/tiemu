/*  TiEmu - OLE component registration
 *
 *  Copyright (c) 2006, Kevin Kofler
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

#include "registry.h"
#include <cstdlib>
using namespace std;

// Size of a CLSID as a string
static const int CLSID_STRING_SIZE = 39;

// Convert a CLSID to a char string.
static void CLSIDtochar(REFCLSID clsid, char* szCLSID, int length)
{
	// Get CLSID
	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	if (!SUCCEEDED(hr)) exit(1);

	// Covert from wide characters to non-wide.
	wcstombs(szCLSID, wszCLSID, length);

	// Free memory.
	CoTaskMemFree(wszCLSID);
}

// Delete a key and all of its descendents.
static LONG recursiveDeleteKey(HKEY hKeyParent,           // Parent of key to delete
                               const char* lpszKeyChild)  // Key to delete
{
	// Open the child.
	HKEY hKeyChild;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0, KEY_ALL_ACCESS, &hKeyChild);
	if(lRes != ERROR_SUCCESS)
		return lRes;

	// Enumerate all of the decendents of this child.
	FILETIME time;
	char szBuffer[256];
	DWORD dwSize = 256;
	while(RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time) == S_OK)
	{
		// Delete the decendents of this child.
		lRes = recursiveDeleteKey(hKeyChild, szBuffer);
		if(lRes != ERROR_SUCCESS)
		{
			// Cleanup before exiting.
			RegCloseKey(hKeyChild);
			return lRes;
		}
		dwSize = 256;
	}

	// Close the child.
	RegCloseKey(hKeyChild);

	// Delete this child.
	return RegDeleteKey(hKeyParent, lpszKeyChild);
}

// Create a key and set its value.
static BOOL setKeyAndValue(const char* szKey, const char* szSubkey, const char* szValue)
{
	HKEY hKey;
	char szKeyBuf[1024];

	// Copy keyname into buffer.
	strcpy(szKeyBuf, szKey);

	// Add subkey name to buffer.
	if(szSubkey != NULL)
	{
		strcat(szKeyBuf, "\\");
		strcat(szKeyBuf, szSubkey );
	}

	// Create and open key and subkey.
	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT, szKeyBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if(lResult != ERROR_SUCCESS)
		return FALSE;

	// Set the Value.
	if(szValue != NULL)
		RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE *)szValue, strlen(szValue)+1);

	RegCloseKey(hKey);
	return TRUE;
}

// Open a key and set a value.
static BOOL setValueInKey(const char* szKey, const char* szNamedValue, const char* szValue)
{
	HKEY hKey;
	char szKeyBuf[1024];

	// Copy keyname into buffer.
	strcpy(szKeyBuf, szKey);

	// Create and open key and subkey.
	long lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, szKeyBuf, 0, KEY_SET_VALUE, &hKey);
	if(lResult != ERROR_SUCCESS)
		return FALSE;

    // Set the Value.
	if(szValue != NULL)
		RegSetValueEx(hKey, szNamedValue, 0, REG_SZ, (BYTE*)szValue, strlen(szValue)+1);

	RegCloseKey(hKey);
	return TRUE;
}

// Register the component in the registry.
HRESULT RegisterServer(const CLSID *clsid,           // Class ID
                       const char* szFriendlyName,   // Friendly Name
                       const char* szVerIndProgID,   // Programmatic
                       const char* szProgID,         // IDs
                       const char* szThreadingModel) // ThreadingModel
{
	// Get server location.
	char szModule[512];
	HMODULE hModule = GetModuleHandle(NULL);
	DWORD dwResult = GetModuleFileName(hModule, szModule, sizeof(szModule)/sizeof(char));
	if (dwResult != 0) exit(1);

	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(*clsid, szCLSID, sizeof(szCLSID));

	// Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);
  
	// Add the CLSID to the registry.
	setKeyAndValue(szKey, NULL, szFriendlyName);

	// Add the server filename subkey under the CLSID key.
	setKeyAndValue(szKey, "LocalServer32", szModule);

	// Add the ProgID subkey under the CLSID key.
	setKeyAndValue(szKey, "ProgID", szProgID);

	// Add the version-independent ProgID subkey under CLSID key.
	setKeyAndValue(szKey, "VersionIndependentProgID", szVerIndProgID);

	// Add the version-independent ProgID subkey under HKEY_CLASSES_ROOT.
	setKeyAndValue(szVerIndProgID, NULL, szFriendlyName); 
	setKeyAndValue(szVerIndProgID, "CLSID", szCLSID);
	setKeyAndValue(szVerIndProgID, "CurVer", szProgID);

	// Add the versioned ProgID subkey under HKEY_CLASSES_ROOT.
	setKeyAndValue(szProgID, NULL, szFriendlyName); 
	setKeyAndValue(szProgID, "CLSID", szCLSID);

	return S_OK;
}

// Remove the component from the registry.
LONG UnregisterServer(const CLSID *clsid,         // Class ID
                      const char* szVerIndProgID, // Programmatic
                      const char* szProgID)       // IDs
{
	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(*clsid, szCLSID, sizeof(szCLSID));

	// Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	// Delete the CLSID Key - CLSID\{...}
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	if((lResult != ERROR_SUCCESS) && (lResult != ERROR_FILE_NOT_FOUND)) exit(1); // Subkey may not exist.

	// Delete the version-independent ProgID Key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	if((lResult != ERROR_SUCCESS) && (lResult != ERROR_FILE_NOT_FOUND)) exit(1); // Subkey may not exist.

	// Delete the ProgID key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	if((lResult != ERROR_SUCCESS) && (lResult != ERROR_FILE_NOT_FOUND)) exit(1); // Subkey may not exist.

	return S_OK;
}
