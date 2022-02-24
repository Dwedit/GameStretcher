#pragma once
#include "D3D9VTable.h"
#include "CUpscaler.h"
#include "Win32Ex.h"

#define SIMULATE_LOST_DEVICE 0

#define LOST_DEVICE_TEST 0
#define NO_DEVICE_HOOK 0

extern CRITICAL_SECTION d3d9CriticalSection;

class CriticalSectionLock
{
	LPCRITICAL_SECTION criticalSection;
public:
	CriticalSectionLock(LPCRITICAL_SECTION criticalSection)
	{
		this->criticalSection = criticalSection;
		EnterCriticalSection(criticalSection);
	}
	~CriticalSectionLock()
	{
		LeaveCriticalSection(criticalSection);
	}
};

class D3D9Context2;
class D3D9DeviceContext;
class D3D9SwapChainContext;
class D3D9SurfaceContext;

extern D3D9Context2* GetD3D9Context(IDirect3D9* key, bool allocate = false);
extern D3D9Context2* GetD3D9Context();
extern void RemoveD3D9Context(IDirect3D9* key);
extern D3D9DeviceContext* GetD3D9DeviceContext(IDirect3DDevice9* key, bool allocate = false);
extern D3D9DeviceContext* GetD3D9DeviceContext();
extern void RemoveD3D9DeviceContext(IDirect3DDevice9* key);
extern D3D9SwapChainContext* GetD3D9SwapChainContext(IDirect3DSwapChain9* key, bool allocate = false);
extern D3D9SwapChainContext* GetD3D9SwapChainContext();
extern void RemoveD3D9SwapChainContext(IDirect3DSwapChain9* key);
extern D3D9SurfaceContext* GetD3D9SurfaceContext(IDirect3DSurface9* key, bool allocate = false);
extern D3D9SurfaceContext* GetD3D9SurfaceContext();
extern void RemoveD3D9SurfaceContext(IDirect3DSurface9* key);
bool SwapChainContextExists(IDirect3DSwapChain9* key);
void RemoveD3D9DisposedObjects();

extern IDirect3D9ExVtbl* GetOriginalVTable(IDirect3D9* d3d9, bool allocate = false);
extern IDirect3DDevice9ExVtbl* GetOriginalVTable(IDirect3DDevice9* device, bool allocate = false);
extern IDirect3DSwapChain9ExVtbl* GetOriginalVTable(IDirect3DSwapChain9* swapChain, bool allocate = false);
extern IDirect3DSurface9Vtbl* GetOriginalVTable(IDirect3DSurface9* surface, bool allocate = false);
//extern IDirect3D9ExVtbl* GetNewVTable(IDirect3D9* d3d9);
//extern IDirect3DDevice9ExVtbl* GetNewVTable(IDirect3DDevice9* device);
//extern IDirect3DSwapChain9ExVtbl* GetNewVTable(IDirect3DSwapChain9* swapChain);
//extern void AssignVTable(IDirect3D9* d3d9, IDirect3D9ExVtbl* vTable);
//extern void AssignVTable(IDirect3DDevice9* d3d9, IDirect3DDevice9ExVtbl* vTable);
//extern void AssignVTable(IDirect3DSwapChain9* d3d9, IDirect3DSwapChain9ExVtbl* vTable);

extern bool GetIsEx(IDirect3D9* d3d9);
extern bool GetIsEx(IDirect3DDevice9* device);
extern bool GetIsEx(IDirect3DSwapChain9* swapChain);

IDirect3D9* CreateAndOverrideDirect3D9();
IDirect3D9Ex* CreateAndOverrideDirect3D9Ex();

