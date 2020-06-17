#pragma once

#define NOMINMAX
struct IUnknown;
#include <Windows.h>
#include <d3d9.h>

#pragma warning (disable: 26812)

class D3D9Context_Old
{
	friend class D3D9Context2;
	friend class D3D9DeviceContext;
	friend class D3D9SwapChainContext;
public:
	HWND mainWindow;
	IDirect3D9* d3d9;
	IDirect3D9Ex* d3d9Ex;
	IDirect3DDevice9* device;
	IDirect3DDevice9Ex* deviceEx;
	IDirect3DSurface9* backBuffer;
	IDirect3DSurface9* depthStencilSurface;
	IDirect3DTexture9* texture;
	IDirect3DSurface9* textureSurface;
	D3DSWAPEFFECT swapEffect;
	static void GetPresentParameters(HWND mainWindow, D3DPRESENT_PARAMETERS& presentParameters, D3DSWAPEFFECT swapEffect);
	bool CreateD3D9(HWND mainWindow, D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_COPY);
	D3D9Context_Old() : d3d9(NULL), d3d9Ex(NULL), device(NULL), deviceEx(NULL), backBuffer(NULL), depthStencilSurface(NULL), mainWindow(NULL), texture(NULL), textureSurface(NULL), swapEffect(D3DSWAPEFFECT_COPY) {}
	void Destroy();
	~D3D9Context_Old() { Destroy(); }
};
