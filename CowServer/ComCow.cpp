// defines entry point for the dll application

#include <Windows.h>
#include <objbase.h>
#include "ComCowCommon.h"
#include <tchar.h>
#include <OleCtl.h>
#include "ComCow.h"

HANDLE g_Module;
HMODULE g_hModule = NULL;
ULONG g_cObjects = 0;
ULONG g_cLocks = 0;

#pragma warning(disable:4996)


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason, LPVOID lpReserved) {
	if (DLL_PROCESS_ATTACH == ul_reason) g_Module = hModule;
	return TRUE;
}



/*
DLL Functions
*/
STDAPI DllCanUnloadNow(void) {
	if (0 == g_cObjects && 0 == g_cLocks)
		return TRUE;
	else
		return FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
	HRESULT hr = NOERROR;
	CCowClassFactory* pCf = NULL;

	if (CLSID_CowServer == rclsid)
	{
		//declare a new CF for CComCow class
		pCf = new CCowClassFactory();

		if (NULL == pCf) return E_OUTOFMEMORY;

		//get requested interface
		hr = pCf->QueryInterface(riid, ppv);

		if (FAILED(hr)) {
			delete(pCf);
			pCf = NULL;
			return hr;
		}
	}
	//object not supported
	else hr = CLASS_E_CLASSNOTAVAILABLE;

	return hr;
}

STDAPI DllRegisterServer(void) {
	BOOL bOk;
	_TCHAR szModulePath[MAXPATH];
	_TCHAR szCLSID[GUID_SIZE];
	_TCHAR szCLSIDKey[MAX_STRING_LENGTH];
	wchar_t wszGUID[GUID_SIZE];

	GetModuleFileName((HMODULE)g_Module, szModulePath, MAX_STRING_LENGTH);
	StringFromGUID2(CLSID_CowServer, wszGUID, sizeof(wszGUID) / sizeof(wchar_t));

	// Convert from the wide character set to the multibyte character set
	WideCharToMultiByte(CP_ACP, 0, wszGUID, -1, szCLSID, sizeof(szCLSID) / sizeof(_TCHAR), NULL, NULL);

	//HKEY_CLASSES_ROOT\CLSID
	_tcscpy(szCLSIDKey, _TEXT("CLSID\\"));
	_tcscat(szCLSIDKey, szCLSID);
	bOk = SetRegKeyValue(HKEY_CLASSES_ROOT, szCLSIDKey, NULL, NULL, _TEXT("Simple Cow In-Proc Server"));

	if (bOk)
		bOk = SetRegKeyValue(HKEY_CLASSES_ROOT, szCLSIDKey, TEXT("InProcServer32"), NULL, szModulePath);
	if (bOk)
		return NOERROR;
	else
		return SELFREG_E_CLASS;
}

STDAPI DllUnregisterServer(void)
{
	long lErrorCode;
	_TCHAR szCLSID[GUID_SIZE + 1];
	_TCHAR szCLSIDKey[MAX_STRING_LENGTH + 1];
	_TCHAR szInprocServer32Key[MAX_STRING_LENGTH + 1];
	wchar_t wszGUID[GUID_SIZE + 1];

	// Convert the CLSID to the format
	StringFromGUID2(CLSID_CowServer, wszGUID, sizeof(wszGUID) / sizeof(wchar_t));

	// Convert from the wide character set to the multibyte character set
	WideCharToMultiByte(CP_ACP, 0, wszGUID, -1, szCLSID, sizeof(szCLSID) / sizeof(_TCHAR), NULL, NULL);

	// HKEY_CLASSES_ROOT\CLSID
	_tcscpy(szCLSIDKey, _TEXT("CLSID\\"));
	_tcscat(szCLSIDKey, szCLSID);
	_tcscpy(szInprocServer32Key, szCLSIDKey);
	_tcscat(szInprocServer32Key, _TEXT("\\InProcServer32"));

	//Delete sub-keys first
	lErrorCode = RegDeleteKey(HKEY_CLASSES_ROOT, szInprocServer32Key);
	//Delete the entry under CLSID
	if (ERROR_SUCCESS == lErrorCode)
		lErrorCode = RegDeleteKey(HKEY_CLASSES_ROOT, szCLSIDKey);
	if (ERROR_SUCCESS == lErrorCode)
		return NOERROR;
	else
		return SELFREG_E_CLASS;


}


	/* This feels like a hack since I had to turn off comformance mode to get it to stop throwing errors... 
	Last value should maybe be a const char* ?
	*/
BOOL   SetRegKeyValue(HKEY hKeyRoot, LPTSTR lpszKey, LPTSTR lpszSubKey, LPTSTR lpszNamedValue, LPTSTR lpszValue) {
	BOOL bOk = FALSE;
	long lErrorCode;
	HKEY hKey;
	_TCHAR szKey[MAX_STRING_LENGTH ]; //do I need to +1 ?

	_tcscpy(szKey, lpszKey);
	if (NULL != lpszSubKey) {
		_tcscat(szKey,  _TEXT("\\"));
		_tcscat(szKey,  lpszSubKey);
	}

	lErrorCode = RegCreateKeyEx(hKeyRoot, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (ERROR_SUCCESS == lErrorCode)
	{
		lErrorCode = RegSetValueEx(hKey, lpszNamedValue, 0, REG_SZ, (BYTE*)lpszValue, (_tcslen(lpszValue) + 1) * sizeof(_TCHAR));
		if (ERROR_SUCCESS == lErrorCode)
			bOk = TRUE;
		RegCloseKey(hKey);
	}

	return bOk;

}

/*
Cow functions (with inherited from IUnknown and IClassFactory
*/

STDMETHODIMP CCow::QueryInterface(REFIID riid, LPVOID* ppv) {
	*ppv = NULL;
	if (IID_IUnknown == riid)
		*ppv = (LPVOID)(IUnknown*)this;
	else if (IID_ICow == riid)
		*ppv = (LPVOID)(ICow*)this;
	else
		return E_NOINTERFACE;
	AddRef();
	return NOERROR;
};
STDMETHODIMP_(ULONG) CCow::AddRef(void) {
	return ++i_refcnt;
};

STDMETHODIMP_(ULONG) CCow::Release(void) {
	--i_refcnt;

	if (0 == i_refcnt) {
		delete this;
		return 0;
	}
	return i_refcnt;
};






