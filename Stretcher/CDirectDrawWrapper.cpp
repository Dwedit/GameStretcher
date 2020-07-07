#include "CDirectDrawWrapper.h"
#include "CDirectDrawSurfaceWrapper.h"

CDirectDraw7Wrapper::CDirectDraw7Wrapper(IDirectDraw7* obj)
{
	realObject = obj;
	SafeAddRef(realObject);
}
CDirectDraw7Wrapper::~CDirectDraw7Wrapper()
{
	SafeRelease(realObject);
}

HRESULT CDirectDraw7Wrapper::QueryInterface(const IID& riid, LPVOID* ppvObj)
{
	IUnknown*& obj = *(IUnknown**)ppvObj;
	HRESULT hr;
	hr = realObject->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr) && obj != NULL)
	{
		if (obj == realObject)
		{
			SafeRelease(obj);
			obj = this;
			AddRef();
		}
		if (riid == IID_IDirectDraw ||
			riid == IID_IDirectDraw2 ||
			riid == IID_IDirectDraw4 ||
			riid == IID_IDirectDraw7)
		{
			CDirectDraw7Wrapper* newObj = new CDirectDraw7Wrapper((IDirectDraw7*)obj);
			SafeRelease(obj);
			obj = newObj;
		}
	}
	return hr;
}

ULONG CDirectDraw7Wrapper::AddRef()
{
	ULONG refCount = ++this->refCount;
	return refCount;
}

ULONG CDirectDraw7Wrapper::Release()
{
	ULONG refCount = --this->refCount;
	if (refCount == 0)
	{
		SafeRelease(realObject);
		delete this;
	}
	return refCount;
}

HRESULT CDirectDraw7Wrapper::Compact()
{
	return realObject->Compact();
}

HRESULT CDirectDraw7Wrapper::CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR* b, IUnknown FAR* c)
{
	return realObject->CreateClipper(a, b, c);
}

HRESULT CDirectDraw7Wrapper::CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR* c, IUnknown FAR* d)
{
	return realObject->CreatePalette(a, b, c, d);
}

HRESULT CDirectDraw7Wrapper::CreateSurface(LPDDSURFACEDESC2 a, LPDIRECTDRAWSURFACE7 FAR* b, IUnknown FAR* c)
{
	IDirectDrawSurface7*& surface = *b;
	HRESULT hr = realObject->CreateSurface(a, b, c);
	if (SUCCEEDED(hr) && surface != NULL)
	{
		CDirectDrawSurface7Wrapper* newSurface = CreateSurfaceWrapper(surface);
		SafeRelease(surface);
		surface = newSurface;
	}
	return hr;
}

CDirectDrawSurface7Wrapper* CDirectDraw7Wrapper::CreateSurfaceWrapper(LPDIRECTDRAWSURFACE7 obj)
{
	return new CDirectDrawSurface7Wrapper(obj);
}

HRESULT CDirectDraw7Wrapper::DuplicateSurface(LPDIRECTDRAWSURFACE7 a, LPDIRECTDRAWSURFACE7 FAR* b)
{
	return realObject->DuplicateSurface(a, b);
}

HRESULT CDirectDraw7Wrapper::EnumDisplayModes(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMMODESCALLBACK2 d)
{
	return realObject->EnumDisplayModes(a, b, c, d);
}

HRESULT CDirectDraw7Wrapper::EnumSurfaces(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMSURFACESCALLBACK7 d)
{
	return realObject->EnumSurfaces(a, b, c, d);
}

HRESULT CDirectDraw7Wrapper::FlipToGDISurface()
{
	return realObject->FlipToGDISurface();
}

HRESULT CDirectDraw7Wrapper::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
	return realObject->GetCaps(lpDDDriverCaps, lpDDHELCaps);
}

HRESULT CDirectDraw7Wrapper::GetDisplayMode(LPDDSURFACEDESC2 a)
{
	return realObject->GetDisplayMode(a);
}

HRESULT CDirectDraw7Wrapper::GetFourCCCodes(LPDWORD a, LPDWORD b)
{
	return realObject->GetFourCCCodes(a, b);
}

HRESULT CDirectDraw7Wrapper::GetGDISurface(LPDIRECTDRAWSURFACE7 FAR* a)
{
	return realObject->GetGDISurface(a);
}

HRESULT CDirectDraw7Wrapper::GetMonitorFrequency(LPDWORD a)
{
	return realObject->GetMonitorFrequency(a);
}

HRESULT CDirectDraw7Wrapper::GetScanLine(LPDWORD a)
{
	return realObject->GetScanLine(a);
}

HRESULT CDirectDraw7Wrapper::GetVerticalBlankStatus(LPBOOL a)
{
	return realObject->GetVerticalBlankStatus(a);
}

HRESULT CDirectDraw7Wrapper::Initialize(GUID FAR* a)
{
	return realObject->Initialize(a);
}

HRESULT CDirectDraw7Wrapper::RestoreDisplayMode()
{
	return realObject->RestoreDisplayMode();
}

HRESULT CDirectDraw7Wrapper::SetCooperativeLevel(HWND a, DWORD b)
{
	return realObject->SetCooperativeLevel(a, b);
}

HRESULT CDirectDraw7Wrapper::SetDisplayMode(DWORD a, DWORD b, DWORD c, DWORD d, DWORD e)
{
	return realObject->SetDisplayMode(a, b, c, d, e);
}

HRESULT CDirectDraw7Wrapper::WaitForVerticalBlank(DWORD a, HANDLE b)
{
	return realObject->WaitForVerticalBlank(a, b);
}

HRESULT CDirectDraw7Wrapper::GetAvailableVidMem(LPDDSCAPS2 a, LPDWORD b, LPDWORD c)
{
	return realObject->GetAvailableVidMem(a, b, c);
}

HRESULT CDirectDraw7Wrapper::GetSurfaceFromDC(HDC a, LPDIRECTDRAWSURFACE7* b)
{
	return realObject->GetSurfaceFromDC(a, b);
}

HRESULT CDirectDraw7Wrapper::RestoreAllSurfaces()
{
	return realObject->RestoreAllSurfaces();
}

HRESULT CDirectDraw7Wrapper::TestCooperativeLevel()
{
	return realObject->TestCooperativeLevel();
}

HRESULT CDirectDraw7Wrapper::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 a, DWORD b)
{
	return realObject->GetDeviceIdentifier(a, b);
}

HRESULT CDirectDraw7Wrapper::StartModeTest(LPSIZE a, DWORD b, DWORD c)
{
	return realObject->StartModeTest(a, b, c);
}

HRESULT CDirectDraw7Wrapper::EvaluateMode(DWORD a, DWORD* b)
{
	return realObject->EvaluateMode(a, b);
}
