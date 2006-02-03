// Registry.cpp (code fourni avec Visual C++ de Microsoft)
#include <objbase.h>
#include <assert.h>
#include "registry.h"

// Size of a CLSID as a string
const int CLSID_STRING_SIZE = 39;

// Register the component in the registry.
HRESULT RegisterServer(const char* szModuleName,     // DLL module handle
                       REFCLSID clsid,               // Class ID
                       const char* szFriendlyName,   // Friendly Name
                       const char* szVerIndProgID,   // Programmatic
                       const char* szProgID,         // IDs
					   const char* szThreadingModel) // ThreadingModel
{
	// Get server location.
	char szModule[512];
	HMODULE hModule = GetModuleHandle(szModuleName);
	DWORD dwResult = GetModuleFileName(hModule, szModule, sizeof(szModule)/sizeof(char));
	assert(dwResult != 0);

	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	// Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);
  
	// Add the CLSID to the registry.
	setKeyAndValue(szKey, NULL, szFriendlyName);

	// Add the server filename subkey under the CLSID key.
	if(strstr(szModuleName, ".exe") == NULL)
	{
		setKeyAndValue(szKey, "InprocServer32", szModule);
		char szInproc[64];
		strcpy(szInproc, szKey);
		strcat(szInproc, "\\InprocServer32");
		setValueInKey(szInproc, "ThreadingModel", szThreadingModel);
	}
	else
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
LONG UnregisterServer(REFCLSID clsid,             // Class ID
                      const char* szVerIndProgID, // Programmatic
                      const char* szProgID)       // IDs
{
	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	// Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	// Delete the CLSID Key - CLSID\{...}
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	// Delete the version-independent ProgID Key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	// Delete the ProgID key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	return S_OK;
}

// Convert a CLSID to a char string.
void CLSIDtochar(REFCLSID clsid, char* szCLSID, int length)
{
	assert(length >= CLSID_STRING_SIZE);
	// Get CLSID
	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	assert(SUCCEEDED(hr));

	// Covert from wide characters to non-wide.
	wcstombs(szCLSID, wszCLSID, length);

	// Free memory.
	CoTaskMemFree(wszCLSID);
}

// Delete a key and all of its descendents.
LONG recursiveDeleteKey(HKEY hKeyParent,           // Parent of key to delete
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
BOOL setKeyAndValue(const char* szKey, const char* szSubkey, const char* szValue)
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
BOOL setValueInKey(const char* szKey, const char* szNamedValue, const char* szValue)
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
HRESULT RegisterServerTreatAs(
                       const char* szModuleName,      // DLL module handle
                       REFCLSID clsid,                // Class ID
                       const char* szFriendlyName,    // Friendly Name
                       const char* szVerIndProgID,    // Programmatic
                       const char* szProgID,          // IDs
					   const char* szThreadingModel,  // ThreadingModel
					   REFCLSID treatedAsClsId,       // Treat As Class ID
                       const char* szTreatedAsFriendlyName, // Treated as Friendly Name
					   const char* szTreatedAsProgId) // Treat As Prog ID
{	HRESULT hr;
	// Get server location.
	char szModule[512];
	HMODULE hModule = GetModuleHandle(szModuleName);
	DWORD dwResult = GetModuleFileName(hModule, szModule, sizeof(szModule)/sizeof(char));
	assert(dwResult != 0);

	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	// Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);
  
	// Add the CLSID to the registry.
	setKeyAndValue(szKey, NULL, szFriendlyName);

	// Add the server filename subkey under the CLSID key.
	if(strstr(szModuleName, ".exe") == NULL)
	{
		setKeyAndValue(szKey, "InprocServer32", szModule);
		char szInproc[64];
		strcpy(szInproc, szKey);
		strcat(szInproc, "\\InprocServer32");
		setValueInKey(szInproc, "ThreadingModel", szThreadingModel);
	}
	else
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

	// This class is treated as
	hr = CoTreatAsClass(treatedAsClsId, clsid);

	// Convert the Treated As CLSID into a char.
	char szTreatedAsCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(treatedAsClsId, szTreatedAsCLSID, sizeof(szTreatedAsCLSID));

	// Build the Treated As key CLSID\\{...}
	char szTreatedAsKey[64];
	strcpy(szTreatedAsKey, "CLSID\\");
	strcat(szTreatedAsKey, szTreatedAsCLSID);

	// Add a ProgID subkey under CLSID key
	setKeyAndValue(szTreatedAsKey,NULL, szTreatedAsFriendlyName);
	setKeyAndValue(szTreatedAsKey,"ProgID", szTreatedAsProgId);

	// Add a ProgID subkey under HKEY_CLASSES_ROOT
	setKeyAndValue(szTreatedAsProgId,NULL, szTreatedAsFriendlyName);
	setKeyAndValue(szTreatedAsProgId,"CLSID", szTreatedAsCLSID);

	return S_OK;
}

// Remove the component from the registry.
LONG UnregisterServerTreatAs(
                      REFCLSID clsid,                // Class ID
                      const char* szVerIndProgID,    // Programmatic
                      const char* szProgID,          // IDs
                      REFCLSID treatedAsClsId,       // Treated As Class ID
                      const char* szTreatedAsProgID) // IDs
{
	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	// Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	// Delete the CLSID Key - CLSID\{...}
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	// Delete the version-independent ProgID Key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	// Delete the ProgID key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	// Convert the CLSID into a char.
	char szTreatedAsCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(treatedAsClsId, szTreatedAsCLSID, sizeof(szTreatedAsCLSID));

	// Build the key CLSID\\{...}
	char szTreatedAsKey[64];
	strcpy(szTreatedAsKey, "CLSID\\");
	strcat(szTreatedAsKey, szCLSID);

	// Delete the Treated as CLSID Key - CLSID\{...}
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szTreatedAsKey);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	// Delete the Treated As ProgID key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szTreatedAsProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	return S_OK;
}
