#include "D3D9Override.h"
#include "MemoryUnlocker.h"

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
	this->IsEx = GetIsEx(d3d9);
	this->myVTable = ((IDirect3D9Ex_*)d3d9)->lpVtbl;
	{
		MemoryUnlocker unlocker(this->myVTable);
		this->myVTable->CreateDevice = CreateDevice;
		this->myVTable->CreateDeviceEx = CreateDeviceEx;
	}
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
	IDirect3DDevice9* device = *ppReturnedDeviceInterface;
	D3D9DeviceContext* deviceContext = GetD3D9DeviceContext(device);
	deviceContext->parent = this;
	return hr;
}
HRESULT D3D9Context2::CreateDeviceEx_(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
	//TODO: override parameters
	HRESULT hr = CreateDeviceExReal(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
	IDirect3DDevice9Ex* device = *ppReturnedDeviceInterface;
	D3D9DeviceContext* deviceContext = GetD3D9DeviceContext(device);
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

