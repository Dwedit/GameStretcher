#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>

RECT GetMonitorRect(HWND window);
RECT GetMonitorWorkAreaRect(HWND window);
bool IsWow64();
bool IsWine();

