#include "D3D9Override.h"
#include "MemoryUnlocker.h"

D3D9DeviceContext::D3D9DeviceContext(IDirect3DDevice9* device)
{
	Init(device);
}
D3D9DeviceContext::D3D9DeviceContext()
{

}
void D3D9DeviceContext::SetVTable()
{
	MemoryUnlocker unlocker(this->myVTable);
	this->myVTable->Release = Release;
	this->myVTable->GetCreationParameters = GetCreationParameters;
	this->myVTable->CreateAdditionalSwapChain = CreateAdditionalSwapChain;
	this->myVTable->Reset = Reset;
	this->myVTable->Present = Present;
	this->myVTable->GetBackBuffer = GetBackBuffer;
	this->myVTable->GetFrontBufferData = GetFrontBufferData;
	this->myVTable->BeginStateBlock = BeginStateBlock;
	this->myVTable->PresentEx = PresentEx;
	this->myVTable->ResetEx = ResetEx;
}
void D3D9DeviceContext::Init(IDirect3DDevice9* device)
{
	this->device = (IDirect3DDevice9Ex*)device;
	this->device->AddRef();
	this->originalVTable = GetOriginalVTable(device);
	this->IsEx = GetIsEx(device);
	this->myVTable = ((IDirect3DDevice9Ex_*)device)->lpVtbl;
	SetVTable();
	HRESULT hr = 0;
	hr = device->GetSwapChain(0, &realSwapChain);
	//hr = device->GetDepthStencilSurface(&initialDepthStencilSurface);
	this->childSwapChainContext = GetD3D9SwapChainContext(realSwapChain);
}
D3D9DeviceContext::~D3D9DeviceContext()
{
	Destroy();
}
void D3D9DeviceContext::Destroy()
{
	//SafeRelease(this->initialDepthStencilSurface);
	SafeRelease(this->realSwapChain);
	SafeRelease(this->device);
	this->childSwapChainContext->Destroy();
}

HRESULT D3D9DeviceContext::CreateVirtualDevice(HWND hwnd, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return this->childSwapChainContext->CreateVirtualDevice(hwnd, BehaviorFlags, pPresentationParameters);
}

ULONG D3D9DeviceContext::Release_()
{
	//TODO
	ULONG value = this->ReleaseReal();
	return value;
}
HRESULT D3D9DeviceContext::GetCreationParameters_(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	//TODO
	HRESULT hr = this->GetCreationParametersReal(pParameters);
	return hr;
}
HRESULT D3D9DeviceContext::CreateAdditionalSwapChain_(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	//TODO
	HRESULT hr = this->CreateAdditionalSwapChainReal(pPresentationParameters, pSwapChain);
	return hr;
}
//HRESULT D3D9DeviceContext::GetSwapChain_(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
//{
//	HRESULT hr = this->GetSwapChainReal(iSwapChain, pSwapChain);
//	return hr;
//}
//UINT D3D9DeviceContext::GetNumberOfSwapChains_()
//{
//	UINT count = this->GetNumberOfSwapChainsReal();
//	return count;
//}
HRESULT D3D9DeviceContext::Reset_(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//TODO
	HRESULT hr = this->ResetReal(pPresentationParameters);
	return hr;
}
HRESULT D3D9DeviceContext::Present_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	HRESULT hr = this->childSwapChainContext->Present_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, 0);
	return hr;
}
HRESULT D3D9DeviceContext::GetBackBuffer_(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	HRESULT hr;
	if (iSwapChain == 0)
	{
		hr = this->childSwapChainContext->GetBackBuffer_(iBackBuffer, Type, ppBackBuffer);
	}
	else
	{
		hr = GetBackBufferReal(iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}
	return hr;
}
HRESULT D3D9DeviceContext::GetFrontBufferData_(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	HRESULT hr = 0;
	if (iSwapChain == 0)
	{
		hr = this->childSwapChainContext->GetFrontBufferData_(pDestSurface);
	}
	else
	{
		hr = GetFrontBufferDataReal(iSwapChain, pDestSurface);
	}
	return hr;
}
HRESULT D3D9DeviceContext::BeginStateBlock_()
{
	HRESULT hr = this->BeginStateBlockReal();
	//BeginStateBlock overwrites the VTable, this will restore it
	SetVTable();
	return hr;
}
HRESULT D3D9DeviceContext::PresentEx_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	return this->childSwapChainContext->Present_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}
HRESULT D3D9DeviceContext::ResetEx_(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
	//TODO
	HRESULT hr = ResetExReal(pPresentationParameters, pFullscreenDisplayMode);
	return hr;
}

