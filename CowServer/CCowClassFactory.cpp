#include "ComCow.h"
#include "ComCowCommon.h"

// IClassFactory methods
STDMETHODIMP CCowClassFactory::CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void** ppv)
{

	HRESULT hr;
	CCow* pCCow = NULL;

	*ppv = NULL;
	if (NULL != pUnknownOuter)
		return CLASS_E_NOAGGREGATION;

	pCCow = new CCow();

	if (NULL == pCCow)
		return E_OUTOFMEMORY;

	hr = pCCow->QueryInterface(riid, ppv);
	if (FAILED(hr))
	{
		delete pCCow;
		pCCow = NULL;
		return hr;
	}

	g_cObjects++;

	return NOERROR;
}
STDMETHODIMP CCowClassFactory::LockServer(BOOL bLock) {
			if (bLock) g_cLocks++;
		else g_cLocks--;

	return NOERROR;


}

