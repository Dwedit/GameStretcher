#include "D3D9Override.h"
#include "MemoryUnlocker.h"
#include "WindowContext.h"

D3D9SurfaceContext::D3D9SurfaceContext()
{
	this->realSurface = NULL;
	this->myVTable = NULL;
	this->originalVTable = NULL;
}
D3D9SurfaceContext::~D3D9SurfaceContext()
{
	Dispose();
}
void D3D9SurfaceContext::SetVTable()
{
	MemoryUnlocker unlocker(this->myVTable);
	this->myVTable->LockRect = LockRect;
	this->myVTable->UnlockRect = UnlockRect;
}
void D3D9SurfaceContext::Init(IDirect3DSurface9* surface)
{
	Dispose();
	this->realSurface = surface;
	this->realSurface->AddRef();
	this->originalVTable = GetOriginalVTable(realSurface, true);
	this->myVTable = ((IDirect3DSurface9_*)realSurface)->lpVtbl;
	SetVTable();
}
void D3D9SurfaceContext::Dispose()
{
	SafeRelease(realSurface);
}

HRESULT __stdcall D3D9SurfaceContext::LockRect(IDirect3DSurface9* This, D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags)
{
	CriticalSectionLock locker(&::d3d9CriticalSection);
	auto context = GetD3D9SurfaceContext(This);
	if (context == NULL || context->realSurface == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->LockRect(This, pLockedRect, pRect, Flags);
	}
	else
	{
		context->LockRect_(pLockedRect, pRect, Flags);
	}
}
HRESULT __stdcall D3D9SurfaceContext::UnlockRect(IDirect3DSurface9* This)
{
	CriticalSectionLock locker(&::d3d9CriticalSection);
	auto context = GetD3D9SurfaceContext(This);
	if (context == NULL || context->realSurface == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->UnlockRect(This);
	}
	else
	{
		return context->UnlockRect_();
	}
}

HRESULT __stdcall D3D9SurfaceContext::LockRect_(D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags)
{
	//TODO
	return LockRectReal(pLockedRect, pRect, Flags);
}
HRESULT __stdcall D3D9SurfaceContext::UnlockRect_()
{
	//TODO
	return UnlockRectReal();
}

HRESULT __stdcall D3D9SurfaceContext::LockRectReal(D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags)
{
	return originalVTable->LockRect(this->realSurface, pLockedRect, pRect, Flags);
}
HRESULT __stdcall D3D9SurfaceContext::UnlockRectReal()
{
	return originalVTable->UnlockRect(this->realSurface);
}
