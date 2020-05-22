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
#include "TinyMap.h"

#include <mutex>
using std::mutex;
using std::unique_lock;

class WindowContext;

static inline bool operator==(const RECT& rect1, const RECT& rect2)
{
	return (0 == memcmp(&rect1, &rect2, sizeof(RECT)));
}

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
	bool IsShown;
	bool EnteringFullScreen;
	bool LeavingFullScreen;
	bool IsFullScreen;
	bool MovingWindow;	//Indicates that Window is being moved by this class, and not by Win32 itself
	typedef void (WindowContext::*VoidMemberFunction)();
	VoidMemberFunction ResizeHandler;

	RECT RealClientRect;		//Real Client Rect (top left is 0,0)
	RECT RealClientBounds;		//Real Client Bounds relative to the screen
	RECT RealWindowRect;		//Real Window Rect relative to the screen
	RECT VirtualClientRect;		//Virtual Client Rect (top left is 0,0)
	RECT VirtualWindowRect;		//Virtual Window Rect relative to the screen
	RECT VirtualClientBounds;	//Virtual Client Bounds relative to the screen
	RECT ScaledClientRect;		//Scaled Rect relative to the Real Client rect (top left is 0,0)

	DWORD VirtualWindowStyle;
	
	int VirtualWidth;
	int VirtualHeight;
	int RealWidth;
	int RealHeight;
	int ScaledWidth;
	int ScaledHeight;
	int RealX, RealY;

	RECT LastInvalidatedRectVirtual;
	RECT LastInvalidatedRectReal;

	float Scale{};
	int XOffset, YOffset;
	int LeftPadding, TopPadding, BottomPadding, RightPadding;
	HDC paintDC;
	HDC realDC;
	HDC lastDC;
	HDC d3dDC;

	RECT paintClipRectReal;
	RECT paintClipRectVirtual;

	Region dirtyRegion;
	CUpscaler upscaler;

	WindowContext* parentWindowContext;
	vector<WindowContext*> childWindows;

	std::mutex myMutex;

public:
	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SimpleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool WindowBeingCreated;

