struct IUnknown;
#define NOMINMAX
#include <Windows.h>

#define USE_CREATE_HOOK 1
#define USE_SHOW_HOOK 1

#include "WinApiWrappers.h"
#include "WindowContext.h"
#include "WindowClassContext.h"

extern void EnableVisualStyles();

/*
DWORD SetPageProtect(ULONG_PTR address, DWORD protection)
{
    BOOL okay;
    DWORD oldProtect;
    okay = VirtualProtect((LPVOID)address, sizeof(void*), protection, &oldProtect);
    if (okay)
    {
        return oldProtect;
    }
    return 0;
}

DWORD EnsurePageReadWrite(ULONG_PTR address)
{
    return SetPageProtect(address, PAGE_READWRITE);
}
*/

/*
FARPROC* FindImportAddress(const PVOID Base, LPCSTR moduleName, LPCSTR functionName);

bool ReplaceImport(LPCSTR dllName, LPCSTR functionName, FARPROC replacementFunction, FARPROC* pOldFunction)
{
    FARPROC* importAddress = FindImportAddress(GetModuleHandle(NULL), dllName, functionName);
    if (importAddress != NULL)
    {
        //ensure page is read-write
        DWORD oldProtect = EnsurePageReadWrite((ULONG_PTR)importAddress);

        if (EnsurePageReadWrite((ULONG_PTR)importAddress))
        {
            if (pOldFunction)
            {
                *pOldFunction = *importAddress;
            }
            *importAddress = replacementFunction;
            SetPageProtect((ULONG_PTR)importAddress, oldProtect);
            return true;
        }
    }
    return false;
}
*/

#include "ImportReplacer.h"

