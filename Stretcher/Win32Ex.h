#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>

HWND FindMainWindow();
RECT GetMonitorRect();
RECT GetMonitorRect(HWND window);
RECT GetMonitorWorkAreaRect(HWND window);
bool IsWow64();
bool IsWine();

template <class TComObject>
inline UINT SafeRelease(TComObject*& obj)
{
	UINT refCount = 0;
	if (obj != NULL)
	{
		refCount = obj->Release();
		obj = NULL;
	}
	return refCount;
}
template <class TComObject>
inline UINT SafeAddRef(TComObject* obj)
{
	UINT refCount = 0;
	if (obj != NULL)
	{
		refCount = obj->AddRef();
	}
	return refCount;
}
template <class TComObject>
inline UINT GetRefCount(TComObject* obj)
{
	if (obj == NULL) return 0;
	obj->AddRef();
	return obj->Release();
}
