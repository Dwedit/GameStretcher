#include "DDrawOverride.h"
#include "WindowContext.h"

DDrawOverride* DDrawOverride::CreateWrapper(IDirectDraw7* obj) //static
{
    return new DDrawOverride(obj);
}

CDirectDrawSurface7Wrapper* DDrawOverride::CreateSurfaceWrapper(IDirectDrawSurface7* obj)
{
    DDrawSurfaceOverride*wrapper = new DDrawSurfaceOverride(obj);
    wrapper->hwnd = hwnd;
    return wrapper;
}

HRESULT __stdcall DDrawOverride::SetCooperativeLevel(HWND hwnd, DWORD flags)
{
    this->hwnd = hwnd;
    return CDirectDraw7Wrapper::SetCooperativeLevel(hwnd, flags);
}

HRESULT DDrawOverride::CreateSurface(LPDDSURFACEDESC2 desc, LPDIRECTDRAWSURFACE7 FAR* pSurface, IUnknown FAR* unused)
{
    if (desc == NULL) return E_POINTER;
    if (pSurface == NULL) return E_POINTER;
    HRESULT hr;
    IDirectDrawSurface7*& surface = *pSurface;
    if ((desc->dwFlags & DDSD_CAPS) && (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE))
    {
        desc->ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
        desc->dwFlags |= DDSD_WIDTH | DDSD_HEIGHT;
        if (this->hwnd == NULL) this->hwnd = FindMainWindow();
        RECT monitorRect = GetMonitorRect(this->hwnd);
        desc->dwWidth = monitorRect.right - monitorRect.left;
        desc->dwHeight = monitorRect.bottom - monitorRect.top;
        HRESULT hr = realObject->CreateSurface(desc, pSurface, unused);
        if (SUCCEEDED(hr) && surface != NULL)
        {
            CDirectDrawSurface7Wrapper* newSurface = CreateSurfaceWrapper(surface);
            surface = newSurface;
            return hr;
        }
    }

    hr = realObject->CreateSurface(desc, pSurface, unused);
    return hr;
    //if (SUCCEEDED(hr) && surface != NULL)
    //{
    //    CDirectDrawSurface7Wrapper* newSurface = CreateSurfaceWrapper(surface);
    //    surface = newSurface;
    //}
    //return hr;
}

CDirectDrawSurface7Wrapper* DDrawSurfaceOverride::CreateSurfaceWrapper(IDirectDrawSurface7* obj)
{
    DDrawSurfaceOverride* wrapper = new DDrawSurfaceOverride(obj);
    wrapper->hwnd = hwnd;
    return wrapper;
}

HRESULT __stdcall DDrawSurfaceOverride::Blt(LPRECT destRect, LPDIRECTDRAWSURFACE7 srcSurface, LPRECT srcRect, DWORD flags, LPDDBLTFX fx)
{
    //move destRect to 0, 0
    RECT destRect2 = *destRect;
    ScreenToClient(hwnd, (LPPOINT)&destRect2);
    ScreenToClient(hwnd, ((LPPOINT)&destRect2) + 1);
    
    HRESULT hr = CDirectDrawSurface7Wrapper::Blt(&destRect2, srcSurface, srcRect, flags, fx);
    BOOL okay;
    
    WindowContext* windowContext = WindowContext::GetWindowContext(this->hwnd);
    if (windowContext != NULL)
    {
        HDC realDC = windowContext->GetDC_();
        {
            auto lock = windowContext->CreateLock();
            HDC d3dDC = windowContext->GetCurrentDC(realDC);
            HDC surfaceDC = NULL;
            HRESULT hr2 = this->GetDC(&surfaceDC);
            if (SUCCEEDED(hr2) && surfaceDC != NULL)
            {
                RECT virtualWindowRect = windowContext->GetClientRect_();
                okay = BitBlt(d3dDC, virtualWindowRect.left + destRect2.left, 
                    virtualWindowRect.top + destRect2.top,
                    destRect2.right - destRect2.left,
                    destRect2.bottom - destRect2.top,
                    surfaceDC, 
                    virtualWindowRect.left + destRect2.left, 
                    virtualWindowRect.top + destRect2.top, 
                    SRCCOPY);

                windowContext->AddDirtyRect();

                hr2 = this->ReleaseDC(surfaceDC);
                surfaceDC = NULL;
            }
            d3dDC = NULL;
        }
        windowContext->ReleaseDC_(realDC);
    }
    return hr;
}

HRESULT __stdcall DDrawSurfaceOverride::BltBatch(LPDDBLTBATCH a, DWORD b, DWORD c)
{
    return CDirectDrawSurface7Wrapper::BltBatch(a, b, c);
}

HRESULT __stdcall DDrawSurfaceOverride::BltFast(DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 srcSurface, LPRECT srcRect, DWORD flags)
{
    return CDirectDrawSurface7Wrapper::BltFast(x, y, srcSurface, srcRect, flags);
}

HRESULT __stdcall DDrawSurfaceOverride::Lock(LPRECT rect, LPDDSURFACEDESC2 desc, DWORD flags, HANDLE handle)
{
    return CDirectDrawSurface7Wrapper::Lock(rect, desc, flags, handle);
}

HRESULT __stdcall DDrawSurfaceOverride::Unlock(LPRECT rect)
{
    return CDirectDrawSurface7Wrapper::Unlock(rect);
}

HRESULT __stdcall DDrawSurfaceOverride::GetDC(HDC* pHdc)
{
    return CDirectDrawSurface7Wrapper::GetDC(pHdc);
}

HRESULT __stdcall DDrawSurfaceOverride::ReleaseDC(HDC hdc)
{
    return CDirectDrawSurface7Wrapper::ReleaseDC(hdc);
}

HRESULT __stdcall DDrawSurfaceOverride::Flip(LPDIRECTDRAWSURFACE7 surface, DWORD flags)
{
    return CDirectDrawSurface7Wrapper::Flip(surface, flags);
}
