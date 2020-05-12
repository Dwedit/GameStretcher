struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <Psapi.h>
#include <algorithm>
using std::wstring;
using std::pair;
using std::make_pair;
using std::max;
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

wstring dialogExeName;
DWORD dialogPid;
UINT_PTR dialogTimer;
HANDLE dialogHProcess;

wstring GetFileName(const wstring &path)
{
    int i;
    for (i = (int)path.length() - 1; i >= 0; i--)
    {
        if (path[i] == '/' || path[i] == '\\')
        {
            return path.substr(i + 1);
        }
    }
    return path;
}

bool DebuggerDialog(const PROCESS_INFORMATION& processInformation)
{
    wstring exeName;
    exeName.resize(256);
    DWORD nameLength = (DWORD)exeName.size();
    QueryFullProcessImageNameW(processInformation.hProcess, 0, &exeName[0], &nameLength);
    exeName.resize(wcslen(&exeName[0]));
    exeName = GetFileName(exeName);
    dialogExeName = exeName;
    dialogPid = processInformation.dwProcessId;
    dialogHProcess = processInformation.hProcess;

    if (DialogBox(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDD_DEBUGGER), NULL, (DLGPROC)DlgProc) == IDOK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
    {
        wstring prompt = L"Waiting for debugger to be attached to process " + dialogExeName + L" (PID " + std::to_wstring(dialogPid) + L")";
        SetDlgItemTextW(hwnd, IDPROMPT, prompt.c_str());
        dialogTimer = SetTimer(hwnd, 0, 250, NULL);
    }
    break;
    case WM_TIMER:
    {
        BOOL debuggerPresent = 0;
        CheckRemoteDebuggerPresent(dialogHProcess, &debuggerPresent);
        if (debuggerPresent)
        {
            EndDialog(hwnd, IDOK);
        }
    }
    break;
    case WM_COMMAND:
    {
        if (wParam == IDOK)
        {
            EndDialog(hwnd, IDOK);
        }
        if (wParam == IDCANCEL)
        {
            EndDialog(hwnd, IDCANCEL);
        }
    }
    break;
    }
    return 0;
}

