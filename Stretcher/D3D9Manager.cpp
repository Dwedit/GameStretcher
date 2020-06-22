#include "D3D9Manager.h"

D3D9Manager* d3d9Manager;

D3D9Manager::D3D9Manager() :
	d3d9(), device(), swapChain(), window(), windowContext(), d3d9Context(), d3d9DeviceContext(), d3d9SwapChainContext(), realBackBuffer(), realDepthStencilSurface()
{

}

D3D9Manager::~D3D9Manager()
{
	Destroy();
}

void D3D9Manager::Destroy()
{
	if (d3d9Context) { RemoveD3D9Context(d3d9); d3d9Context = NULL; }
	if (d3d9DeviceContext) { RemoveD3D9DeviceContext(device); d3d9DeviceContext = NULL; }
	if (d3d9SwapChainContext) { RemoveD3D9SwapChainContext(swapChain); d3d9SwapChainContext = NULL; }

	SafeRelease(d3d9);
	SafeRelease(device);
	SafeRelease(swapChain);

	window = NULL;
	windowContext = NULL;
	deviceIsEx = false;

	SafeRelease(realBackBuffer);
	SafeRelease(realDepthStencilSurface);
}

IDirect3D9* D3D9Manager::GetD3D9()
{
	return this->d3d9;
}
IDirect3DDevice9* D3D9Manager::GetDevice()
{
	return this->device;
}
IDirect3DSwapChain9* D3D9Manager::GetSwapChain()
{
	return this->swapChain;
}

HRESULT D3D9Manager::CreateD3D9(bool createEx)
{
	if (this->d3d9 != NULL)
	{
		if (this->deviceIsEx != createEx)
		{
			Destroy();
		}
		else
		{
			return 0;
		}
	}
	HRESULT hr = 0;
	HMODULE d3d9Module = LoadLibraryA("d3d9.dll");
	if (createEx)
	{
		typedef HRESULT(__stdcall* Direct3DCreate9Ex_Func)(UINT, IDirect3D9Ex**);
		FARPROC _createEx = GetProcAddress(d3d9Module, "Direct3DCreate9Ex");
		Direct3DCreate9Ex_Func _Direct3DCreate9Ex = (Direct3DCreate9Ex_Func)_createEx;
		if (_Direct3DCreate9Ex)
		{
			hr = _Direct3DCreate9Ex(D3D_SDK_VERSION, (IDirect3D9Ex**)this->d3d9);
		}
		if (this->d3d9 != NULL)
		{
			this->deviceIsEx = true;
		}
	}
	if (this->d3d9 == NULL)
	{
		FARPROC _create = GetProcAddress(d3d9Module, "Direct3DCreate9");
		typedef IDirect3D9* (__stdcall* Direct3DCreate9_Func)(UINT);
		Direct3DCreate9_Func _Direct3DCreate9 = (Direct3DCreate9_Func)_create;
		this->deviceIsEx = false;
		if (_Direct3DCreate9)
		{
			this->d3d9 = _Direct3DCreate9(D3D_SDK_VERSION);
		}
	}
	if (this->d3d9 != NULL)
	{
		this->d3d9Context = GetD3D9Context(this->d3d9);
	}
	return hr;
}

