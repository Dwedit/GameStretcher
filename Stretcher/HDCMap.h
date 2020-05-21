#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>

#include "TinyMap.h"

class WindowContext;
struct HDCMapValue
{
	int refCount;
	WindowContext* windowContext;
	HDCMapValue() : refCount(0), windowContext(NULL) {}
	HDCMapValue(int refCount, WindowContext *windowContext) : refCount(refCount), windowContext(windowContext) {}
};

class HDCMap
{
	TinyMap<HDC, HDCMapValue> hdcMap;
public:
	WindowContext* Get(HDC hdc);
	int GetRefCount(HDC hdc);
	int AddRef(HDC hdc, WindowContext* windowContext);
	int SubtractRef(HDC hdc);
};
