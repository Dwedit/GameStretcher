#include "D3D9Override.h"
#include "TinyMap.h"
#include <memory>

#include "D3D9Context.h"

using std::unique_ptr;
using std::make_unique;

CachedVectorMap<IDirect3D9*, unique_ptr<D3D9Context2>> d3d9ContextMap;
CachedVectorMap<IDirect3DDevice9*, unique_ptr<D3D9DeviceContext>> d3d9DeviceContextMap;
CachedVectorMap<IDirect3DSwapChain9*, unique_ptr<D3D9SwapChainContext>> d3d9SwapChainContextMap;

CachedVectorMap<IDirect3D9ExVtbl*, IDirect3D9ExVtbl*> d3d9VTableMap;
CachedVectorMap<IDirect3DDevice9ExVtbl*, IDirect3DDevice9ExVtbl*> d3d9DeviceVTableMap;
CachedVectorMap<IDirect3DSwapChain9ExVtbl*, IDirect3DSwapChain9ExVtbl*> d3d9SwapChainVTableMap;

D3D9Context2* GetD3D9Context(IDirect3D9* key)
{
	//Given a D3D9 object, create or find an existing D3D9 Context
	auto &map = d3d9ContextMap;
	typedef std::remove_reference<decltype(map)>::type::ValueType::element_type TValue;

	auto *ref = map.GetReference(key);
	if (ref != NULL) { return ref->get(); }
	else
	{
		map.Set(key, make_unique<TValue>());
		ref = map.GetReference(key);
		ref->get()->Init(key);
		return ref->get();
	}
}
D3D9Context2* GetD3D9Context()
{
	//Gets the last used D3D9 Context
	auto* ref = d3d9ContextMap.GetMostRecentValue();
	if (ref == NULL) return NULL;
	return ref->get();
}
void RemoveD3D9Context(IDirect3D9* key)
{
	//Deletes a D3D9 Context
	d3d9ContextMap.Remove(key);
}
D3D9DeviceContext* GetD3D9DeviceContext(IDirect3DDevice9* key)
{
	auto& map = d3d9DeviceContextMap;
	typedef std::remove_reference<decltype(map)>::type::ValueType::element_type TValue;

	//Given a D3D9 Device object, create or find an existing D3D9 Device Context
	auto* ref = map.GetReference(key);
	if (ref != NULL) { return ref->get(); }
	else
	{
		map.Set(key, make_unique<TValue>());
		ref = map.GetReference(key);
		ref->get()->Init(key);
		return ref->get();
	}
}
D3D9DeviceContext* GetD3D9DeviceContext()
{
	//Gets the last used D3D9 Device Context
	auto* ref = d3d9DeviceContextMap.GetMostRecentValue();
	if (ref == NULL) return NULL;
	return ref->get();
}
void RemoveD3D9DeviceContext(IDirect3DDevice9* key)
{
	//Deletes a D3D9 Device Context
	d3d9DeviceContextMap.Remove(key);
}
D3D9SwapChainContext* GetD3D9SwapChainContext(IDirect3DSwapChain9* key)
{
	auto& map = d3d9SwapChainContextMap;
	typedef std::remove_reference<decltype(map)>::type::ValueType::element_type TValue;

	//Given a D3D9 Swapchain object, create or find an existing D3D9 Swapchain Context
	auto* ref = map.GetReference(key);
	if (ref != NULL) { return ref->get(); }
	else
	{
		map.Set(key, make_unique<TValue>());
		ref = map.GetReference(key);
		ref->get()->Init(key);
		return ref->get();
	}
}
D3D9SwapChainContext* GetD3D9SwapChainContext()
{
	//Gets the last used D3D9 Swapchain Context
	auto* ref = d3d9SwapChainContextMap.GetMostRecentValue();
	if (ref == NULL) return NULL;
	return ref->get();
}
void RemoveD3D9SwapChainContext(IDirect3DSwapChain9* key)
{
	//Deletes a D3D9 Swapchain Context
	d3d9SwapChainContextMap.Remove(key);
}

