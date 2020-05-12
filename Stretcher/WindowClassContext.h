#pragma once

#define USE_CLASS_HOOK 1

#if USE_CLASS_HOOK

struct IUnknown;
#define NOMINMAX
#include <Windows.h>

#include <string>
using std::string;
using std::wstring;
#include <vector>
using std::vector;

class WindowClassContext
{
public:
	wstring classNameW;
	string classNameA;
	ATOM classAtom;
	WNDPROC wndProc;
	bool isUnicode;
	WindowClassContext() : wndProc(), isUnicode(), classAtom()
	{

	}
};

class WindowClassSet
{
	vector<WindowClassContext> vec;
private:
	int FindIndex(ATOM atom) const;
	int FindIndex(LPCSTR className) const;
	int FindIndex(LPCWSTR className) const;
	int FindIndex(HWND hwnd) const;
	int FindIndex(ATOM atom);
	int FindIndex(LPCSTR className);
	int FindIndex(LPCWSTR className);
	int FindIndex(HWND hwnd);
	void SwapToFront(int index);
public:
	void AddClass(LPCSTR className, ATOM atom, WNDPROC wndProc);
	void AddClass(LPCWSTR className, ATOM atom, WNDPROC wndProc);
	//void AddClass(LPCSTR className);
	//void AddClass(LPCWSTR className);
	//void SetClassAtom(LPCSTR className, ATOM atom);
	//void SetClassAtom(LPCWSTR className, ATOM atom);
	//void SetWndProc(LPCSTR className, WNDPROC wndProc);
	//void SetWndProc(LPCWSTR className, WNDPROC wndProc);
	//WNDPROC GetClassWndProc(ATOM atom) const;
	WNDPROC GetClassWndProc(HWND window);
	//WNDPROC GetClassWndProc(LPCSTR className) const;
	//WNDPROC GetClassWndProc(LPCWSTR className) const;
	//ATOM GetClassAtom(HWND window) const;
	//ATOM GetClassAtom(LPCSTR className) const;
	//ATOM GetClassAtom(LPWSTR className) const;
	//void SetWindowCreated(LPCSTR className);
	//void SetWindowCreated(LPCWSTR className);
	//bool IsWindowClassUnicode(ATOM atom) const;
	//bool IsWindowClassUnicode(LPCSTR className) const;
	//bool IsWindowClassUnicode(LPCWSTR className) const;
	//bool RemoveClass(HWND hwnd);
	//bool RemoveClass(ATOM atom);
	//void RemoveClass(LPCSTR className);
	//void RemoveClass(LPCWSTR className);
	//bool ClassExixts(HWND hwnd) const;
	//bool ClassExixts(ATOM atom) const;
	//bool ClassExists(LPCSTR className) const;
	//bool ClassExists(LPCWSTR className) const;
	//WindowClassContext& Find(LPCSTR className);
	//WindowClassContext& Find(LPCWSTR className);
	//const WindowClassContext& Find(LPCSTR className) const;
	//const WindowClassContext& Find(LPCWSTR className) const;
	WindowClassContext* Find(HWND hwnd);
	WindowClassContext* Find(LPCSTR className);
	WindowClassContext* Find(LPCWSTR className);
	//const WindowClassContext* Find(LPCSTR className) const;
	//const WindowClassContext* Find(LPCWSTR className) const;
};

extern WindowClassSet windowClassSet;

#endif