//Replace the Functions
void ReplaceImports()
{
    //object released when it goes out of scope
    ImportReplacer replacer(GetModuleHandle(NULL));
    replacer.GetImports("user32.dll");
    replacer.GetImports("gdi32.dll");
#define ReplaceImport(dllName, functionName, replacementFunction, pOldFunction) \
    replacer.ReplaceImport((dllName),(functionName),(replacementFunction),(pOldFunction))
    
    ReplaceImport("User32.dll", "ClientToScreen", (FARPROC)ClientToScreen_Replacement, (FARPROC*)&ClientToScreen_OLD);
    ReplaceImport("User32.dll", "ScreenToClient", (FARPROC)ScreenToClient_Replacement, (FARPROC*)&ScreenToClient_OLD);
    ReplaceImport("User32.dll", "MapWindowPoints", (FARPROC)MapWindowPoints_Replacement, (FARPROC*)&MapWindowPoints_OLD);
#if USE_CREATE_HOOK
    ReplaceImport("User32.dll", "CreateWindowExA", (FARPROC)CreateWindowExA_Replacement, (FARPROC*)&CreateWindowExA_OLD);
    ReplaceImport("User32.dll", "CreateWindowExW", (FARPROC)CreateWindowExW_Replacement, (FARPROC*)&CreateWindowExW_OLD);
#endif
    ReplaceImport("User32.dll", "ShowWindow", (FARPROC)ShowWindow_Replacement, (FARPROC*)&ShowWindow_OLD);
    ReplaceImport("User32.dll", "GetClientRect", (FARPROC)GetClientRect_Replacement, (FARPROC*)&GetClientRect_OLD);
	ReplaceImport("User32.dll", "GetWindowPlacement", (FARPROC)GetWindowPlacement_Replacement, (FARPROC*)&GetWindowPlacement_OLD);
	ReplaceImport("User32.dll", "GetWindowRect", (FARPROC)GetWindowRect_Replacement, (FARPROC*)&GetWindowRect_OLD);
	ReplaceImport("User32.dll", "MoveWindow", (FARPROC)MoveWindow_Replacement, (FARPROC*)&MoveWindow_OLD);
#if USE_CLASS_HOOK
    ReplaceImport("User32.dll", "RegisterClassA", (FARPROC)RegisterClassA_Replacement, (FARPROC*)&RegisterClassA_OLD);
	ReplaceImport("User32.dll", "RegisterClassW", (FARPROC)RegisterClassW_Replacement, (FARPROC*)&RegisterClassW_OLD);
    ReplaceImport("User32.dll", "RegisterClassExA", (FARPROC)RegisterClassExA_Replacement, (FARPROC*)&RegisterClassExA_OLD);
    ReplaceImport("User32.dll", "RegisterClassExW", (FARPROC)RegisterClassExW_Replacement, (FARPROC*)&RegisterClassExW_OLD);
    ReplaceImport("User32.dll", "UnregisterClassA", (FARPROC)UnregisterClassA_Replacement, (FARPROC*)&UnregisterClassA_OLD);
    ReplaceImport("User32.dll", "UnregisterClassW", (FARPROC)UnregisterClassW_Replacement, (FARPROC*)&UnregisterClassW_OLD);
    ReplaceImport("User32.dll", "GetClassLongA", (FARPROC)GetClassLongA_Replacement, (FARPROC*)&GetClassLongA_OLD);
    ReplaceImport("User32.dll", "GetClassLongW", (FARPROC)GetClassLongW_Replacement, (FARPROC*)&GetClassLongW_OLD);
    ReplaceImport("User32.dll", "SetClassLongA", (FARPROC)SetClassLongA_Replacement, (FARPROC*)&SetClassLongA_OLD);
    ReplaceImport("User32.dll", "SetClassLongW", (FARPROC)SetClassLongW_Replacement, (FARPROC*)&SetClassLongW_OLD);
    ReplaceImport("User32.dll", "GetClassLongPtrA", (FARPROC)GetClassLongPtrA_Replacement, (FARPROC*)&GetClassLongPtrA_OLD);
    ReplaceImport("User32.dll", "GetClassLongPtrW", (FARPROC)GetClassLongPtrW_Replacement, (FARPROC*)&GetClassLongPtrW_OLD);
    ReplaceImport("User32.dll", "SetClassLongPtrA", (FARPROC)SetClassLongPtrA_Replacement, (FARPROC*)&SetClassLongPtrA_OLD);
    ReplaceImport("User32.dll", "SetClassLongPtrW", (FARPROC)SetClassLongPtrW_Replacement, (FARPROC*)&SetClassLongPtrW_OLD);
#endif
    ReplaceImport("User32.dll", "GetWindowLongA", (FARPROC)GetWindowLongA_Replacement, (FARPROC*)&GetWindowLongA_OLD);
    ReplaceImport("User32.dll", "GetWindowLongW", (FARPROC)GetWindowLongW_Replacement, (FARPROC*)&GetWindowLongW_OLD);
    ReplaceImport("User32.dll", "GetWindowLongPtrA", (FARPROC)GetWindowLongPtrA_Replacement, (FARPROC*)&GetWindowLongPtrA_OLD);
    ReplaceImport("User32.dll", "GetWindowLongPtrW", (FARPROC)GetWindowLongPtrW_Replacement, (FARPROC*)&GetWindowLongPtrW_OLD);
    ReplaceImport("User32.dll", "SetWindowLongA", (FARPROC)SetWindowLongA_Replacement, (FARPROC*)&SetWindowLongA_OLD);
	ReplaceImport("User32.dll", "SetWindowLongW", (FARPROC)SetWindowLongW_Replacement, (FARPROC*)&SetWindowLongW_OLD);
	ReplaceImport("User32.dll", "SetWindowLongPtrA", (FARPROC)SetWindowLongPtrA_Replacement, (FARPROC*)&SetWindowLongPtrA_OLD);
	ReplaceImport("User32.dll", "SetWindowLongPtrW", (FARPROC)SetWindowLongPtrW_Replacement, (FARPROC*)&SetWindowLongPtrW_OLD);
	ReplaceImport("User32.dll", "SetWindowPlacement", (FARPROC)SetWindowPlacement_Replacement, (FARPROC*)&SetWindowPlacement_OLD);
	ReplaceImport("User32.dll", "SetWindowPos", (FARPROC)SetWindowPos_Replacement, (FARPROC*)&SetWindowPos_OLD);
    ReplaceImport("User32.dll", "GetCursorPos", (FARPROC)GetCursorPos_Replacement, (FARPROC*)&GetCursorPos_OLD);
    ReplaceImport("User32.dll", "SetCursorPos", (FARPROC)SetCursorPos_Replacement, (FARPROC*)&SetCursorPos_OLD);
    ReplaceImport("User32.dll", "BeginPaint", (FARPROC)BeginPaint_Replacement, (FARPROC*)&BeginPaint_OLD);
    ReplaceImport("User32.dll", "EndPaint", (FARPROC)EndPaint_Replacement, (FARPROC*)&EndPaint_OLD);
    ReplaceImport("User32.dll", "GetDC", (FARPROC)GetDC_Replacement, (FARPROC*)&GetDC_OLD);
    ReplaceImport("User32.dll", "InvalidateRect", (FARPROC)InvalidateRect_Replacement, (FARPROC*)&InvalidateRect_OLD);
    ReplaceImport("User32.dll", "ReleaseDC", (FARPROC)ReleaseDC_Replacement, (FARPROC*)&ReleaseDC_OLD);
    ReplaceImport("User32.dll", "ValidateRect", (FARPROC)ValidateRect_Replacement, (FARPROC*)&ValidateRect_OLD);
    ReplaceImport("User32.dll", "SetWindowsHookA", (FARPROC)SetWindowsHookA_Replacement, (FARPROC*)&SetWindowsHookA_OLD);
    ReplaceImport("User32.dll", "SetWindowsHookW", (FARPROC)SetWindowsHookW_Replacement, (FARPROC*)&SetWindowsHookW_OLD);
    ReplaceImport("User32.dll", "UnhookWindowsHook", (FARPROC)UnhookWindowsHook_Replacement, (FARPROC*)&UnhookWindowsHook_OLD);
    ReplaceImport("User32.dll", "SetWindowsHookExA", (FARPROC)SetWindowsHookExA_Replacement, (FARPROC*)&SetWindowsHookExA_OLD);
    ReplaceImport("User32.dll", "SetWindowsHookExW", (FARPROC)SetWindowsHookExW_Replacement, (FARPROC*)&SetWindowsHookExW_OLD);
    ReplaceImport("User32.dll", "UnhookWindowsHookEx", (FARPROC)UnhookWindowsHookEx_Replacement, (FARPROC*)&UnhookWindowsHookEx_OLD);
    ReplaceImport("User32.dll", "CallNextHookEx", (FARPROC)CallNextHookEx_Replacement, (FARPROC*)&CallNextHookEx_OLD);
    ReplaceImport("User32.dll", "SetWinEventHook", (FARPROC)SetWinEventHook_Replacement, (FARPROC*)&SetWinEventHook_OLD);
    ReplaceImport("User32.dll", "IsWinEventHookInstalled", (FARPROC)IsWinEventHookInstalled_Replacement, (FARPROC*)&IsWinEventHookInstalled_OLD);
    ReplaceImport("User32.dll", "UnhookWinEvent", (FARPROC)UnhookWinEvent_Replacement, (FARPROC*)&UnhookWinEvent_OLD);

    void ReplaceImports_AllGDI(ImportReplacer & replacer);
    ReplaceImports_AllGDI(replacer);

/*
    ReplaceImport("Gdi32.dll", "BitBlt", (FARPROC)BitBlt_Replacement, (FARPROC*)&BitBlt_OLD);
    ReplaceImport("Gdi32.dll", "StretchDIBits", (FARPROC)StretchDIBits_Replacement, (FARPROC*)&StretchDIBits_OLD);
    ReplaceImport("gdi32.dll", "SetDIBits", (FARPROC)SetDIBits_Replacement, (FARPROC*)&SetDIBits_OLD);
    ReplaceImport("gdi32.dll", "StretchBlt", (FARPROC)StretchBlt_Replacement, (FARPROC*)&StretchBlt_OLD);
    ReplaceImport("Gdi32.dll", "TextOutA", (FARPROC)TextOutA_Replacement, (FARPROC*)&TextOutA_OLD);
    ReplaceImport("Gdi32.dll", "TextOutW", (FARPROC)TextOutW_Replacement, (FARPROC*)&TextOutW_OLD);
    ReplaceImport("Gdi32.dll", "GetClipBox", (FARPROC)GetClipBox_Replacement, (FARPROC*)&GetClipBox_OLD);
    ReplaceImport("Gdi32.dll", "Rectangle", (FARPROC)Rectangle_Replacement, (FARPROC*)&Rectangle_OLD);
    ReplaceImport("Gdi32.dll", "RoundRect", (FARPROC)RoundRect_Replacement, (FARPROC*)&RoundRect_OLD);
*/
}

