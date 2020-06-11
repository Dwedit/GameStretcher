#include "TimeWrapper.h"
#include "ImportReplacer.h"

//Replace the Functions
void ReplaceImports()
{
	ImportReplacer replacer(GetModuleHandle(NULL));
	replacer.GetImports("winmm.dll");
#define ReplaceImport(dllName, functionName, replacementFunction, pOldFunction) \
    replacer.ReplaceImport((dllName),(functionName),(replacementFunction),(pOldFunction))

	ReplaceImport("winmm.dll", "timeGetSystemTime", (FARPROC)timeGetSystemTime_Replacement, (FARPROC*)&timeGetSystemTime_OLD);
	ReplaceImport("winmm.dll", "timeGetTime", (FARPROC)timeGetTime_Replacement, (FARPROC*)&timeGetTime_OLD);
	ReplaceImport("winmm.dll", "timeGetDevCaps", (FARPROC)timeGetDevCaps_Replacement, (FARPROC*)&timeGetDevCaps_OLD);
	ReplaceImport("winmm.dll", "timeBeginPeriod", (FARPROC)timeBeginPeriod_Replacement, (FARPROC*)&timeBeginPeriod_OLD);
	ReplaceImport("winmm.dll", "timeEndPeriod", (FARPROC)timeEndPeriod_Replacement, (FARPROC*)&timeEndPeriod_OLD);
}

//Import Backups (Definitions)
timeGetSystemTime_FUNC timeGetSystemTime_OLD = NULL;
timeGetTime_FUNC timeGetTime_OLD = NULL;
timeGetDevCaps_FUNC timeGetDevCaps_OLD = NULL;
timeBeginPeriod_FUNC timeBeginPeriod_OLD = NULL;
timeEndPeriod_FUNC timeEndPeriod_OLD = NULL;

//Replacement Functions Code
MMRESULT WINAPI timeGetSystemTime_Replacement(LPMMTIME pmmt, UINT cbmmt)
{
	return timeGetSystemTime_OLD(pmmt, cbmmt);
}

bool timeInitialized = false;
DWORD timeZero = 0;

DWORD WINAPI timeGetTime_Replacement()
{
	if (!timeInitialized)
	{
		timeZero = timeGetTime_OLD();
		timeInitialized;
	}
	return timeGetTime_OLD() - timeZero;
}

MMRESULT WINAPI timeGetDevCaps_Replacement(LPTIMECAPS ptc, UINT cbtc)
{
	return timeGetDevCaps_OLD(ptc, cbtc);
}
MMRESULT WINAPI timeBeginPeriod_Replacement(UINT uPeriod)
{
	return timeBeginPeriod_OLD(uPeriod);
}
MMRESULT WINAPI timeEndPeriod_Replacement(UINT uPeriod)
{
	return timeEndPeriod_OLD(uPeriod);
}
