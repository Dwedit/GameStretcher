#include "WindowContext.h"
#include <math.h>
#include <string>
using std::string;
using std::wstring;

#define NOD3D 0
#define NOVIRTUALIZE 0
#define NOHOOKWINDOW 0
#define PAINT_USE_CLIP_BOX 1

#define REDRAW_AFTER_GETDC 1
#define INVALIDATE_ENTIRE_WINDOW 1

#define DO_NOT_RESET_WNDPROC 0
#define DELAYED_HOOK 0

#include "WindowClassContext.h"
#include "Win32Ex.h"

CachedVectorMap<HWND, std::unique_ptr<WindowContext>> windowMap;
CachedVectorMap<HDC, WindowContext*> hdcMap;

extern WindowContext* lastWindowContext;
extern WindowContext* lastWindowContext2;

#define _DefWindowProc(hwnd, uMsg, wParam, lParam)\
 (isWindowUnicode ? DefWindowProcW((hwnd), (uMsg), (wParam), (lParam))\
 : DefWindowProcA((hwnd), (uMsg), (wParam), (lParam)))

#define _CallWindowProc(proc, hwnd, uMsg, wParam, lParam)\
 (isWindowUnicode ? CallWindowProcW((proc), (hwnd), (uMsg), (wParam), (lParam))\
 : CallWindowProcA((proc), (hwnd), (uMsg), (wParam), (lParam)))

#define _SetWindowLongPtr(hwnd, index, value)\
 (isWindowUnicode ? SetWindowLongPtrW((hwnd), (index), (value)) :\
 SetWindowLongPtrA((hwnd), (index), (value)))

#define _GetWindowLongPtr(hwnd, index)\
 (isWindowUnicode ? GetWindowLongPtrW((hwnd), (index)) :\
 GetWindowLongPtrA((hwnd), (index)))

#define _SetWindowLong(hwnd, index, value)\
 (isWindowUnicode ? SetWindowLongW((hwnd), (index), (value)) :\
 SetWindowLongA((hwnd), (index), (value)))

#define _GetWindowLong(hwnd, index)\
 (isWindowUnicode ? GetWindowLongW((hwnd), (index)) :\
 GetWindowLongA((hwnd), (index)))

void WindowContext::Init(HWND hwnd)
{
	if (window != NULL) Release();
	isWindowUnicode = IsWindowUnicode(hwnd);
	window = hwnd;
	HWND parentWindow = GetParent(hwnd);
	this->parentWindowContext = GetWindowContext(parentWindow);
	if (this->parentWindowContext != NULL)
	{
		this->parentWindowContext->childWindows.push_back(this);
	}

	WNDPROC currentWndProc = (WNDPROC)_GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	if (currentWndProc != DefaultWndProc && currentWndProc != SimpleWndProc)
	{
		oldWindowProc = currentWndProc;
	}
	else
	{
#if USE_CLASS_HOOK
		oldWindowProc = windowClassSet.GetClassWndProc(hwnd);
		if (oldWindowProc == NULL)
		{
			//Should Never happen, but it needs a secondary window proc just in case
			oldWindowProc = DefaultWndProc;
		}
#endif
	}
	_SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)SimpleWndProc);
	
	//Initialize Virtual Client Bounds (For a brand new window, this is zeroes)
	UpdateSize();
}

template <class T>
void RemoveFromVector(std::vector<T>& vec, const T& item)
{
	auto found = std::find(vec.begin(), vec.end(), item);
	if (found == vec.end()) return;
	vec.erase(found, found + 1);
}

void WindowContext::Release()
{
	if (this->parentWindowContext != NULL)
	{
		RemoveFromVector(this->parentWindowContext->childWindows, this);
		this->parentWindowContext = NULL;
	}

	if (this->realDC != NULL)
	{
		ReleaseDC(window, this->realDC);
		this->realDC = NULL;
	}
	HdcRemoveWindow(this);

	if (IsWindow(window) && oldWindowProc != NULL)
	{
		_SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oldWindowProc);
	}
	d3d9Context.Destroy();
	window = NULL;
	oldWindowProc = NULL;
	if (lastWindowContext == this)
	{
		lastWindowContext = NULL;
	}
	if (lastWindowContext2 == this)
	{
		lastWindowContext2 = NULL;
	}
}

WindowContext* WindowContext::Get(HWND hwnd)	//static
{
	return GetWindowContext(hwnd);
}
WindowContext* WindowContext::GetByHdc(HDC hdc)	//static
{
	return hdcMap.Get(hdc);
}
void WindowContext::HdcAdd(HDC hdc, WindowContext* windowContext)	//static
{
	hdcMap.Set(hdc, windowContext);
}
bool WindowContext::HdcRemove(HDC hdc)	//static
{
	return hdcMap.Remove(hdc);
}
bool WindowContext::HdcRemoveWindow(WindowContext* windowContext)	//static
{
	return hdcMap.RemoveValue(windowContext);
}

WindowContext* lastWindowContext = NULL;
WindowContext* lastWindowContext2 = NULL;

WindowContext* WindowContext::GetWindowContext(HWND hwnd)	//static
{
	auto result = windowMap.GetReference(hwnd);
	if (result == NULL) return NULL;
	WindowContext *wc = (*result).get();
	lastWindowContext2 = wc;
	if (wc->IsShown)
	{
		lastWindowContext = wc;
	}
	return wc;
}
WindowContext* WindowContext::GetWindowContext()	//static
{
	if (lastWindowContext == NULL) return lastWindowContext2;
	return lastWindowContext;
}
WindowContext* WindowContext::CreateNewWindowContext(HWND hwnd)	//static
{
	windowMap.Set(hwnd, std::make_unique<WindowContext>());
	WindowContext* windowContext = GetWindowContext(hwnd);
	windowContext->Init(hwnd);
	return windowContext;
}
bool WindowContext::DeleteWindowContext(HWND hwnd)	//static
{
	return windowMap.Remove(hwnd);
}
bool WindowContext::WindowContextExists(HWND hwnd)	//static
{
	return windowMap.ContainsKey(hwnd);
}

inline int Round(float f)
{
	return ((int)(f + 32768.5)) - 32768;
}
inline int Floor(float f)
{
	const float epsilon = 1.0f / 256.0f;
	return ((int)(f + 32768.0f + epsilon)) - 32768;
}
inline int Ceil(float f)
{
	const float epsilon = 1.0f / 256.0f;
	return ((int)(f + 32768.0f + (1.0f - epsilon))) - 32768;
}

void WindowContext::ClientToVirtualFloat(float& x, float& y) const
{
	x -= XOffset;
	y -= YOffset;
	x /= Scale;
	y /= Scale;
}
void WindowContext::VirtualToClientFloat(float& x, float& y) const
{
	x *= Scale;
	y *= Scale;
	x += XOffset;
	y += YOffset;
}
#pragma warning(disable:4244)
void WindowContext::ClientToVirtualRounded(int& x, int& y) const
{
	float xF = x, yF = y;
	ClientToVirtualFloat(xF, yF);
	x = Round(xF);
	y = Round(yF);
}
void WindowContext::VirtualToClientRounded(int& x, int& y) const
{
	float xF = x, yF = y;
	VirtualToClientFloat(xF, yF);
	x = Round(xF);
	y = Round(yF);
}

