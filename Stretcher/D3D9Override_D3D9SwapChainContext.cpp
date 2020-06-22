#include "D3D9Override.h"
#include "MemoryUnlocker.h"
#include "WindowContext.h"

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
void D3D9SwapChainContext::SetVTable()
{
	MemoryUnlocker unlocker(this->myVTable);
	this->myVTable->Present = Present;
	this->myVTable->GetFrontBufferData = GetFrontBufferData;
	this->myVTable->GetBackBuffer = GetBackBuffer;
	this->myVTable->GetPresentParameters = GetPresentParameters;
}
void D3D9SwapChainContext::Init(IDirect3DSwapChain9* swapChain)
{
	this->realSwapChain = (IDirect3DSwapChain9Ex*)swapChain;
	this->originalVTable = GetOriginalVTable(swapChain);
	this->IsEx = GetIsEx(swapChain);
	this->myVTable = ((IDirect3DSwapChain9Ex_*)realSwapChain)->lpVtbl;
	SetVTable();
	HRESULT hr = 0;
	forceReal = true;
	hr = this->realSwapChain->GetDevice(&this->device);	
	hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &this->realBackBuffer);
	hr = device->GetDepthStencilSurface(&this->realDepthStencilSurface);
	forceReal = false;
}

void D3D9SwapChainContext::Destroy()
{
	SafeRelease(realSwapChain);
	SafeRelease(realBackBuffer);
	SafeRelease(realDepthStencilSurface);
	SafeRelease(virtualBackBufferTexture);
	SafeRelease(virtualBackBuffer);
	SafeRelease(virtualBackBufferMultisampled);
	SafeRelease(virtualDepthStencilSurface);
	SafeRelease(device);
	upscaler.Destroy();
}

HRESULT D3D9SwapChainContext::CreateVirtualDevice(HWND hwnd, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (pPresentationParameters == NULL) return E_POINTER;
	this->presentParameters = *pPresentationParameters;

	WindowContext* windowContext = WindowContext::Get(hwnd);

	width = pPresentationParameters->BackBufferWidth;
	height = pPresentationParameters->BackBufferHeight;
	D3DFORMAT pixelFormat = pPresentationParameters->BackBufferFormat;
	swapEffect = pPresentationParameters->SwapEffect;

	if (width <= 0 || height <= 0)
	{
		RECT rect = windowContext->GetClientRect_();
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	
	HRESULT hr = 0;
	SafeRelease(virtualBackBufferTexture);
	SafeRelease(virtualBackBuffer);
	SafeRelease(virtualBackBufferMultisampled);
	
	hr = device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, pixelFormat, D3DPOOL_DEFAULT, &virtualBackBufferTexture, NULL);
	if (SUCCEEDED(hr) && virtualBackBufferTexture != NULL)
	{
		hr = virtualBackBufferTexture->GetSurfaceLevel(0, &virtualBackBuffer);
	}

	if (pPresentationParameters->MultiSampleType != D3DMULTISAMPLE_NONE)
	{
		hr = device->CreateRenderTarget(width, height, pixelFormat, pPresentationParameters->MultiSampleType, pPresentationParameters->MultiSampleQuality, false, &virtualBackBufferMultisampled, NULL);
	}

	SafeRelease(virtualDepthStencilSurface);
	if (pPresentationParameters->EnableAutoDepthStencil)
	{
		hr = device->CreateDepthStencilSurface(width, height, pPresentationParameters->AutoDepthStencilFormat, pPresentationParameters->MultiSampleType, pPresentationParameters->MultiSampleQuality, swapEffect == D3DSWAPEFFECT_DISCARD, &virtualDepthStencilSurface, NULL);
	}

	forceReal = true;
	upscaler.SetBorderDirty();
	upscaler.SetInputRectangle(0, 0, width, height);
	upscaler.SetSourceTexture(virtualBackBufferTexture);
	forceReal = false;

	hr = device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 255), 0, 0);

	hr = device->SetDepthStencilSurface(virtualDepthStencilSurface);
	if (virtualBackBufferMultisampled != NULL)
	{
		hr = device->SetRenderTarget(0, virtualBackBufferMultisampled);
	}
	else
	{
		hr = device->SetRenderTarget(0, virtualBackBuffer);
	}

	hr = device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 255, 0), 0, 0);
	if (BehaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING)
	{
		device->SetSoftwareVertexProcessing(true);
	}
	else
	{
		device->SetSoftwareVertexProcessing(false);
	}

	return hr;
}

HRESULT D3D9SwapChainContext::Present_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	if (forceReal || insidePresent)
	{
		return PresentReal(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	HRESULT hr = 0;

	insidePresent = true;
	if (virtualBackBufferMultisampled != NULL)
	{
		device->StretchRect(virtualBackBufferMultisampled, NULL, virtualBackBuffer, NULL, D3DTEXF_NONE);
	}
	forceReal = true;
	
	Region updateRegion;

	if (pSourceRect != NULL && pDestRect != NULL && *pSourceRect == *pDestRect)
	{
		updateRegion.AddRectangle(*pSourceRect);
	}
	else
	{
		updateRegion.AddRectangle(0, 0, width, height);
	}
	upscaler.Update(updateRegion);
	forceReal = false;
	//TODO
	//HRESULT hr = PresentReal(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	insidePresent = false;
	return hr;
}
HRESULT D3D9SwapChainContext::GetFrontBufferData_(IDirect3DSurface9* pDestSurface)
{
	return GetFrontBufferDataReal(pDestSurface);
}
HRESULT D3D9SwapChainContext::GetBackBuffer_(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	if (forceReal)
	{
		return GetBackBufferReal(iBackBuffer, Type, ppBackBuffer);
	}
	if (ppBackBuffer == NULL)
	{
		return E_POINTER;
	}
	if (virtualBackBufferMultisampled != NULL)
	{
		*ppBackBuffer = virtualBackBufferMultisampled;
	}
	else
	{
		*ppBackBuffer = virtualBackBuffer;
	}
	if (*ppBackBuffer != NULL)
	{
		(*ppBackBuffer)->AddRef();
		return 0;
	}
	else
	{
		return E_POINTER;
	}
}
HRESULT D3D9SwapChainContext::GetPresentParameters_(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (forceReal)
	{
		return GetPresentParametersReal(pPresentationParameters);
	}
	if (pPresentationParameters == NULL)
	{
		return E_POINTER;
	}
	*pPresentationParameters = this->presentParameters;
	return 0;
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