//Import Backups (Definitions)
ClientToScreen_FUNC ClientToScreen_OLD = NULL;
ScreenToClient_FUNC ScreenToClient_OLD = NULL;
MapWindowPoints_FUNC MapWindowPoints_OLD = NULL;
CreateWindowExA_FUNC CreateWindowExA_OLD = NULL;
CreateWindowExW_FUNC CreateWindowExW_OLD = NULL;
ShowWindow_FUNC ShowWindow_OLD = NULL;
GetClientRect_FUNC GetClientRect_OLD = NULL;
GetWindowPlacement_FUNC GetWindowPlacement_OLD = NULL;
GetWindowRect_FUNC GetWindowRect_OLD = NULL;
MoveWindow_FUNC MoveWindow_OLD = NULL;
RegisterClassA_FUNC RegisterClassA_OLD = NULL;
RegisterClassW_FUNC RegisterClassW_OLD = NULL;
RegisterClassExA_FUNC RegisterClassExA_OLD = NULL;
RegisterClassExW_FUNC RegisterClassExW_OLD = NULL;
UnregisterClassA_FUNC UnregisterClassA_OLD = NULL;
UnregisterClassW_FUNC UnregisterClassW_OLD = NULL;
GetClassLongA_FUNC GetClassLongA_OLD = NULL;
GetClassLongW_FUNC GetClassLongW_OLD = NULL;
SetClassLongA_FUNC SetClassLongA_OLD = NULL;
SetClassLongW_FUNC SetClassLongW_OLD = NULL;
GetClassLongPtrA_FUNC GetClassLongPtrA_OLD = NULL;
GetClassLongPtrW_FUNC GetClassLongPtrW_OLD = NULL;
SetClassLongPtrA_FUNC SetClassLongPtrA_OLD = NULL;
SetClassLongPtrW_FUNC SetClassLongPtrW_OLD = NULL;
GetWindowLongA_FUNC GetWindowLongA_OLD = NULL;
GetWindowLongW_FUNC GetWindowLongW_OLD = NULL;
GetWindowLongPtrA_FUNC GetWindowLongPtrA_OLD = NULL;
GetWindowLongPtrW_FUNC GetWindowLongPtrW_OLD = NULL;
SetWindowLongA_FUNC SetWindowLongA_OLD = NULL;
SetWindowLongW_FUNC SetWindowLongW_OLD = NULL;
SetWindowLongPtrA_FUNC SetWindowLongPtrA_OLD = NULL;
SetWindowLongPtrW_FUNC SetWindowLongPtrW_OLD = NULL;
SetWindowPlacement_FUNC SetWindowPlacement_OLD = NULL;
SetWindowPos_FUNC SetWindowPos_OLD = NULL;
GetCursorPos_FUNC GetCursorPos_OLD = NULL;
SetCursorPos_FUNC SetCursorPos_OLD = NULL;
BeginPaint_FUNC BeginPaint_OLD = NULL;
EndPaint_FUNC EndPaint_OLD = NULL;
GetDC_FUNC GetDC_OLD = NULL;
InvalidateRect_FUNC InvalidateRect_OLD = NULL;
ReleaseDC_FUNC ReleaseDC_OLD = NULL;
ValidateRect_FUNC ValidateRect_OLD = NULL;
SetWindowsHookA_FUNC SetWindowsHookA_OLD = NULL;
SetWindowsHookW_FUNC SetWindowsHookW_OLD = NULL;
UnhookWindowsHook_FUNC UnhookWindowsHook_OLD = NULL;
SetWindowsHookExA_FUNC SetWindowsHookExA_OLD = NULL;
SetWindowsHookExW_FUNC SetWindowsHookExW_OLD = NULL;
UnhookWindowsHookEx_FUNC UnhookWindowsHookEx_OLD = NULL;
CallNextHookEx_FUNC CallNextHookEx_OLD = NULL;
SetWinEventHook_FUNC SetWinEventHook_OLD = NULL;
IsWinEventHookInstalled_FUNC IsWinEventHookInstalled_OLD = NULL;
UnhookWinEvent_FUNC UnhookWinEvent_OLD = NULL;