HRESULT D3D9Manager::CreateRealDevice(HWND hwnd, bool createEx, bool vsync)
{
	if (hwnd == NULL)
	{
		WindowContext* windowContext = WindowContext::GetWindowContext();
		if (windowContext != NULL)
		{
			hwnd = windowContext->GetHwnd();
		}
	}
	if (createEx != this->deviceIsEx || vsync != this->deviceIsVsync)
	{
		Destroy();
	}
	CreateD3D9(createEx);
	if (this->device != NULL) return 0;
	if (this->d3d9 == NULL) return E_POINTER;
	HRESULT hr = 0;
	D3DPRESENT_PARAMETERS presentParameters = {};
	HMONITOR currentMonitor = NULL;
	if (hwnd != NULL)
	{
		currentMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	}
	else
	{
		currentMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	}
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoW(currentMonitor, &monitorInfo);
	int screenWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	int screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
	presentParameters.BackBufferWidth = screenWidth;
	presentParameters.BackBufferHeight = screenHeight;
	presentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	presentParameters.BackBufferCount = 1;
	presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	presentParameters.MultiSampleQuality = 0;
	presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
	presentParameters.hDeviceWindow = hwnd;
	presentParameters.Windowed = true;
	presentParameters.EnableAutoDepthStencil = true;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.Flags = 0;
	presentParameters.FullScreen_RefreshRateInHz = 0;
	presentParameters.PresentationInterval = vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	if (createEx)
	{
		hr = this->d3d9Context->CreateDeviceExReal(0, D3DDEVTYPE_HAL, hwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &presentParameters, NULL, (IDirect3DDevice9Ex**)&this->device);
	}
	if (this->device == NULL)
	{
		hr = this->d3d9Context->CreateDeviceReal(0, D3DDEVTYPE_HAL, hwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &presentParameters, &device);
	}
	if (SUCCEEDED(hr) && device != NULL)
	{
		this->d3d9DeviceContext = GetD3D9DeviceContext(this->device);
		hr |= device->SetSoftwareVertexProcessing(false);
		hr |= d3d9DeviceContext->GetBackBufferReal(0, 0, D3DBACKBUFFER_TYPE_MONO, &realBackBuffer);
		hr |= device->GetDepthStencilSurface(&realDepthStencilSurface);
	}
	return hr;
}

HRESULT D3D9Manager::CreateVirtualDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	bool createEx = false;
	bool vsync = false;
	HWND hwnd = hFocusWindow;

	if (pPresentationParameters != NULL)
	{
		if (pPresentationParameters->PresentationInterval != D3DPRESENT_INTERVAL_IMMEDIATE)
		{
			vsync = true;
		}
		if (hwnd == NULL)
		{
			hwnd = pPresentationParameters->hDeviceWindow;
		}
	}
	HRESULT hr = CreateRealDevice(hwnd, createEx, vsync);
	if (SUCCEEDED(hr) && this->device != NULL)
	{
		if (ppReturnedDeviceInterface != NULL)
		{
			hr = this->device->QueryInterface(IID_IDirect3DDevice9, (void**)ppReturnedDeviceInterface);
			return hr;
		}
		else
		{
			return E_POINTER;
		}
	}
	return hr;
}

HRESULT D3D9Manager::CreateVirtualDeviceEx(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
	bool createEx = true;
	bool vsync = false;
	HWND hwnd = hFocusWindow;

	if (pPresentationParameters != NULL)
	{
		if (pPresentationParameters->PresentationInterval != D3DPRESENT_INTERVAL_IMMEDIATE)
		{
			vsync = true;
		}
		if (hwnd == NULL)
		{
			hwnd = pPresentationParameters->hDeviceWindow;
		}
	}
	HRESULT hr = CreateRealDevice(hwnd, createEx, vsync);
	if (SUCCEEDED(hr) && this->device != NULL)
	{
		if (ppReturnedDeviceInterface != NULL)
		{
			hr = this->device->QueryInterface(IID_IDirect3DDevice9Ex, (void**)ppReturnedDeviceInterface);
			return hr;
		}
		else
		{
			return E_POINTER;
		}
	}
	return hr;
}

IDirect3DSurface9* D3D9Manager::GetD3DBackBuffer()
{
	return nullptr;
}

IDirect3DTexture9* D3D9Manager::GetD3DBackBufferTexture()
{
	return nullptr;
}

IDirect3DSurface9* D3D9Manager::GetGDIBackBuffer()
{
	return nullptr;
}

IDirect3DTexture9* D3D9Manager::GetGDIBackBufferTexture()
{
	return nullptr;
}

HRESULT D3D9Manager::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	return E_NOTIMPL;
}
