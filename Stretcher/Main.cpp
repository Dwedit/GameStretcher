struct IUnknown;
#define NOMINMAX
#include <Windows.h>

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



void DoHacks()
{
    ReplaceImports();
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
