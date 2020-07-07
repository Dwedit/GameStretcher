#pragma once
struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <ddraw.h>
#include "Win32Ex.h"

struct CDirectDrawSurface7Wrapper;

struct CDirectDraw7Wrapper : public IDirectDraw7
{
protected:
    int refCount = 1;
    int internalRefCount = 0;
    IDirectDraw7* realObject = NULL;
public:
    CDirectDraw7Wrapper(IDirectDraw7* obj);
    virtual ~CDirectDraw7Wrapper();

    virtual  HRESULT __stdcall QueryInterface(const IID& riid, LPVOID* ppvObj);
    virtual  ULONG __stdcall AddRef(void);
    virtual  ULONG __stdcall Release(void);

    virtual  HRESULT __stdcall Compact(void);
    virtual  HRESULT __stdcall CreateClipper(DWORD, LPDIRECTDRAWCLIPPER*, IUnknown*);
    virtual  HRESULT __stdcall CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE*, IUnknown*);
    virtual  HRESULT __stdcall CreateSurface(LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7*, IUnknown*);
    virtual  HRESULT __stdcall DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7*);
    virtual  HRESULT __stdcall EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2);
    virtual  HRESULT __stdcall EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7);
    virtual  HRESULT __stdcall FlipToGDISurface(void);
    virtual  HRESULT __stdcall GetCaps(LPDDCAPS, LPDDCAPS);
    virtual  HRESULT __stdcall GetDisplayMode(LPDDSURFACEDESC2);
    virtual  HRESULT __stdcall GetFourCCCodes(LPDWORD, LPDWORD);
    virtual  HRESULT __stdcall GetGDISurface(LPDIRECTDRAWSURFACE7*);
    virtual  HRESULT __stdcall GetMonitorFrequency(LPDWORD);
    virtual  HRESULT __stdcall GetScanLine(LPDWORD);
    virtual  HRESULT __stdcall GetVerticalBlankStatus(LPBOOL);
    virtual  HRESULT __stdcall Initialize(GUID*);
    virtual  HRESULT __stdcall RestoreDisplayMode(void);
    virtual  HRESULT __stdcall SetCooperativeLevel(HWND, DWORD);
    virtual  HRESULT __stdcall SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD);
    virtual  HRESULT __stdcall WaitForVerticalBlank(DWORD, HANDLE);

    virtual  HRESULT __stdcall GetAvailableVidMem(LPDDSCAPS2, LPDWORD, LPDWORD);

    virtual  HRESULT __stdcall GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7*);
    virtual  HRESULT __stdcall RestoreAllSurfaces(void);
    virtual  HRESULT __stdcall TestCooperativeLevel(void);
    virtual  HRESULT __stdcall GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD);
    virtual  HRESULT __stdcall StartModeTest(LPSIZE, DWORD, DWORD);
    virtual  HRESULT __stdcall EvaluateMode(DWORD, DWORD*);

    virtual CDirectDrawSurface7Wrapper* CreateSurfaceWrapper(IDirectDrawSurface7* obj);
};