/*
BitBlt_FUNC BitBlt_OLD = NULL;
StretchDIBits_FUNC StretchDIBits_OLD = NULL;
SetDIBits_FUNC SetDIBits_OLD = NULL;
StretchBlt_FUNC StretchBlt_OLD = NULL;
TextOutA_FUNC TextOutA_OLD = NULL;
TextOutW_FUNC TextOutW_OLD = NULL;
GetClipBox_FUNC GetClipBox_OLD = NULL;
Rectangle_FUNC Rectangle_OLD = NULL;
RoundRect_FUNC RoundRect_OLD = NULL;
*/

/*

void TryOverrideWindowClassA(LPCSTR windowClassName)
{
    bool alreadyOverridenA = overridenWindowClassesA.ContainsKey(windowClassName);
    if (alreadyOverridenA) return;
    bool windowClassIsAnsi;
    bool windowClassIsUnicode;
    if (atomToWindowClassNameA.ContainsValue(windowClassName))
    {
        BOOL okay;
        WNDCLASSEXA wndClass = {};
        wndClass.cbSize = sizeof(wndClass);
        okay = GetClassInfoExA(GetModuleHandleA(NULL), windowClassName, &wndClass);
        WNDPROC oldWndProc = wndClass.lpfnWndProc;
        overridenWindowClassesA.Add(windowClassName, oldWndProc);
        wndClass.style |= CS_OWNDC;
        wndClass.lpfnWndProc = ???;
    }
    else
    {
        wstring windowClassNameW = ToUnicode(windowClassName);
        if (atomToWindowClassNameW.ContainsValue(windowClassNameW))
        {
            if (overridenWindowClassesW.ContainsKey(windowClassNameW)) return;
            BOOL okay;
            WNDCLASSEXW wndClass = {};
            wndClass.cbSize = sizeof(wndClass);
            okay = GetClassInfoExW(GetModuleHandleA(NULL), windowClassNameW.c_str(), &wndClass);
            WNDPROC oldWndProc = wndClass.lpfnWndProc;
            overridenWindowClassesW.Add(windowClassNameW.c_str(), oldWndProc);
            wndClass.style |= CS_OWNDC;
            wndClass.lpfnWndProc = ? ? ? ;
        }
        else
        {
            return;
        }
    }
    if (windowClassIsAnsi)
    {
    }
    else if (windowClassIsUnicode)
    {
    }

}
*/




//Replacement Functions Code
BOOL WINAPI ClientToScreen_Replacement(HWND hWnd, LPPOINT lpPoint)
{
    return ClientToScreen_OLD(hWnd, lpPoint);
    //convert Virtual Client coordinates to Virtual Screen coordinates
    //if (lpPoint == NULL) return false;
    //WindowContext *windowContext = WindowContext::Get(hWnd);
    //if (windowContext == NULL) return ClientToScreen_OLD(hWnd, lpPoint);
    //windowContext->MouseVirtualToVirtualScreen(lpPoint);
    //return true;
}
BOOL WINAPI ScreenToClient_Replacement(HWND hWnd, LPPOINT lpPoint)
{
    return ScreenToClient_OLD(hWnd, lpPoint);
    //convert Virtual Screen coordinates to Virtual Client coordinates
    //if (lpPoint == NULL) return false;
    //WindowContext* windowContext = WindowContext::Get(hWnd);
    //if (windowContext == NULL) return ScreenToClient_OLD(hWnd, lpPoint);
    //windowContext->MouseVirtualScreenToVirtual(lpPoint);
    //return true;
}
int WINAPI MapWindowPoints_Replacement(HWND hWndFrom, HWND hWndTo, LPPOINT lpPoints, UINT cPoints)
{
    return MapWindowPoints_OLD(hWndFrom, hWndTo, lpPoints, cPoints);
    //if (lpPoints == NULL) return 0;
    //if (hWndFrom == NULL && hWndTo != NULL)
    //{
    //    for (UINT i = 0; i < cPoints; i++)
    //    {
    //        ScreenToClient_Replacement(
    //    }
    //}
}
BOOL WINAPI GetCursorPos_Replacement(LPPOINT lpPoint)
{
    if (lpPoint == NULL) return false;
    BOOL okay = GetCursorPos_OLD(lpPoint);
    WindowContext* windowContext = WindowContext::GetWindowContext();
    if (windowContext == NULL) return okay;
    windowContext->MouseScreenToVirtualScreen(lpPoint);
    return okay;
}
BOOL WINAPI SetCursorPos_Replacement(int x, int y)
{
    POINT point{ x,y };
    WindowContext* windowContext = WindowContext::GetWindowContext();
    if (windowContext == NULL) return SetCursorPos_OLD(x, y);
    windowContext->MouseVirtualScreenToScreen(&point);
    return SetCursorPos_OLD(point.x, point.y);
}

