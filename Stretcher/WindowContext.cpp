#include "WindowContext.h"
#include <math.h>
#include <string>
using std::string;
using std::wstring;

#define NOD3D 0
#define NOVIRTUALIZE 0
#define NOHOOKWINDOW 0
#define PAINT_USE_CLIP_BOX 0
#define NO_PAINT_HOOK 0
#define USE_BEGINPAINT 1

#define DO_NOT_RESET_WNDPROC 0
#define DELAYED_HOOK 0
//#define USE_IDLE_HOOK 0

#include "WindowClassContext.h"

TinyMapUnique<HWND, WindowContext> windowMap;
TinyMap<HDC, WindowContext*> hdcMap;

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

/*
LRESULT CALLBACK WindowContext::WndProc_Static(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowContext* context = GetWindowContext(hwnd);
	if (context != NULL)
	{
		return context->WndProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		//emergency code
		WNDPROC originalWndProc = windowClassSet.GetOriginalWndProc(hwnd);
		if (originalWndProc != NULL)
		{
			return _CallWindowProc(originalWndProc, hwnd, uMsg, wParam, lParam);
		}
		return _DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
*/

void WindowContext::Init(HWND hwnd)
{
	if (window != NULL) Release();
	isWindowUnicode = IsWindowUnicode(hwnd);
	window = hwnd;
	HWND parentWindow = GetParent(hwnd);
	if (WindowContextExists(parentWindow))
	{
		this->parentWindowContext = GetWindowContext(parentWindow);
		this->parentWindowContext->childWindows.push_back(this);
	}

	WNDPROC currentWndProc = (WNDPROC)_GetWindowLongPtr(hwnd, GWL_WNDPROC);
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
	_SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)SimpleWndProc);
/*
#if USE_IDLE_HOOK
	SetWindowsHookExA(WH_FOREGROUNDIDLE, (HOOKPROC)ForegroundIdleProc, NULL, GetCurrentThreadId());
#endif
*/
	if (parentWindow == NULL)
	{
		SetForegroundWindow(hwnd);
	}
}

WindowContext* WindowContext::Get(HWND hwnd)	//static
{
	return windowMap.Get(hwnd);
}
WindowContext* WindowContext::GetByHdc(HDC hdc)	//static
{
	return hdcMap.Get(hdc);
}
WindowContext* WindowContext::GetWindowContext(HWND hwnd)	//static
{
	return windowMap.Get(hwnd);
}
WindowContext* WindowContext::GetWindowContext()	//static
{
	return windowMap.GetFirstValue();
}
WindowContext* WindowContext::CreateNewWindowContext(HWND hwnd)	//static
{
	windowMap.AddNew(hwnd);
	WindowContext* windowContext = windowMap.Get(hwnd);
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


template <class T>
void RemoveFromVector(std::vector<T> vec, const T& item)
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

	if (IsWindow(window) && oldWindowProc != NULL)
	{
		_SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)oldWindowProc);
	}
	d3d9Context.Destroy();
	window = NULL;
	oldWindowProc = NULL;
	//if (texture != NULL)
	//{
	//	texture->Release();
	//	texture = NULL;
	//}
}

/*
DWORD CALLBACK WindowContext::ForegroundIdleProc(int code, DWORD wParam, LONG lParam)
{
	if (code == HC_ACTION)
	{
		WindowContext* context = GetWindowContext();
		if (context->WantToHook)
		{
			context->WantToHook = false;
			context->VirtualizeWindow();
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}
*/

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
//void WindowContext::MouseVirtualToClientClamp(LPPOINT lpPoint) const
//{
//	int& x = reinterpret_cast<int&>(lpPoint->x);
//	int& y = reinterpret_cast<int&>(lpPoint->y);
//	VirtualToClientRounded(x, y);
//}

void WindowContext::MouseScreenToVirtualScreen(LPPOINT lpPoint) const
{
	ScreenToClient(window, lpPoint);
	MouseClientToVirtual(lpPoint);
	ClientToScreen(window, lpPoint);
}
void WindowContext::MouseVirtualScreenToScreen(LPPOINT lpPoint) const
{
	ScreenToClient(window, lpPoint);
	MouseVirtualToClient(lpPoint);
	ClientToScreen(window, lpPoint);
}
void WindowContext::MouseVirtualScreenToVirtual(LPPOINT lpPoint) const
{
	ScreenToClient(window, lpPoint);
}
void WindowContext::MouseVirtualToVirtualScreen(LPPOINT lpPoint) const
{
	ClientToScreen(window, lpPoint);
}

