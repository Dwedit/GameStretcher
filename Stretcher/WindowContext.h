#pragma once

#define USE_REAL_MOUSE_COORDINATES 0
#define MOVE_WINDOW_TO_UPPER_LEFT 0

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include "D3D9Context_Old.h"
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

#include "RectUtil.h"
#include <vector>
using std::vector;

//Object which extends a Window to implement window scaling management, and scaled drawing
class WindowContext
{
	HWND window;	//The Window associated with this instance
	WNDPROC oldWindowProc;	//The original WndProc function of the Window
	D3D9Context_Old d3d9Context;	//Direct3D 9 objects, including the device and target texture
	ATOM windowClassAtom;	//The Atom for the Window Class that was used to create the window
	bool isWindowUnicode;	//If the window was created using a Unicode function, this is true
	bool VirtualizeWindowSize;	//Main Flag to indicate that the window is virtualized
	bool IgnoreResizeEvents;	//This is set when the window is being resized programmatically, and window messages related to moving or resizing should be ignored.
	bool IsShown;	//Indicates if the window has ever been shown or not
	bool IsFullScreen;	//Indicates if the window is currently Fullscreen or not
	bool MovingWindow;	//Indicates that Window is being moved by this class, and not by Win32 itself
	typedef void (WindowContext::*VoidMemberFunction)();
	VoidMemberFunction ResizeHandler;	//Handler for Move/Resize messages to allow deferred execution of a function

	//TODO: Fix these to distingush screen coorinates from Child Window to Parent Window coordinates
	RECT RealClientRect;		//Real Client Rect (top left is 0,0)
	RECT RealClientBounds;		//Real Client Bounds relative to the screen
	RECT RealWindowRect;		//Real Window Rect relative to the screen
	RECT VirtualClientRect;		//Virtual Client Rect (top left is 0,0)
	RECT VirtualWindowRect;		//Virtual Window Rect relative to the screen
	RECT VirtualClientBounds;	//Virtual Client Bounds relative to the screen
	RECT ScaledClientRect;		//Scaled Rect relative to the Real Client rect (top left is 0,0)

	DWORD VirtualWindowStyle;	//The window style to present back to the original window
	
	int VirtualWidth;	//Virtual Client Width - The app will think it is this sizze
	int VirtualHeight;	//Virtual Client Height - The app will think it is this sizze
	int RealWidth;	//Real Client Width - To resize the window, Assign to this variable, then call MakeWindowResizable/MakeWindowBorderless
	int RealHeight;	//Real Client Height - To resize the window, Assign to this variable, then call MakeWindowResizable/MakeWindowBorderless
	int ScaledWidth;	//Scaled Client Width after applying scaling and aspect ratio correction
	int ScaledHeight;	//Scaled Client Height after applying scaling and aspect ratio correction
	int RealX;	//Real X coordinate of Window's client area on the screen, also for moving/resizing with MakeWindowResizable/MakeWindowBorderless
	int RealY;	//Real Y coordinate of Window's client area on the screen, also for moving/resizing with MakeWindowResizable/MakeWindowBorderless

	RECT LastInvalidatedRectVirtual;	//The last invalidated RECT in virtual client coordinates, used to correct rounding errors
	RECT LastInvalidatedRectReal;	//The last invalidated RECT in real client coordinates, used to correct rounding errors

	float Scale;	//The scaling factor of the window (1 = no change, <1 = shrink, >1 = enlarge)
	int XOffset;	//X coordinate of the scaled screen image (real client coordinates)
	int YOffset;	//Y coordinate of the scaled screen image (real client coordinates)
	int LeftPadding, TopPadding, BottomPadding, RightPadding;	//Padding sizes of Left, Top, Bottom and Right edges
	HDC paintDC;	//The DC returned by BeginPaint
	HDC realDC;		//The DC returned by GetDC
	HDC lastDC;		//The Last DC used (either paintDC or realDC), used to clip to a child window or beginpaint/endpaint region
	HDC d3dDC;		//The DC returned by the D3D9 Texture, changes very frequently