HWND WINAPI CreateWindowExA_Replacement(DWORD exStyle, LPCSTR lpClassName, LPCSTR windowName, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, HINSTANCE instance, LPVOID param)
{
#if USE_CREATE_HOOK
    //WindowContext* parentWindowContext = WindowContext::Get(parent);
    bool _windowBeingCreated = WindowContext::WindowBeingCreated;
    bool wantToHook = false;
    if (style & WS_CAPTION) wantToHook = true;
    //if (parentWindowContext != NULL) wantToHook = true;
    
    if (!wantToHook)
    {
        //windowClassSet.SetWindowCreated(lpClassName);
    }
    else
    {
        //windowClassSet.TryHookWindowClass(lpClassName);
        //windowClassSet.SetWindowCreated(lpClassName);
        //style |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;
        WindowContext::WindowBeingCreated = true;
    }
    HWND hwnd = CreateWindowExA_OLD(exStyle, lpClassName, windowName, style, x, y, width, height, parent, menu, instance, param);
    WindowContext::WindowBeingCreated = _windowBeingCreated;

    WindowContext::TryHookWindow(hwnd);
    return hwnd;
#else
    return CreateWindowExA_OLD(exStyle, lpClassName, windowName, style, x, y, width, height, parent, menu, instance, param);
#endif
}
HWND WINAPI CreateWindowExW_Replacement(DWORD exStyle, LPCWSTR lpClassName, LPCWSTR windowName, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, HINSTANCE instance, LPVOID param)
{
#if USE_CREATE_HOOK
    bool _windowBeingCreated = WindowContext::WindowBeingCreated;
    bool wantToHook = 0 != (style & WS_CAPTION);
    if (!wantToHook)
    {
        //windowClassSet.SetWindowCreated(lpClassName);
    }
    else
    {
        //windowClassSet.TryHookWindowClass(lpClassName);
        //windowClassSet.SetWindowCreated(lpClassName);
        style |= WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;
        WindowContext::WindowBeingCreated = true;
    }
    HWND hwnd = CreateWindowExW_OLD(exStyle, lpClassName, windowName, style, x, y, width, height, parent, menu, instance, param);
    WindowContext::WindowBeingCreated = _windowBeingCreated;

    WindowContext::TryHookWindow(hwnd);
    return hwnd;
#else
    return CreateWindowExW_OLD(exStyle, lpClassName, windowName, style, x, y, width, height, parent, menu, instance, param);
#endif
}
ATOM WINAPI RegisterClassA_Replacement(CONST WNDCLASSA* lpWndClass)
{
    //EnableVisualStyles();

#if USE_CLASS_HOOK
    WNDCLASSA wndClass = *lpWndClass;
    LPCSTR className = wndClass.lpszClassName;
    WNDPROC wndProc = wndClass.lpfnWndProc;
    wndClass.lpfnWndProc = WindowContext::DefaultWndProc;
    ATOM atom = RegisterClassA_OLD(&wndClass);
    windowClassSet.AddClass(className, atom, wndProc);
    return atom;
#else
    ATOM atom = RegisterClassA_OLD(lpWndClass);
    return atom;
#endif
}
ATOM WINAPI RegisterClassW_Replacement(CONST WNDCLASSW* lpWndClass)
{
#if USE_CLASS_HOOK
    WNDCLASSW wndClass = *lpWndClass;
    LPCWSTR className = wndClass.lpszClassName;
    WNDPROC wndProc = wndClass.lpfnWndProc;
    wndClass.lpfnWndProc = WindowContext::DefaultWndProc;
    ATOM atom = RegisterClassW_OLD(&wndClass);
    windowClassSet.AddClass(className, atom, wndProc);
    return atom;
#else
    ATOM atom = RegisterClassW_OLD(lpWndClass);
    return atom;
#endif
}
ATOM WINAPI RegisterClassExA_Replacement(CONST WNDCLASSEXA* lpWndClassEx)
{
#if USE_CLASS_HOOK
    WNDCLASSEXA wndClass = *lpWndClassEx;
    LPCSTR className = wndClass.lpszClassName;
    WNDPROC wndProc = wndClass.lpfnWndProc;
    wndClass.lpfnWndProc = WindowContext::DefaultWndProc;
    ATOM atom = RegisterClassExA_OLD(&wndClass);
    windowClassSet.AddClass(className, atom, wndProc);
    return atom;
#else
    ATOM atom = RegisterClassExA_OLD(lpWndClassEx);
    return atom;
#endif
}
ATOM WINAPI RegisterClassExW_Replacement(CONST WNDCLASSEXW* lpWndClassEx)
{
#if USE_CLASS_HOOK
    WNDCLASSEXW wndClass = *lpWndClassEx;
    LPCWSTR className = wndClass.lpszClassName;
    WNDPROC wndProc = wndClass.lpfnWndProc;
    wndClass.lpfnWndProc = WindowContext::DefaultWndProc;
    ATOM atom = RegisterClassExW_OLD(&wndClass);
    windowClassSet.AddClass(className, atom, wndProc);
    return atom;
#else
    ATOM atom = RegisterClassExW_OLD(lpWndClassEx);
    return atom;
#endif
}
BOOL WINAPI UnregisterClassA_Replacement(LPCSTR lpClassName, HINSTANCE hInstance)
{
    return UnregisterClassA_OLD(lpClassName, hInstance);
}
BOOL WINAPI UnregisterClassW_Replacement(LPCWSTR lpClassName, HINSTANCE hInstance)
{
    return UnregisterClassW_OLD(lpClassName, hInstance);
}
BOOL WINAPI ShowWindow_Replacement(HWND hWnd, int nCmdShow)
{
restart:
    WindowContext* windowContext = WindowContext::Get(hWnd);
    if (windowContext == NULL)
    {
#if USE_SHOW_HOOK
        if (WindowContext::TryHookWindow(hWnd))
        {
            goto restart;
        }
#endif
        return ShowWindow_OLD(hWnd, nCmdShow);
    }
    windowContext->WindowShown();
    return ShowWindow_OLD(hWnd, nCmdShow);
}

