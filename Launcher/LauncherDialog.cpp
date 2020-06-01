struct IUnknown;
#define NOMINMAX
#include <Windows.h>

#include "resource.h"
#include "CommandLineArguments.h"
#include <string>
#include <vector>
#include <algorithm>

static CommandLineData* cmd;
static INT_PTR CALLBACK LauncherDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static vector<wstring> filenames;
static vector<wstring> filenames2;

static bool CaseInsensitiveEqual(const wstring& a, const wstring& b);

static wstring GetFileName(const wstring& path)
{
    for (int i = path.size() - 1; i >= 0; i--)
    {
        if (path[i] == '/' || path[i] == '\\')
        {
            return path.substr(i + 1);
        }
    }
    return path;
}

static void GetFiles(vector<wstring> &files, const wstring &_path, const wstring &filter)
{
    wstring path = AppendSlash(_path);
    wstring findFilter = path + filter;
    WIN32_FIND_DATAW findData = {};
    HANDLE findHandle = FindFirstFileW(findFilter.c_str(), &findData);
    if (findHandle == INVALID_HANDLE_VALUE) return;
    files.push_back(path + findData.cFileName);
    while (true)
    {
        bool found = FindNextFileW(findHandle, &findData);
        if (!found) break;
        files.push_back(path + findData.cFileName);
    }
}

static void RemoveString(vector<wstring>& vec, const wstring& str)
{
    int i = 0;
    for (i = 0; i < vec.size(); i++)
    {
        if (CaseInsensitiveEqual(vec[i], str))
        {
            vec.erase(vec.begin() + i, vec.begin() + i + 1);
            return;
        }
    }
}

static void AddString(vector<wstring>& vec, const wstring& str)
{
    if (str.empty()) return;
    int i = 0;
    for (i = 0; i < vec.size(); i++)
    {
        if (CaseInsensitiveEqual(vec[i], str))
        {
            return;
        }
    }
    vec.push_back(str);
}

static bool CaseInsensitiveLess_(LPCWSTR a, LPCWSTR b)
{
    int i = 0;
    while (true)
    {
        wchar_t c1, c2;
        c1 = a[i];
        c2 = b[i];
        if (c1 >= 'a' && c1 <= 'z') c1 -= 'a' - 'A';
        if (c2 >= 'a' && c2 <= 'z') c2 -= 'a' - 'A';
        if (c1 < c2) return true;
        if (c1 > c2) return false;
        if (c1 == 0 || c2 == 0) return false;
        i++;
    }
}

static bool CaseInsensitiveEqual_(LPCWSTR a, LPCWSTR b)
{
    int i = 0;
    while (true)
    {
        wchar_t c1, c2;
        c1 = a[i];
        c2 = b[i];
        if (c1 >= 'a' && c1 <= 'z') c1 -= 'a' - 'A';
        if (c2 >= 'a' && c2 <= 'z') c2 -= 'a' - 'A';
        if (c1 != c2) return false;
        if (c1 == 0 && c2 == 0) return true;
        i++;
    }
}

static bool CaseInsensitiveLess(const wstring& a, const wstring& b)
{
    return CaseInsensitiveLess_(a.c_str(), b.c_str());
}

static bool CaseInsensitiveEqual(const wstring& a, const wstring& b)
{
    return CaseInsensitiveEqual_(a.c_str(), b.c_str());
}

static void GetFileNames()
{
    filenames.clear();
    filenames2.clear();
    vector<wstring> paths;
    AddString(paths, AppendSlash(cmd->targetDirectory));
    AddString(paths, AppendSlash(cmd->explorerDirectory));
    AddString(paths, AppendSlash(cmd->currentDirectory));
    AddString(paths, AppendSlash(cmd->launcherDirectory));
    for (int i = 0; i < paths.size(); i++)
    {
        GetFiles(filenames, paths[i], L"*.exe");
    }
    RemoveString(filenames, cmd->launcherExe);
    std::sort(filenames.begin(), filenames.end(), &CaseInsensitiveLess);
    for (int i = 0; i < filenames.size(); i++)
    {
        filenames2.push_back(GetFileName(filenames[i]));
    }
}

bool LauncherDialog(CommandLineData& cmd)
{
    ::cmd = &cmd;
    GetFileNames();

    if (DialogBoxW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDD_LAUNCHER), NULL, (DLGPROC)LauncherDlgProc) == IDOK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
wstring CreateFilter(const wstring &filterString)
{
    wstring filter = filterString;
    for (int i = 0; i < filter.length(); i++)
    {
        if (filter[i] == '|') filter[i] = '\0';
    }
    filter.push_back('\0');
    return filter;
}

static INT_PTR CALLBACK LauncherDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
    {
        HWND listBox = GetDlgItem(hwnd, IDC_LIST1);
        for (int i = 0; i < filenames2.size(); i++)
        {
            int pos = SendMessageW(listBox, LB_ADDSTRING, 0, (LPARAM)filenames2[i].c_str());
            SendMessageW(listBox, LB_SETITEMDATA, pos, i);
        }
    }
    break;
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDC_LIST1)
        {
            if (HIWORD(wParam) == LBN_DBLCLK)
            {
                wParam = IDOK;
            }
        }
        
        if (LOWORD(wParam) == IDC_BROWSE)
        {
            OPENFILENAMEW ofn = {};
            ofn.lStructSize = sizeof(ofn);
            wstring filter = CreateFilter(L"EXE Files (*.exe)|*.exe|All Files (*.*)|*.*");
            ofn.lpstrFilter = filter.c_str();
            ofn.nFilterIndex = 1;
            ofn.hwndOwner = hwnd;
            ofn.hInstance = GetModuleHandle(NULL);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
            ofn.lpstrDefExt = L".exe";
            wchar_t buffer[1024];
            buffer[0] = 0;
            wchar_t buffer2[1024];
            buffer2[0] = 0;
            ofn.lpstrFile = buffer;
            ofn.lpstrFileTitle = buffer2;
            ofn.nMaxFile = 1024;
            ofn.nMaxFileTitle = 1024;
            BOOL ok = GetOpenFileNameW(&ofn);
            if (ok)
            {
                cmd->targetExe = GetFile(ofn.lpstrFile);
                cmd->targetDirectory = GetDirectory(cmd->targetExe);
                cmd->BuildFullCommandLine();
                EndDialog(hwnd, IDOK);
                return 0;
            }
        }
        if (LOWORD(wParam) == IDOK)
        {
            HWND listBox = GetDlgItem(hwnd, IDC_LIST1);
            int selectedIndex = SendMessageW(listBox, LB_GETCURSEL, 0, 0);
            int itemData = SendMessageW(listBox, LB_GETITEMDATA, selectedIndex, 0);
            int i = itemData;
            if (filenames.size() == 1) i = 0;
            if (i >= 0 && i < filenames.size())
            {
                cmd->targetExe = GetFile(filenames[i]);
                cmd->targetDirectory = GetDirectory(cmd->targetExe);
                cmd->BuildFullCommandLine();
                EndDialog(hwnd, IDOK);
                return 0;
            }
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hwnd, IDCANCEL);
            return 0;
        }
    }
    break;
    }
    return 0;
}

