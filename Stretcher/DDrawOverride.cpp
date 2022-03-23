#include "DDrawOverride.h"
#include "WindowContext.h"

//Make DirectDraw use system memory instead of video memory  (The performance impact of picking System vs Video memory is currently unknown)
#define DDRAW_SURFACE_USE_SYSTEM_MEMORY 1
//When blitting to the primary surface, if blit is not scaled, skip the intermediate DirectDraw Surface, and instead blit to the Direct3D backbuffer
#define DDRAW_BLIT_ALLOW_SKIP_PRIMARY_SURFACE 1
//When blitting to the primary surface, force it to be considered a full screen blit (breaks partial updates, but fixes bad window coordinates)
#define DDRAW_BLIT_FULL_WINDOW 1
//When blitting to the primary surface, assume there is no scaling
#define DDRAW_BLIT_ASSUME_UNSCALED 1

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

    DDSURFACEDESC2 desc2 = *desc;
    bool wantPrimarySurface = false;
    if (desc2.dwFlags & DDSD_CAPS)
    {
    	//creating a primary surface
        if (desc2.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE)
        {
            wantPrimarySurface = true;
            //primary surfaces do not provide a width and height, so use the screen size here
            desc2.dwFlags |= DDSD_WIDTH | DDSD_HEIGHT;
            if (this->hwnd == NULL) this->hwnd = FindMainWindow();
            RECT monitorRect = GetMonitorRect(this->hwnd);
            desc2.dwWidth = monitorRect.right - monitorRect.left;
            desc2.dwHeight = monitorRect.bottom - monitorRect.top;
            //don't actually create a real primary surface
            #if DDRAW_SURFACE_USE_SYSTEM_MEMORY
            desc2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
            #else
            desc2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
            #endif
        }
        else
        {
        	//creating a plain offscreen surface
        	
        	//if surface would be created as a video memory surface
            if (0 == (desc2.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY))
            {
                #if DDRAW_SURFACE_USE_SYSTEM_MEMORY
                desc2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
                #else
                desc2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
                #endif
            }
        }
    }
    //Create the surface, create the wrapper, return the wrapper instead of the surface
    IDirectDrawSurface7*& surface = *pSurface;
    hr = realObject->CreateSurface(&desc2, &surface, unused);
    if (SUCCEEDED(hr) && surface != NULL)
    {
        CDirectDrawSurface7Wrapper* newSurface = CreateSurfaceWrapper(surface);
        newSurface->isPrimarySurface = wantPrimarySurface;
        SafeRelease(surface);
        surface = newSurface;
    }
    return hr;
}

CDirectDrawSurface7Wrapper* DDrawSurfaceOverride::CreateSurfaceWrapper(IDirectDrawSurface7* obj)
{
    DDrawSurfaceOverride* wrapper = new DDrawSurfaceOverride(obj);
    wrapper->hwnd = hwnd;
    return wrapper;
}

