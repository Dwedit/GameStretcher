#pragma once

#include "CDirectDrawWrapper.h"
#include "CDirectDrawSurfaceWrapper.h"

struct DDrawOverride : public CDirectDraw7Wrapper
{
    HWND hwnd = NULL;

    static DDrawOverride* CreateWrapper(IDirectDraw7* obj);
    DDrawOverride(IDirectDraw7* obj) : CDirectDraw7Wrapper(obj) {}
    virtual CDirectDrawSurface7Wrapper* CreateSurfaceWrapper(IDirectDrawSurface7* obj);
    virtual HRESULT __stdcall SetCooperativeLevel(HWND, DWORD);
    virtual HRESULT __stdcall CreateSurface(LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7*, IUnknown*);

};

struct DDrawSurfaceOverride : public CDirectDrawSurface7Wrapper
{
    HWND hwnd = NULL;

    DDrawSurfaceOverride(IDirectDrawSurface7* obj) : CDirectDrawSurface7Wrapper(obj) {}
    virtual CDirectDrawSurface7Wrapper* CreateSurfaceWrapper(IDirectDrawSurface7* obj);

    virtual  HRESULT __stdcall Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX);
    virtual  HRESULT __stdcall BltBatch(LPDDBLTBATCH, DWORD, DWORD);
    virtual  HRESULT __stdcall BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD);
    virtual  HRESULT __stdcall Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE);
    virtual  HRESULT __stdcall Unlock(LPRECT);
    virtual  HRESULT __stdcall GetDC(HDC*);
    virtual  HRESULT __stdcall ReleaseDC(HDC);
    virtual  HRESULT __stdcall Flip(LPDIRECTDRAWSURFACE7, DWORD);
};