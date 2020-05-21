#define MAXPATH 255
#define MAX_STRING_LENGTH 255
#define GUID_SIZE 128

#include "ComCowCommon.h"

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* pOut);
STDAPI DllCanUnloadNow(void);
STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);
BOOL   SetRegKeyValue(HKEY hKeyRoot, LPTSTR lpszKey, LPTSTR lpszSubKey, LPTSTR lpszNamedValue, LPTSTR lpszValue);


/* 
https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iclassfactory-createinstance
https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown
*/
class CCowClassFactory : public IClassFactory {
private:
	int i_refcnt;

public:
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// IClassFactory methods
	STDMETHODIMP CreateInstance(IUnknown* pk, REFIID riid, void** ppv);
	STDMETHODIMP LockServer(BOOL fLock);

	//Constructer & Deconstructor
	CCowClassFactory() { i_refcnt = 0; }
	~CCowClassFactory() {}
};

class CCow : public ICow
{
private:
	int i_refcnt;
public:
	CCow() { i_refcnt = 0; };
	~CCow(){};

	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// ICow methods
	STDMETHOD(HelloCow)(void);

};


