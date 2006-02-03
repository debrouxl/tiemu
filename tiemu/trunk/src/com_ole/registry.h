// Registry.h (code fourni avec Visual C++ de Microsoft)

// This function will register a component.
HRESULT RegisterServer(const char* szModuleName, REFCLSID clsid, const char* szFriendlyName, const char* szVerIndProgID, const char* szProgID, const char* szThreadingModel);

HRESULT RegisterServerTreatAs(
                       const char* szModuleName,      // DLL module handle
                       REFCLSID clsid,                // Class ID
                       const char* szFriendlyName,    // Friendly Name
                       const char* szVerIndProgID,    // Programmatic
                       const char* szProgID,          // IDs
					   const char* szThreadingModel,  // ThreadingModel
					   REFCLSID treatedAsClsId,       // Treat As Class ID
                       const char* szTreatedAsFriendlyName, // Treated as Friendly Name
					   const char* szTreatedAsProgId);// Treat As Prog ID

// This function will unregister a component.
HRESULT UnregisterServer(REFCLSID clsid, const char* szVerIndProgID, const char* szProgID);

LONG UnregisterServerTreatAs(REFCLSID clsid,                // Class ID
                      const char* szVerIndProgID,    // Programmatic
                      const char* szProgID,          // IDs
					  REFCLSID treatedAsClsId,       // Treated As Class ID
                      const char* szTreatedAsProgID);// IDs

// Set the given key and its value.
BOOL setKeyAndValue(const char* pszPath, const char* szSubkey, const char* szValue);

// Open a key and set a value.
BOOL setValueInKey(const char* szKey, const char* szNamedValue, const char* szValue);

// Convert a CLSID into a char string.
void CLSIDtochar(REFCLSID clsid, char* szCLSID, int length);

// Delete szKeyChild and all of its descendents.
LONG recursiveDeleteKey(HKEY hKeyParent, const char* szKeyChild);
