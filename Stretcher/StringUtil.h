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

bool StringStartsWith(const wstring& str, const wstring& lookFor);
bool StringStartsWithCaseInsensitive(const wstring& str, const wstring& prefix);
bool StringEndsWithCaseInsensitive(const wstring& str, const wstring& suffix);
bool StringContainsCaseInsensitive(const wstring& str, const wstring& lookFor);

bool AddSlash(wstring& str);
bool AddSlash(string& str);