	RECT paintClipRectReal;		//The update rect used by BeginPaint in real coordinates
	RECT paintClipRectVirtual;	//The update rect used by BeginPaint in virtual coordinates

	Region dirtyRegion;	//The dirty region, accumulates the dirty rectangles of the GDI drawing commands
	CUpscaler upscaler;	//The main upscaler object, which draws the upscaled frame

	WindowContext* parentWindowContext;	//The parent window context object, used for a child window
	vector<WindowContext*> childWindows;	//The list of child windows for this window

	bool paintDCIsOpen;	//Indicates if a Paint DC has been released or not, which can be used to ignore certain BitBlt commands

	std::mutex myMutex;	//Mutex used for preventing D3D drawing and GDI actions on the D3D texture from happening at the same time, avoiding crashes

	bool deleteThis;	//indicates that the object should be deleted once the stack depth reaches 0
	int wndProcStackDepth = 0;	//stack depth for wndproc, when it reaches 0, it is okay to delete the object

	static bool staticRecursionGuard;
public:
	//Main WndProc handler
	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//WndProc used for hooking, then SimpleWndProc is used afterwards
	static LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//WndProc used for Win32, calls the Main WndProc handler
	static LRESULT CALLBACK SimpleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//Indicates that the window is currently being created and is okay to attempt to hook
	static bool WindowBeingCreated;

public:
	//Initializes the WindowContext object for a given window handle
	void Init(HWND hwnd);
	//Calls PreDispose and detaches the window from the WindowContext object
	void Release();
	//Detaches the window from the child windows tree, releases DCs, and disposes the D3D objects
	void PreDispose();

	//Default Constructor
	WindowContext() :
		Scale(1.0f),
		window(), oldWindowProc(), windowClassAtom(), isWindowUnicode(), VirtualizeWindowSize(), IgnoreResizeEvents(), VirtualWidth(), VirtualHeight(), RealWidth(), RealHeight(), ScaledWidth(), ScaledHeight(), RealX(), RealY(), XOffset(), YOffset(), LeftPadding(), TopPadding(), BottomPadding(), RightPadding(), d3dDC(), paintDC(),
		LastInvalidatedRectReal(), LastInvalidatedRectVirtual(), VirtualWindowRect(), VirtualWindowStyle(),
		IsShown(), IsFullScreen(),
		ResizeHandler(), MovingWindow(), realDC(), paintDCIsOpen(),
		parentWindowContext(),RealClientBounds(), RealClientRect(), RealWindowRect(),
		ScaledClientRect(), VirtualClientBounds(), VirtualClientRect(),
		lastDC(), paintClipRectReal(), paintClipRectVirtual(),
		deleteThis(), wndProcStackDepth()
	{
	}
	//Destructor - calls Release
	~WindowContext()
	{
		Release();
	}