IDirect3D9ExVtbl* GetOriginalVTable(IDirect3D9* d3d9)
{
	//Given a D3D9 object that has a modified VTable, return the original VTable
	auto &map = d3d9VTableMap;
	IDirect3D9Ex_* obj = (IDirect3D9Ex_*)d3d9;
	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Find the key that was associated with that VTable
	auto key = map.GetKey(inputVTable);
	//If nothing was found, return the original object's VTable
	if (key == NULL) return inputVTable;
	//Return the VTable that was found
	return key;
}
IDirect3DDevice9ExVtbl* GetOriginalVTable(IDirect3DDevice9* device)
{
	//Given a D3D9 Device object that has a modified VTable, return the original VTable
	auto& map = d3d9DeviceVTableMap;
	IDirect3DDevice9Ex_* obj = (IDirect3DDevice9Ex_*)device;
	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Find the key that was associated with that VTable
	auto key = map.GetKey(inputVTable);
	//If nothing was found, return the original object's VTable
	if (key == NULL) return inputVTable;
	//Return the VTable that was found
	return key;
}
IDirect3DSwapChain9ExVtbl* GetOriginalVTable(IDirect3DSwapChain9* swapChain)
{
	//Given a D3D9 Swap chain object that has a modified VTable, return the original VTable
	auto& map = d3d9SwapChainVTableMap;
	IDirect3DSwapChain9Ex_* obj = (IDirect3DSwapChain9Ex_*)swapChain;
	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Find the key that was associated with that VTable
	auto key = map.GetKey(inputVTable);
	//If nothing was found, return the original object's VTable
	if (key == NULL) return inputVTable;
	//Return the VTable that was found
	return key;
}

IDirect3D9ExVtbl* GetNewVTable(IDirect3D9* d3d9)
{
	//Given a D3D9 object which has the original VTable, allocate and return a new VTable, or return a previously found VTable
	auto& map = d3d9VTableMap;
	IDirect3D9Ex_* obj = (IDirect3D9Ex_*)d3d9;

	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Is this VTable already hooked, with a reference back to the original?
	auto key = map.GetKey(inputVTable);
	if (key != NULL)
	{
		//return the original object
		return inputVTable;
	}
	//Is this VTable already hooked, but we provided the original VTable?
	auto value = map.Get(inputVTable);
	if (value != NULL)
	{
		return value;
	}
	//allocate a new object
	typedef std::remove_pointer<decltype(inputVTable)>::type TValue;
	auto newVTable = new TValue(*inputVTable);
	map.Set(inputVTable, newVTable);
	return newVTable;
}
IDirect3DDevice9ExVtbl* GetNewVTable(IDirect3DDevice9* device)
{
	//Given a D3D9 Device object which has the original VTable, allocate and return a new VTable, or return a previously found VTable
	auto& map = d3d9DeviceVTableMap;
	IDirect3DDevice9Ex_* obj = (IDirect3DDevice9Ex_*)device;

	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Is this VTable already hooked, with a reference back to the original?
	auto key = map.GetKey(inputVTable);
	if (key != NULL)
	{
		//return the original object
		return inputVTable;
	}
	//Is this VTable already hooked, but we provided the original VTable?
	auto value = map.Get(inputVTable);
	if (value != NULL)
	{
		return value;
	}
	//allocate a new object
	typedef std::remove_pointer<decltype(inputVTable)>::type TValue;
	auto newVTable = new TValue(*inputVTable);
	map.Set(inputVTable, newVTable);
	return newVTable;
}
IDirect3DSwapChain9ExVtbl* GetNewVTable(IDirect3DSwapChain9* swapChain)
{
	//Given a D3D9 Swapchain object which has the original VTable, allocate and return a new VTable, or return a previously found VTable
	auto& map = d3d9SwapChainVTableMap;
	IDirect3DSwapChain9Ex_* obj = (IDirect3DSwapChain9Ex_*)swapChain;

	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Is this VTable already hooked, with a reference back to the original?
	auto key = map.GetKey(inputVTable);
	if (key != NULL)
	{
		//return the original object
		return inputVTable;
	}
	//Is this VTable already hooked, but we provided the original VTable?
	auto value = map.Get(inputVTable);
	if (value != NULL)
	{
		return value;
	}
	//allocate a new object
	typedef std::remove_pointer<decltype(inputVTable)>::type TValue;
	auto newVTable = new TValue(*inputVTable);
	map.Set(inputVTable, newVTable);
	return newVTable;
}

template <class TComObject>
void SafeRelease(TComObject *&obj)
{
	if (obj != NULL)
	{
		obj->Release();
		obj = NULL;
	}
}

