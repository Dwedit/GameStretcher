#include "CDirectDrawSurfaceWrapper.h"
#include "Win32Ex.h"
#include <ddraw.h>

CDirectDrawSurface7Wrapper::CDirectDrawSurface7Wrapper(IDirectDrawSurface7* obj)
{
	this->realObject = obj;
	SafeAddRef(obj);
}
CDirectDrawSurface7Wrapper::~CDirectDrawSurface7Wrapper()
{
	SafeRelease(realObject);
}

IDirectDrawSurface7* CDirectDrawSurface7Wrapper::GetRealObject(IDirectDrawSurface7* obj)
{
	if (obj != NULL)
	{
		CDirectDrawSurface7Wrapper* wrapper = NULL;
		HRESULT hr = obj->QueryInterface(CLSID_DirectDrawSurface7Wrapper, (void**)&wrapper);
		if (SUCCEEDED(hr) && wrapper != NULL)
		{
			IDirectDrawSurface7* realObject = wrapper->realObject;
			SafeRelease(wrapper);
			return realObject;
		}
	}
	return obj;
}

HRESULT CDirectDrawSurface7Wrapper::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	IUnknown*& obj = *(IUnknown**)ppvObj;
	HRESULT hr;
	if (riid == CLSID_DirectDrawSurface7Wrapper && ppvObj != NULL)
	{
		obj = this;
		AddRef();
		return 0;
	}
	hr = realObject->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr) && obj != NULL)
	{
		if (obj == realObject)
		{
			SafeRelease(obj);
			obj = this;
			AddRef();
		}
		if (riid == IID_IDirectDrawSurface ||
			riid == IID_IDirectDrawSurface2 ||
			riid == IID_IDirectDrawSurface3 ||
			riid == IID_IDirectDrawSurface4 ||
			riid == IID_IDirectDrawSurface7)
		{
			CDirectDrawSurface7Wrapper* newObj = CreateSurfaceWrapper((IDirectDrawSurface7*)obj);
			SafeRelease(obj);
			obj = newObj;
		}
	}
	return hr;
}

CDirectDrawSurface7Wrapper* CDirectDrawSurface7Wrapper::CreateSurfaceWrapper(IDirectDrawSurface7* obj)
{
	return new CDirectDrawSurface7Wrapper(obj);
}

ULONG CDirectDrawSurface7Wrapper::AddRef()
{
	ULONG refCount = ++this->refCount;
	return refCount;
}

ULONG CDirectDrawSurface7Wrapper::Release()
{
	ULONG refCount = --this->refCount;
	if (refCount == 0)
	{
		SafeRelease(realObject);
		delete this;
	}
	return refCount;
}

HRESULT CDirectDrawSurface7Wrapper::AddAttachedSurface(LPDIRECTDRAWSURFACE7 a)
{
	a = GetRealObject(a);
	return realObject->AddAttachedSurface(a);
}

HRESULT CDirectDrawSurface7Wrapper::AddOverlayDirtyRect(LPRECT a)
{
	return realObject->AddOverlayDirtyRect(a);
}

HRESULT CDirectDrawSurface7Wrapper::Blt(LPRECT a, LPDIRECTDRAWSURFACE7 b, LPRECT c, DWORD d, LPDDBLTFX e)
{
	//surfaces found in DDBLTFX were declared obsolete, and are not used, thus do not need to be unwrapped
	b = GetRealObject(b);
	return realObject->Blt(a, b, c, d, e);
}

HRESULT CDirectDrawSurface7Wrapper::BltBatch(LPDDBLTBATCH a, DWORD b, DWORD c)
{
	//According to MSDN: Not Implemented
	return realObject->BltBatch(a, b, c);
}

HRESULT CDirectDrawSurface7Wrapper::BltFast(DWORD a, DWORD b, LPDIRECTDRAWSURFACE7 c, LPRECT d, DWORD e)
{
	c = GetRealObject(c);
	return realObject->BltFast(a, b, c, d, e);
}