	//Returns a known WindowContext object from the map given a HWND
	static WindowContext* Get(HWND hwnd);
	//Returns a known WindowContext object from the map given a HDC
	static WindowContext* GetByHdc(HDC hdc);
	//Adds a HDC and WindowContext pair to the map
	static void HdcAdd(HDC hdc, WindowContext* windowContext);
	//Removes a HDC from the map
	static bool HdcRemove(HDC hdc);
	//Removes a WindowContext from the HDC->WindowContext map
	static bool HdcRemoveWindow(WindowContext* windowContext);
	//Returns a known WindowContext object from the map given a HWND
	static WindowContext* GetWindowContext(HWND hwnd);
	//Returns the last used WindowContext for a hooked and visible window
	static WindowContext* GetWindowContext();
	//Checks if the window is okay to hook (Has Title Bar, not a Dialog, not already hooked, etc...), then calls CreateNewWindowContext to hook the window
	static bool TryHookWindow(HWND hwnd);
	//Creates a Window Context object to hook the window, then adds the window to the map
	static WindowContext* CreateNewWindowContext(HWND hwnd);
	//Deletes a Window Context object - do not call if any Window Context objects are on the call stack
	static bool DeleteWindowContext(HWND hwnd);
	//Determines if a window context exists for the given HWND
	static bool WindowContextExists(HWND hwnd);

	
	//Transforms client coordinates to virtual coordinates
	void ClientToVirtualFloat(float& x, float& y) const;
	//Transforms virtual coordinates to client coordinates
	void VirtualToClientFloat(float& x, float& y) const;
	//Transforms client coordinates to virtual coordinates, and rounds to nearest integer values
	void ClientToVirtualRounded(int& x, int& y) const;	
	//Transforms virtual coordinates to client coordinates, and rounds to nearest integer values
	void VirtualToClientRounded(int& x, int& y) const;
	//Transforms client coordinates to virtual coordinates using floor operation, for Left or Top of bounding box
	void ClientToVirtualFloor(int& x, int& y) const;
	//Transforms virtual coordinates to client coordinates using floor operation, for Left or Top of bounding box
	void VirtualToClientFloor(int& x, int& y) const;
	//Transforms client coordinates to virtual coordinates using ceiling operation, for Right or Bottom of bounding box
	void ClientToVirtualCeil(int& x, int& y) const;
	//Transforms virtual coordinates to client coordinates using ceiling operation, for Right or Bottom of bounding box
	void VirtualToClientCeil(int& x, int& y) const;

	//Moves the points of the rectangle to be valid as virtual coordinates
	void ClampRect(LPRECT lpRect) const;
	//Moves the point to be valid as a virtual coordinate
	void ClampPoint(LPPOINT lpPoint) const;

	//Transforms a virtual client rectangle to a real client rectangle, using floor/ceiling operations
	RECT RectVirtualToClient(const RECT& rect) const;
	//Transforms a virtual client rectangle to a real client rectangle, using floor/ceiling operations
	void RectVirtualToClient(LPRECT lpRect) const;
	//Transforms a real client rectangle to a virtual client rectangle, using floor/ceiling operations
	void RectClientToVirtual(LPRECT lpRect) const;
	//Transforms a real client rectangle to a virtual client rectangle, using floor/ceiling operations, and ensures all coordinates are within the virutal client bounds
	void RectClientToVirtualClamp(LPRECT lpRect) const;
	//Transforms a real client rectangle to a virtual client rectangle, using floor/ceiling operations, and ensures all coordinates are within the virutal client bounds
	RECT RectClientToVirtualClamp(const RECT &rect) const;
	//Transforms a real client point to a virtual client point using rounding
	void MouseClientToVirtual(LPPOINT lpPoint) const;
	//Transforms a virtual client point to a real client point using rounding
	void MouseVirtualToClient(LPPOINT lpPoint) const;
	//Transforms a real client point to a virtual client point using rounding, and ensures the point is inside the virtual client bounds
	void MouseClientToVirtualClamp(LPPOINT lpPoint) const;
	//Transforms a real screen coordinate to a "Virtual Screen" coordinate - point is scaled by the Scale factor, and the origin is the client area top-left coordinate on the screen
	void MouseScreenToVirtualScreen(LPPOINT lpPoint) const;
	//Transforms a "Virtual Screen" coordinate to a real screen coordinate - point is scaled by the Scale factor, and the origin is the client area top-left coordinate on the screen
	void MouseVirtualScreenToScreen(LPPOINT lpPoint) const;
	//Transforms a "Virtual Screen" coordinate to a virtual client coordinate - point is scaled by the Scale factor, and the origin is the client area top-left coordinate on the screen
	void MouseVirtualScreenToVirtual(LPPOINT lpPoint) const;
	//Transforms a virtual client coordinate to a "Virtual Screen" coordinate - point is scaled by the Scale factor, and the origin is the client area top-left coordinate on the screen
	void MouseVirtualToVirtualScreen(LPPOINT lpPoint) const;

	//Returns the DC for the window (same HDC for entire life of the window), registers the HDC in the HDC Map, and triggers a redraw
	HDC GetDC_();
	//Not Implemented
	HDC GetDCEx_(HRGN hrgnClip, DWORD flags);
	//Triggers a Redraw (paramater is ignored)
	int ReleaseDC_(HDC hdcToRelease);
	//Triggers a Redraw
	int ReleaseDC_();
	//Triggers a Redraw
	BOOL GdiFlush_();