//void WindowContext::MouseScreenToVirtual(LPPOINT lpPoint)
//{
//	ScreenToClient(window, lpPoint);
//	MouseClientToVirtual(lpPoint);
//}
//void WindowContext::MouseVirtualToScreen(LPPOINT lpPoint)
//{
//	MouseVirtualToClient(lpPoint);
//	ClientToScreen(window, lpPoint);
//}
/*


		ScreenToClient(hwnd, &mousePoint);
		MouseClientToVirtual(&mousePoint);
		ClientToScreen(hwnd, &mousePoint);

*/

//#undef _CallWindowProc
//#define _CallWindowProc(a,b,c,d,e) (isWindowUnicode ? CallWindowProcW((a),(b),(c),(d),(e)) : CallWindowProcA((a),(b),(c),(d),(e)))

static RECT GetMonitorRect(HWND window)
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoA(monitor, &monitorInfo);
	return monitorInfo.rcMonitor;
}
static RECT GetMonitorWorkAreaRect(HWND window)
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = {};
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoA(monitor, &monitorInfo);
	return monitorInfo.rcMonitor;
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
		//if (result == 0)
		//{
		//	BackgroundNeedsErasing = true;
		//}
		return result;
	}
	case WM_PAINT:
	{
		int dummy = 0;
		break;
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
		//Hide move/resize messages from window
		case WM_MOVING:
		case WM_MOVE:
		case WM_SIZING:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
		//case WM_GETMINMAXINFO:
		case WM_NCCALCSIZE:
		case WM_SYSCOMMAND:
		{
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		//these two are separate from above (triggers constantly)
		case WM_WINDOWPOSCHANGING:
		case WM_WINDOWPOSCHANGED:
		{
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_SIZE:
		{
			//TODO: handle Resize to redraw the window, and rescale the window
			int width = (int)(short)LOWORD(lParam); //client area width
			int height = (int)(short)HIWORD(lParam); //client area height
			
			if (BorderChanging == 1)
			{
				FinishBorderChange();
			}
			else if (BorderChanging == 0)
			{
				UpdateSize(width, height);
			}
			return _DefWindowProc(hwnd, uMsg, wParam, lParam);
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
					//remove caption
					style &= ~(WS_CAPTION | WS_THICKFRAME | WS_BORDER | WS_DLGFRAME);
					exStyle &= ~(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE);
					SetWindowLong(window, GWL_EXSTYLE, exStyle);
					SetWindowLong(window, GWL_STYLE, style);

					RECT monitorRect = GetMonitorRect(window);
					//SetWindowPos(NULL, HWND_TOPMOST, monitorRect.left, monitorRect.top, monitorRect.right - monitorRect.left, monitorRect.bottom - monitorRect.top, 0);
					//SetForegroundWindow(window);
					ShowWindow(window, SW_MAXIMIZE);
					//SetWindowPos(NULL, HWND_TOPMOST, 0, 0, 1919, 1080, 0);
					//SetWindowPos(NULL, HWND_TOPMOST, 0, 0, 1920, 1080, 0);
					//SetWindowPos(NULL, HWND_TOPMOST, 0, 0, 1920, 1080, 0);
				}
				else
				{
					IsFullScreen = false;
					//add caption
					style |= (WS_CAPTION | WS_THICKFRAME);
					style &= ~WS_MAXIMIZE;
					//SetWindowPos(NULL, HWND_NOTOPMOST, 0, 0, 1920, 1080, 0);
					SetWindowLong(window, GWL_STYLE, style);
					//ShowWindow(window, SW_MAXIMIZE);
					//SetWindowPos(NULL, HWND_NOTOPMOST, 0, 0, 1919, 1080, 0);
					//SetWindowPos(NULL, HWND_NOTOPMOST, 0, 0, 1920, 1080, 0);
					//SetWindowPos(NULL, HWND_NOTOPMOST, 0, 0, 1920, 1080, 0);
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
	/*

	switch (uMsg)
	{
	case WM_NCDESTROY:
	{
		LRESULT result = _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		DeleteWindowContext(hwnd);
		return result;
	}
	case WM_MOVING:
	{
		LPRECT rect = (LPRECT)lParam;
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_MOVE:
	{
		int xPos = (int)(short)LOWORD(lParam);
		int yPos = (int)(short)HIWORD(lParam);
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_SIZING:
	{
		int sizingEdge = wParam;
		LPRECT rect = (LPRECT)lParam;
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_SIZE:
	{
		int resizeType = wParam;
		int width = (int)(short)LOWORD(lParam); //client area width
		int height = (int)(short)HIWORD(lParam); //client area height
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_WINDOWPOSCHANGING:
	{
		LPWINDOWPOS windowPos = (LPWINDOWPOS)lParam;
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_WINDOWPOSCHANGED:
	{
		LPWINDOWPOS windowPos = (LPWINDOWPOS)lParam;
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_ENTERSIZEMOVE:
	{
		//no parameters
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_EXITSIZEMOVE:
	{
		//no parameters
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		break;
	}
	case WM_SHOWWINDOW:
	{
		bool visible = (bool)wParam;
		if (visible)
		{
			WindowShown();
		}
		int showStatus = (int)lParam;
		break;
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
		LRESULT result = _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		//LRESULT result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
		return result;
		break;
	}
	case WM_NCCALCSIZE:
	{
		LPNCCALCSIZE_PARAMS params = (LPNCCALCSIZE_PARAMS)lParam;
		LPRECT rect = (LPRECT)lParam;
		if (wParam)
		{
			//using params
			//TryInitializeWindowSize(params->rgrc[2].right - params->rgrc[2].left, params->rgrc[2].bottom - params->rgrc[2].top);
			int dummy = 0;
		}
		else
		{
			//using rect
			int dummy = 0;
		}
		LRESULT result = _CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
		return result;
	}
	}
	*/
	return _CallWindowProc(oldWindowProc,hwnd, uMsg, wParam, lParam);
}

HDC WindowContext::GetDC_()
{
	if (parentWindowContext != NULL)
	{
		//TODO
		HDC hdc = parentWindowContext->GetDC_();
		//RECT windowRect;
		//GetWindowRect(window, &windowRect);

		return hdc;
	}

	if (!IsVirtualized() || NOD3D)
	{
		this->hdc = ::GetDC(window);
		hdcMap.Add(this->hdc, this);
		return this->hdc;
	}
	else
	{
		if (d3d9Context.backBuffer == NULL)
		{
			d3d9Context.CreateD3D9(window);
			upscaler.SetSourceTexture(d3d9Context.texture);
		}
		if (d3d9Context.textureSurface != NULL)
		{
			HRESULT hr;
			hr = d3d9Context.textureSurface->GetDC(&this->hdc);
		}
		else
		{
			this->hdc = ::GetDC(window);
		}
		hdcMap.Add(this->hdc, this);
		return this->hdc;
	}
}

int WindowContext::ReleaseDC_()
{
	if (parentWindowContext != NULL)
	{
		return parentWindowContext->ReleaseDC_();
	}

	int result;
	if (!IsVirtualized() || NOD3D)
	{
		result = ::ReleaseDC(window, hdc);
		hdcMap.Remove(hdc);
		hdc = NULL;

#if DELAYED_HOOK
		if (!IsVirtualized())
		{
			WantToHook = true;
		}
#endif
		return result;
	}
	if (d3d9Context.textureSurface != NULL)
	{
		result = d3d9Context.textureSurface->ReleaseDC(this->hdc);
		result = SUCCEEDED(result);
		//TODO: Blit Dirty Region
		Redraw();
		//Do we want to sleep 1 here or not?  Would fix old programs that have no CPU speed control
		Sleep(1);
	}
	else
	{
		result = ::ReleaseDC(window, hdc);
	}
	hdcMap.Remove(hdc);
	hdc = NULL;
	return result;
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


	//TODO: partial updates

	HRESULT hr;
	hr = d3d9Context.device->BeginScene();
	RECT sourceRect = { 0, 0, VirtualWidth, VirtualHeight};
	RECT destRect = { XOffset, YOffset, XOffset + ScaledWidth, YOffset + ScaledHeight };

	hr = d3d9Context.device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 0, 0);
	hr = d3d9Context.device->StretchRect(d3d9Context.textureSurface, &sourceRect, d3d9Context.backBuffer, &destRect, D3DTEXF_LINEAR);
	RECT clientRect;
	::GetClientRect(window, &clientRect);
	

	hr = d3d9Context.device->EndScene();
	hr = d3d9Context.device->Present(&clientRect, &clientRect, NULL, NULL);
	return true;
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


//void WindowContext::RectVirtualToClientClamp(LPRECT lpRect) const
//{
//	RectVirtualToClient(lpRect);
//	ClampRect(lpRect);
//}

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

void WindowContext::AddDirtyRect(const RECT &rect)
{
	dirtyRegion.AddRectangle(rect);
}
void WindowContext::AddDirtyRect(int x, int y, int width, int height)
{
	RECT rect{ x, y, x + width, y + height };
	AddDirtyRect(rect);
}
void WindowContext::AddDirtyRectWithPen(int x, int y, int width, int height)
{
	HGDIOBJ currentObject = GetCurrentObject(hdc, OBJ_PEN);
	if (currentObject == NULL)
	{
		AddDirtyRect(x, y, width, height);
		return;
	}
	else
	{
		LOGPEN pen = {};
		GetObjectA(currentObject, sizeof(pen), &pen);
		int penWidth = pen.lopnWidth.x;
		if (pen.lopnStyle == PS_INSIDEFRAME)
		{
			penWidth = 0;
		}
		AddDirtyRect(x - penWidth, x - penWidth, width + penWidth * 2, height + penWidth * 2);
	}
}

/*
void WindowContext::TryInitializeWindowSize(int width, int height)
{
	if (virtualWidth || virtualHeight) return;
	virtualWidth  = width;
	virtualHeight = height;
}
*/

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
	WNDPROC currentWndProc = (WNDPROC)_GetWindowLongPtr(hwnd, GWL_WNDPROC);
	if (currentWndProc != DefaultWndProc && currentWndProc != SimpleWndProc)
	{
		return _CallWindowProc(currentWndProc, hwnd, uMsg, wParam, lParam);
	}
#if USE_CLASS_HOOK
	WNDPROC oldWndProc = windowClassSet.GetClassWndProc(hwnd);
	if (oldWndProc != NULL)
	{
#if !DO_NOT_RESET_WNDPROC
		_SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)oldWndProc);
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
		VirtualizeWindowSize = true;
		RECT windowRect;
		RECT clientRect;
		POINT clientZero = {};
		ClientToScreen(window, &clientZero);
		::GetWindowRect(window, &windowRect);
		::GetClientRect(window, &clientRect);
		VirtualWindowStyle = GetWindowLong(window, GWL_STYLE);
		VirtualWindowRect = windowRect;
		VirtualWidth = clientRect.right - clientRect.left;
		VirtualHeight = clientRect.bottom - clientRect.top;
		RealWidth = -1;
		RealHeight = -1;
		UpdateSize(VirtualWidth, VirtualHeight);
	}
}

