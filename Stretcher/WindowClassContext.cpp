#include "WindowClassContext.h"

#if USE_CLASS_HOOK

#include "StringUtil.h"
//#include "WndProcMap.h"
#include "WindowContext.h"
using std::swap;

WindowClassSet windowClassSet;

void WindowClassSet::AddClass(LPCSTR className, ATOM atom, WNDPROC wndProc)
{
	int index = FindIndex(className);
	if (index == -1)
	{
		vec.push_back(WindowClassContext());
		index = vec.size() - 1;
	}
	WindowClassContext& context = vec[index];
	context.classNameA = className;
	context.classNameW = ToUnicode(className);
	context.isUnicode = false;
	context.classAtom = atom;
	context.wndProc = wndProc;
}
void WindowClassSet::AddClass(LPCWSTR className, ATOM atom, WNDPROC wndProc)
{
	int index = FindIndex(className);
	if (index == -1)
	{
		vec.push_back(WindowClassContext());
		index = vec.size() - 1;
	}
	WindowClassContext& context = vec[index];
	context.classNameA = ToAnsi(className);
	context.classNameW = className;
	context.isUnicode = true;
	context.classAtom = atom;
	context.wndProc = wndProc;
}

//void WindowClassSet::AddClass(LPCSTR className)
//{
//	if (FindIndex(className) != -1) return;;
//	vec.push_back(WindowClassContext());
//	WindowClassContext& context = vec[vec.size() - 1];
//	context.classNameA = className;
//	context.classNameW = ToUnicode(className);
//	context.isUnicode = false;
//}
//void WindowClassSet::AddClass(LPCWSTR className)
//{
//	if (FindIndex(className) != -1) return;;
//	vec.push_back(WindowClassContext());
//	WindowClassContext& context = vec[vec.size() - 1];
//	context.classNameA = ToAnsi(className);
//	context.classNameW = className;
//	context.isUnicode = true;
//}
//void WindowClassSet::SetClassAtom(LPCSTR className, ATOM atom)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec[index].classAtom = atom;
//	vec[index].currentClassAtom = atom;
//}
//void WindowClassSet::SetClassAtom(LPCWSTR className, ATOM atom)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec[index].classAtom = atom;
//	vec[index].currentClassAtom = atom;
//}
//void WindowClassSet::SetWndProc(LPCSTR className, WNDPROC wndProc)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec[index].wndProc = wndProc;
//}
//void WindowClassSet::SetWndProc(LPCWSTR className, WNDPROC wndProc)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec[index].wndProc = wndProc;
//}
WNDPROC WindowClassSet::GetClassWndProc(HWND hwnd)
{
	int index = FindIndex(hwnd);
	if (index == -1) return nullptr;
	return vec[index].wndProc;
}
//WNDPROC WindowClassSet::GetClassWndProc(LPCSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return nullptr;
//	return vec[index].wndProc;
//}
//WNDPROC WindowClassSet::GetClassWndProc(LPCWSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return nullptr;
//	return vec[index].wndProc;
//}
/*
bool WindowClassSet::TryHookWindowClass(LPCSTR className)
{
	int index = FindIndex(className);
	if (index == -1) return false;
	return TryHookWindowClass(vec[index]);
}
bool WindowClassSet::TryHookWindowClass(LPCWSTR className)
{
	int index = FindIndex(className);
	if (index == -1) return false;
	return TryHookWindowClass(vec[index]);
}
*/
//bool WindowClassSet::IsWindowClassUnicode(LPCSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return false;
//	return (vec[index].isUnicode);
//}
//bool WindowClassSet::IsWindowClassUnicode(LPCWSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return false;
//	return (vec[index].isUnicode);
//}
int WindowClassSet::FindIndex(ATOM atom) const
{
	//search by original or new ATOM
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i].classAtom == atom)
		{
			return (int)i;
		}
	}
	return -1;
}
int WindowClassSet::FindIndex(LPCSTR className) const
{
	LONG_PTR classNamePtrValue = (LONG_PTR)className;
	if (classNamePtrValue < 65536) return FindIndex((ATOM)classNamePtrValue);
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i].classNameA == className)
		{
			return (int)i;
		}
	}
	return -1;
}
int WindowClassSet::FindIndex(LPCWSTR className) const
{
	LONG_PTR classNamePtrValue = (LONG_PTR)className;
	if (classNamePtrValue < 65536) return FindIndex((ATOM)classNamePtrValue);
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i].classNameW == className)
		{
			return (int)i;
		}
	}
	return -1;
}
int WindowClassSet::FindIndex(HWND hwnd) const
{
	wchar_t buffer[256];
	RealGetWindowClassW(hwnd, &buffer[0], 256);
	return FindIndex(buffer);
}
int WindowClassSet::FindIndex(ATOM atom)
{
	const WindowClassSet* this_const = static_cast<const WindowClassSet*>(this);
	int index = this_const->FindIndex(atom);
	if (index == -1) return -1;
	SwapToFront(index);
	return 0;
}
int WindowClassSet::FindIndex(LPCSTR className)
{
	const WindowClassSet* this_const = static_cast<const WindowClassSet*>(this);
	int index = this_const->FindIndex(className);
	if (index == -1) return -1;
	SwapToFront(index);
	return 0;
}
int WindowClassSet::FindIndex(LPCWSTR className) 
{
	const WindowClassSet* this_const = static_cast<const WindowClassSet*>(this);
	int index = this_const->FindIndex(className);
	if (index == -1) return -1;
	SwapToFront(index);
	return 0;
}
int WindowClassSet::FindIndex(HWND hwnd)
{
	wchar_t buffer[256];
	RealGetWindowClassW(hwnd, &buffer[0], 256);
	return FindIndex(buffer);
}

