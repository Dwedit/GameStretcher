#include "D3D9Override.h"

D3D9SwapChainContext::D3D9SwapChainContext(IDirect3DSwapChain9* swapChain)
{
	Init(swapChain);
}
D3D9SwapChainContext::D3D9SwapChainContext()
{

}
D3D9SwapChainContext::~D3D9SwapChainContext()
{
	Destroy();
}
void D3D9SwapChainContext::Init(IDirect3DSwapChain9* swapChain)
{
	this->realSwapChain = (IDirect3DSwapChain9Ex*)swapChain;
	this->originalVTable = GetOriginalVTable(swapChain);
	this->myVTable = GetNewVTable(swapChain);
	this->IsEx = GetIsEx(swapChain);
	HRESULT hr;
	hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &this->realBackBuffer);
}

void D3D9SwapChainContext::Destroy()
{
	SafeRelease(this->realSwapChain);
	SafeRelease(this->realBackBuffer);
}

HRESULT D3D9SwapChainContext::Present_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	//TODO
	return PresentReal(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}
HRESULT D3D9SwapChainContext::GetFrontBufferData_(IDirect3DSurface9* pDestSurface)
{
	return GetFrontBufferDataReal(pDestSurface);
}
HRESULT D3D9SwapChainContext::GetBackBuffer_(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return GetBackBufferReal(iBackBuffer, Type, ppBackBuffer);
}
HRESULT D3D9SwapChainContext::GetPresentParameters_(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return GetPresentParametersReal(pPresentationParameters);
}

HRESULT D3D9SwapChainContext::PresentReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	return originalVTable->Present(realSwapChain, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}
HRESULT D3D9SwapChainContext::GetFrontBufferDataReal(IDirect3DSurface9* pDestSurface)
{
	return originalVTable->GetFrontBufferData(realSwapChain, pDestSurface);
}
HRESULT D3D9SwapChainContext::GetBackBufferReal(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return originalVTable->GetBackBuffer(realSwapChain, iBackBuffer, Type, ppBackBuffer);
}
HRESULT D3D9SwapChainContext::GetPresentParametersReal(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return originalVTable->GetPresentParameters(realSwapChain, pPresentationParameters);
}

HRESULT __stdcall D3D9SwapChainContext::Present(IDirect3DSwapChain9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	auto context = GetD3D9SwapChainContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->Present(This, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	else
	{
		return context->Present_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
}
HRESULT __stdcall D3D9SwapChainContext::GetFrontBufferData(IDirect3DSwapChain9Ex* This, IDirect3DSurface9* pDestSurface)
{
	auto context = GetD3D9SwapChainContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetFrontBufferData(This, pDestSurface);
	}
	else
	{
		return context->GetFrontBufferData_(pDestSurface);
	}

}
HRESULT __stdcall D3D9SwapChainContext::GetBackBuffer(IDirect3DSwapChain9Ex* This, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	auto context = GetD3D9SwapChainContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetBackBuffer(This, iBackBuffer, Type, ppBackBuffer);
	}
	else
	{
		return context->GetBackBuffer_(iBackBuffer, Type, ppBackBuffer);
	}
}
HRESULT __stdcall D3D9SwapChainContext::GetPresentParameters(IDirect3DSwapChain9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	auto context = GetD3D9SwapChainContext(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->GetPresentParameters(This, pPresentationParameters);
	}
	else
	{
		return context->GetPresentParameters_(pPresentationParameters);
	}
}

bool GetIsEx(IDirect3DSwapChain9* swapChain)
{
	HRESULT hr = 0;
	IDirect3DSwapChain9Ex* swapChainEx = NULL;
	hr = swapChain->QueryInterface(IID_IDirect3DSwapChain9Ex, (void**)&swapChainEx);
	if (SUCCEEDED(hr) && swapChainEx != NULL)
	{
		SafeRelease(swapChainEx);
		return true;
	}
	return false;
}