void WindowContext::UpdateSize(int newWidth, int newHeight)
{
	if (IgnoreResizeEvents) return;
	if (!IsVirtualized())
	{
		return;
	}
	if (RealWidth != newWidth || RealHeight != newHeight)
	{
		RealWidth = newWidth;
		RealHeight = newHeight;

		{
			float scaleX = (float)RealWidth / (float)VirtualWidth;
			float scaleY = (float)RealHeight / (float)VirtualHeight;
			Scale = std::min(scaleX, scaleY);
		}
		ScaledWidth = Round(Scale * VirtualWidth);
		ScaledHeight = Round(Scale * VirtualHeight);
		LeftPadding = (RealWidth - ScaledWidth) / 2;
		RightPadding = RealWidth - ScaledWidth - LeftPadding;
		TopPadding = (RealHeight - ScaledHeight) / 2;
		BottomPadding = RealHeight - ScaledHeight - TopPadding;
		XOffset = LeftPadding;
		YOffset = TopPadding;
		POINT clientUpperLeftCorner = {};
		ClientToScreen(window, &clientUpperLeftCorner);
		RealX = clientUpperLeftCorner.x;
		RealY = clientUpperLeftCorner.y;
		upscaler.SetInputRectangle(0, 0, VirtualWidth, VirtualHeight);
		upscaler.SetViewRectangle(XOffset, YOffset, ScaledWidth, ScaledHeight);

		ChangeWindowResizable(true);

		InvalidateRect(window, NULL, false);
		//UpdateWindow(window);
		//ValidateRect(window, NULL);
	}
}

