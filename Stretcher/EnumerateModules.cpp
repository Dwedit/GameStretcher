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
#include "StringUtil.h"
using std::vector;
using std::wstring;
using std::min;
using std::max;

//Given a module, return its filename as a unicode string
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

//Return all modules of this process into a vector
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

//Gets the directory from a full path, and validates the directory before returning it (otherwise returns empty string)
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

//Gets the directory from a full path, and validates the directory before returning it (otherwise returns empty string)
wstring GetDirectory(const wstring &path)
{
    return GetDirectory(path.c_str());
}

wstring SHGetFolderPath(int csidl)
{
    wchar_t buffer[MAX_PATH];
    SHGetFolderPathW(NULL, csidl, NULL, SHGFP_TYPE_CURRENT, &buffer[0]);
    wstring folder = buffer;
    AddSlash(folder);
    folder.shrink_to_fit();
    return folder;
}

template <int csidl>
static const wstring& SHGetFolderPathCached()
{
    static bool isChecked;
    static wstring path;
    if (isChecked) return path;
    path = SHGetFolderPath(csidl);
    return path;
}

const wstring& GetWindowsDirectory()
{
    return SHGetFolderPathCached<CSIDL_WINDOWS>();
}
const wstring& GetCommonFilesDirectory()
{
    return SHGetFolderPathCached<CSIDL_PROGRAM_FILES_COMMON>();
}
const wstring& GetCommonFilesX86Directory()
{
    return SHGetFolderPathCached<CSIDL_PROGRAM_FILES_COMMONX86>();
}

bool IsSystemDll(const wstring& path)
{
    {
        const wstring& windowsDirectory = GetWindowsDirectory();
        if (!windowsDirectory.empty() && StringStartsWithCaseInsensitive(path, windowsDirectory)) return true;
    }
    {
        const wstring& commonFilesDirectory = GetCommonFilesDirectory();
        if (!commonFilesDirectory.empty() && StringStartsWithCaseInsensitive(path, commonFilesDirectory)) return true;
    }
    {
        const wstring& commonFilesX86Directory = GetCommonFilesX86Directory();
        if (!commonFilesX86Directory.empty() && StringStartsWithCaseInsensitive(path, commonFilesX86Directory)) return true;
    }
    return false;
}

bool IsApplicationDLL(HMODULE module)
{
    HMODULE thisDll = NULL;
    HMODULE thisExe = GetModuleHandleA(NULL);
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&IsApplicationDLL, &thisDll);

    wstring myPath = GetModuleFileName(thisExe);
    wstring myDirectory = GetDirectory(myPath);

    wstring path = GetModuleFileName(module);
    if (module != thisExe && module != thisDll && StringStartsWithCaseInsensitive(path, myDirectory) && !IsSystemDll(path))
    {
        return true;
    }
    return false;
}

//Returns Modules from the current process that are DLLs, and are relative the program (and not System)
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
        if (module != thisExe && module != thisDll && StringStartsWithCaseInsensitive(path, myDirectory) && !IsSystemDll(path))
        {
            result.push_back(module);
        }
    }
    return result;
}