public:
	void Init(HWND hwnd);
	void Release();

	WindowContext() :
		Scale(1.0f),
		window(), oldWindowProc(), windowClassAtom(), isWindowUnicode(), VirtualizeWindowSize(), IgnoreResizeEvents(), SuspendDrawing(), VirtualWidth(), VirtualHeight(), RealWidth(), RealHeight(), ScaledWidth(), ScaledHeight(), RealX(), RealY(), XOffset(), YOffset(), LeftPadding(), TopPadding(), BottomPadding(), RightPadding(), d3dDC(), paintDC(),
		LastInvalidatedRectReal(), LastInvalidatedRectVirtual(), VirtualWindowRect(), VirtualWindowStyle(),
		IsShown(), IsFullScreen(), EnteringFullScreen(), LeavingFullScreen(),
		ResizeHandler(), MovingWindow(), realDC(),
		parentWindowContext()
	{
	}
	~WindowContext()
	{
		Release();
	}

	static WindowContext* Get(HWND hwnd);
	static WindowContext* GetByHdc(HDC hdc);
	static void HdcAdd(HDC hdc, WindowContext* windowContext);
	static bool HdcRemove(HDC hdc);
	static bool HdcRemoveWindow(WindowContext* windowContext);
	static WindowContext* GetWindowContext(HWND hwnd);
	static WindowContext* GetWindowContext();
	static bool TryHookWindow(HWND hwnd);
	static WindowContext* CreateNewWindowContext(HWND hwnd);
	static bool DeleteWindowContext(HWND hwnd);
	static bool WindowContextExists(HWND hwnd);

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
	void RectClientToVirtualClamp(LPRECT lpRect) const;
	RECT RectClientToVirtualClamp(const RECT &rect) const;
	void MouseClientToVirtual(LPPOINT lpPoint) const;
	void MouseVirtualToClient(LPPOINT lpPoint) const;
	void MouseClientToVirtualClamp(LPPOINT lpPoint) const;
	void MouseScreenToVirtualScreen(LPPOINT lpPoint) const;
	void MouseVirtualScreenToScreen(LPPOINT lpPoint) const;
	void MouseVirtualScreenToVirtual(LPPOINT lpPoint) const;
	void MouseVirtualToVirtualScreen(LPPOINT lpPoint) const;

	HDC GetDC_();
	int ReleaseDC_(HDC hdcToRelease);
	int ReleaseDC_();

	bool CompleteDraw();	//TODO
	HDC GetD3DDC();	//TODO
	bool Redraw();	//TODO

	void AddDirtyRect(const RECT &rect);
	void AddDirtyRect(int x, int y, int width, int height);
	void AddDirtyRectWithPen(int x, int y, int width, int height);

	void WindowShown();
	void VirtualizeWindow();
	void UpdateSize(int newWidth, int newHeight);
	void UpdateSize();
	void UpdateSizeReal();		//Updates Real size and position from real window coordinates
	void UpdateSizeNonVirtualized();	//Assigns real client bounds and real window rect to all size variables
	void UpdateSizeScaled();	//Updates Scale factor, and Scaled dimensions.  Ensure that Virtual dimensions and Real dimensions are up to date before calling.
	void UpdateSizeVirtualized();
	void MoveResizeChildWindow();
	void MoveResizeChildWindows();
	bool MakeWindowResizable();
	bool MakeWindowBorderless();
	void FinishBorderChangeHandler();
	void FinishBorderChange();
	BOOL GetWindowRect_(LPRECT rect) const;
	BOOL GetClientRect_(LPRECT rect) const;
	RECT GetClientRect_() const;

	bool IsVirtualized() const;
	LONG_PTR SetWindowLong_(int index, LONG_PTR newLong);
	LONG_PTR GetWindowLong_(int index) const;

	void UpdateRectVirtualToClient(LPRECT lpRect) const;
	void UpdateRectClientToVirtual(LPRECT lpRect) const;
	BOOL HasUpdateRect() const; //Returns true if an update rect exists
	BOOL GetUpdateRect_(LPRECT rect, BOOL bErase); //rect in virtual coordinates
	BOOL InvalidateRect_(LPCRECT rect, BOOL bErase); //rect in virtual coordinates
	BOOL ValidateRect_(LPCRECT rect); //rect in virtual coordinates

	HDC BeginPaint_(LPPAINTSTRUCT lpPaintStruct);
	BOOL EndPaint_(const PAINTSTRUCT *lpPaintStruct);

	BOOL GetWindowPlacement_(WINDOWPLACEMENT* windowPlacement) const;
	BOOL MoveWindow_(int x, int y, int width, int height, BOOL repaint);
	BOOL SetWindowPlacement_(const WINDOWPLACEMENT* windowPlacement);
	BOOL SetWindowPos_(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags);

	//Calls Win32 MoveWindow, indicates that the window is being moved by this class, and not by Win32 internally
	BOOL _MoveWindow(int x, int y, int width, int height, BOOL repaint);
	//Calls Win32 SetWindowPlacement, indicates that the window is being moved by this class, and not by Win32 internally
	BOOL _SetWindowPlacement(const WINDOWPLACEMENT* windowPlacement);
	//Calls Win32 SetWindowPos, indicates that the window is being moved by this class, and not by Win32 internally
	BOOL _SetWindowPos(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags);

	RECT GetRealClientBounds() const;
	RECT GetRealWindowRect() const;
	RECT GetRealClientRect() const;

	void GetRealNonClientArea(int& left, int& top, int& right, int& bottom) const;

	HDC GetCurrentDC(HDC inputDC);
	HDC& GetLastDC();

	static int SetClipRect(HDC hdc, const RECT* rect);

	unique_lock<mutex> CreateLock();
};