bool WindowContext::ChangeWindowResizable(bool resizable)
{
	DWORD windowStyle = GetWindowLong(window, GWL_STYLE);
	bool hasResizeBorder = windowStyle & WS_THICKFRAME;
	if (hasResizeBorder != resizable)
	{
		BorderChanging = 1;
		IgnoreResizeEvents = true;

		if (hasResizeBorder)
		{
			windowStyle &= ~WS_THICKFRAME;
		}
		else
		{
			windowStyle |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
		}
		SetWindowLong(window, GWL_STYLE, windowStyle);
		//FinishBorderChange();
		//BorderChanging = 1;
		return true;
	}
	return false;
}

void WindowContext::FinishBorderChange()
{
	int numberOfAttempts = 5;
tryAgain:
	BorderChanging = 2;
	RECT windowRect;
	RECT clientRect;
	::GetWindowRect(window, &windowRect);
	::GetClientRect(window, &clientRect);
	POINT clientZero = {};
	ClientToScreen(window, &clientZero);
	int leftBorder = clientZero.x - windowRect.left;
	int topBorder = clientZero.y - windowRect.top;
	int extraWidth = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
	int extraHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
	SetWindowPos(window, NULL, RealX - leftBorder, RealY - topBorder, RealWidth + extraWidth, RealHeight + extraHeight, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER);
	BorderChanging = 0;

	::GetWindowRect(window, &windowRect);
	::GetClientRect(window, &clientRect);

	if (clientRect.right - clientRect.left != RealWidth)
	{
		if (numberOfAttempts > 0)
		{
			numberOfAttempts--;
			goto tryAgain;
		}

	}


	IgnoreResizeEvents = false;
}

