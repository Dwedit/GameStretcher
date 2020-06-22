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
