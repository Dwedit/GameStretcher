#include "HDCMap.h"

WindowContext* HDCMap::Get(HDC hdc)
{
	HDCMapValue *value = hdcMap.GetReference(hdc);
	if (value == NULL) return NULL;
	return value->windowContext;
}

int HDCMap::GetRefCount(HDC hdc)
{
	HDCMapValue* value = hdcMap.GetReference(hdc);
	if (value == NULL) return -1;
	return value->refCount;
}

int HDCMap::AddRef(HDC hdc, WindowContext* windowContext)
{
	hdcMap.AddDefault(hdc);
	HDCMapValue* value = hdcMap.GetReference(hdc);
	value->refCount++;
	value->windowContext = windowContext;
	return value->refCount;
}

int HDCMap::SubtractRef(HDC hdc)
{
	HDCMapValue* value = hdcMap.GetReference(hdc);
	if (value == NULL) return -1;
	value->refCount--;
	int refCount = value->refCount;
	if (refCount == 0)
	{
		hdcMap.Remove(hdc);
	}
	return refCount;
}