bool GetIsEx(IDirect3D9* d3d9)
{
	HRESULT hr = 0;
	IDirect3D9Ex* d3d9Ex = NULL;
	hr = d3d9->QueryInterface(IID_IDirect3D9Ex, (void**)&d3d9Ex);
	if (SUCCEEDED(hr) && d3d9Ex != NULL)
	{
		SafeRelease(d3d9Ex);
		return true;
	}
	return false;
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

D3D9Context2::D3D9Context2(IDirect3D9* d3d9)
{
	Init(d3d9);
}
D3D9Context2::D3D9Context2()
{

}

void D3D9Context2::Init(IDirect3D9* d3d9)
{
	this->d3d9 = d3d9;
	this->d3d9->AddRef();
	this->originalVTable = GetOriginalVTable(d3d9);
	this->myVTable = GetNewVTable(d3d9);
	this->IsEx = GetIsEx(d3d9);
	this->myVTable->CreateDevice = CreateDevice;
	this->myVTable->CreateDeviceEx = CreateDeviceEx;
}
D3D9Context2::~D3D9Context2()
{
	Destroy();
}
void D3D9Context2::Destroy()
{
	SafeRelease(this->d3d9);
}
HRESULT D3D9Context2::CreateDeviceReal(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	return this->originalVTable->CreateDevice(static_cast<IDirect3D9Ex*>(d3d9), Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
}
HRESULT D3D9Context2::CreateDeviceExReal(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
	return this->originalVTable->CreateDeviceEx(static_cast<IDirect3D9Ex*>(d3d9), Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
}
HRESULT D3D9Context2::CreateDevice_(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	//TODO: override parameters


	HRESULT hr = CreateDeviceReal(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	D3D9DeviceContext *deviceContext = GetD3D9DeviceContext(*ppReturnedDeviceInterface);
	deviceContext->parent = this;
	return hr;
}
HRESULT D3D9Context2::CreateDeviceEx_(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
	//TODO: override parameters
	HRESULT hr = CreateDeviceExReal(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
	D3D9DeviceContext* deviceContext = GetD3D9DeviceContext(*ppReturnedDeviceInterface);
	deviceContext->parent = this;
	return hr;
}
HRESULT __stdcall D3D9Context2::CreateDevice(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)	//static
{
	auto context = GetD3D9Context(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->CreateDevice(This, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	}
	else
	{
		return context->CreateDevice_(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	}
}
HRESULT __stdcall D3D9Context2::CreateDeviceEx(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)	//static
{
	auto context = GetD3D9Context(This);
	if (context == NULL)
	{
		auto vtable = GetOriginalVTable(This);
		return vtable->CreateDeviceEx(This, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
	}
	else
	{
		return context->CreateDeviceEx_(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
	}
}

D3D9DeviceContext::D3D9DeviceContext(IDirect3DDevice9* device)
{
	Init(device);
}
D3D9DeviceContext::D3D9DeviceContext()
{

}
void D3D9DeviceContext::Init(IDirect3DDevice9* device)
{
	this->device = (IDirect3DDevice9Ex*)device;
	this->device->AddRef();
	this->originalVTable = GetOriginalVTable(device);
	this->myVTable = GetNewVTable(device);
	this->IsEx = GetIsEx(device);
	this->myVTable->Release = Release;
	this->myVTable->GetCreationParameters = GetCreationParameters;
	this->myVTable->CreateAdditionalSwapChain = CreateAdditionalSwapChain;
	this->myVTable->Reset = Reset;
	this->myVTable->Present = Present;
	this->myVTable->GetBackBuffer = GetBackBuffer;
	this->myVTable->GetFrontBufferData = GetFrontBufferData;
	this->myVTable->PresentEx = PresentEx;
	this->myVTable->ResetEx = ResetEx;

	HRESULT hr = 0;
	hr = device->GetSwapChain(0, &realSwapChain);
	hr = device->GetDepthStencilSurface(&initialDepthStencilSurface);
	this->childSwapChainContext = GetD3D9SwapChainContext(realSwapChain);
}
D3D9DeviceContext::~D3D9DeviceContext()
{
	Destroy();
}
void D3D9DeviceContext::Destroy()
{
	SafeRelease(this->initialDepthStencilSurface);
	SafeRelease(this->realSwapChain);
	SafeRelease(this->device);
	this->childSwapChainContext->Destroy();
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
HRESULT D3D9DeviceContext::PresentExReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	HRESULT hr = originalVTable->PresentEx(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	return hr;
}
HRESULT D3D9DeviceContext::ResetExReal(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
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