	//Complete a draw operation, release the texture DC, and calls Redraw to draw the texture to the screen
	bool CompleteDraw();
	//Gets DC for the Direct3D Texture (Create a lock before calling this)
	HDC GetD3DDC();
	//Draws the texture to the screen upscaled
	bool Redraw();

	//Gets a dirty rectangle from the DC, and adds that to the dirty region
	void AddDirtyRect();

	//Handles a window becoming shown to start to virtualize the window
	void WindowShown();
	//Makes the window become virtualized - After this point, the program will think the window size never changes
	void VirtualizeWindow();
	//Calls UpdateSize() if newWidth and newHeight have not changed
	void UpdateSize(int newWidth, int newHeight);
	//Updates the known size of a window, including virtual, real, and scaled dimensions, and may trigger a redraw
	void UpdateSize();
	//currently not used
	void UpdateSizeVirtual();
	//Updates Real size and position from real window coordinates
	void UpdateSizeReal();
	//Assigns real client bounds and real window rect to all size variables
	void UpdateSizeNonVirtualized();
	//Updates Scale factor, and Scaled dimensions.  Ensure that Virtual dimensions and Real dimensions are up to date before calling this.
	void UpdateSizeScaled();
	//calls UpdateSizeReal, UpdateSizeScaled, UpdateSizeVirtual
	void UpdateSizeVirtualized();
	//Moves a child window to coordinates based on the parent's scale
	void MoveResizeChildWindow();
	//Moves all child windows to coordinates based on the parent's scale
	void MoveResizeChildWindows();
	//Makes the window border resizable, rather than fixed or borderless.  Moves/Resizes the window to the current value of RealX, RealY, RealWidth, RealHeight
	bool MakeWindowResizable();
	//Makes the window border borderless, rather than resizable or fixed.  Moves/Resizes the window to the current value of RealX, RealY, RealWidth, RealHeight
	bool MakeWindowBorderless();
	//Handler function to call FinishBorderChange once a WM_SIZE message has been received
	void FinishBorderChangeHandler();
	//Updates the window border, and Moves/Resizes the window to the current value of RealX, RealY, RealWidth, RealHeight.  May take multiple attempts.
	void FinishBorderChange();
	//Return the virtual client rect, moved to the real window position, with real nonclient area added on
	BOOL GetWindowRect_(LPRECT rect) const;
	//Return the virtual client rect
	BOOL GetClientRect_(LPRECT rect) const;
	//Return the virtual client rect
	RECT GetClientRect_() const;

	//True if the window is virtualized (all window sizing hidden from the real application)
	bool IsVirtualized() const;
	//Sets a Window Long, but if the Window Style would change, maintains the current resize border, also protects the WndProc
	LONG_PTR SetWindowLong_(int index, LONG_PTR newLong);
	//Gets a Window Long, if it gets the Window Style, instead spoof the border style the app thinks it has, also returns the app's WndProc
	LONG_PTR GetWindowLong_(int index) const;

	//Translates the Update Rect from virtual client coordinates to real client coordinates (and remembers it to avoid rounding errors)
	void UpdateRectVirtualToClient(LPRECT lpRect) const;
	//Translates the Update Rect from real client coordinates to virtual client coordinates (and remembers it to avoid rounding erorrs)
	void UpdateRectClientToVirtual(LPRECT lpRect) const;
	//Returns true if an update rect exists
	BOOL HasUpdateRect() const;
	//Gets the Update Rect from the window, then translates to virtual client coordinates
	BOOL GetUpdateRect_(LPRECT rect, BOOL bErase);
	//Gets the Update Region from the window, then translates to virtual client coordinates
	int GetUpdateRgn_(HRGN hrgn, BOOL bErase);

