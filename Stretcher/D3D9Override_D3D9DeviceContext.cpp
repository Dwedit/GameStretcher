#include "D3D9Override.h"
#include "MemoryUnlocker.h"

D3D9DeviceContext::D3D9DeviceContext(IDirect3DDevice9* device)
{
	Init(device);
}
D3D9DeviceContext::D3D9DeviceContext()
{
	
}

void D3D9DeviceContext::SetVTable(IDirect3DDevice9ExVtbl* myVTable)
{
	MemoryUnlocker unlocker(myVTable);
	myVTable->Release = Release;
	myVTable->GetCreationParameters = GetCreationParameters;
	myVTable->CreateAdditionalSwapChain = CreateAdditionalSwapChain;
	myVTable->Reset = Reset;
	myVTable->Present = Present;
	myVTable->GetBackBuffer = GetBackBuffer;
	myVTable->GetFrontBufferData = GetFrontBufferData;
	myVTable->BeginStateBlock = BeginStateBlock;
	myVTable->PresentEx = PresentEx;
	myVTable->ResetEx = ResetEx;
	myVTable->TestCooperativeLevel = TestCooperativeLevel;
}

void D3D9DeviceContext::SetVTable()
{
	if (this->myVTable == NULL) return;
	SetVTable(this->myVTable);
}

#if LOST_DEVICE_TEST && NO_DEVICE_HOOK