void WindowContext::ClientToVirtualFloor(int& x, int& y) const
{
	float xF = x, yF = y;
	ClientToVirtualFloat(xF, yF);
	x = Floor(xF);
	y = Floor(yF);
}
void WindowContext::VirtualToClientFloor(int& x, int& y) const
{
	float xF = x, yF = y;
	VirtualToClientFloat(xF, yF);
	x = Floor(xF);
	y = Floor(yF);
}
void WindowContext::ClientToVirtualCeil(int& x, int& y) const
{
	float xF = x, yF = y;
	ClientToVirtualFloat(xF, yF);
	x = Ceil(xF);
	y = Ceil(yF);
}
void WindowContext::VirtualToClientCeil(int& x, int& y) const
{
	float xF = x, yF = y;
	VirtualToClientFloat(xF, yF);
	x = Ceil(xF);
	y = Ceil(yF);
}

void WindowContext::MouseClientToVirtual(LPPOINT lpPoint) const
{
	int& x = reinterpret_cast<int&>(lpPoint->x);
	int& y = reinterpret_cast<int&>(lpPoint->y);
	ClientToVirtualRounded(x, y);
}
void WindowContext::MouseVirtualToClient(LPPOINT lpPoint) const
{
	int& x = reinterpret_cast<int&>(lpPoint->x);
	int& y = reinterpret_cast<int&>(lpPoint->y);
	VirtualToClientRounded(x, y);
}

void WindowContext::MouseClientToVirtualClamp(LPPOINT lpPoint) const
{
	int& x = reinterpret_cast<int&>(lpPoint->x);
	int& y = reinterpret_cast<int&>(lpPoint->y);
	ClientToVirtualRounded(x, y);
	ClampPoint(lpPoint);
}

void WindowContext::MouseScreenToVirtualScreen(LPPOINT lpPoint) const
{
#if !USE_REAL_MOUSE_COORDINATES
	ScreenToClient(window, lpPoint);
	MouseClientToVirtual(lpPoint);
	ClientToScreen(window, lpPoint);
#endif
}
void WindowContext::MouseVirtualScreenToScreen(LPPOINT lpPoint) const
{
#if !USE_REAL_MOUSE_COORDINATES
	ScreenToClient(window, lpPoint);
	MouseVirtualToClient(lpPoint);
	ClientToScreen(window, lpPoint);
#endif
}
void WindowContext::MouseVirtualScreenToVirtual(LPPOINT lpPoint) const
{
#if !USE_REAL_MOUSE_COORDINATES
	ScreenToClient(window, lpPoint);
#else
	ScreenToClient(window, lpPoint);
	MouseClientToVirtual(lpPoint);
#endif
}
void WindowContext::MouseVirtualToVirtualScreen(LPPOINT lpPoint) const
{
#if !USE_REAL_MOUSE_COORDINATES
	ClientToScreen(window, lpPoint);
#else
	MouseVirtualToClient(lpPoint);
	ClientToScreen(window, lpPoint);
#endif
}

