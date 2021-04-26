struct IUnknown;
#define NOMINMAX
#include <Windows.h>

#include <vector>
using std::vector;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include "WinApiWrappers.h"

HINSTANCE hInstance;
bool visualStylesEnabled = false;


void EnableVisualStyles()
{
    if (visualStylesEnabled) return;
    visualStylesEnabled = true;
    ACTCTXA activationContext = {};
    activationContext.cbSize = sizeof(activationContext);
    activationContext.dwFlags = ACTCTX_FLAG_HMODULE_VALID | ACTCTX_FLAG_SET_PROCESS_DEFAULT | ACTCTX_FLAG_RESOURCE_NAME_VALID;
    activationContext.lpSource = NULL;
    activationContext.wProcessorArchitecture = 0;
    activationContext.wLangId = 0;
    activationContext.lpAssemblyDirectory = NULL;
    activationContext.lpResourceName = MAKEINTRESOURCEA(2);
    activationContext.lpApplicationName = NULL;
    activationContext.hModule = hInstance;

    /*
    char dir[MAX_PATH];
    GetSystemDirectoryA(dir, MAX_PATH);

    ACTCTXA activationContext = {};
    activationContext.cbSize = sizeof(activationContext);
    activationContext.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_SET_PROCESS_DEFAULT | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID;
    activationContext.lpSource = "Shell32.dll";
    activationContext.wProcessorArchitecture = 0;
    activationContext.wLangId = 0;
    activationContext.lpAssemblyDirectory = dir;
    activationContext.lpResourceName = MAKEINTRESOURCEA(124);
    activationContext.lpApplicationName = NULL;
    activationContext.hModule = NULL;
    */
    ULONG_PTR activationCookie = 0;
    HANDLE hContext = CreateActCtxA(&activationContext);
    ActivateActCtx(hContext, &activationCookie);

}

void SetDPIAware()
{
    HMODULE user32 = LoadLibraryA("user32.dll");
    FARPROC pSetProcessDpiAwarenessContext = GetProcAddress(user32, "SetProcessDpiAwarenessContext");
    FARPROC pSetProcessDpiAwareness = NULL;
    FARPROC pSetProcessDPIAware = GetProcAddress(user32, "SetProcessDPIAware");
    if (pSetProcessDpiAwarenessContext != NULL)
    {
        typedef BOOL(WINAPI* SetProcessDpiAwarenessContext_FUNC)(DWORD value);
        SetProcessDpiAwarenessContext_FUNC SetProcessDpiAwarenessContext_ = (SetProcessDpiAwarenessContext_FUNC)pSetProcessDpiAwarenessContext;
        SetProcessDpiAwarenessContext_(-4); //DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
    }
    else
    {
        HMODULE shcore = LoadLibraryA("Shcore.dll");
        pSetProcessDpiAwareness = GetProcAddress(shcore, "SetProcessDpiAwareness");
        if (pSetProcessDpiAwareness != NULL)
        {
            typedef HRESULT(WINAPI* SetProcessDpiAwareness_FUNC)(DWORD value);
            SetProcessDpiAwareness_FUNC SetProcessDpiAwareness_ = (SetProcessDpiAwareness_FUNC)pSetProcessDpiAwareness;
            SetProcessDpiAwareness_(2); //PROCESS_PER_MONITOR_DPI_AWARE
        }
        else if (pSetProcessDPIAware)
        {
            typedef BOOL(WINAPI* SetProcessDPIAware_FUNC)();
            SetProcessDPIAware_FUNC SetProcessDPIAware_ = (SetProcessDPIAware_FUNC)pSetProcessDPIAware;
            SetProcessDPIAware_();
        }
    }
}

void HideDebugger()
{
    typedef struct _PEB {
        BYTE Reserved1[2];
        BYTE BeingDebugged;
        //remaining data removed
    } PEB, * PPEB;

    typedef struct _TEB {
        PVOID Reserved1[12];
        PPEB ProcessEnvironmentBlock;
        ULONG LastErrorValue;
        //remaining data removed
    } TEB, * PTEB;

    BOOL beingDebugged = IsDebuggerPresent();
    if (beingDebugged)
    {
        _TEB* teb = (_TEB*)NtCurrentTeb();
        teb->ProcessEnvironmentBlock->BeingDebugged = 0;
    }
}

extern vector<HMODULE> GetApplicationDLLs();

void DoHacks()
{
    HideDebugger();
    SetDPIAware();

    BuildImportMap();
    ReplaceImports();

    //enumerate modules
    vector<HMODULE> modules = GetApplicationDLLs();
    for (int i = 0; i < modules.size(); i++)
    {
        ReplaceImports(modules[i]);
    }
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        hInstance = hinstDLL;
        extern CRITICAL_SECTION d3d9CriticalSection;
        InitializeCriticalSection(&d3d9CriticalSection);
        DoHacks();
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
