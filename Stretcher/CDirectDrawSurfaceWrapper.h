#pragma once
struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <ddraw.h>

// {0438FBD8-FDD5-4d64-B8D4-79ED51C6D104}
static const GUID CLSID_DirectDrawSurface7Wrapper =
{ 0x438fbd8, 0xfdd5, 0x4d64, { 0xb8, 0xd4, 0x79, 0xed, 0x51, 0xc6, 0xd1, 0x4 } };


struct CDirectDrawSurface7Wrapper : public IDirectDrawSurface7
{
protected:
    int refCount = 1;
    int internalRefCount = 0;
    IDirectDrawSurface7* realObject = NULL;
public:
    CDirectDrawSurface7Wrapper(IDirectDrawSurface7* obj);
    virtual ~CDirectDrawSurface7Wrapper();
    static IDirectDrawSurface7* GetRealObject(IDirectDrawSurface7* obj);

    virtual  HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppvObj);
    virtual  ULONG __stdcall AddRef(void);
    virtual  ULONG __stdcall Release(void);

    virtual  HRESULT __stdcall AddAttachedSurface(LPDIRECTDRAWSURFACE7);
    virtual  HRESULT __stdcall AddOverlayDirtyRect(LPRECT);
    virtual  HRESULT __stdcall Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX);
    virtual  HRESULT __stdcall BltBatch(LPDDBLTBATCH, DWORD, DWORD);
    virtual  HRESULT __stdcall BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD);
    virtual  HRESULT __stdcall DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE7);
    virtual  HRESULT __stdcall EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK7);
    virtual  HRESULT __stdcall EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7);
    virtual  HRESULT __stdcall Flip(LPDIRECTDRAWSURFACE7, DWORD);
    virtual  HRESULT __stdcall GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE7*);
    virtual  HRESULT __stdcall GetBltStatus(DWORD);
    virtual  HRESULT __stdcall GetCaps(LPDDSCAPS2);
    virtual  HRESULT __stdcall GetClipper(LPDIRECTDRAWCLIPPER*);
    virtual  HRESULT __stdcall GetColorKey(DWORD, LPDDCOLORKEY);
    virtual  HRESULT __stdcall GetDC(HDC*);
    virtual  HRESULT __stdcall GetFlipStatus(DWORD);
    virtual  HRESULT __stdcall GetOverlayPosition(LPLONG, LPLONG);
    virtual  HRESULT __stdcall GetPalette(LPDIRECTDRAWPALETTE*);
    virtual  HRESULT __stdcall GetPixelFormat(LPDDPIXELFORMAT);
    virtual  HRESULT __stdcall GetSurfaceDesc(LPDDSURFACEDESC2);
    virtual  HRESULT __stdcall Initialize(LPDIRECTDRAW, LPDDSURFACEDESC2);
    virtual  HRESULT __stdcall IsLost(void);
    virtual  HRESULT __stdcall Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE);
    virtual  HRESULT __stdcall ReleaseDC(HDC);
    virtual  HRESULT __stdcall Restore(void);
    virtual  HRESULT __stdcall SetClipper(LPDIRECTDRAWCLIPPER);
    virtual  HRESULT __stdcall SetColorKey(DWORD, LPDDCOLORKEY);
    virtual  HRESULT __stdcall SetOverlayPosition(LONG, LONG);
    virtual  HRESULT __stdcall SetPalette(LPDIRECTDRAWPALETTE);
    virtual  HRESULT __stdcall Unlock(LPRECT);
    virtual  HRESULT __stdcall UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX);
    virtual  HRESULT __stdcall UpdateOverlayDisplay(DWORD);
    virtual  HRESULT __stdcall UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE7);

    virtual  HRESULT __stdcall GetDDInterface(LPVOID*);
    virtual  HRESULT __stdcall PageLock(DWORD);
    virtual  HRESULT __stdcall PageUnlock(DWORD);

    virtual  HRESULT __stdcall SetSurfaceDesc(LPDDSURFACEDESC2, DWORD);

    virtual  HRESULT __stdcall SetPrivateData(REFGUID, LPVOID, DWORD, DWORD);
    virtual  HRESULT __stdcall GetPrivateData(REFGUID, LPVOID, LPDWORD);
    virtual  HRESULT __stdcall FreePrivateData(REFGUID);
    virtual  HRESULT __stdcall GetUniquenessValue(LPDWORD);
    virtual  HRESULT __stdcall ChangeUniquenessValue(void);

    virtual  HRESULT __stdcall SetPriority(DWORD);
    virtual  HRESULT __stdcall GetPriority(LPDWORD);
    virtual  HRESULT __stdcall SetLOD(DWORD);
    virtual  HRESULT __stdcall GetLOD(LPDWORD);

    virtual CDirectDrawSurface7Wrapper *CreateSurfaceWrapper(IDirectDrawSurface7* obj);
};