HRESULT CDirectDrawSurface7Wrapper::DeleteAttachedSurface(DWORD a, LPDIRECTDRAWSURFACE7 b)
{
	b = GetRealObject(b);
	return realObject->DeleteAttachedSurface(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::EnumAttachedSurfaces(LPVOID a, LPDDENUMSURFACESCALLBACK7 b)
{
	return realObject->EnumAttachedSurfaces(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::EnumOverlayZOrders(DWORD a, LPVOID b, LPDDENUMSURFACESCALLBACK7 c)
{
	return realObject->EnumOverlayZOrders(a, b, c);
}

HRESULT CDirectDrawSurface7Wrapper::Flip(LPDIRECTDRAWSURFACE7 a, DWORD b)
{
	a = GetRealObject(a);
	return realObject->Flip(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::GetAttachedSurface(LPDDSCAPS2 a, LPDIRECTDRAWSURFACE7 FAR* b)
{
	return realObject->GetAttachedSurface(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::GetBltStatus(DWORD a)
{
	return realObject->GetBltStatus(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetCaps(LPDDSCAPS2 a)
{
	return realObject->GetCaps(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetClipper(LPDIRECTDRAWCLIPPER FAR* a)
{
	return realObject->GetClipper(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetColorKey(DWORD a, LPDDCOLORKEY b)
{
	return realObject->GetColorKey(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::GetDC(HDC FAR* a)
{
	return realObject->GetDC(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetFlipStatus(DWORD a)
{
	return realObject->GetFlipStatus(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetOverlayPosition(LPLONG a, LPLONG b)
{
	return realObject->GetOverlayPosition(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::GetPalette(LPDIRECTDRAWPALETTE FAR* a)
{
	return realObject->GetPalette(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetPixelFormat(LPDDPIXELFORMAT a)
{
	return realObject->GetPixelFormat(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetSurfaceDesc(LPDDSURFACEDESC2 a)
{
	return realObject->GetSurfaceDesc(a);
}

HRESULT CDirectDrawSurface7Wrapper::Initialize(LPDIRECTDRAW a, LPDDSURFACEDESC2 b)
{
	return realObject->Initialize(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::IsLost()
{
	return realObject->IsLost();
}

HRESULT CDirectDrawSurface7Wrapper::Lock(LPRECT a, LPDDSURFACEDESC2 b, DWORD c, HANDLE d)
{
	return realObject->Lock(a, b, c, d);
}

HRESULT CDirectDrawSurface7Wrapper::ReleaseDC(HDC a)
{
	return realObject->ReleaseDC(a);
}

HRESULT CDirectDrawSurface7Wrapper::Restore()
{
	return realObject->Restore();
}

HRESULT CDirectDrawSurface7Wrapper::SetClipper(LPDIRECTDRAWCLIPPER a)
{
	return realObject->SetClipper(a);
}

HRESULT CDirectDrawSurface7Wrapper::SetColorKey(DWORD a, LPDDCOLORKEY b)
{
	return realObject->SetColorKey(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::SetOverlayPosition(LONG a, LONG b)
{
	return realObject->SetOverlayPosition(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::SetPalette(LPDIRECTDRAWPALETTE a)
{
	return realObject->SetPalette(a);
}

HRESULT CDirectDrawSurface7Wrapper::Unlock(LPRECT a)
{
	return realObject->Unlock(a);
}

HRESULT CDirectDrawSurface7Wrapper::UpdateOverlay(LPRECT a, LPDIRECTDRAWSURFACE7 b, LPRECT c, DWORD d, LPDDOVERLAYFX e)
{
	b = GetRealObject(b);
	//unwrap DirectDraw surfaces contained in DDOVERLAYFX if they exist
	if ((d & (DDOVER_ALPHADESTSURFACEOVERRIDE | DDOVER_ALPHASRCCONSTOVERRIDE)) != 0 && e != NULL)
	{
		DDOVERLAYFX fx = *e;
		if (d & DDOVER_ALPHASRCCONSTOVERRIDE)
			fx.lpDDSAlphaSrc = (IDirectDrawSurface*)GetRealObject((IDirectDrawSurface7*)fx.lpDDSAlphaSrc);
		if (d & DDOVER_ALPHADESTSURFACEOVERRIDE)
			fx.lpDDSAlphaDest = (IDirectDrawSurface*)GetRealObject((IDirectDrawSurface7*)fx.lpDDSAlphaDest);
		return realObject->UpdateOverlay(a, b, c, d, &fx);
	}
	return realObject->UpdateOverlay(a, b, c, d, e);
}

HRESULT CDirectDrawSurface7Wrapper::UpdateOverlayDisplay(DWORD a)
{
	return realObject->UpdateOverlayDisplay(a);
}

HRESULT CDirectDrawSurface7Wrapper::UpdateOverlayZOrder(DWORD a, LPDIRECTDRAWSURFACE7 b)
{
	b = GetRealObject(b);
	return realObject->UpdateOverlayZOrder(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::GetDDInterface(LPVOID FAR* a)
{
	return realObject->GetDDInterface(a);
}

HRESULT CDirectDrawSurface7Wrapper::PageLock(DWORD a)
{
	return realObject->PageLock(a);
}

HRESULT CDirectDrawSurface7Wrapper::PageUnlock(DWORD a)
{
	return realObject->PageUnlock(a);
}

HRESULT CDirectDrawSurface7Wrapper::SetSurfaceDesc(LPDDSURFACEDESC2 a, DWORD b)
{
	return realObject->SetSurfaceDesc(a, b);
}

HRESULT CDirectDrawSurface7Wrapper::SetPrivateData(REFGUID a, LPVOID b, DWORD c, DWORD d)
{
	return realObject->SetPrivateData(a, b, c, d);
}

HRESULT CDirectDrawSurface7Wrapper::GetPrivateData(REFGUID a, LPVOID b, LPDWORD c)
{
	return realObject->GetPrivateData(a, b, c);
}

HRESULT CDirectDrawSurface7Wrapper::FreePrivateData(REFGUID a)
{
	return realObject->FreePrivateData(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetUniquenessValue(LPDWORD a)
{
	return realObject->GetUniquenessValue(a);
}

HRESULT CDirectDrawSurface7Wrapper::ChangeUniquenessValue()
{
	return realObject->ChangeUniquenessValue();
}

HRESULT CDirectDrawSurface7Wrapper::SetPriority(DWORD a)
{
	return realObject->SetPriority(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetPriority(LPDWORD a)
{
	return realObject->GetPriority(a);
}

HRESULT CDirectDrawSurface7Wrapper::SetLOD(DWORD a)
{
	return realObject->SetLOD(a);
}

HRESULT CDirectDrawSurface7Wrapper::GetLOD(LPDWORD a)
{
	return realObject->GetLOD(a);
}