ULONG D3D9DeviceContext::ReleaseReal()
{
	ULONG value = originalVTable->Release(device);
	return value;
}
HRESULT D3D9DeviceContext::GetCreationParametersReal(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	HRESULT hr = originalVTable->GetCreationParameters(device, pParameters);
	return hr;
}
HRESULT D3D9DeviceContext::CreateAdditionalSwapChainReal(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	//TODO
	HRESULT hr = originalVTable->CreateAdditionalSwapChain(device, pPresentationParameters, pSwapChain);
	return hr;
}
//HRESULT D3D9DeviceContext::GetSwapChainReal(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
//{
//	HRESULT hr = originalVTable->GetSwapChain(device, iSwapChain, pSwapChain);
//	return hr;
//}
//UINT D3D9DeviceContext::GetNumberOfSwapChainsReal()
//{
//	UINT count = originalVTable->GetNumberOfSwapChains(device);
//	return count;
//}
HRESULT D3D9DeviceContext::ResetReal(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	HRESULT hr = originalVTable->Reset(device, pPresentationParameters);
	return hr;
}
HRESULT D3D9DeviceContext::PresentReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	HRESULT hr = originalVTable->Present(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return hr;
}
HRESULT D3D9DeviceContext::GetBackBufferReal(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	HRESULT hr = originalVTable->GetBackBuffer(device, iSwapChain, iBackBuffer, Type, ppBackBuffer);
	return hr;
}
HRESULT D3D9DeviceContext::GetFrontBufferDataReal(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	HRESULT hr = originalVTable->GetFrontBufferData(device, iSwapChain, pDestSurface);
	return hr;
}
HRESULT D3D9DeviceContext::BeginStateBlockReal()
{
	HRESULT hr = originalVTable->BeginStateBlock(device);
	return hr;
}
HRESULT D3D9DeviceContext::PresentExReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	if (!IsEx) return E_NOTIMPL;
	HRESULT hr = originalVTable->PresentEx(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	return hr;
}
HRESULT D3D9DeviceContext::ResetExReal(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
	if (!IsEx) return E_NOTIMPL;
	HRESULT hr = originalVTable->ResetEx(device, pPresentationParameters, pFullscreenDisplayMode);
	return hr;
}

ULONG __stdcall D3D9DeviceContext::Release(IDirect3DDevice9Ex* This)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->Release(This);
	}
	else
	{
		return context->Release_();
	}
}
HRESULT __stdcall D3D9DeviceContext::GetCreationParameters(IDirect3DDevice9Ex* This, D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetCreationParameters(This, pParameters);
	}
	else
	{
		return context->GetCreationParameters_(pParameters);
	}
}
HRESULT __stdcall D3D9DeviceContext::CreateAdditionalSwapChain(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->CreateAdditionalSwapChain(This, pPresentationParameters, pSwapChain);
	}
	else
	{
		return context->CreateAdditionalSwapChain_(pPresentationParameters, pSwapChain);
	}
}
/*
HRESULT __stdcall D3D9DeviceContext::GetSwapChain(IDirect3DDevice9Ex* This, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetSwapChain(This, iSwapChain, pSwapChain);
	}
	else
	{
		return context->GetSwapChain_(iSwapChain, pSwapChain);
	}
}
UINT __stdcall D3D9DeviceContext::GetNumberOfSwapChains(IDirect3DDevice9Ex* This)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetNumberOfSwapChains(This);
	}
	else
	{
		return context->GetNumberOfSwapChains_();
	}
}
*/
HRESULT __stdcall D3D9DeviceContext::Reset(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->Reset(This, pPresentationParameters);
	}
	else
	{
		return context->Reset_(pPresentationParameters);
	}
}
HRESULT __stdcall D3D9DeviceContext::Present(IDirect3DDevice9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->Present(This, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}
	else
	{
		return context->Present_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}
}
HRESULT __stdcall D3D9DeviceContext::GetBackBuffer(IDirect3DDevice9Ex* This, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetBackBuffer(This, iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}
	else
	{
		return context->GetBackBuffer_(iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}
}
HRESULT __stdcall D3D9DeviceContext::GetFrontBufferData(IDirect3DDevice9Ex* This, UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetFrontBufferData(This, iSwapChain, pDestSurface);
	}
	else
	{
		return context->GetFrontBufferData_(iSwapChain, pDestSurface);
	}
}
HRESULT __stdcall D3D9DeviceContext::BeginStateBlock(IDirect3DDevice9Ex* This)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->BeginStateBlock(This);
	}
	else
	{
		return context->BeginStateBlock_();
	}
}
HRESULT __stdcall D3D9DeviceContext::PresentEx(IDirect3DDevice9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->PresentEx(This, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	else
	{
		return context->PresentEx_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
}
HRESULT __stdcall D3D9DeviceContext::ResetEx(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->ResetEx(This, pPresentationParameters, pFullscreenDisplayMode);
	}
	else
	{
		return context->ResetEx_(pPresentationParameters, pFullscreenDisplayMode);
	}
}

bool GetIsEx(IDirect3DDevice9* device)
{
	HRESULT hr = 0;
	IDirect3DDevice9Ex* deviceEx = NULL;
	hr = device->QueryInterface(IID_IDirect3DDevice9Ex, (void**)&deviceEx);
	if (SUCCEEDED(hr) && deviceEx != NULL)
	{
		SafeRelease(deviceEx);
		return true;
	}
	return false;
}