BOOL WindowContext::GetWindowRect_(LPRECT rect) const
{
	if (IsVirtualized())
	{
		if (rect)
		{
			*rect = VirtualWindowRect;
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
			*rect = { 0, 0, VirtualWidth, VirtualHeight };
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

/*
DWORD WindowContext::GetVirtualWindowStyle()
{
	if (IsVirtualized())
	{
		return VirtualWindowStyle;
	}
	return ::GetWindowLong(window, GWL_STYLE);
}
*/

bool WindowContext::IsVirtualized() const
{
	if (parentWindowContext != NULL) return parentWindowContext->IsVirtualized();
	return VirtualizeWindowSize;
}

LONG_PTR WindowContext::SetWindowLong_(int index, LONG_PTR newLong)
{
	switch (index)
	{
	case GWL_STYLE:
		if (IsVirtualized())
		{
			DWORD oldWindowLong = GetWindowLong_(index);
			newLong &= WS_DISABLED | WS_MINIMIZE | WS_VISIBLE;
			newLong |= oldWindowLong;
			_SetWindowLongPtr(window, index, newLong);
			return oldWindowLong;
		}
		break;
	case GWL_WNDPROC:
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
LONG_PTR WindowContext::GetWindowLong_(int index)
{
	switch (index)
	{
	case GWL_STYLE:
		if (IsVirtualized())
		{
			LONG_PTR windowLong = _GetWindowLongPtr(window, index);
			windowLong &= WS_DISABLED | WS_MINIMIZE | WS_VISIBLE;
			VirtualWindowStyle &= ~(WS_DISABLED | WS_MINIMIZE | WS_VISIBLE);
			windowLong |= VirtualWindowStyle;
			return windowLong;
		}
		break;
	case GWL_WNDPROC:
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
		*lpRect = LastInvalidatedRectClient;
	}
	else
	{
		RectVirtualToClient(lpRect);
	}
}
void WindowContext::UpdateRectClientToVirtual(LPRECT lpRect) const
{
	if (*lpRect == LastInvalidatedRectClient)
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
	/*
	if (rect == NULL)
	{
		return GetUpdateRect(window, NULL, bErase);
	}
	BOOL okay;
	RECT updateRectClient;
	okay = GetUpdateRect(window, &updateRectClient, bErase);
	RECT updateRectVirtual = (LastInvalidatedRectClient == updateRectClient) ?
		LastInvalidatedRectVirtual :
		RectClientToVirtualClamp(updateRectClient);
	*rect = updateRectVirtual;
	return okay;
	*/
}
BOOL WindowContext::InvalidateRect_(LPCRECT rect, BOOL bErase) //rect in virtual coordinates
{
	const RECT clientRect = GetClientRect_();
	if (rect == NULL)
	{
		rect = &clientRect;
	}
	LastInvalidatedRectVirtual = *rect;
	LastInvalidatedRectClient = RectVirtualToClient(*rect);

	BOOL okay;
	if (*rect == clientRect)
	{
		okay = InvalidateRect(window, NULL, bErase);
	}
	else
	{
		okay = InvalidateRect(window, &LastInvalidatedRectClient, bErase);
	}
	return okay;
}
BOOL WindowContext::ValidateRect_(LPCRECT rect) //rect in virtual coordinates
{
	const RECT clientRect = GetClientRect_();
	if (rect == NULL)
	{
		rect = &clientRect;
	}

	RECT updateRectClient = *rect;
	UpdateRectVirtualToClient(&updateRectClient);

	//RECT updateRectClient = RectVirtualToClient(*rect);
	//if (LastInvalidatedRectVirtual == *rect)
	//{
	//	updateRectClient = LastInvalidatedRectClient;
	//}

	BOOL okay;
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



	return okay;
}

HDC WindowContext::BeginPaint_(LPPAINTSTRUCT lpPaintStruct)
{
	HDC hdc;
	BOOL okay = true;
#if NO_PAINT_HOOK
	//RECT clientRect, clipRect;
	//HRGN clipRgn;
	//int regionType;
	//GetClientRect(window, &clientRect);
	//clipRgn = CreateRectRgnIndirect(&clientRect);
	//int okay = 0;
	//hdc = GetDC(window);
	//okay = GetClipRgn(hdc, clipRgn);
	//regionType = GetRgnBox(clipRgn, &clipRect);
	//okay = GetClipBox(hdc, &clipRect);
	//ReleaseDC(window, hdc);

	paintDc = BeginPaint(window, lpPaintStruct);
	
	//GetClientRect(window, &clientRect);
	//okay = GetClipRgn(hdc, clipRgn);
	//regionType = GetRgnBox(clipRgn, &clipRect);
	//okay = GetClipBox(hdc, &clipRect);
	//DeleteObject(clipRgn);

	return hdc;
#endif
#if USE_BEGINPAINT
	paintDc = BeginPaint(window, lpPaintStruct);
	hdc = GetDC_();
	lpPaintStruct->hdc = hdc;
	UpdateRectClientToVirtual(&(lpPaintStruct->rcPaint));

	//int mapMode = GetMapMode(hdc); //MM_TEXT is default


#else
	hdc = GetDC_();
	if (lpPaintStruct != NULL)
	{
		memset(lpPaintStruct, 0, sizeof(PAINTSTRUCT));
		lpPaintStruct->hdc = hdc;
		lpPaintStruct->fErase = BackgroundNeedsErasing;
		okay = GetUpdateRect_(&(lpPaintStruct->rcPaint), false);
#if PAINT_USE_CLIP_BOX
		RECT updateRectClient;
		okay = GetUpdateRect(window, &updateRectClient, false);
		HRGN clipRegion = CreateRectRgnIndirect(&updateRectClient);
		okay = SelectClipRgn(hdc, clipRegion);
		DeleteObject(clipRegion);
#endif
	}
#endif
	return hdc;
}

BOOL WindowContext::EndPaint_(const PAINTSTRUCT* lpPaintStruct)
{
	BOOL okay;
#if NO_PAINT_HOOK
	//RECT clientRect, clipRect;
	//HRGN clipRgn;
	//GetClientRect(window, &clientRect);
	//clipRgn = CreateRectRgnIndirect(&clientRect);
	//okay = GetClipRgn(lpPaintStruct->hdc, clipRgn);
	//okay = GetClipBox(lpPaintStruct->hdc, &clipRect);

	okay = EndPaint(window, lpPaintStruct);

	//GetClientRect(window, &clientRect);
	//okay = GetClipRgn(lpPaintStruct->hdc, clipRgn);
	//okay = GetClipBox(lpPaintStruct->hdc, &clipRect);
	//DeleteObject(clipRgn);

	return okay;
#endif
#if USE_BEGINPAINT
	if (lpPaintStruct != NULL)
	{
		PAINTSTRUCT paintStructCopy = *lpPaintStruct;
		paintStructCopy.hdc = paintDc;
		//okay = ValidateRect_(&(paintStructCopy.rcPaint));
		UpdateRectVirtualToClient(&(paintStructCopy.rcPaint));
		okay = EndPaint(window, &paintStructCopy);
		okay &= ReleaseDC_();
		paintDc = NULL;
		return okay;
	}
	return false;
#else
#if PAINT_USE_CLIP_BOX
	okay = SelectClipRgn(lpPaintStruct->hdc, NULL);
#endif
	if (lpPaintStruct != NULL)
	{
		okay = ValidateRect_(&(lpPaintStruct->rcPaint));
		okay &= ReleaseDC_();
		return okay;
	}
	else
	{
		return false;
	}
#endif
}

BOOL WindowContext::GetWindowPlacement_(WINDOWPLACEMENT* windowPlacement)
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
	//TODO
	return MoveWindow(window, x, y, width, height, repaint);
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
	return SetWindowPos(window, hwndInsertAfter, x, y, cx, cy, flags);
}
