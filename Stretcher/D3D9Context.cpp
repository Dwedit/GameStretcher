const int VSYNC = 0;
#define USE_OPTIMUS 0

#if USE_OPTIMUS
extern "C"
{
	_declspec(dllexport)int NvOptimusEnablement = 1;
}
#endif

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
struct IUnknown;
#include <Windows.h>
#include <initguid.h>
#include <d3d9.h>

#include "D3D9Context.h"

/*
Current Design:
	Window Context owns a D3D9 Context Object  (Change this to each window owning a backbuffer texture instad)
	* D3D9 Created  (Always EX if available)
	* D3D9 Device Created  (Always EX if available)
		Default swap chain:
			Swapeffect: Copy
			Backbuffer size equal to screen resolution
			Pixel Format XRGB with no SRGB correction
			Not Lockable
			Single Back Buffer
			Windowed Mode
			D16 Depth Buffer
			No Multisampling
			Attached to Monitor that the window spawned on
		Hardware Vertex Processing  (change this to Mixed to allow apps to request software vertex processing)
	* Texture created
		Dynamic texture (Change this to a render-target texture)
		Not lockable  (Need to make it lockable)
	* Texture sampled and upscaled to 2x scale
	* 2x Texture drawn to Back Buffer and Presented to Screen

Upcoming Design:
	Global D3D9 Context Object
	* Create D3D9 Ex Object
	* Create D3D9 Ex Device Object
		Real swap chain:
			SwapEffect: Copy
			Backbuffer equal to Screen Resolution
			Pixel Format XRGB with no SRGB correction
			Not Lockable
			Single Back Buffer
			Windowed Mode
			D16 Depth Buffer  (change if the upscaler ever needs anything better)
			No Multisampling
			Attached to Window's initial monitor
		Mixed Vertex Processing
			Use Hardware Vertex Processing by default
			If app requests Software or Mixed vertex processing, preserve Software mode selection between calls to the upscaled
	* Game doesn't use D3D9:
		Create texture for backbuffer
			XRGB
			Render Target Texture
			Lockable
			Match intitial window size  (but resize if window size changes, if D3D isn't used, and more than 2 resizes have happened, proceed to match screen resolution)
	* Game uses D3D9:
		Create texture for backbuffer
			Render Target Texture
			Match requested multisampling
			Match requested size
			Match requested pixel format
			If it's RGB/XRGB, has no multisampling, and matches client size, share with the GDI texture and make it lockable
	* Use a "Virtual Swap Chain"
		Virtual swap chain will be owner of GDI backbuffer texture, and D3D backbuffer texture
		When virtual swap chain presents, instead:
			Upscale back buffer texture to real back buffer
			present real back buffer

Reference Counting Design:
	D3D9 Device
		Initial Reference Count
		TODO
*/

template <class T>
static inline void SafeRelease(T*& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}

bool IsWine()
{
	HMODULE ntdllModule = GetModuleHandleA("ntdll.dll");
	return NULL != GetProcAddress(ntdllModule, "wine_get_version");
}

//#define SafeRelease(xxxx) if ((xxxx) != NULL) { (xxxx)->Release(); (xxxx) = NULL; }
//#pragma comment( lib, "d3d9.lib" )
#pragma warning (disable: 26812)

void D3D9Context::GetPresentParameters(HWND mainWindow, D3DPRESENT_PARAMETERS& presentParameters, D3DSWAPEFFECT swapEffect)
{
	HMONITOR currentMonitor = NULL;
	currentMonitor = MonitorFromWindow(mainWindow, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoW(currentMonitor, &monitorInfo);
	int screenWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	int screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	presentParameters = {};
	if (swapEffect == D3DSWAPEFFECT_COPY)
	{
		presentParameters.BackBufferWidth = screenWidth;
		presentParameters.BackBufferHeight = screenHeight;
	}
	presentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	if (swapEffect == D3DSWAPEFFECT_COPY || swapEffect == D3DSWAPEFFECT_DISCARD)
	{
		presentParameters.BackBufferCount = 1;
	}
	else
	{
		presentParameters.BackBufferCount = 2;
	}
	presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	presentParameters.MultiSampleQuality = 0;
	presentParameters.SwapEffect = swapEffect;
	presentParameters.hDeviceWindow = mainWindow;
	presentParameters.Windowed = true;
	presentParameters.EnableAutoDepthStencil = true;
	//presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.Flags = 0;
	presentParameters.FullScreen_RefreshRateInHz = 0;
	if (VSYNC)
	{
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

}

bool D3D9Context::CreateD3D9(HWND mainWindow, D3DSWAPEFFECT swapEffect)
{
	HRESULT hr = 0;

	HMODULE d3d9Module = LoadLibraryA("d3d9.dll");
	FARPROC _createEx = GetProcAddress(d3d9Module, "Direct3DCreate9Ex");
	FARPROC _create = GetProcAddress(d3d9Module, "Direct3DCreate9");

	typedef HRESULT(__stdcall* Direct3DCreate9Ex_Func)(UINT, IDirect3D9Ex**);
	typedef IDirect3D9* (__stdcall* Direct3DCreate9_Func)(UINT);

	Direct3DCreate9Ex_Func _Direct3DCreate9Ex = (Direct3DCreate9Ex_Func)_createEx;
	Direct3DCreate9_Func _Direct3DCreate9 = (Direct3DCreate9_Func)_create;

	if (d3d9 == NULL && d3d9Ex == NULL)
	{
		if (_Direct3DCreate9Ex != NULL)
		{
			hr |= _Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9Ex);
			hr |= d3d9Ex->QueryInterface(IID_IDirect3D9, (void**)&d3d9);
		}
		else if (_Direct3DCreate9 != NULL)
		{
			d3d9 = _Direct3DCreate9(D3D_SDK_VERSION);
		}
	}

	HRESULT result;
	if (deviceEx == NULL && device == NULL)
	{
		D3DPRESENT_PARAMETERS presentParameters = {};
		GetPresentParameters(mainWindow, presentParameters, swapEffect);
		if (d3d9Ex != NULL)
		{
			result = d3d9Ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mainWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, NULL, &deviceEx);
			if (deviceEx)
			{
				deviceEx->QueryInterface(IID_IDirect3DDevice9, (void**)&device);
			}
			if (device)
			{
				result = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
				result = device->GetDepthStencilSurface(&depthStencilSurface);
			}
		}
		else if (d3d9 != NULL)
		{
			result = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mainWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device);
			if (device)
			{
				result = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
				result = device->GetDepthStencilSurface(&depthStencilSurface);
			}
		}
		this->swapEffect = swapEffect;
	}

	if (texture == NULL)
	{
		D3DSURFACE_DESC backBufferDesc = {};
		result = backBuffer->GetDesc(&backBufferDesc);
		result = device->CreateTexture(backBufferDesc.Width, backBufferDesc.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
		if (texture != NULL)
		{
			result = texture->GetSurfaceLevel(0, &textureSurface);
		}
	}
	return (device != NULL);
}

void D3D9Context::Destroy()
{
	SafeRelease(texture);
	SafeRelease(textureSurface);
	SafeRelease(backBuffer);
	SafeRelease(depthStencilSurface);
	SafeRelease(device);
	SafeRelease(deviceEx);
	SafeRelease(d3d9);
	SafeRelease(d3d9Ex);
}
