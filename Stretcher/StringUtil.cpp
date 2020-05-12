#include "StringUtil.h"

wstring ToUnicode(const string& str)
{
    return ToUnicode(str.c_str());
}

string ToAnsi(const wstring& str)
{
    return ToAnsi(str.c_str());
}

wstring ToUnicode(LPCSTR str)
{
    const UINT codePage = CP_ACP;
    //size_t strLengthAnsi = str.length();
    int requiredBufferSize = MultiByteToWideChar(codePage, 0, str, -1, NULL, 0);
    wstring result;
    result.resize(requiredBufferSize - 1);
    int okay;
    okay = MultiByteToWideChar(codePage, 0, str, strlen(str), &result[0], requiredBufferSize);
    return result;
}

string ToAnsi(LPCWSTR str)
{
    const UINT codePage = CP_ACP;
    //size_t strLengthAnsi = str.length();
    int requiredBufferSize = WideCharToMultiByte(codePage, 0, str, -1, NULL, 0, NULL, NULL);
    string result;
    result.resize(requiredBufferSize - 1);
    int okay;
    okay = WideCharToMultiByte(codePage, 0, str, wcslen(str), &result[0], requiredBufferSize, NULL, NULL);
    return result;
}