void WindowClassSet::SwapToFront(int index)
{
	if (index == 0) return;
	swap(vec[index], vec[0]);
}

//bool WindowClassSet::ClassExists(LPCSTR className) const
//{
//	return FindIndex(className) != -1;
//}
//bool WindowClassSet::ClassExists(LPCWSTR className) const
//{
//	return FindIndex(className) != -1;
//}

//WindowClassContext& WindowClassSet::Find(LPCSTR className)
//{
//	int index = FindIndex(className);
//	if (index == -1) return _Dummy;
//	return vec[index];
//}
//WindowClassContext& WindowClassSet::Find(LPCWSTR className)
//{
//	int index = FindIndex(className);
//	if (index == -1) return _Dummy;
//	return vec[index];
//}
//const WindowClassContext& WindowClassSet::Find(LPCSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return _Dummy;
//	return vec[index];
//}
//const WindowClassContext& WindowClassSet::Find(LPCWSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return _Dummy;
//	return vec[index];
//}
WindowClassContext* WindowClassSet::Find(LPCSTR className)
{
	int index = FindIndex(className);
	if (index == -1) return nullptr;
	return &vec[index];
}
WindowClassContext* WindowClassSet::Find(LPCWSTR className)
{
	int index = FindIndex(className);
	if (index == -1) return nullptr;
	return &vec[index];
}
//const WindowClassContext* WindowClassSet::Find(LPCSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return nullptr;
//	return &vec[index];
//}
//const WindowClassContext* WindowClassSet::Find(LPCWSTR className) const
//{
//	int index = FindIndex(className);
//	if (index == -1) return nullptr;
//	return &vec[index];
//}

//void WindowClassSet::RemoveClass(LPCSTR className)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec.erase(vec.begin() + index, vec.begin() + index + 1);
//}
//void WindowClassSet::RemoveClass(LPCWSTR className)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec.erase(vec.begin() + index, vec.begin() + index + 1);
//}
//void WindowClassSet::SetWindowCreated(LPCSTR className)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec[index].anyWindowsCreated = true;
//}
//void WindowClassSet::SetWindowCreated(LPCWSTR className)
//{
//	int index = FindIndex(className);
//	if (index == -1) return;
//	vec[index].anyWindowsCreated = true;
//}

//WNDPROC AllocateWndProc(ATOM atom, WNDPROC oldWndProc)
//{
//	return WndProcMap::AllocateWndProc(atom, oldWndProc);
//}
/*
bool WindowClassSet::TryHookWindowClass(WindowClassContext& context)
{
	if (context.anyWindowsCreated) return false;
	if (context.isHooked) return false;
	HINSTANCE hInstance = GetModuleHandleA(NULL);
	BOOL okay;
	if (context.isUnicode)
	{
		WNDCLASSEXW classInfo = {};
		classInfo.cbSize = sizeof(classInfo);
		okay = GetClassInfoExW(hInstance, context.classNameW.c_str(), &classInfo);
		if (!okay) return false;
		okay = UnregisterClassW(context.classNameW.c_str(), hInstance);
		if (!okay) return false;
		context.originalWndProc = classInfo.lpfnWndProc;
		context.currentWndProc = DefaultWndProc;
		classInfo.lpfnWndProc = context.currentWndProc;
		classInfo.style |= CS_OWNDC;
		context.currentClassAtom = RegisterClassExW(&classInfo);
		context.isHooked = true;
	}
	else
	{
		WNDCLASSEXA classInfo = {};
		classInfo.cbSize = sizeof(classInfo);
		okay = GetClassInfoExA(hInstance, context.classNameA.c_str(), &classInfo);
		if (!okay) return false;
		okay = UnregisterClassA(context.classNameA.c_str(), hInstance);
		if (!okay) return false;
		context.originalWndProc = classInfo.lpfnWndProc;
		context.currentWndProc = DefaultWndProc;
		classInfo.lpfnWndProc = context.currentWndProc;
		classInfo.style |= CS_OWNDC;
		context.currentClassAtom = RegisterClassExA(&classInfo);
		context.isHooked = true;
	}
	if (context.currentWndProc == NULL) return false;
	return true;
}
*/

#endif
