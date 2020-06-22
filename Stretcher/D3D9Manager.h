#pragma once
#include "D3D9Override.h"
#include "WindowContext.h"
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

class D3D9Manager;
class WindowContext;

extern D3D9Manager* d3d9Manager;

class D3D9Manager
{
	IDirect3D9* d3d9;
	IDirect3DDevice9* device;
	IDirect3DSwapChain9* swapChain;
	
	HWND window;
	WindowContext* windowContext;
	
	D3D9Context2 *d3d9Context;
	D3D9DeviceContext* d3d9DeviceContext;
	D3D9SwapChainContext* d3d9SwapChainContext;

	IDirect3DSurface9* realBackBuffer;
	IDirect3DSurface9* realDepthStencilSurface;

	bool deviceIsEx = false;
	bool deviceIsVsync = false;
private:
public:
	D3D9Manager();
	~D3D9Manager();
	void Destroy();
	IDirect3D9 *GetD3D9();
	IDirect3DDevice9* GetDevice();
	IDirect3DSwapChain9* GetSwapChain();
	HRESULT CreateD3D9(bool createEx = false);
	HRESULT CreateRealDevice(HWND hwnd = NULL, bool createEx = false, bool vsync = false);
	HRESULT CreateVirtualDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
	HRESULT CreateVirtualDeviceEx(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
	IDirect3DSurface9* GetD3DBackBuffer();
	IDirect3DTexture9* GetD3DBackBufferTexture();
	IDirect3DSurface9* GetGDIBackBuffer();
	IDirect3DTexture9* GetGDIBackBufferTexture();
	HRESULT Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
};
