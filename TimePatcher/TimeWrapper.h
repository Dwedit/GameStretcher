#pragma once
struct IUnknown;
#define NOMINMAX
#include <Windows.h>

//Function Pointer Typedefs
typedef MMRESULT(WINAPI* timeGetSystemTime_FUNC)(LPMMTIME pmmt, UINT cbmmt);
typedef DWORD(WINAPI* timeGetTime_FUNC)();
typedef MMRESULT(WINAPI* timeGetDevCaps_FUNC)(LPTIMECAPS ptc, UINT cbtc);
typedef MMRESULT(WINAPI* timeBeginPeriod_FUNC)(UINT uPeriod);
typedef MMRESULT(WINAPI* timeEndPeriod_FUNC)(UINT uPeriod);

//Import Backups (Declarations)
extern timeGetSystemTime_FUNC timeGetSystemTime_OLD;
extern timeGetTime_FUNC timeGetTime_OLD;
extern timeGetDevCaps_FUNC timeGetDevCaps_OLD;
extern timeBeginPeriod_FUNC timeBeginPeriod_OLD;
extern timeEndPeriod_FUNC timeEndPeriod_OLD;

//Replacement Functions (Declarations)
MMRESULT WINAPI timeGetSystemTime_Replacement(LPMMTIME pmmt, UINT cbmmt);
DWORD WINAPI timeGetTime_Replacement();
MMRESULT WINAPI timeGetDevCaps_Replacement(LPTIMECAPS ptc, UINT cbtc);
MMRESULT WINAPI timeBeginPeriod_Replacement(UINT uPeriod);
MMRESULT WINAPI timeEndPeriod_Replacement(UINT uPeriod);

extern void ReplaceImports();
