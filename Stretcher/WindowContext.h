#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include "D3D9Context.h"
#include "Region.h"

#include "TinyMap.h"
#include <string>
using std::string;
using std::wstring;

#include <string.h>
#include "CUpscaler.h"

class WindowContext;
extern TinyMapUnique<HWND, WindowContext> windowMap;
extern TinyMap<HDC, WindowContext*> hdcMap;

static inline bool operator==(const RECT& rect1, const RECT& rect2)
{
	return (0 == memcmp(&rect1, &rect2, sizeof(RECT)));
}

//LRESULT CALLBACK SimpleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//extern bool WindowBeingCreated;

#include <vector>
using std::vector;

class WindowContext
{
	HWND window;
	WNDPROC oldWindowProc;
	D3D9Context d3d9Context;
	ATOM windowClassAtom;
	bool isWindowUnicode;
	bool VirtualizeWindowSize;
	bool IgnoreResizeEvents;
	bool SuspendDrawing;
	bool InsideWndProc;
	bool IsShown;
	bool WantToHook;
	bool BackgroundNeedsErasing;
	bool EnteringFullScreen;
	bool LeavingFullScreen;
	bool IsFullScreen;
	int BorderChanging;
	RECT VirtualWindowRect;
	DWORD VirtualWindowStyle;
	//RECT VirtualClientRect;
	//RECT RealClientRect;
	int VirtualWidth;
	int VirtualHeight;
	int RealWidth;
	int RealHeight;
	int ScaledWidth;
	int ScaledHeight;
	int RealX, RealY;

	RECT LastInvalidatedRectVirtual;
	RECT LastInvalidatedRectClient;

	//ULONG_PTR window_long_extra;
	//IDirect3DDevice9* device;
	//IDirect3DTexture9* texture;
	
	//RECT virtualClientRect;
	//RECT realClientRect;
	float Scale;
	int XOffset, YOffset;
	int LeftPadding, TopPadding, BottomPadding, RightPadding;
	HDC hdc;
	HDC paintDc;
	Region dirtyRegion;
	CUpscaler upscaler;

	WindowContext* parentWindowContext;
	vector<WindowContext*> childWindows;

public:
	//static LRESULT CALLBACK WndProc_Static(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SimpleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool WindowBeingCreated;

	static DWORD CALLBACK ForegroundIdleProc(int code, DWORD wParam, LONG  lParam);

public:
	void Init(HWND hwnd);
	void Release();

	WindowContext() :
		Scale(1.0f),
		window(), oldWindowProc(), windowClassAtom(), isWindowUnicode(), VirtualizeWindowSize(), IgnoreResizeEvents(), SuspendDrawing(), VirtualWidth(), VirtualHeight(), RealWidth(), RealHeight(), ScaledWidth(), ScaledHeight(), RealX(), RealY(), XOffset(), YOffset(), LeftPadding(), TopPadding(), BottomPadding(), RightPadding(), hdc(), paintDc(),
		BorderChanging(), InsideWndProc(), LastInvalidatedRectClient(), LastInvalidatedRectVirtual(), VirtualWindowRect(), VirtualWindowStyle(),
		IsShown(), WantToHook(), BackgroundNeedsErasing(), IsFullScreen(), EnteringFullScreen(), LeavingFullScreen(),
		parentWindowContext()
	{
	}
	~WindowContext()
	{
		Release();
	}

	void ClientToVirtualFloat(float& x, float& y) const;
	void VirtualToClientFloat(float& x, float& y) const;
	void ClientToVirtualRounded(int& x, int& y) const;
	void VirtualToClientRounded(int& x, int& y) const;
	void ClientToVirtualFloor(int& x, int& y) const;
	void VirtualToClientFloor(int& x, int& y) const;
	void ClientToVirtualCeil(int& x, int& y) const;
	void VirtualToClientCeil(int& x, int& y) const;

	void ClampRect(LPRECT lpRect) const;
	void ClampPoint(LPPOINT lpPoint) const;

	RECT RectVirtualToClient(const RECT& rect) const;
	void RectVirtualToClient(LPRECT lpRect) const;
	void RectClientToVirtual(LPRECT lpRect) const;
	//void RectVirtualToClientClamp(LPRECT lpRect) const;
	void RectClientToVirtualClamp(LPRECT lpRect) const;
	RECT RectClientToVirtualClamp(const RECT &rect) const;
	void MouseClientToVirtual(LPPOINT lpPoint) const;
	void MouseVirtualToClient(LPPOINT lpPoint) const;
	void MouseClientToVirtualClamp(LPPOINT lpPoint) const;
	//void MouseVirtualToClientClamp(LPPOINT lpPoint) const;
	void MouseScreenToVirtualScreen(LPPOINT lpPoint) const;
	void MouseVirtualScreenToScreen(LPPOINT lpPoint) const;
	void MouseVirtualScreenToVirtual(LPPOINT lpPoint) const;
	void MouseVirtualToVirtualScreen(LPPOINT lpPoint) const;

	HDC GetDC_();
	int ReleaseDC_();

	bool Redraw();	//TODO

	void AddDirtyRect(const RECT &rect);
	void AddDirtyRect(int x, int y, int width, int height);

	void WindowShown();
	void VirtualizeWindow();
	void UpdateSize(int newWidth, int newHeight);
	bool ChangeWindowResizable(bool resizable);
	void FinishBorderChange();
	BOOL GetWindowRect_(LPRECT rect) const;
	BOOL GetClientRect_(LPRECT rect) const;
	RECT GetClientRect_() const;

	bool IsVirtualized() const;
	LONG_PTR SetWindowLong_(int index, LONG_PTR newLong);
	LONG_PTR GetWindowLong_(int index);

	void UpdateRectVirtualToClient(LPRECT lpRect) const;
	void UpdateRectClientToVirtual(LPRECT lpRect) const;
	BOOL HasUpdateRect() const; //Returns true if an update rect exists
	BOOL GetUpdateRect_(LPRECT rect, BOOL bErase); //rect in virtual coordinates
	BOOL InvalidateRect_(LPCRECT rect, BOOL bErase); //rect in virtual coordinates
	BOOL ValidateRect_(LPCRECT rect); //rect in virtual coordinates

	HDC BeginPaint_(LPPAINTSTRUCT lpPaintStruct);
	BOOL EndPaint_(const PAINTSTRUCT *lpPaintStruct);

	BOOL GetWindowPlacement_(WINDOWPLACEMENT* windowPlacement);
	BOOL MoveWindow_(int x, int y, int width, int height, BOOL repaint);
	BOOL SetWindowPlacement_(const WINDOWPLACEMENT* windowPlacement);
	BOOL SetWindowPos_(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags);
};

extern bool TryHookWindow(HWND hwnd);
