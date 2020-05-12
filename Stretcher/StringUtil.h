#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <string>
using std::string;
using std::wstring;

wstring ToUnicode(const string& str);
string ToAnsi(const wstring& str);

wstring ToUnicode(LPCSTR str);
string ToAnsi(LPCWSTR str);
