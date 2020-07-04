struct IUnknown;
#define NOMINMAX
#include <Windows.h>

RECT GetMonitorRect(HWND window)
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoA(monitor, &monitorInfo);
	return monitorInfo.rcMonitor;
}
RECT GetMonitorWorkAreaRect(HWND window)
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
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