	//Invalidates a rect specified in virtual client coordinates
	BOOL InvalidateRect_(LPCRECT rect, BOOL bErase); //rect virtual => real coordinates
	//Validates a rect specified in virtual client coordinates
	BOOL ValidateRect_(LPCRECT rect); //rect virtual => real coordinates

	//Calls BeginPaint, stores paint DC, stores the paint clip rectangle, and triggers a redraw
	HDC BeginPaint_(LPPAINTSTRUCT lpPaintStruct);
	//Calls EndPaint, indicates the Paint DC is closed, and triggers a redraw
	BOOL EndPaint_(const PAINTSTRUCT *lpPaintStruct);

	//Gets Window Placement (Not Yet Implemented)
	BOOL GetWindowPlacement_(WINDOWPLACEMENT* windowPlacement) const;
	//Moves the window, and updates the virtual width and height.  If scale = 1, resizes the window as well.
	BOOL MoveWindow_(int x, int y, int width, int height, BOOL repaint);
	//Sets Window Placement (Not Yet Implemented)
	BOOL SetWindowPlacement_(const WINDOWPLACEMENT* windowPlacement);
	//Moves the window, and updates the virtual width and height.  If scale = 1, resizes the window as well.
	BOOL SetWindowPos_(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags);

	//Calls Win32 MoveWindow, indicates that the window is being moved by this class, and not by Win32 internally
	BOOL _MoveWindow(int x, int y, int width, int height, BOOL repaint);
	//Calls Win32 SetWindowPlacement, indicates that the window is being moved by this class, and not by Win32 internally
	BOOL _SetWindowPlacement(const WINDOWPLACEMENT* windowPlacement);
	//Calls Win32 SetWindowPos, indicates that the window is being moved by this class, and not by Win32 internally
	BOOL _SetWindowPos(HWND hwndInsertAfter, int x, int y, int cx, int cy, UINT flags);

	//Gets client bounds relative to the parent window
	RECT GetRealClientBounds() const;
	//Gets the real window rect
	RECT GetRealWindowRect() const;
	//Get the real client rect
	RECT GetRealClientRect() const;

	//Gets the real nonclient edge areas of the window
	void GetRealNonClientArea(int& left, int& top, int& right, int& bottom) const;

	//Returns the Direct3D DC, and sets the DC bounds and clipping area to match the window (get a lock before calling this)
	HDC GetCurrentDC(HDC inputDC);
	//Returns a reference to the real LastDC variable (so child windows can get the parent's variable)
	HDC& GetLastDC();

	//Sets the GDI clip rectangle
	static int SetClipRect(HDC hdc, const RECT* rect);
	//Sets the GDI clip rectangle
	int SetClipRect(const RECT* rect);
	//Gets the GDI clip rectangle
	RECT GetClipRect() const;

	//Create a lock object (for D3D synchronization) - When the returned lock object goes out of scope it is unlocked
	unique_lock<mutex> CreateLock();

	//Transforms a region from virtual coordinates to real coordinates
	void TransformRegionVirtualToReal(HRGN hrgn) const;
	//Returns a new region transformed from virtual coordinates to real coordinates
	HRGN TransformRegionVirtualToRealCopy(HRGN hrgn) const;
	//Transforms a region from real coordinates to virtual coordinates
	void TransformRegionRealToVirtual(HRGN hrgn) const;
	//Returns a new region transformed from real coordinates to virtual coordinates
	HRGN TransformRegionRealToVirtualCopy(HRGN hrgn) const;
	
	//Given a window rect, returns what its real client rect would be
	RECT GetFutureClientRect(const RECT &windowRect);

	//Returns the Scale factor
	float GetScale() const;

	//Invalidates and draws part of the window (given virtual coordinates)
	BOOL RedrawWindow_(CONST RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags);
	//Indicates if the paint DC has already been released and is stale (to prevent drawing black from BitBlt)
	bool PaintDCIsExpired(HDC hdc) const;

	//Returns the real window handle assocated with this WindowContext object
	HWND GetHwnd() const
	{
		return this->window;
	}

	//Returns the scaled bounds of the window
	const RECT& GetScaledBounds() const;
};
