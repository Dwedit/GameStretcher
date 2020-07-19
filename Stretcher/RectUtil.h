#pragma once
struct IUnknown;
#define NOMINMAX
#include <Windows.h>

static inline bool operator==(const RECT& rect1, const RECT& rect2)
{
	return (0 == memcmp(&rect1, &rect2, sizeof(RECT)));
}
static inline bool operator!=(const RECT& rect1, const RECT& rect2)
{
	return (0 != memcmp(&rect1, &rect2, sizeof(RECT)));
}
static inline bool RectHasArea(LPCRECT rect)
{
	if (rect == NULL) return false;
	if (rect->right - rect->left <= 0) return false;
	if (rect->bottom - rect->top <= 0) return false;
	return true;
}