BOOL WINAPI GetClientRect_Replacement(HWND hwnd, LPRECT clientRect)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return GetClientRect_OLD(hwnd, clientRect);
    return windowContext->GetClientRect_(clientRect);
}
BOOL WINAPI GetWindowPlacement_Replacement(HWND hwnd, WINDOWPLACEMENT *windowPlacement)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return GetWindowPlacement_OLD(hwnd, windowPlacement);
    return windowContext->GetWindowPlacement_(windowPlacement);
}
BOOL WINAPI GetWindowRect_Replacement(HWND hwnd, LPRECT windowRect)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return GetWindowRect_OLD(hwnd, windowRect);
    return windowContext->GetWindowRect_(windowRect);
}
BOOL WINAPI MoveWindow_Replacement(HWND hwnd, int x, int y, int width, int height, BOOL repaint)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return MoveWindow_OLD(hwnd, x, y, width, height, repaint);
    return windowContext->MoveWindow_(x, y, width, height, repaint);
}
DWORD WINAPI GetClassLongA_Replacement(HWND hWnd, int nIndex)
{
    return GetClassLongA_OLD(hWnd, nIndex);
}
DWORD WINAPI GetClassLongW_Replacement(HWND hWnd, int nIndex)
{
    return GetClassLongW_OLD(hWnd, nIndex);
}
DWORD WINAPI SetClassLongA_Replacement(HWND hWnd, int nIndex, LONG dwNewLong)
{
    return SetClassLongA_OLD(hWnd, nIndex, dwNewLong);
}
DWORD WINAPI SetClassLongW_Replacement(HWND hWnd, int nIndex, LONG dwNewLong)
{
    return SetClassLongW_OLD(hWnd, nIndex, dwNewLong);
}
ULONG_PTR WINAPI GetClassLongPtrA_Replacement(HWND hWnd, int nIndex)
{
    return GetClassLongPtrA_OLD(hWnd, nIndex);
}
ULONG_PTR WINAPI GetClassLongPtrW_Replacement(HWND hWnd, int nIndex)
{
    return GetClassLongPtrW_OLD(hWnd, nIndex);
}
ULONG_PTR WINAPI SetClassLongPtrA_Replacement(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
    return SetClassLongPtrA_OLD(hWnd, nIndex, dwNewLong);
}
ULONG_PTR WINAPI SetClassLongPtrW_Replacement(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
    return SetClassLongPtrW_OLD(hWnd, nIndex, dwNewLong);
}
LONG WINAPI GetWindowLongA_Replacement(HWND hWnd, int nIndex)
{
    WindowContext* windowContext = WindowContext::Get(hWnd);
    if (windowContext == NULL) return GetWindowLongA_OLD(hWnd, nIndex);
    return windowContext->GetWindowLong_(nIndex);
}
LONG WINAPI GetWindowLongW_Replacement(HWND hWnd, int nIndex)
{
    WindowContext* windowContext = WindowContext::Get(hWnd);
    if (windowContext == NULL) return GetWindowLongW_OLD(hWnd, nIndex);
    return windowContext->GetWindowLong_(nIndex);
}
LONG_PTR WINAPI GetWindowLongPtrA_Replacement(HWND hWnd, int nIndex)
{
    WindowContext* windowContext = WindowContext::Get(hWnd);
    if (windowContext == NULL) return GetWindowLongPtrA_OLD(hWnd, nIndex);
    return windowContext->GetWindowLong_(nIndex);
}
LONG_PTR WINAPI GetWindowLongPtrW_Replacement(HWND hWnd, int nIndex)
{
    WindowContext* windowContext = WindowContext::Get(hWnd);
    if (windowContext == NULL) return GetWindowLongW_OLD(hWnd, nIndex);
    return windowContext->GetWindowLong_(nIndex);
}
LONG_PTR SetWindowLong_(HWND hwnd, int index, LONG_PTR value)
{
    bool isWindowUnicode = IsWindowUnicode(hwnd);
tryAgain:
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL)
    {
        if (index == GWL_WNDPROC && WindowContext::WindowBeingCreated)
        {
            WNDPROC oldWndProc = (WNDPROC)(isWindowUnicode ? 
                GetWindowLongPtrW(hwnd, index) :
                GetWindowLongPtrA(hwnd, index));
            if (oldWndProc == WindowContext::DefaultWndProc)
            {
                if (WindowContext::TryHookWindow(hwnd))
                {
                    WindowContext::WindowBeingCreated = false;
                    goto tryAgain;
                }
            }
        }
        return isWindowUnicode ?
            SetWindowLongPtrW(hwnd, index, value) :
            SetWindowLongPtrA(hwnd, index, value);
    }
    return windowContext->SetWindowLong_(index, value);
}

