#include <iostream>
#define INITGUID
#include <objbase.h>

#include "..\CowServer\ComCowCommon.h"


void main() {
	IClassFactory* pCf;
	IUnknown* pUnk;
	ICow* pCow;
	HRESULT hr;

	std::cout << "main: calling CoInitialize() " << std::endl;

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		std::cout << "main: CoInitialize Failed!" << std::endl;
		exit(-1);
	}

	std::cout << "main: CoInitialize OK" << std::endl;
	std::cout << "main: Calling CoGetClassObject" << std::endl;

	hr = CoGetClassObject(CLSID_CowServer, CLSCTX_INPROC_SERVER, NULL, IID_IClassFactory, (void**)&pCf);
	if (FAILED(hr))
	{
		std::cout << "main: CoGetClassObject Failed!\n" << std::endl;
		goto EXIT00;
	}
	std::cout << "main: CoGetClassObject OK\n" << std::endl;


	std::cout << "main:  calling pCf->CreateInstance(NULL,IID_IUnknown,...)" << std::endl;
	hr = pCf->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);
	if (FAILED(hr))
	{
		std::cout << "main: pCf->CreateInstance(NULL,IID_IUnknown,...) Failed!" << std::endl;
		goto CLEANUP00;


	}
	std::cout << "main:  pCf->CreateInstance(NULL,IID_IUnknown,...) OK" << std::endl;

	std::cout << "main:  calling pUnk->QueryInterface(IID_ICow,...)" << std::endl;
	hr = pUnk->QueryInterface(IID_ICow, (void**)&pCow);
	if (FAILED(hr))
	{
		std::cout << "main: pUnk->QueryInterface(IID_ICow,...) Failed!\n" << std::endl;
		goto CLEANUP01;
	}
	std::cout << "main: pUnk->QueryInterface(IID_ICow,...) OK\n" << std::endl;


	std::cout << "main: Now Calling into the ICow Interface...Hello Cow!\n" << std::endl;
	hr = pCow->HelloCow();
	if (FAILED(hr))
	{
		std::cout << "main: pCow->HelloCow() Failed!\n" << std::endl;
		goto CLEANUP02;
	}
	std::cout << "main: pCow->HelloCow() OK!\n" << std::endl;

/*
CLEANUP
*/

CLEANUP02:
	std::cout << "main: calling ICow::Release" << std::endl;
	pCow->Release();

CLEANUP01:
	std::cout << "main: calling IUnknown::Release" << std::endl;
	pUnk->Release();



CLEANUP00:
	std::cout << "main: calling IClassFactory::Release" << std::endl;
	pCf->Release();

EXIT00:
	CoUninitialize();
	std::cout << "main: We are done!" << std::endl;


}