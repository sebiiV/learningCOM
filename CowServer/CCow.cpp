#include "ComCow.h"
#include "ComCowCommon.h"

// ICow methods
STDMETHODIMP CCow::HelloCow(void) {
	MessageBox(NULL, "Moo", "Moo", MB_OK);
	return S_OK;
};


/*
COWCLASS FACTORY
*/

STDMETHODIMP CCowClassFactory::QueryInterface(REFIID riid, LPVOID* ppv) {
	*ppv = NULL;
	if (IID_IUnknown == riid)
		*ppv = (LPVOID)(IUnknown*)this;
	else if (IID_IClassFactory == riid)
		*ppv = (LPVOID)(IClassFactory*)this;
	else
		return E_NOINTERFACE;

	AddRef();

	return NOERROR;
}

STDMETHODIMP_(ULONG) CCowClassFactory::AddRef(void) {
	return ++i_refcnt;
}
STDMETHODIMP_(ULONG) CCowClassFactory::Release(void) {
	--i_refcnt;

	if (0 == i_refcnt)
	{
		delete this;
		return 0;
	}

	return i_refcnt;
}



