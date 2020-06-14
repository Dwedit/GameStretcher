struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <InitGuid.h>
#include <KnownFolders.h>
#include <psapi.h>
#include <Shlobj.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Shlwapi.lib")

#include <algorithm>
#include <vector>
#include <string>
#include <Shlwapi.h>
using std::vector;
using std::wstring;
using std::min;
using std::max;

wstring GetModuleFileName(HMODULE module)
{
    wstring path;
    path.resize(2047);
    PVOID oldValue = 0;
    DWORD result = GetModuleFileNameW(module, &path[0], path.size());
    path.resize(wcslen(path.c_str()));
    path.shrink_to_fit();
    return path;
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

vector<HMODULE> GetModules()
{
    //enumerate modules
    vector<HMODULE> modules;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
    if (hProcess == NULL) return modules;
    DWORD bytesNeeded;
    EnumProcessModules(hProcess, NULL, 0, &bytesNeeded);
    modules.resize(bytesNeeded / sizeof(HMODULE));
    EnumProcessModules(hProcess, &modules[0], modules.size() * sizeof(HMODULE), &bytesNeeded);
    return modules;
}

wstring GetDirectory(LPCWSTR path)
{
    if (PathIsDirectoryW(path))
    {
        return path;
    }
    wstring str = path;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (str[i] == '/' || str[i] == '\\')
        {
            str.resize(i + 1);
            if (PathIsDirectoryW(str.c_str()))
            {
                return str;
            }
        }
    }
    return wstring();
}

wstring GetDirectory(const wstring &path)
{
    return GetDirectory(path.c_str());
}

vector<HMODULE> GetApplicationDLLs()
{
    HMODULE thisDll = NULL;
    HMODULE thisExe = GetModuleHandleA(NULL);
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&GetApplicationDLLs, &thisDll);

    wstring myPath = GetModuleFileName(thisExe);
    wstring myDirectory = GetDirectory(myPath);
    
    vector<HMODULE> modules = GetModules();
    vector<HMODULE> result;
    for (int i = 0; i < modules.size(); i++)
    {
        HMODULE module = modules[i];
        wstring path = GetModuleFileName(module);
        if (module != thisExe && module != thisDll && StringStartsWithCaseInsensitive(path, myDirectory))
        {
            result.push_back(module);
        }
    }
    return result;
}
