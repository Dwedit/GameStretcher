struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include "Win32Ex.h"

#define HAVE_WINDOW_CONTEXT 1

struct _searchForMainWindowData
{
	HWND foundHwnd;
	DWORD targetProcessId;
};
BOOL CALLBACK _searchForMainWindowCb(HWND handle, LPARAM lParam);
HWND SearchForMainWindow()
{
	_searchForMainWindowData data = {};
	DWORD currentProcessId = GetCurrentProcessId();
	data.targetProcessId = currentProcessId;
	EnumWindows(_searchForMainWindowCb, (LPARAM)&data);
	return data.foundHwnd;
}
BOOL CALLBACK _searchForMainWindowCb(HWND hwnd, LPARAM lParam)
{
	_searchForMainWindowData* data = (_searchForMainWindowData*)lParam;
	DWORD windowProcessId, windowThreadId;
	windowThreadId = GetWindowThreadProcessId(hwnd, &windowProcessId);
	if (windowProcessId == data->targetProcessId)
	{
		if (IsWindowVisible(hwnd) && NULL == GetWindow(hwnd, GW_OWNER))
		{
			wchar_t classNameBuffer[256];
			RealGetWindowClassW(hwnd, &classNameBuffer[0], 256);
			if (0 != wcscmp(classNameBuffer, L"#32770"))
			{
				data->foundHwnd = hwnd;
				return false;
			}
		}
	}
	return true;
}

#if HAVE_WINDOW_CONTEXT
#include "WindowContext.h"
HWND FindMainWindow()
{
	auto context = WindowContext::GetWindowContext();
	if (context != NULL)
	{
		return context->GetHwnd();
	}
	return SearchForMainWindow();
}
#else
HWND FindMainWindow()
{
	return SearchForMainWindow();
}
#endif

RECT GetMonitorRect()
{
	return GetMonitorRect(FindMainWindow());
}
RECT GetMonitorRect(HWND window)
{
	HMONITOR monitor;
	if (window == NULL)
	{
		monitor = MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY);
	}
	else
	{
		monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	}
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoA(monitor, &monitorInfo);
	return monitorInfo.rcMonitor;
}
RECT GetMonitorWorkAreaRect(HWND window)
{
	HMONITOR monitor;
	if (window == NULL)
	{
		monitor = MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY);
	}
	else
	{
		monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	}
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoA(monitor, &monitorInfo);
	return monitorInfo.rcMonitor;
}

static int _isWow64 = -1;

bool IsWow64()
{
	if (_isWow64 == -1)
	{
		HMODULE kernel32 = LoadLibraryA("kernel32.dll");
		FARPROC isWow64Proc = GetProcAddress(kernel32, "IsWow64Process");

		typedef BOOL (__stdcall*IsWow64Process_FUNC)(HANDLE hProcess,PBOOL  Wow64Process);
		IsWow64Process_FUNC IsWow64Process_ = (IsWow64Process_FUNC)isWow64Proc;
		if (IsWow64Process_ != NULL)
		{
			BOOL isWow64 = 0;
			BOOL success = 0;
			success = IsWow64Process_(GetCurrentProcess(), &isWow64);
			if (success && isWow64)
			{
				_isWow64 = isWow64;
			}
		}
		if (_isWow64 == -1) _isWow64 = 0;
	}
	return _isWow64;
}

static int _isWine = -1;

bool IsWine()
{
	if (_isWine == -1)
	{
		HMODULE ntdllModule = GetModuleHandleA("ntdll.dll");
		_isWine = NULL != GetProcAddress(ntdllModule, "wine_get_version");
	}
	return _isWine;
}