LONG WINAPI SetWindowLongA_Replacement(HWND hwnd, int index, LONG value)
{
    return SetWindowLong_(hwnd, index, value);
}
LONG WINAPI SetWindowLongW_Replacement(HWND hwnd, int index, LONG value)
{
    return SetWindowLong_(hwnd, index, value);
}
LONG_PTR WINAPI SetWindowLongPtrA_Replacement(HWND hwnd, int index, LONG_PTR value)
{
    return SetWindowLong_(hwnd, index, value);
}
LONG_PTR WINAPI SetWindowLongPtrW_Replacement(HWND hwnd, int index, LONG_PTR value)
{
    return SetWindowLong_(hwnd, index, value);
}
BOOL WINAPI SetWindowPlacement_Replacement(HWND hwnd, const WINDOWPLACEMENT* windowPlacement)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return SetWindowPlacement_OLD(hwnd, windowPlacement);
    return windowContext->SetWindowPlacement_(windowPlacement);
}
BOOL WINAPI SetWindowPos_Replacement(HWND hwnd, HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return SetWindowPos_OLD(hwnd, hwndInsertAfter, x, y, cx, cy, flags);
    return windowContext->SetWindowPos_(hwndInsertAfter, x, y, cx, cy, flags);
}
HDC WINAPI BeginPaint_Replacement(HWND hwnd, LPPAINTSTRUCT lpPaintStruct)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return BeginPaint_OLD(hwnd, lpPaintStruct);
    return windowContext->BeginPaint_(lpPaintStruct);
}
BOOL WINAPI EndPaint_Replacement(HWND hwnd, const PAINTSTRUCT* paintStruct)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return EndPaint_OLD(hwnd, paintStruct);
    return windowContext->EndPaint_(paintStruct);
}
HDC WINAPI GetDC_Replacement(HWND hwnd)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return GetDC_OLD(hwnd);
    return windowContext->GetDC_();
}
BOOL WINAPI InvalidateRect_Replacement(HWND hwnd, LPCRECT lpRect, BOOL bErase)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return InvalidateRect_OLD(hwnd, lpRect, bErase);
    return windowContext->InvalidateRect_(lpRect, bErase);
}
int WINAPI ReleaseDC_Replacement(HWND hwnd, HDC hdc)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return ReleaseDC_OLD(hwnd, hdc);
    return windowContext->ReleaseDC_(hdc);
}
BOOL WINAPI ValidateRect_Replacement(HWND hwnd, LPCRECT rect)
{
    WindowContext* windowContext = WindowContext::Get(hwnd);
    if (windowContext == NULL) return ValidateRect(hwnd, rect);
    return windowContext->ValidateRect_(rect);
    //if (rect != NULL)
    //{
    //    RECT newRect = *rect;
    //    RECT clientRect;
    //    windowContext->GetClientRect_(&clientRect);
    //    if (newRect == clientRect)
    //    {
    //        return ValidateRect(hwnd, NULL);
    //    }
    //    windowContext->RectVirtualToClient(&newRect);
    //    return ValidateRect(hwnd, &newRect);
    //}
    //return ValidateRect(hwnd, rect);
}
HHOOK WINAPI SetWindowsHookA_Replacement(int nFilterType, HOOKPROC pfnFilterProc)
{
    return SetWindowsHookA_OLD(nFilterType, pfnFilterProc);
}
HHOOK WINAPI SetWindowsHookW_Replacement(int nFilterType, HOOKPROC pfnFilterProc)
{
    return SetWindowsHookW_OLD(nFilterType, pfnFilterProc);
}
BOOL WINAPI UnhookWindowsHook_Replacement(int nCode, HOOKPROC pfnFilterProc)
{
    return UnhookWindowsHook_OLD(nCode, pfnFilterProc);
}
HHOOK WINAPI SetWindowsHookExA_Replacement(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
{
    return SetWindowsHookExA_OLD(idHook, lpfn, hmod, dwThreadId);
}
HHOOK WINAPI SetWindowsHookExW_Replacement(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
{
    return SetWindowsHookExW_OLD(idHook, lpfn, hmod, dwThreadId);
}
BOOL WINAPI UnhookWindowsHookEx_Replacement(HHOOK hhk)
{
    return UnhookWindowsHookEx_OLD(hhk);
}
LRESULT WINAPI CallNextHookEx_Replacement(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx_OLD(hhk, nCode, wParam, lParam);
}
HWINEVENTHOOK WINAPI SetWinEventHook_Replacement(DWORD eventMin, DWORD eventMax, HMODULE hmodWinEventProc, WINEVENTPROC pfnWinEventProc, DWORD idProcess, DWORD idThread, DWORD dwFlags)
{
    return SetWinEventHook_OLD(eventMin, eventMax, hmodWinEventProc, pfnWinEventProc, idProcess, idThread, dwFlags);
}
BOOL WINAPI IsWinEventHookInstalled_Replacement(DWORD event)
{
    return IsWinEventHookInstalled_OLD(event);
}
BOOL WINAPI UnhookWinEvent_Replacement(HWINEVENTHOOK hWinEventHook)
{
    return UnhookWinEvent_OLD(hWinEventHook);
}



/*
BOOL WINAPI BitBlt_Replacement(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return BitBlt_OLD(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
    hdc = windowContext->GetCurrentDC(hdc);
    windowContext->AddDirtyRect(x, y, cx, cy);
    return BitBlt_OLD(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}
int WINAPI StretchDIBits_Replacement(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return StretchDIBits_OLD(hdc, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop);
    hdc = windowContext->GetCurrentDC(hdc);
    windowContext->AddDirtyRect(xDest, yDest, DestWidth, DestHeight);
    return StretchDIBits_OLD(hdc, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop);
}
int WINAPI SetDIBits_Replacement(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, CONST VOID* lpBits, CONST BITMAPINFO *bitmapInfo, UINT ColorUse)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return SetDIBits_OLD(hdc, hbm, start, cLines, lpBits, bitmapInfo, ColorUse);
    hdc = windowContext->GetCurrentDC(hdc);
    return SetDIBits_OLD(hdc, hbm, start, cLines, lpBits, bitmapInfo, ColorUse);
}
BOOL WINAPI StretchBlt_Replacement(HDC hdcDest, int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdcDest);
    if (windowContext == NULL) return StretchBlt_OLD(hdcDest, xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop);
    hdcDest = windowContext->GetCurrentDC(hdcDest);
    windowContext->AddDirtyRect(xDest, yDest, wDest, hDest);
    return StretchBlt_OLD(hdcDest, xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop);
}
BOOL WINAPI TextOutA_Replacement(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return TextOutA_OLD(hdc, x, y, lpString, c);
    hdc = windowContext->GetCurrentDC(hdc);
    SIZE size;
    BOOL okay;
    okay = GetTextExtentPoint32A(hdc, lpString, c, &size);
    if (okay)
    {
        windowContext->AddDirtyRect(x, y, size.cx, size.cy);
    }
    return TextOutA_OLD(hdc, x, y, lpString, c);
}
BOOL WINAPI TextOutW_Replacement(HDC hdc, int x, int y, LPCWSTR lpString, int c)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return TextOutW_OLD(hdc, x, y, lpString, c);
    hdc = windowContext->GetCurrentDC(hdc);
    SIZE size;
    BOOL okay;
    okay = GetTextExtentPoint32W(hdc, lpString, c, &size);
    if (okay)
    {
        windowContext->AddDirtyRect(x, y, size.cx, size.cy);
    }
    return TextOutW_OLD(hdc, x, y, lpString, c);
}
int WINAPI GetClipBox_Replacement(HDC hdc, LPRECT rect)
{
    return GetClipBox_OLD(hdc, rect);
}
BOOL WINAPI Rectangle_Replacement(HDC hdc, int left, int top, int right, int bottom)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return Rectangle_OLD(hdc, left, top, right, bottom);
    hdc = windowContext->GetCurrentDC(hdc);
    windowContext->AddDirtyRectWithPen(left, top, right - left, bottom - top);
    return Rectangle_OLD(hdc, left, top, right, bottom);
}
BOOL WINAPI RoundRect_Replacement(HDC hdc, int left, int top, int right, int bottom, int width, int height)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext == NULL) return RoundRect_OLD(hdc, left, top, right, bottom, width, height);
    hdc = windowContext->GetCurrentDC(hdc);
    windowContext->AddDirtyRectWithPen(left, top, right - left, bottom - top);
    return RoundRect_OLD(hdc, left, top, right, bottom, width, height);
}
*/

void SubstituteDC(HDC& hdc)
{
    WindowContext* windowContext = WindowContext::GetByHdc(hdc);
    if (windowContext != NULL)
    {
        hdc = windowContext->GetCurrentDC(hdc);
    }
}