LRESULT WindowContext::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//Always handle these
	case WM_NCDESTROY:
	{
		LRESULT result = _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		DeleteWindowContext(hwnd);
		return result;
	}
	case WM_SHOWWINDOW:
	{
		WindowShown();
		break;
	}
	case WM_ERASEBKGND:
	{
		LRESULT result = _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		return result;
	}
	case WM_PAINT:
	{
		int dummy = 0;
		break;
	}
	case WM_SIZE:
	{
		//handle Resize to redraw the window, and rescale the window
		int width = (int)(short)LOWORD(lParam); //client area width
		int height = (int)(short)HIWORD(lParam); //client area height

		if (!IgnoreResizeEvents)
		{
			UpdateSize(width, height);
		}
		if (ResizeHandler != NULL)
		{
			(this->*ResizeHandler)();
		}
		//if (!IgnoreResizeEvents && ResizeHandler == NULL && IsVirtualized())
		//{
		//	DWORD windowStyle = _GetWindowLong(hwnd, GWL_STYLE);
		//	if (!this->IsFullScreen & !(windowStyle & WS_THICKFRAME))
		//	{
		//		int dummy = 0;
		//		MakeWindowResizable();
		//		return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		//	}
		//}


		if (IsVirtualized())
		{
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		else
		{
			return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		}
	}

	} //end switch

	if (IsVirtualized())
	{
		//For using virtualized window size only:
		switch (uMsg)
		{
		//Mouse Coordinate Virtualization
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDBLCLK:
		case WM_MOUSEHOVER:
		{
			if (lParam == -1)
			{
				return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
			}
			if (this->parentWindowContext != NULL)
			{
				//TODO
				int dummy = 0;
			}

			POINT mousePoint{ (short)LOWORD(lParam), (short)HIWORD(lParam) };
			MouseClientToVirtualClamp(&mousePoint);
			lParam = (mousePoint.x & 0xFFFF) | ((mousePoint.y & 0xFFFF) << 16);
			return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		}
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		{
			if (this->parentWindowContext != NULL)
			{
				//TODO
				int dummy = 0;
			}
			POINT mousePoint{ (short)LOWORD(lParam), (short)HIWORD(lParam) };
			MouseScreenToVirtualScreen(&mousePoint);
			lParam = (mousePoint.x & 0xFFFF) | ((mousePoint.y & 0xFFFF) << 16);
			return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		}
		case WM_MOVE:
		{
			#if MOVE_WINDOW_TO_UPPER_LEFT
			if (IsVirtualized() && !IgnoreResizeEvents)
			{
				UpdateSize();
				lParam = (VirtualWindowRect.left & 0xFFFF) | ((VirtualWindowRect.top & 0xFFFF) << 16);
				return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
			}
			#endif
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_MOVING:
		{
			#if MOVE_WINDOW_TO_UPPER_LEFT
			if (IsVirtualized() && !IgnoreResizeEvents)
			{
				UpdateSize();
				RECT rect = VirtualWindowRect;
				lParam = reinterpret_cast<LPARAM>(&rect);
				return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
			}
			#endif
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		//new: hide window style changes from app
		case WM_STYLECHANGED:
		{
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_STYLECHANGING:
		{
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		//Hide move/resize messages from window
		//case WM_MOVING:
		case WM_SIZING:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
		//case WM_GETMINMAXINFO:
		case WM_SYSCOMMAND:
		{
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		//these two are separate from above (triggers constantly)
		case WM_WINDOWPOSCHANGING:
		case WM_WINDOWPOSCHANGED:
		{
			#if MOVE_WINDOW_TO_UPPER_LEFT
			LPWINDOWPOS inputWindowPos = (LPWINDOWPOS)lParam;
			if (IsVirtualized() && !IgnoreResizeEvents && !(inputWindowPos->flags & SWP_NOMOVE))
			{
				UpdateSize();
				WINDOWPOS windowPos = *inputWindowPos;
				windowPos.x = VirtualWindowRect.left;
				windowPos.y = VirtualWindowRect.top;
				windowPos.cx = VirtualWindowRect.right - VirtualWindowRect.left;
				windowPos.cy = VirtualWindowRect.bottom - VirtualWindowRect.top;
				lParam = (LPARAM)&windowPos;
				return _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
			}
			#endif
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_NCCALCSIZE:
		{
			if (MovingWindow)
			{
				return _DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			else
			{
				int dummy = 0;
				return _DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		}
		case WM_KEYDOWN:
		{
			if (wParam == VK_F11)
			{
				DWORD style = GetWindowLong(window, GWL_STYLE);
				DWORD exStyle = GetWindowLong(window, GWL_EXSTYLE);
				if (style & WS_CAPTION)
				{
					IsFullScreen = true;
					
					RECT monitorRect = GetMonitorRect(window);
					RealX = monitorRect.left;
					RealY = monitorRect.top;
					RealWidth = monitorRect.right - monitorRect.left;
					RealHeight = monitorRect.bottom - monitorRect.top;
					MakeWindowBorderless();
				}
				else
				{
					IsFullScreen = false;
					RealX = this->VirtualClientBounds.left;
					RealY = this->VirtualClientBounds.top;
					RealWidth = this->VirtualWidth;
					RealHeight = this->VirtualHeight;
					MakeWindowResizable();
				}
				return 0;
			}
			break;
		}
		case WM_GETMINMAXINFO:
		{
			if (IsFullScreen)
			{
				LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
				RECT monitorRect = GetMonitorRect(window);
				minMaxInfo->ptMaxPosition = { monitorRect.left, monitorRect.top };
				minMaxInfo->ptMaxSize = { monitorRect.right - monitorRect.left, monitorRect.bottom - monitorRect.top };
				return 0;
			}
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		break;
		} //end swtich
	}
	return _CallWindowProc(oldWindowProc,hwnd, uMsg, wParam, lParam);
}

HDC WindowContext::GetDC_()
{
#if REDRAW_AFTER_GETDC
	CompleteDraw();
#endif
	if (this->realDC == NULL)
	{
		this->realDC = ::GetDC(window);
		if (this->realDC != NULL)
		{
			HdcAdd(this->realDC, this);
		}
	}

	return this->realDC;
}

HDC WindowContext::GetDCEx_(HRGN hrgnClip, DWORD flags)
{
	if (flags & DCX_WINDOW)
	{
		return ::GetDCEx(window, hrgnClip, flags);
	}

	//TODO
	return GetDC_();
}

int WindowContext::ReleaseDC_()
{
	CompleteDraw();
	return true;
}

int WindowContext::ReleaseDC_(HDC hdcToRelease)
{
	CompleteDraw();
	return true;
}

BOOL WindowContext::GdiFlush_()
{
	CompleteDraw();
	return true;
}

bool WindowContext::CompleteDraw()
{
	if (this->parentWindowContext != NULL)
	{
		return this->parentWindowContext->CompleteDraw();
	}
	if (this->d3dDC == NULL) return true;

	RECT boundsRect = {};
	bool isEmpty;
	UINT boundsRectMode = GetBoundsRect(this->d3dDC, &boundsRect, DCB_RESET);
	isEmpty = boundsRectMode == DCB_RESET && this->dirtyRegion.IsEmpty();
	if (isEmpty)
	{
		return true;
	}

	//Want a lock here
	auto lock = CreateLock();
	HRESULT hr = 0;
	int result = 0;
	bool okay = true;
	if (d3d9Context.textureSurface != NULL)
	{
		this->lastDC = NULL;
		hr = d3d9Context.textureSurface->ReleaseDC(this->d3dDC);
		okay &= !SUCCEEDED(hr);
		this->d3dDC = NULL;
		//Do we want to sleep 1 here or not?  Would fix old programs that have no CPU speed control
		//Sleep(1);
		okay &= Redraw();
	}
	return okay;
}

HDC WindowContext::GetD3DDC()
{
	//Create a lock before calling this

	if (this->parentWindowContext != NULL)
	{
		return this->parentWindowContext->GetD3DDC();
	}
	if (this->d3dDC != NULL) return this->d3dDC;

	HRESULT hr = 0;
	int result = 0;
	//Create Direct3D if it's not ready
	if (d3d9Context.backBuffer == NULL)
	{
		d3d9Context.CreateD3D9(window);
		upscaler.SetSourceTexture(d3d9Context.texture);
	}
	//Get DC from Direct3D
	if (d3d9Context.textureSurface != NULL)
	{
		hr = d3d9Context.textureSurface->GetDC(&this->d3dDC);
		UINT oldBoundsRectMode = SetBoundsRect(this->d3dDC, NULL, DCB_ENABLE | DCB_RESET);
		return this->d3dDC;
	}
	return NULL;
}

bool WindowContext::Redraw()
{
	if (d3d9Context.device == NULL)
	{
		return false;
	}
	bool okay = true;
	upscaler.SetUpscaleFilter(1);
	okay &= upscaler.Update(this->dirtyRegion);
	this->dirtyRegion.Clear();
	return okay;
}

RECT WindowContext::RectVirtualToClient(const RECT &rect) const
{
	RECT newRect = rect;
	RectVirtualToClient(&newRect);
	return newRect;
}

void WindowContext::RectVirtualToClient(LPRECT lpRect) const
{
	int& left = reinterpret_cast<int&>(lpRect->left);
	int& top = reinterpret_cast<int&>(lpRect->top);
	int& right = reinterpret_cast<int&>(lpRect->right);
	int& bottom = reinterpret_cast<int&>(lpRect->bottom);
	VirtualToClientFloor(left, top);
	VirtualToClientCeil(right, bottom);
}

void WindowContext::RectClientToVirtual(LPRECT lpRect) const
{
	int& left = reinterpret_cast<int&>(lpRect->left);
	int& top = reinterpret_cast<int&>(lpRect->top);
	int& right = reinterpret_cast<int&>(lpRect->right);
	int& bottom = reinterpret_cast<int&>(lpRect->bottom);
	ClientToVirtualFloor(left, top);
	ClientToVirtualCeil(right, bottom);
}

void WindowContext::ClampRect(LPRECT lpRect) const
{
	RECT bounds;
	GetClientRect_(&bounds);

	int& left = reinterpret_cast<int&>(lpRect->left);
	int& top = reinterpret_cast<int&>(lpRect->top);
	int& right = reinterpret_cast<int&>(lpRect->right);
	int& bottom = reinterpret_cast<int&>(lpRect->bottom);
	if (left < bounds.left) left = bounds.left;
	if (right < bounds.left) right = bounds.left;

	if (left > bounds.right) left = bounds.right;
	if (right > bounds.right) right = bounds.right;
	
	if (top < bounds.top) top = bounds.top;
	if (bottom < bounds.top) bottom = bounds.top;

	if (top > bounds.bottom) top = bounds.bottom;
	if (bottom > bounds.bottom) bottom = bounds.bottom;

	if (right < left) right = left;
	if (bottom < top) bottom = top;
}

void WindowContext::ClampPoint(LPPOINT lpPoint) const
{
	RECT bounds;
	GetClientRect_(&bounds);

	int& x = reinterpret_cast<int&>(lpPoint->x);
	int& y = reinterpret_cast<int&>(lpPoint->y);
	if (x < bounds.left) x = bounds.left;
	if (x >= bounds.right) x = bounds.right - 1;
	if (y < bounds.top) y = bounds.top;
	if (y >= bounds.bottom) y = bounds.bottom - 1;
}

void WindowContext::RectClientToVirtualClamp(LPRECT lpRect) const
{
	RectClientToVirtual(lpRect);
	ClampRect(lpRect);
}

RECT WindowContext::RectClientToVirtualClamp(const RECT &rect) const
{
	RECT result = rect;
	RectClientToVirtualClamp(&result);
	return result;
}

void WindowContext::AddDirtyRect()
{
	if (parentWindowContext != NULL)
	{
		parentWindowContext->AddDirtyRect();
		return;
	}

	RECT boundsRect = {};
	bool isEmpty;
	UINT boundsRectMode = GetBoundsRect(this->d3dDC, &boundsRect, DCB_RESET);
	isEmpty = boundsRectMode == DCB_RESET;
	if (!isEmpty)
	{
		LPtoDP(this->d3dDC, (LPPOINT)&boundsRect, 2);
		dirtyRegion.AddRectangle(boundsRect);
	}
}

bool WindowContext::TryHookWindow(HWND hwnd)
{
#if NOHOOKWINDOW
	return false;
#endif
	if (WindowContextExists(hwnd)) return false;

	bool doHook = false;
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	if (style & WS_CAPTION)
	{
		doHook = true;
	}
	if ((style & WS_CHILDWINDOW) && (WindowContextExists(GetParent(hwnd))))
	{
		doHook = true;
	}
	DWORD threadId = 0;
	DWORD processId = 0;
	threadId = GetWindowThreadProcessId(hwnd, &processId);
	if (processId != GetCurrentProcessId())
	{
		doHook = false;
	}
	{
		char buffer[256];
		GetClassNameA(hwnd, &buffer[0], 256);
		if (0 == strcmp(buffer, "#32770"))
		{
			doHook = false;
		}
	}

	if (doHook)
	{
		CreateNewWindowContext(hwnd);
		return true;
	}
	return false;
}

_declspec(noinline)
LRESULT CALLBACK WindowContext::SimpleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowContext* context = GetWindowContext(hwnd);
	if (context != NULL)
	{
		LRESULT result = context->WndProc(hwnd, uMsg, wParam, lParam);
		return result;
	}
	else
	{
		//should never happen
		return DefaultWndProc(hwnd, uMsg, wParam, lParam);
	}
}

bool WindowContext::WindowBeingCreated;

_declspec(noinline)
LRESULT CALLBACK WindowContext::DefaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//normal: use a registered window, and call the handler there.
	WindowContext* context = GetWindowContext(hwnd);
	if (context != NULL)
	{
		bool isWindowUnicode = context->isWindowUnicode;
		//Prevent recursion
		return _DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	//Hook windows really early, before CreateWindow has finished
	if (WindowBeingCreated)
	{
		switch (uMsg)
		{
		case WM_NCCREATE:
		case WM_GETMINMAXINFO:
		case WM_CREATE:
			if (TryHookWindow(hwnd))
			{
				WindowBeingCreated = false;
				//If window is to be hooked, proceed to registered window proc
				context = GetWindowContext(hwnd);
				LRESULT result = SimpleWndProc(hwnd, uMsg, wParam, lParam);
				return result;
			}
			break;
		}
	}
	//Did not want to hook window, restore the normal WNDPROC so it doesn't go through this code again.
	//Is current WND proc already different than This procedure?  (can somehow happen)  Just run that.
	bool isWindowUnicode = IsWindowUnicode(hwnd);
	WNDPROC currentWndProc = (WNDPROC)_GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	if (currentWndProc != DefaultWndProc && currentWndProc != SimpleWndProc)
	{
		return _CallWindowProc(currentWndProc, hwnd, uMsg, wParam, lParam);
	}
#if USE_CLASS_HOOK
	WNDPROC oldWndProc = windowClassSet.GetClassWndProc(hwnd);
	if (oldWndProc != NULL)
	{
#if !DO_NOT_RESET_WNDPROC
		_SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)oldWndProc);
#endif
		return _CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
	}
#endif
	//Emergency, no WndProc found (should never happen), just call DefWndProc instead.
	return _DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowContext::WindowShown()
{
	IsShown = true;
#if NOVIRTUALIZE
	return;
#endif
#if !DELAYED_HOOK
	VirtualizeWindow();
#endif
}
void WindowContext::VirtualizeWindow()
{
	if (!IsVirtualized())
	{
		UpdateSizeNonVirtualized();
		VirtualWindowStyle = GetWindowLong(window, GWL_STYLE);
		VirtualizeWindowSize = true;
		HWND parent = GetParent(window);
		if (parent == NULL)
		{
			MakeWindowResizable();
		}
		else
		{
			MoveResizeChildWindow();
		}
	}
}

void WindowContext::UpdateSize()
{
	bool virtualized = IsVirtualized();
	bool parentVirtualized = parentWindowContext != NULL && parentWindowContext->IsVirtualized();
	HWND parent = GetParent(window);
	if (!virtualized)
	{
		if (!parentVirtualized)
		{
			UpdateSizeNonVirtualized();
		}
		else
		{
			VirtualizeWindow();
		}
	}
	else
	{
		int oldWidth = RealWidth;
		int oldHeight = RealHeight;
		UpdateSizeVirtualized();
		if (oldWidth != RealWidth || oldHeight != RealHeight)
		{
			InvalidateRect(window, NULL, true);
		}
		//TODO: Update size and position of child windows
		MoveResizeChildWindows();
	}
}

void WindowContext::UpdateSizeReal()
{
	RealClientRect = GetRealClientRect();
	RealClientBounds = GetRealClientBounds();
	RealWindowRect = GetRealWindowRect();
	RealWidth = RealClientRect.right;
	RealHeight = RealClientRect.bottom;
	RealX = RealClientBounds.left;
	RealY = RealClientBounds.top;
}

void WindowContext::UpdateSizeVirtual()
{
#if MOVE_WINDOW_TO_UPPER_LEFT
	if (IsVirtualized())
	{
		int newX = RealX + LeftPadding;
		int newY = RealY + TopPadding;

		int leftBorder = RealClientBounds.left - RealWindowRect.left;
		int topBorder = RealClientBounds.top - RealWindowRect.top;
		int rightBorder = RealWindowRect.right - RealClientBounds.right;
		int bottomBorder = RealWindowRect.bottom - RealClientBounds.bottom;

		VirtualClientBounds = { newX, newY, newX + VirtualWidth, newY + VirtualHeight };
		int newLeft = newX - leftBorder;
		int newTop = newY - topBorder;
		if (newTop < 0) newTop = 0;
		if (newLeft < 0) newLeft = 0;

		
		VirtualWindowRect = { newLeft, newTop,
			newLeft + VirtualWidth + leftBorder + rightBorder, newTop + VirtualHeight + topBorder + bottomBorder };

	}
#endif
}

void WindowContext::UpdateSizeScaled()
{
	//Before calling this, ensure that Virtual dimensions and Real dimensions are up to date
	if (VirtualWidth != 0 && VirtualHeight != 0)
	{
		float scaleX = (float)RealWidth / (float)VirtualWidth;
		float scaleY = (float)RealHeight / (float)VirtualHeight;
		Scale = std::min(scaleX, scaleY);
	}
	else
	{
		Scale = 1.0f;
	}
	ScaledWidth = Round(Scale * VirtualWidth);
	ScaledHeight = Round(Scale * VirtualHeight);
	LeftPadding = (RealWidth - ScaledWidth) / 2;
	RightPadding = RealWidth - ScaledWidth - LeftPadding;
	TopPadding = (RealHeight - ScaledHeight) / 2;
	BottomPadding = RealHeight - ScaledHeight - TopPadding;
	XOffset = LeftPadding;
	YOffset = TopPadding;

	ScaledClientRect = { LeftPadding, TopPadding, LeftPadding + ScaledWidth, TopPadding + ScaledHeight };

	upscaler.SetInputRectangle(0, 0, VirtualWidth, VirtualHeight);
	upscaler.SetViewRectangle(XOffset, YOffset, ScaledWidth, ScaledHeight);
}

//Assigns real client bounds and real window rect to all size variables
void WindowContext::UpdateSizeNonVirtualized()
{
	UpdateSizeReal();
	VirtualClientRect = RealClientRect;
	VirtualClientBounds = RealClientBounds;
	VirtualWindowRect = RealWindowRect;
	VirtualWidth = RealWidth;
	VirtualHeight = RealHeight;
	UpdateSizeScaled();
}

void WindowContext::UpdateSizeVirtualized()
{
	UpdateSizeReal();
	UpdateSizeScaled();
	UpdateSizeVirtual();
}

void WindowContext::UpdateSize(int newWidth, int newHeight)
{
	if (RealWidth == newWidth && RealHeight == newHeight)
	{
		return;
	}
	UpdateSize();
}

void WindowContext::MoveResizeChildWindow()
{
	if (this->parentWindowContext == NULL) return;
	if (!this->IsVirtualized() && parentWindowContext->IsVirtualized())
	{
		//calls Virtualize, which will call this function again
		UpdateSize();
		return;
	}
	RECT newRect = this->parentWindowContext->RectVirtualToClient(VirtualClientBounds);
	_MoveWindow(newRect.left, newRect.top, newRect.right - newRect.left, newRect.bottom - newRect.top, true);
}

void WindowContext::MoveResizeChildWindows()
{
	for (size_t i = 0; i < childWindows.size(); i++)
	{
		childWindows[i]->MoveResizeChildWindow();
	}
}

bool WindowContext::MakeWindowResizable()
{
	DWORD windowStyle = _GetWindowLong(window, GWL_STYLE);
	bool hasResizeBorder = windowStyle & WS_THICKFRAME;
	if (!hasResizeBorder)
	{
		IgnoreResizeEvents = true;
		windowStyle |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION;
		_SetWindowLong(window, GWL_STYLE, windowStyle);
		windowStyle = _GetWindowLong(window, GWL_STYLE);
		if (windowStyle & WS_VISIBLE)
		{
			FinishBorderChange();
			IgnoreResizeEvents = false;
		}
		else
		{
			ResizeHandler = &WindowContext::FinishBorderChangeHandler;
		}
		return true;
	}
	return false;
}

bool WindowContext::MakeWindowBorderless()
{
	DWORD windowStyle = _GetWindowLong(window, GWL_STYLE);
	bool hasCaption = windowStyle & WS_CAPTION;
	if (hasCaption)
	{
		IgnoreResizeEvents = true;
		windowStyle &= ~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION);
		_SetWindowLong(window, GWL_STYLE, windowStyle);
		windowStyle = _GetWindowLong(window, GWL_STYLE);
		if (windowStyle & WS_VISIBLE)
		{
			FinishBorderChange();
			IgnoreResizeEvents = false;
		}
		else
		{
			ResizeHandler = &WindowContext::FinishBorderChangeHandler;
		}
		return true;
	}
	return false;
}

void WindowContext::FinishBorderChangeHandler()
{
	ResizeHandler = NULL;
	IgnoreResizeEvents = true;
	FinishBorderChange();
	IgnoreResizeEvents = false;
}

void WindowContext::FinishBorderChange()
{
	const int MAX_ATTEMPTS = 5;
	int numberOfAttempts = MAX_ATTEMPTS;
	int oldRealWidth = this->RealClientBounds.right - this->RealClientBounds.left;
	int oldRealHeight = this->RealClientBounds.bottom - this->RealClientBounds.top;

tryAgain:
	RECT windowRect = GetRealWindowRect();
	RECT clientRect = GetRealClientRect();
	RECT clientRectScreen = GetRealClientBounds();
	
	int currentClientWidth, futureClientWidth, currentClientHeight, futureClientHeight;
	{
		RECT futureClientRect = GetFutureClientRect(windowRect);

		currentClientWidth = clientRect.right - clientRect.left;
		futureClientWidth = futureClientRect.right - futureClientRect.left;
		currentClientHeight = clientRect.bottom - clientRect.top;
		futureClientHeight = futureClientRect.bottom - futureClientRect.top;
	}
	int currentWindowWidth = windowRect.right - windowRect.left;
	int currentWindowHeight = windowRect.bottom - windowRect.top;

	int addedWindowWidth = currentClientWidth - futureClientWidth;
	int addedWindowHeight = currentClientHeight - futureClientHeight;

	int currentLeftBorder = clientRectScreen.left - windowRect.left;
	int currentTopBorder = clientRectScreen.top - windowRect.top;

	int leftBorder = currentLeftBorder + addedWindowWidth / 2;
	int topBorder = currentTopBorder + addedWindowHeight / 2;

	int extraWidth = currentWindowWidth + addedWindowWidth - currentClientWidth;
	int extraHeight = currentWindowHeight + addedWindowHeight - currentClientHeight;

	bool ignoreResizeEvents = IgnoreResizeEvents;
	IgnoreResizeEvents = true;

	DWORD flags = SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER;
	DWORD windowStyle = _GetWindowLong(window, GWL_STYLE);
	HWND windowInsertAfter = NULL;
	bool willHaveBorder = windowStyle & WS_CAPTION;
	if (willHaveBorder)
	{
		flags = SWP_FRAMECHANGED;// | SWP_NOACTIVATE | SWP_NOZORDER;
		//windowInsertAfter = HWND_NOTOPMOST;
	}
	else
	{
		flags = SWP_FRAMECHANGED;
		//windowInsertAfter = HWND_TOPMOST;
	}
	
	_SetWindowPos(windowInsertAfter, RealX - leftBorder, RealY - topBorder, RealWidth + extraWidth, RealHeight + extraHeight, flags);
	::GetWindowRect(window, &windowRect);
	::GetClientRect(window, &clientRect);
	windowStyle = _GetWindowLong(window, GWL_STYLE);
	IgnoreResizeEvents = ignoreResizeEvents;

	if (clientRect.right - clientRect.left != RealWidth || clientRect.bottom - clientRect.top != RealHeight)
	{
		if (numberOfAttempts > 0)
		{
			numberOfAttempts--;
			goto tryAgain;
		}
	}
	if (!willHaveBorder && numberOfAttempts == MAX_ATTEMPTS)
	{
		//force a Try Again to ensure that window has the correct height
		numberOfAttempts--;
		goto tryAgain;
	}

	if (oldRealWidth != RealWidth || oldRealHeight != RealHeight)
	{
		UpdateSize();
	}
	if (willHaveBorder && !(windowStyle & WS_THICKFRAME))
	{
		windowStyle |= WS_THICKFRAME;
		_SetWindowLong(window, GWL_STYLE, windowStyle);
		numberOfAttempts--;
		goto tryAgain;
	}


	RedrawWindow(window, NULL, NULL, RDW_INVALIDATE);  //redraw to fix early white screen for slow-loading games
	SetForegroundWindow(window);
}

BOOL WindowContext::GetWindowRect_(LPRECT rect) const
{
	if (IsVirtualized())
	{
		if (rect)
		{
			if (parentWindowContext != NULL)
			{
				*rect = VirtualWindowRect;
				return true;
			}
			int extraLeft, extraTop, extraRight, extraBottom;
			GetRealNonClientArea(extraLeft, extraTop, extraRight, extraBottom);
			*rect = { this->VirtualClientBounds.left - extraLeft,
				this->VirtualClientBounds.top - extraTop,
				this->VirtualClientBounds.right + extraRight,
				this->VirtualClientBounds.bottom + extraBottom };
		}
		return true;
	}
	return ::GetWindowRect(window, rect);
}

BOOL WindowContext::GetClientRect_(LPRECT rect) const
{
	if (IsVirtualized())
	{
		if (rect)
		{
			*rect = VirtualClientRect;
		}
		return true;
	}
	return ::GetClientRect(window, rect);
}

RECT WindowContext::GetClientRect_() const
{
	if (IsVirtualized())
	{
		return RECT { 0, 0, VirtualWidth, VirtualHeight };
	}
	else
	{
		RECT clientRect;
		::GetClientRect(window, &clientRect);
		return clientRect;
	}
}

bool WindowContext::IsVirtualized() const
{
	return VirtualizeWindowSize;
}

LONG_PTR WindowContext::SetWindowLong_(int index, LONG_PTR newLong)
{
	switch (index)
	{
	case GWL_STYLE:
		if (IsVirtualized())
		{
			DWORD oldWindowLongVirtual = GetWindowLong_(index);
			DWORD oldWindowLongReal = _GetWindowLong(window, index);
			
			DWORD windowLong = oldWindowLongReal & ~(WS_DISABLED | WS_MINIMIZE | WS_VISIBLE);
			windowLong |= (newLong & (WS_DISABLED | WS_MINIMIZE | WS_VISIBLE));
			
			_SetWindowLongPtr(window, index, windowLong);
			return oldWindowLongVirtual;
		}
		break;
	case GWLP_WNDPROC:
		if (true)
		{
			WNDPROC returnValue = oldWindowProc;
			oldWindowProc = (WNDPROC)newLong;
			return (LONG_PTR)returnValue;
		}
		break;
	}
	return _SetWindowLongPtr(window, index, newLong);
}
LONG_PTR WindowContext::GetWindowLong_(int index) const
{
	switch (index)
	{
	case GWL_STYLE:
		if (IsVirtualized())
		{
			LONG_PTR windowLong = _GetWindowLongPtr(window, index);
			windowLong &= WS_DISABLED | WS_MINIMIZE | WS_VISIBLE;
			LONG_PTR virtualWindowStyle = VirtualWindowStyle;
			virtualWindowStyle &= ~(WS_DISABLED | WS_MINIMIZE | WS_VISIBLE);
			windowLong |= virtualWindowStyle;
			return windowLong;
		}
		break;
	case GWLP_WNDPROC:
		if (true)
		{
			return (LONG_PTR)this->oldWindowProc;
		}
		break;
	}
	return _GetWindowLongPtr(window, index);
}
void WindowContext::UpdateRectVirtualToClient(LPRECT lpRect) const
{
	if (*lpRect == LastInvalidatedRectVirtual)
	{
		*lpRect = LastInvalidatedRectReal;
	}
	else
	{
		RectVirtualToClient(lpRect);
	}
}
void WindowContext::UpdateRectClientToVirtual(LPRECT lpRect) const
{
	if (*lpRect == LastInvalidatedRectReal)
	{
		*lpRect = LastInvalidatedRectVirtual;
	}
	else
	{
		RectClientToVirtualClamp(lpRect);
	}
}
BOOL WindowContext::HasUpdateRect() const
{
	return GetUpdateRect(window, NULL, false);
}
BOOL WindowContext::GetUpdateRect_(LPRECT rect, BOOL bErase) //rect in virtual coordinates
{
	BOOL okay;
	okay = GetUpdateRect(window, rect, bErase);
	if (rect)
	{
		UpdateRectClientToVirtual(rect);
	}
	return okay;
}
int WindowContext::GetUpdateRgn_(HRGN hrgn, BOOL bErase) //rect in virtual coordinates
{
	BOOL okay;
	int result = GetUpdateRgn(window, hrgn, bErase);
	TransformRegionRealToVirtual(hrgn);
	return result;
}
BOOL WindowContext::InvalidateRect_(LPCRECT rect, BOOL bErase) //rect in virtual coordinates
{
	const RECT clientRect = GetClientRect_();
	bool entireWindow = false;
	if (rect == NULL)
	{
		rect = &clientRect;
		entireWindow = true;
	}
	LastInvalidatedRectVirtual = *rect;
	LastInvalidatedRectReal = RectVirtualToClient(*rect);

	BOOL okay;
#if INVALIDATE_ENTIRE_WINDOW
	if (*rect == clientRect)
	{
		okay = InvalidateRect(window, NULL, bErase);
	}
	else
	{
		okay = InvalidateRect(window, &LastInvalidatedRectReal, bErase);
	}
#else
	if (entireWindow)
	{
		okay = InvalidateRect(window, NULL, bErase);
	}
	else
	{
		okay = InvalidateRect(window, &LastInvalidatedRectReal, bErase);
	}
#endif
	return okay;
}
BOOL WindowContext::ValidateRect_(LPCRECT rect) //rect in virtual coordinates
{
	bool entireWindow = false;
	const RECT clientRect = GetClientRect_();
	if (rect == NULL)
	{
		rect = &clientRect;
		entireWindow = true;
	}

	RECT updateRectClient = *rect;
	UpdateRectVirtualToClient(&updateRectClient);

	BOOL okay;
#if INVALIDATE_ENTIRE_WINDOW
	if (*rect == clientRect)
	{
		okay = ValidateRect(window, NULL);
	}
	else
	{
		okay = ValidateRect(window, &updateRectClient);
		RECT updateRect2;
		BOOL okay2;
		okay2 = GetUpdateRect_(&updateRect2, false);
		if (updateRect2.right - updateRect2.left == 0 || updateRect2.bottom - updateRect2.top == 0)
		{
			okay = ValidateRect(window, NULL);
		}
	}
#else
	if (entireWindow)
	{
		okay = ValidateRect(window, NULL);
	}
	else
	{
		okay = ValidateRect(window, &updateRectClient);
	}
#endif
	return okay;
}

HDC WindowContext::BeginPaint_(LPPAINTSTRUCT lpPaintStruct)
{
	if (lpPaintStruct == NULL) return NULL;
	HDC oldPaintDC = this->paintDC;
	this->paintDC = BeginPaint(window, lpPaintStruct);
	this->paintDCIsOpen = true;
	this->paintClipRectReal = lpPaintStruct->rcPaint;
	int realWidth = this->paintClipRectReal.right - this->paintClipRectReal.left;
	int realHeight = this->paintClipRectReal.bottom - this->paintClipRectReal.top;
	if (realWidth > 0 || realHeight > 0)
	{
		if (this->paintClipRectReal.left < XOffset || this->paintClipRectReal.right > XOffset + ScaledWidth ||
			this->paintClipRectReal.top < YOffset || this->paintClipRectReal.bottom > YOffset + ScaledHeight)
		{
			this->upscaler.SetBorderDirty();
		}
	}


	UpdateRectClientToVirtual(&(lpPaintStruct->rcPaint));
	this->paintClipRectVirtual = lpPaintStruct->rcPaint;

	if (oldPaintDC != this->paintDC)
	{
		HdcAdd(this->paintDC, this);
	}
#if REDRAW_AFTER_GETDC
	CompleteDraw();
#endif
	return this->paintDC;
}

BOOL WindowContext::EndPaint_(const PAINTSTRUCT* lpPaintStruct)
{
	if (lpPaintStruct == NULL) return false;
	BOOL okay = true;
	PAINTSTRUCT paintStructCopy = *lpPaintStruct;
	UpdateRectVirtualToClient(&(paintStructCopy.rcPaint));
	okay = EndPaint(window, &paintStructCopy);
	this->paintDCIsOpen = false;
	CompleteDraw();
	return okay;
}

BOOL WindowContext::GetWindowPlacement_(WINDOWPLACEMENT* windowPlacement) const
{
	if (!IsVirtualized())
	{
		return GetWindowPlacement(window, windowPlacement);
	}
	//TODO
	return GetWindowPlacement(window, windowPlacement);
}
BOOL WindowContext::MoveWindow_(int x, int y, int width, int height, BOOL repaint)
{
	if (!IsVirtualized())
	{
		return MoveWindow(window, x, y, width, height, repaint);
	}
	int extraLeft, extraTop, extraRight, extraBottom;
	GetRealNonClientArea(extraLeft, extraTop, extraRight, extraBottom);
	int newClientWidth = width - extraLeft - extraRight;
	int newClientHeight = height - extraTop - extraBottom;
	int newClientX = x + extraLeft;
	int newClientY = y + extraTop;
	VirtualWidth = newClientWidth;
	VirtualHeight = newClientHeight;
	VirtualWindowRect = { x, y, x + width, y + height };
	VirtualClientRect = { 0, 0, newClientWidth, newClientHeight };
	VirtualClientBounds = { newClientX, newClientY, newClientX + newClientWidth, newClientY + newClientHeight };
	if (Scale == 1.0)
	{
		RealWidth = newClientWidth;
		RealHeight = newClientHeight;
		FinishBorderChange();
		UpdateSize();
	}
	else
	{
		UpdateSize();
	}
	return true;
}
BOOL WindowContext::SetWindowPlacement_(const WINDOWPLACEMENT* windowPlacement)
{
	if (!IsVirtualized())
	{
		return SetWindowPlacement(window, windowPlacement);
	}
	//TODO
	return SetWindowPlacement(window, windowPlacement);
}
BOOL WindowContext::SetWindowPos_(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags)
{
	if (!IsVirtualized())
	{
		return SetWindowPos(window, hwndInsertAfter, x, y, cx, cy, flags);
	}
	//TODO
	if (!(flags & SWP_NOSIZE))
	{
		MoveWindow_(x, y, cx, cy, false);
	}
	flags |= SWP_NOMOVE | SWP_NOSIZE;
	return SetWindowPos(window, hwndInsertAfter, x, y, cx, cy, flags);
}
//Calls Win32 MoveWindow, indicates that the window is being moved by this class, and not by Win32 internally
BOOL WindowContext::_MoveWindow(int x, int y, int width, int height, BOOL repaint)
{
	bool movingWindow = MovingWindow;
	MovingWindow = true;
	BOOL result = MoveWindow(window, x, y, width, height, repaint);
	MovingWindow = movingWindow;
	return result;
}
//Calls Win32 SetWindowPlacement, indicates that the window is being moved by this class, and not by Win32 internally
BOOL WindowContext::_SetWindowPlacement(const WINDOWPLACEMENT* windowPlacement)
{
	bool movingWindow = MovingWindow;
	MovingWindow = true;
	BOOL result = SetWindowPlacement(window, windowPlacement);
	MovingWindow = movingWindow;
	return result;
}
//Calls Win32 SetWindowPos, indicates that the window is being moved by this class, and not by Win32 internally
BOOL WindowContext::_SetWindowPos(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags)
{
	bool movingWindow = MovingWindow;
	MovingWindow = true;
	BOOL result = SetWindowPos(window, hwndInsertAfter, x, y, cx, cy, flags);
	MovingWindow = movingWindow;
	return result;
}

RECT WindowContext::GetRealClientBounds() const
{
	RECT result;
	HWND parent = GetParent(window);
	GetClientRect(window, &result);
	MapWindowPoints(window, parent, (LPPOINT)&result, 2);
	return result;
}

RECT WindowContext::GetRealWindowRect() const
{
	RECT result;
	GetWindowRect(window, &result);
	return result;
}

RECT WindowContext::GetRealClientRect() const
{
	RECT result;
	GetClientRect(window, &result);
	return result;
}

void WindowContext::GetRealNonClientArea(int& extraLeft, int& extraTop, int& extraRight, int& extraBottom) const
{
	RECT windowRect = GetRealWindowRect();
	RECT windowBounds = GetRealClientBounds();
	extraLeft = windowBounds.left - windowRect.left;
	extraRight = windowRect.right - windowBounds.right;
	extraTop = windowBounds.top - windowRect.top;
	extraBottom = windowRect.bottom - windowBounds.bottom;
}

HDC WindowContext::GetCurrentDC(HDC inputDC)
{
	#if NOD3D
	return inputDC;
	#endif
	//Create a lock before calling this
	HDC d3dDC = GetD3DDC();
	if (d3dDC == NULL) return inputDC;

	bool hasParent = this->parentWindowContext != NULL;

	HDC& lastDC = GetLastDC();
	if (lastDC == inputDC)
	{
		return d3dDC;
	}
	lastDC = inputDC;
	if (inputDC == paintDC)
	{
		if (hasParent)
		{
			//set clip rect to Paint's clip rectangle (moved into client bounds within parent)
			#if PAINT_USE_CLIP_BOX
			RECT clipRect = this->paintClipRectVirtual;
			clipRect.left += VirtualClientBounds.left;
			clipRect.top += VirtualClientBounds.top;
			clipRect.right += VirtualClientBounds.left;
			clipRect.bottom += VirtualClientBounds.top;
			#endif
			SetClipRect(d3dDC, &clipRect);
			//set origin to upper-left corner of client bound within parent
			SetViewportOrgEx(d3dDC, VirtualClientBounds.left, VirtualClientBounds.top, NULL);
		}
		else
		{
			//set clip rect to Paint's clip rectangle
			#if PAINT_USE_CLIP_BOX
			SetClipRect(d3dDC, &this->paintClipRectVirtual);
			#endif
			//set origin to 0,0
			SetViewportOrgEx(d3dDC, 0, 0, NULL);
		}
	}
	else
	{
		if (hasParent)
		{
			//set clip rect to client bounds within parent
			#if PAINT_USE_CLIP_BOX
			SetClipRect(d3dDC, &VirtualClientBounds);
			#endif
			//set origin to upper-left corner of client bounds within parent
			SetViewportOrgEx(d3dDC, VirtualClientBounds.left, VirtualClientBounds.top, NULL);
		}
		else
		{
			//set clip rect to entire window client area
			#if PAINT_USE_CLIP_BOX
			SetClipRect(d3dDC, &VirtualClientRect);
			#endif
			//set origin to 0,0
			SetViewportOrgEx(d3dDC, 0, 0, NULL);
		}
	}
	return d3dDC;
}

HDC& WindowContext::GetLastDC()
{
	if (this->parentWindowContext != NULL)
	{
		return this->parentWindowContext->GetLastDC();
	}
	else
	{
		return this->lastDC;
	}
}

int WindowContext::SetClipRect(HDC hdc, const RECT* clipRect) //static
{
	int result;
	if (clipRect == NULL)
	{
		result = SelectClipRgn(hdc, NULL);
	}
	else
	{
		HRGN clipRgn = CreateRectRgnIndirect(clipRect);
		result = SelectClipRgn(hdc, clipRgn);
		DeleteObject(clipRgn);
	}
	return result;
}

int WindowContext::SetClipRect(const RECT* clipRect)
{
	return SetClipRect(this->d3dDC, clipRect);
}

RECT WindowContext::GetClipRect() const
{
	RECT result = {};
	int returnValue = GetClipBox(this->d3dDC, &result);
	return result;
}

unique_lock<mutex> WindowContext::CreateLock()
{
	return unique_lock<mutex>(this->myMutex);
}

void WindowContext::TransformRegionVirtualToReal(HRGN hrgn) const
{
	if (XOffset == 0 && YOffset == 0 && Scale == 1.0f) return;
	Region region;
	region.AttachHrgn(hrgn);
	region = region.ZoomAndDialate(this->Scale, 0.0f, XOffset, YOffset, XOffset, YOffset, XOffset + ScaledWidth, YOffset + ScaledHeight);
	region.DetachHrgn();
}

HRGN WindowContext::TransformRegionVirtualToRealCopy(HRGN hrgn) const
{
	Region region = hrgn;
	HRGN returnRgn = region.DetachHrgn();
	TransformRegionVirtualToReal(returnRgn);
	return returnRgn;
}

void WindowContext::TransformRegionRealToVirtual(HRGN hrgn) const
{
	if (XOffset == 0 && YOffset == 0 && Scale == 1.0f) return;
	Region region;
	region.AttachHrgn(hrgn);
	if (XOffset != 0 || YOffset != 0)
	{
		region = region.ZoomAndDialate(1.0f, 0.0f, -XOffset, -YOffset, -65536, -65536, 65536, 65536);
	}
	region = region.ZoomAndDialate(1.0f / Scale, 0.0f, 0, 0, 0, 0, VirtualWidth, VirtualHeight);
	region.DetachHrgn();
}

HRGN WindowContext::TransformRegionRealToVirtualCopy(HRGN hrgn) const
{
	Region region = hrgn;
	HRGN returnRgn = region.DetachHrgn();
	TransformRegionRealToVirtual(returnRgn);
	return returnRgn;
}

RECT WindowContext::GetFutureClientRect(const RECT &windowRect)
{
	bool movingWindow = this->MovingWindow;
	this->MovingWindow = true;
	RECT newClientRect = windowRect;
	SendMessage(window, WM_NCCALCSIZE, FALSE, reinterpret_cast<LPARAM>(&newClientRect));
	this->MovingWindow = movingWindow;
	return newClientRect;
}

float WindowContext::GetScale() const
{
	return Scale;
}

BOOL WindowContext::RedrawWindow_(CONST RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags)
{
	if (hrgnUpdate != NULL)
	{
		HRGN transformedRgn = TransformRegionVirtualToRealCopy(hrgnUpdate);
		BOOL result = RedrawWindow(window, lprcUpdate, transformedRgn, flags);
		DeleteObject(transformedRgn);
		return result;
	}
	else if (lprcUpdate != NULL)
	{
		LastInvalidatedRectVirtual = *lprcUpdate;
		LastInvalidatedRectReal = RectVirtualToClient(*lprcUpdate);
#if INVALIDATE_ENTIRE_WINDOW
		BOOL result;
		if (LastInvalidatedRectVirtual == VirtualWindowRect)
		{
			result = RedrawWindow(window, NULL, NULL, flags);
		}
		else
		{
			result = RedrawWindow(window, &LastInvalidatedRectReal, hrgnUpdate, flags);
		}
#else
		BOOL result = RedrawWindow(window, &LastInvalidatedRectReal, hrgnUpdate, flags);
#endif
		return result;
	}
	else
	{
		LastInvalidatedRectVirtual = VirtualClientRect;
		LastInvalidatedRectReal = RealClientRect;
		BOOL result = RedrawWindow(window, NULL, NULL, flags);
		return result;
	}
}

bool WindowContext::PaintDCIsExpired(HDC hdc) const
{
	return hdc != NULL && hdc == paintDC && !paintDCIsOpen;
}

const RECT& WindowContext::GetScaledBounds() const
{
	return this->ScaledClientRect;
}