HRESULT __stdcall DDrawSurfaceOverride::Blt(LPRECT pDestRect, LPDIRECTDRAWSURFACE7 srcSurface, LPRECT pSrcRect, DWORD flags, LPDDBLTFX fx)
{
    HRESULT hr = S_OK;
    BOOL okay = true;
    //not blitting to primary surface?  Just blit it.
    if (!this->isPrimarySurface)
    {
        hr = CDirectDrawSurface7Wrapper::Blt(pDestRect, srcSurface, pSrcRect, flags, fx);
        return hr;
    }
    //check for invalid arguments
    if (srcSurface == NULL)
    {
        return E_POINTER;
    }
    //TODO: possibly consider FX, but I don't think they're ever used for blits to the primary buffer

    //ensure we have a window handle (FindMainWindow won't get called here normally)
    if (this->hwnd == NULL) this->hwnd = FindMainWindow();
    //ensure we have a window context
    WindowContext* windowContext = WindowContext::GetWindowContext(this->hwnd);
    if (!windowContext)
    {
        return E_POINTER;
    }
    //Client Rect is virtualized, has unscaled width and height
    RECT clientRect;
    windowContext->GetClientRect_(&clientRect);
    //Window Rect is virtualized, has unscaled width and height, has real origin
    RECT windowRect;
    windowContext->GetWindowRect_(&windowRect);

    //Source Rect
    RECT srcSurfaceRect;
    if (pSrcRect == NULL)
    {
        //Get from description if src rect was not provided
        DDSURFACEDESC2 desc = {};
        desc.dwSize = sizeof(desc);
        srcSurface->GetSurfaceDesc(&desc);
        srcSurfaceRect = {};
        srcSurfaceRect.right = desc.dwWidth;
        srcSurfaceRect.bottom = desc.dwHeight;
    }
    else
    {
        srcSurfaceRect = *pSrcRect;
    }

    //Destination Rect
    RECT destSurfaceRect;
    if (pDestRect == NULL)
    {
        //Get from client rect if dest rect was not provided
        destSurfaceRect = clientRect;
        destSurfaceRect.left = 0;
        destSurfaceRect.top = 0;
    }
    else
    {
        destSurfaceRect = *pDestRect;
#if DDRAW_BLIT_FULL_WINDOW
        destSurfaceRect.right -= destSurfaceRect.left;
        destSurfaceRect.bottom -= destSurfaceRect.top;
        destSurfaceRect.left = 0;
        destSurfaceRect.top = 0;
#else
        ScreenToClient(hwnd, (LPPOINT)&destSurfaceRect);
        ScreenToClient(hwnd, ((LPPOINT)&destSurfaceRect) + 1);
#endif
    }

    int destWidth = destSurfaceRect.right - destSurfaceRect.left;
    int destHeight = destSurfaceRect.bottom - destSurfaceRect.top;
    int srcWidth = srcSurfaceRect.right - srcSurfaceRect.left;
    int srcHeight = srcSurfaceRect.bottom - srcSurfaceRect.top;

    bool blitIsUnscaled = false;
    if (destWidth == srcWidth && destHeight == srcHeight)
    {
        blitIsUnscaled = true;
    }
#if DDRAW_BLIT_ASSUME_UNSCALED
    blitIsUnscaled = true;
    destWidth = srcWidth;
    destHeight = srcHeight;
    destSurfaceRect.right = destSurfaceRect.left + destWidth;
    destSurfaceRect.bottom = destSurfaceRect.top + destHeight;
#endif

    bool skipPrimarySurface = false;
#if DDRAW_BLIT_ALLOW_SKIP_PRIMARY_SURFACE
    if (blitIsUnscaled)
    {
        skipPrimarySurface = true;
    }
#endif

    //do drawing
    if (!skipPrimarySurface)
    {
        hr = CDirectDrawSurface7Wrapper::Blt(pDestRect, srcSurface, pSrcRect, flags, fx);
    }

	//Get the Real DC so we can look it up and override it
    HDC realDC = windowContext->GetDC_();
    {
        auto lock = windowContext->CreateLock();
        //Get the D3D DC (does not get releaseed by this code)
        HDC d3dDC = windowContext->GetCurrentDC(realDC);
        HDC surfaceDC = NULL;
        if (skipPrimarySurface)
        {
        	//Get DC from source surface
            hr = srcSurface->GetDC(&surfaceDC);
            //Blit using source coordinates
            okay = BitBlt(d3dDC, destSurfaceRect.left, destSurfaceRect.top, destWidth, destHeight,
                surfaceDC, srcSurfaceRect.left, srcSurfaceRect.top, SRCCOPY);
            hr = srcSurface->ReleaseDC(surfaceDC);
            surfaceDC = NULL;
        }
        else
        {
        	//Get DC from destination surface
            hr = this->GetDC(&surfaceDC);
            //Blit using destination coordinates
            okay = BitBlt(d3dDC, destSurfaceRect.left, destSurfaceRect.top, destWidth, destHeight,
                surfaceDC, destSurfaceRect.left, destSurfaceRect.top, SRCCOPY);
            hr = this->ReleaseDC(surfaceDC);
            surfaceDC = NULL;
        }
		windowContext->AddDirtyRect();
        d3dDC = NULL;
        //d3d DC is released by CompleteDraw, called by ReleaseDC_
    }
    //Releases Direct3D DC, and triggers screen update
    windowContext->ReleaseDC_(realDC);
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
