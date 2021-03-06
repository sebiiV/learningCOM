#ifndef COWCOMMON_H
#define COWCOMMON_H
#include <objbase.h>
#include <OAIdl.h>
#include <initguid.h>

extern ULONG g_cObjects;
extern ULONG g_cLocks;

// generated with https://www.guidgenerator.com/online-guid-generator.aspx

// {5c5a00ab - 8f75 - 4389 - 9bdd - fc1dff8413b2} 
// 0x5c5a00ab,0x8f75,0x4389,0x9b,0xdd,0xfc,0x1d,0xff,0x84,0x13,0xb2
DEFINE_GUID(CLSID_CowServer, 0x5c5a00ab, 0x8f75, 0x4389, 0x9b, 0xdd, 0xfc, 0x1d, 0xff, 0x84, 0x13, 0xb2);


// {d38e82d6 - 7c14 - 4997 - bb51 - ffb205d5670c}
// 0xd38e82d6,0x7c14,0x4997,0xbb,0x51,0xff,0xb2,0x05,0xd5,0x67,0x0c
DEFINE_GUID(IID_ICow, 0xd38e82d6, 0x7c14, 0x4997, 0xbb, 0x51, 0xff, 0xb2, 0x05, 0xd5, 0x67, 0x0c);

DECLARE_INTERFACE_(ICow, IUnknown)
{
	// expands to virtual  __declspec(nothrow) HRESULT __stdcall HelloCow = 0
	// pure virtual function (subclasses have to implement)
	STDMETHOD(HelloCow)() PURE;
};
#endif