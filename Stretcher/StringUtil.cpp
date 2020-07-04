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
    wstring result;
    if (str == NULL) return result;
    int requiredBufferSize = MultiByteToWideChar(codePage, 0, str, -1, NULL, 0);
    result.resize(requiredBufferSize - 1);
    int okay;
    okay = MultiByteToWideChar(codePage, 0, str, strlen(str), &result[0], requiredBufferSize);
    return result;
}

string ToAnsi(LPCWSTR str)
{
    const UINT codePage = CP_ACP;
    //size_t strLengthAnsi = str.length();
    string result;
    if (str == NULL) return result;
    int requiredBufferSize = WideCharToMultiByte(codePage, 0, str, -1, NULL, 0, NULL, NULL);
    result.resize(requiredBufferSize - 1);
    int okay;
    okay = WideCharToMultiByte(codePage, 0, str, wcslen(str), &result[0], requiredBufferSize, NULL, NULL);
    return result;
}

bool StringStartsWith(const wstring& str, const wstring& lookFor)
{
    if (lookFor.empty()) return false;
    if (lookFor.length() > str.length()) return false;
    for (int i = 0; i < lookFor.length(); i++)
    {
        if (str[i] != lookFor[i]) return false;
    }
    return true;
}

bool StringStartsWithCaseInsensitive(const wstring& str, const wstring& prefix)
{
    if (prefix.empty()) return false;
    if (prefix.size() > str.size()) return false;
    for (int i = 0; i < prefix.size(); i++)
    {
        int c1 = str[i];
        int c2 = prefix[i];
        if (c1 >= 'a' && c1 <= 'z') c1 -= 'a' - 'A';
        if (c2 >= 'a' && c2 <= 'z') c2 -= 'a' - 'A';
        if (c1 != c2)
        {
            return false;
        }
    }
    return true;
}

bool StringEndsWithCaseInsensitive(const wstring& str, const wstring& suffix)
{
    if (suffix.empty()) return false;
    if (suffix.size() > str.size()) return false;
    for (int i = 0; i < suffix.size(); i++)
    {
        int c1 = str[str.size() - i - 1];
        int c2 = suffix[suffix.size() - i - 1];
        if (c1 >= 'a' && c1 < 'z') c1 -= 'a' - 'A';
        if (c2 >= 'a' && c2 < 'z') c2 -= 'a' - 'A';
        if (c1 != c2)
        {
            return false;
        }
    }
    return true;
}

bool StringContainsCaseInsensitive(const wstring& str, const wstring& lookFor)
{
    if (lookFor.empty()) return false;
    if (lookFor.size() > str.size()) return false;
    
    int c1;
    int c2 = lookFor[0];
    if (c2 >= 'a' && c2 < 'z') c2 -= 'a' - 'A';
    int firstChar2 = c2;

    int i1 = 0, i2 = 0;
    for (i1 = 0; i1 <= str.size() - lookFor.size(); i1++)
    {
        c1 = str[i1];
        if (c1 >= 'a' && c1 < 'z') c1 -= 'a' - 'A';
        if (c1 == firstChar2)
        {
            for (i2 = 1; i2 < lookFor.size(); i2++)
            {
                c1 = str[i1 + i2];
                c2 = lookFor[i2];
                if (c1 >= 'a' && c1 < 'z') c1 -= 'a' - 'A';
                if (c2 >= 'a' && c2 < 'z') c2 -= 'a' - 'A';
                if (c1 != c2)
                {
                    break;
                }
            }
            if (i2 == lookFor.size())
            {
                return true;
            }
        }
    }
    return false;
}

bool AddSlash(wstring& str)
{
    if (str.size() >= 1 && str[str.size() - 1] == '\\')
    {
        return false;
    }
    str.push_back('\\');
    return true;
}

bool AddSlash(string& str)
{
    if (str.size() >= 1 && str[str.size() - 1] == '\\')
    {
        return false;
    }
    str.push_back('\\');
    return true;
}