class D3D9Context2
{
public:
	int internalRefCount = 0;
	IDirect3D9Ex* d3d9 = NULL;
	IDirect3D9ExVtbl* originalVTable = NULL;
	IDirect3D9ExVtbl* myVTable = NULL;
	bool IsEx = false;
public:
	D3D9Context2(IDirect3D9* d3d9);
	D3D9Context2();
	void Init(IDirect3D9* d3d9);
	~D3D9Context2();
	void Dispose();
	HRESULT CreateDeviceReal(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
	HRESULT CreateDeviceExReal(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
	ULONG ReleaseReal();
	static void GetPresentParameters(D3DPRESENT_PARAMETERS& presentParameters, HWND hwnd, bool vsync);
	HRESULT CreateDevice_(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
	HRESULT CreateDeviceEx_(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
	ULONG Release_();

	static HRESULT __stdcall CreateDevice(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
	static HRESULT __stdcall CreateDeviceEx(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
	static ULONG __stdcall Release(IDirect3D9Ex* This);
};

class D3D9DeviceContext
{
	friend D3D9Context2;
public:
	IDirect3DDevice9Ex* device = NULL;
	IDirect3DDevice9ExVtbl* originalVTable = NULL;
	IDirect3DDevice9ExVtbl* myVTable = NULL;

	D3D9Context2* parent = NULL;
	D3D9SwapChainContext* childSwapChainContext = NULL;
	IDirect3DSwapChain9* realSwapChain = NULL;
	//IDirect3DSurface9* initialRenderTarget = NULL;
	//IDirect3DSurface9* initialDepthStencilSurface = NULL;

	bool forceReal = false;
	int simulateLostDevice = 0;
	bool IsEx = false;
	int internalRefCount = 0;
	int refCountFirstCapture = 0;
	int refCountTrackerCount = 0;
	bool disposing = false;

	D3DDEVICE_CREATION_PARAMETERS originalDeviceCreationParameters = {};
public:
	D3D9DeviceContext();
	static void SetVTable(IDirect3DDevice9ExVtbl* myVTable);
	void SetVTable();
	void Init(IDirect3DDevice9* device);
	~D3D9DeviceContext();
	void Dispose();
	void DisposeExcludingDevice();
	int GetRefCount() const;
	int BeginTrackingRefCount();
	int EndTrackingRefCount();

	HRESULT CreateVirtualDevice(HWND hwnd, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters);

	ULONG Release_();
	HRESULT GetCreationParameters_(D3DDEVICE_CREATION_PARAMETERS* pParameters);
	HRESULT CreateAdditionalSwapChain_(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
	//HRESULT GetSwapChain_(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
	//UINT GetNumberOfSwapChains_();
	HRESULT Reset_(D3DPRESENT_PARAMETERS* pPresentationParameters);
	HRESULT Present_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
	HRESULT GetBackBuffer_(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	HRESULT GetFrontBufferData_(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
	HRESULT BeginStateBlock_();
	HRESULT PresentEx_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
	HRESULT ResetEx_(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode);
	HRESULT TestCooperativeLevel_();

	ULONG ReleaseReal();
	HRESULT GetCreationParametersReal(D3DDEVICE_CREATION_PARAMETERS* pParameters);
	HRESULT CreateAdditionalSwapChainReal(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
	//HRESULT GetSwapChainReal(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
	//UINT GetNumberOfSwapChainsReal();
	HRESULT ResetReal(D3DPRESENT_PARAMETERS* pPresentationParameters);
	HRESULT PresentReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
	HRESULT GetBackBufferReal(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	HRESULT GetFrontBufferDataReal(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
	HRESULT BeginStateBlockReal();
	HRESULT PresentExReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
	HRESULT ResetExReal(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode);

	static ULONG __stdcall Release(IDirect3DDevice9Ex* This);
	static HRESULT __stdcall GetCreationParameters(IDirect3DDevice9Ex* This, D3DDEVICE_CREATION_PARAMETERS* pParameters);
	static HRESULT __stdcall CreateAdditionalSwapChain(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
	//static HRESULT __stdcall GetSwapChain(IDirect3DDevice9Ex* This, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
	//static UINT __stdcall GetNumberOfSwapChains(IDirect3DDevice9Ex* This);
	static HRESULT __stdcall Reset(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters);
	static HRESULT __stdcall Present(IDirect3DDevice9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
	static HRESULT __stdcall GetBackBuffer(IDirect3DDevice9Ex* This, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	static HRESULT __stdcall GetFrontBufferData(IDirect3DDevice9Ex* This, UINT iSwapChain, IDirect3DSurface9* pDestSurface);
	static HRESULT __stdcall BeginStateBlock(IDirect3DDevice9Ex* This);
	static HRESULT __stdcall PresentEx(IDirect3DDevice9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
	static HRESULT __stdcall ResetEx(IDirect3DDevice9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode);
	static HRESULT __stdcall TestCooperativeLevel(IDirect3DDevice9Ex* This);
};

class D3D9SwapChainContext
{
	friend D3D9DeviceContext;
public:
	IDirect3DSwapChain9Ex* realSwapChain = NULL;
	IDirect3DSurface9* realBackBuffer = NULL;
	IDirect3DSurface9* realDepthStencilSurface = NULL;

	IDirect3DTexture9* virtualBackBufferTexture = NULL;
	IDirect3DSurface9* virtualBackBuffer = NULL;
	IDirect3DSurface9* virtualBackBufferMultisampled = NULL;
	IDirect3DSurface9* virtualDepthStencilSurface = NULL;
	CUpscaler upscaler;
	D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_COPY;

	IDirect3DSwapChain9ExVtbl* originalVTable = NULL;
	IDirect3DSwapChain9ExVtbl* myVTable = NULL;
	IDirect3DDevice9* device = NULL;

	D3D9DeviceContext* parent = NULL;
	bool IsEx = false;
	bool isDestroying = false;
	bool insidePresent = false;
	bool forceReal = false;
	int width, height;
	bool lockable = false;
	D3DPRESENT_PARAMETERS presentParameters = {};

	D3D9SurfaceContext* virtualBackBufferContext = NULL;
public:
	D3D9SwapChainContext();
	void SetVTable();
	void Init(IDirect3DSwapChain9* swapChain);
	~D3D9SwapChainContext();
	void Dispose();

	HRESULT CreateVirtualDevice(HWND hwnd, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters);

	HRESULT Present_(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
	HRESULT GetFrontBufferData_(IDirect3DSurface9* pDestSurface);
	HRESULT GetBackBuffer_(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	HRESULT GetPresentParameters_(D3DPRESENT_PARAMETERS* pPresentationParameters);

	HRESULT PresentReal(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
	HRESULT GetFrontBufferDataReal(IDirect3DSurface9* pDestSurface);
	HRESULT GetBackBufferReal(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	HRESULT GetPresentParametersReal(D3DPRESENT_PARAMETERS* pPresentationParameters);

	static HRESULT __stdcall Present(IDirect3DSwapChain9Ex* This, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
	static HRESULT __stdcall GetFrontBufferData(IDirect3DSwapChain9Ex* This, IDirect3DSurface9* pDestSurface);
	static HRESULT __stdcall GetBackBuffer(IDirect3DSwapChain9Ex* This, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	static HRESULT __stdcall GetPresentParameters(IDirect3DSwapChain9Ex* This, D3DPRESENT_PARAMETERS* pPresentationParameters);
};

class D3D9SurfaceContext
{
	friend D3D9DeviceContext;
public:
	IDirect3DSurface9* realSurface = NULL;

	IDirect3DSurface9Vtbl* originalVTable = NULL;
	IDirect3DSurface9Vtbl* myVTable = NULL;
public:
	D3D9SurfaceContext();
	void SetVTable();
	void Init(IDirect3DSurface9* surface);
	~D3D9SurfaceContext();
	void Dispose();

	static HRESULT __stdcall LockRect(IDirect3DSurface9* This, D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags);
	static HRESULT __stdcall UnlockRect(IDirect3DSurface9* This);

	HRESULT __stdcall LockRect_(D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags);
	HRESULT __stdcall UnlockRect_();

	HRESULT __stdcall LockRectReal(D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags);
	HRESULT __stdcall UnlockRectReal();
};