int _frameCount = 0;
const int _maxFrameCount = 150;
bool _loseDevice = false;
bool _returnedLost = false;
HRESULT __stdcall _Present(IDirect3DDevice9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	auto vtable = GetOriginalVTable(This);
	HRESULT hr;

	_frameCount++;
	if (_frameCount == _maxFrameCount)
	{
		_loseDevice = true;
		_returnedLost = false;
	}

	if (_loseDevice)
	{
		return D3DERR_DEVICELOST;
	}

	hr = vtable->Present(This, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return hr;
}

HRESULT __stdcall _Reset(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	auto vtable = GetOriginalVTable(This);
	HRESULT hr;

	_loseDevice = false;
	_frameCount = 0;
	hr = vtable->Reset(This, pPresentationParameters);
	return hr;
}

HRESULT __stdcall _TestCooperativeLevel(IDirect3DDevice9Ex* This)
{
	auto vtable = GetOriginalVTable(This);
	HRESULT hr;
	if (_loseDevice)
	{
		if (_returnedLost)
		{
			return D3DERR_DEVICENOTRESET;
		}
		_returnedLost = true;
		return D3DERR_DEVICELOST;
	}
	hr = vtable->TestCooperativeLevel(This);
	return hr;
}

HRESULT __stdcall _BeginStateBlock(IDirect3DDevice9Ex* This)
{
	HRESULT hr;
	auto context = GetD3D9DeviceContext(This);
	auto vtable = GetOriginalVTable(This);
	auto myVTable = context->myVTable;

	hr = vtable->BeginStateBlock(This);

	myVTable->Present = _Present;
	myVTable->Reset = _Reset;
	myVTable->TestCooperativeLevel = _TestCooperativeLevel;
	myVTable->BeginStateBlock = _BeginStateBlock;

	return hr;
}

#endif

void D3D9DeviceContext::Init(IDirect3DDevice9* device)
{
#if NO_DEVICE_HOOK
	DWORD currentThreadId = GetCurrentThreadId();
	this->device = (IDirect3DDevice9Ex*)device;
	this->originalVTable = GetOriginalVTable(device, true);
	this->myVTable = ((IDirect3DDevice9Ex_*)device)->lpVtbl;
#if LOST_DEVICE_TEST
	this->myVTable->Present = _Present;
	this->myVTable->Reset = _Reset;
	this->myVTable->TestCooperativeLevel = _TestCooperativeLevel;
	this->myVTable->BeginStateBlock = _BeginStateBlock;
#endif
	return;
#endif
	Dispose();
	this->device = (IDirect3DDevice9Ex*)device;
	BeginTrackingRefCount();
	this->internalRefCount++;  //for the object that contains this in ObjectStore
	this->device->AddRef();
	this->originalVTable = GetOriginalVTable(device, true);
	this->IsEx = GetIsEx(device);
	this->myVTable = ((IDirect3DDevice9Ex_*)device)->lpVtbl;
	SetVTable();
	HRESULT hr = 0;
	hr = device->GetSwapChain(0, &realSwapChain);
	this->childSwapChainContext = GetD3D9SwapChainContext(realSwapChain, true);
	this->childSwapChainContext->Init(realSwapChain);
	EndTrackingRefCount();
}
D3D9DeviceContext::~D3D9DeviceContext()
{
	Dispose();
}
void D3D9DeviceContext::Dispose()
{
	if (disposing) return;
	disposing = true;
	internalRefCount = 0;
	refCountFirstCapture = 0;
	refCountTrackerCount = 0;
	this->childSwapChainContext = NULL;
	D3D9SwapChainContext* swapChainContext = GetD3D9SwapChainContext(this->realSwapChain);
	if (swapChainContext != NULL)
	{
		swapChainContext->Dispose();
	}
	SafeRelease(this->realSwapChain);
	SafeRelease(this->device);
	disposing = false;
}

void D3D9DeviceContext::DisposeExcludingDevice()
{
	if (disposing) return;
	disposing = true;
	BeginTrackingRefCount();
	this->childSwapChainContext = NULL;
	D3D9SwapChainContext* swapChainContext = GetD3D9SwapChainContext(this->realSwapChain);
	if (swapChainContext != NULL)
	{
		swapChainContext->Dispose();
	}
	SafeRelease(this->realSwapChain);
	EndTrackingRefCount();
	disposing = false;
}

int D3D9DeviceContext::GetRefCount() const
{
	return (this->device != NULL) ? ::GetRefCount(this->device) : 0;
}

int D3D9DeviceContext::BeginTrackingRefCount()
{
	refCountTrackerCount++;
	int refCount = GetRefCount();
	if (refCountTrackerCount == 1)
	{
		this->refCountFirstCapture = refCount;
	}
	return refCount;
}

int D3D9DeviceContext::EndTrackingRefCount()
{
	int refCount = GetRefCount();
	refCountTrackerCount--;
	if (refCountTrackerCount == 0)
	{
		int refCountChange = refCount - this->refCountFirstCapture;
		this->internalRefCount += refCountChange;
	}
	return refCount;
}

HRESULT D3D9DeviceContext::CreateVirtualDevice(HWND hwnd, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (this->childSwapChainContext == NULL) return E_POINTER;
#if NO_DEVICE_HOOK
	return 0;
#endif
	BeginTrackingRefCount();
	HRESULT hr = this->childSwapChainContext->CreateVirtualDevice(hwnd, BehaviorFlags, pPresentationParameters);
	EndTrackingRefCount();
	return hr;
}

ULONG D3D9DeviceContext::Release_()
{
	ULONG value = this->ReleaseReal();
	if (refCountTrackerCount == 0 && internalRefCount == value && internalRefCount > 0)
	{
		auto dev = this->device;
		auto releasePtr = this->originalVTable->Release;
		value = dev->AddRef();
		Dispose();
		RemoveD3D9DisposedObjects();  //after calling, the "this" pointer is now invalid

		value = releasePtr(dev);
	}
	return value;
}
HRESULT D3D9DeviceContext::GetCreationParameters_(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	HRESULT hr;
	if (forceReal)
	{
		hr = this->GetCreationParametersReal(pParameters);
	}
	else
	{
		//TODO - Return the creation parameters provided by the application rather than by the stretcher
		hr = this->GetCreationParametersReal(pParameters);
	}
	return hr;
}
HRESULT D3D9DeviceContext::CreateAdditionalSwapChain_(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	HRESULT hr;
	if (forceReal)
	{
		hr = this->CreateAdditionalSwapChainReal(pPresentationParameters, pSwapChain);
	}
	else
	{
		//TODO - Support multiple swap chains?
		hr = this->CreateAdditionalSwapChainReal(pPresentationParameters, pSwapChain);
	}
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
	HRESULT hr;
	if (forceReal)
	{
		hr = this->ResetReal(pPresentationParameters);
	}
	else
	{
		this->simulateLostDevice = 0;
		bool vsync = true;
		HWND hwnd = NULL;
		D3DDEVICE_CREATION_PARAMETERS creationParameters = {};
		hr = device->GetCreationParameters(&creationParameters);
		if (SUCCEEDED(hr))
		{
			hwnd = creationParameters.hFocusWindow;
		}
		if (pPresentationParameters != NULL)
		{
			if (pPresentationParameters->PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE) { vsync = false; }
			if (hwnd == NULL) { hwnd = pPresentationParameters->hDeviceWindow; }
		}
		D3DPRESENT_PARAMETERS presentParameters;
		D3D9Context2::GetPresentParameters(presentParameters, hwnd, vsync);
		this->DisposeExcludingDevice();
		hr = this->ResetReal(&presentParameters);
		this->Init(this->device);
		this->CreateVirtualDevice(hwnd, creationParameters.BehaviorFlags, pPresentationParameters);

		//TODO - Override present parameters, recreate device, and recreate swap chain object
		//hr = this->ResetReal(pPresentationParameters);
		//hr = this->device->TestCooperativeLevel();
	}
	return hr;
}
HRESULT D3D9DeviceContext::Present_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	HRESULT hr;
	if (forceReal)
	{
		hr = this->PresentReal(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}
	else
	{
		hr = this->childSwapChainContext->Present_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, 0);
	}
	if (hr == D3DERR_DEVICELOST)
	{
		this->DisposeExcludingDevice();
		this->simulateLostDevice = 1;
	}
	return hr;
}
HRESULT D3D9DeviceContext::GetBackBuffer_(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	HRESULT hr;
	if (forceReal)
	{
		hr = GetBackBufferReal(iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}
	else
	{
		if (iSwapChain == 0)
		{
			hr = this->childSwapChainContext->GetBackBuffer_(iBackBuffer, Type, ppBackBuffer);
		}
		else
		{
			hr = GetBackBufferReal(iSwapChain, iBackBuffer, Type, ppBackBuffer);
		}
	}
	return hr;
}
HRESULT D3D9DeviceContext::GetFrontBufferData_(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	HRESULT hr = 0;
	if (forceReal)
	{
		hr = GetFrontBufferDataReal(iSwapChain, pDestSurface);
	}
	else
	{
		if (iSwapChain == 0)
		{
			hr = this->childSwapChainContext->GetFrontBufferData_(pDestSurface);
		}
		else
		{
			hr = GetFrontBufferDataReal(iSwapChain, pDestSurface);
		}
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
	HRESULT hr = 0;
	if (forceReal)
	{
		hr = this->PresentExReal(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	else
	{
		hr = this->childSwapChainContext->Present_(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	if (hr == D3DERR_DEVICELOST)
	{
		DisposeExcludingDevice();
	}
	return hr;
}
HRESULT D3D9DeviceContext::ResetEx_(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
	HRESULT hr;
	if (forceReal)
	{
		hr = this->ResetReal(pPresentationParameters);
	}
	else
	{
		bool vsync = true;
		HWND hwnd = NULL;
		D3DDEVICE_CREATION_PARAMETERS creationParameters = {};
		hr = device->GetCreationParameters(&creationParameters);
		if (SUCCEEDED(hr))
		{
			hwnd = creationParameters.hFocusWindow;
		}
		if (pPresentationParameters != NULL)
		{
			if (pPresentationParameters->PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE) { vsync = false; }
			if (hwnd == NULL) { hwnd = pPresentationParameters->hDeviceWindow; }
		}
		D3DPRESENT_PARAMETERS presentParameters;
		D3D9Context2::GetPresentParameters(presentParameters, hwnd, vsync);
		this->DisposeExcludingDevice();
		hr = this->ResetExReal(&presentParameters, NULL);
		this->Init(this->device);
		this->CreateVirtualDevice(hwnd, creationParameters.BehaviorFlags, pPresentationParameters);

		//TODO - Override present parameters, recreate device, and recreate swap chain object
		//hr = this->ResetReal(pPresentationParameters);
		//hr = this->device->TestCooperativeLevel();
	}
	return hr;
}
HRESULT D3D9DeviceContext::TestCooperativeLevel_()
{
	HRESULT hr;
	hr = this->originalVTable->TestCooperativeLevel(this->device);
	if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
	{
		return hr;
	}
#if SIMULATE_LOST_DEVICE
	if (simulateLostDevice == 1)
	{
		simulateLostDevice++;
		return D3DERR_DEVICELOST;
	}
	if (simulateLostDevice > 1)
	{
		return D3DERR_DEVICENOTRESET;
	}
#endif
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context != NULL && context->device == NULL)
	{
		context->device = This;
		context->device->AddRef();
	}
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context == NULL || context->device == NULL)
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
	if (context != NULL && context->device == NULL)
	{
		context->device = This;
		context->device->AddRef();
	}
	if (context == NULL || context->device == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->ResetEx(This, pPresentationParameters, pFullscreenDisplayMode);
	}
	else
	{
		return context->ResetEx_(pPresentationParameters, pFullscreenDisplayMode);
	}
	/*
	auto context = GetD3D9DeviceContext(This);
	if (context == NULL || context->device == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->ResetEx(This, pPresentationParameters, pFullscreenDisplayMode);
	}
	else
	{
		return context->ResetEx_(pPresentationParameters, pFullscreenDisplayMode);
	}
	*/
}
HRESULT __stdcall D3D9DeviceContext::TestCooperativeLevel(IDirect3DDevice9Ex* This)
{
	auto context = GetD3D9DeviceContext(This);
	if (context != NULL && context->device == NULL)
	{
		context->device = This;
		context->device->AddRef();
	}
	if (context == NULL || context->device == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->TestCooperativeLevel(This);
	}
	else
	{
		return context->TestCooperativeLevel_();
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

