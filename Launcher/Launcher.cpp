struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <algorithm>
using std::wstring;
using std::pair;
using std::make_pair;
using std::max;

#include "CommandLineArguments.h"

bool StartSuspendedProcess(wstring &commandLine, const wstring &startDirectory, int nShowCmd, PROCESS_INFORMATION &processInformation)
{
    STARTUPINFOW startupInfo = {};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESHOWWINDOW;
    startupInfo.wShowWindow = nShowCmd;

    BOOL okay;
    DWORD lastError;
    okay = CreateProcessW(NULL, &commandLine[0], NULL, NULL, false, CREATE_SUSPENDED, NULL, startDirectory.c_str(), &startupInfo, &processInformation);
    lastError = GetLastError();
    return okay;
}

pair<DWORD_PTR, SIZE_T> FindMemoryBlock(void* ptr)
{
    MEMORY_BASIC_INFORMATION memInfo = {};
    SIZE_T result;
    result = VirtualQuery(ptr, &memInfo, sizeof(memInfo));
    return std::make_pair((DWORD_PTR)memInfo.BaseAddress, (SIZE_T)memInfo.RegionSize);
}

DWORD InjectDllIntoRemoteProcess(const PROCESS_INFORMATION& processInformation, const wchar_t *const _payloadDllName)
{
    //Load Injector DLL
    wstring LauncherDirectory = FindLauncherDirectory();
    wstring injectorDllName = LauncherDirectory + L"Injector.dll";
    wstring payloadDllName = LauncherDirectory + _payloadDllName;

    HMODULE injectorDll = NULL;
    injectorDll = LoadLibraryW(injectorDllName.c_str());
    if (injectorDll == NULL) return GetLastError();
    
    //Find the function
    LPCSTR funcName = "_CallLoadLibraryThreadEntryW@4";
    if (sizeof(void*) == 8) { funcName = "CallLoadLibraryThreadEntryW"; }
    FARPROC callLoadLibrary = (FARPROC)GetProcAddress(injectorDll, funcName);
    if (callLoadLibrary == NULL) return GetLastError();

    //Find the memory block used by the DLL
    auto block = FindMemoryBlock(callLoadLibrary);
    //block contains only the TEXT section of the DLL
    DWORD_PTR dllBaseAddress = max((DWORD_PTR)injectorDll, block.first);
    DWORD_PTR dllEndAddress = block.first + block.second;
    SIZE_T dllSize = dllEndAddress - dllBaseAddress;
    
    DWORD_PTR procAddressWithinDll = (DWORD_PTR)callLoadLibrary - dllBaseAddress;

    DWORD_PTR memWithinOtherProcess = (DWORD_PTR)VirtualAllocEx(processInformation.hProcess, 0, dllSize, MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (memWithinOtherProcess == 0) return GetLastError();
    memWithinOtherProcess = (DWORD_PTR)VirtualAllocEx(processInformation.hProcess, (LPVOID)memWithinOtherProcess, dllSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (memWithinOtherProcess == 0) return GetLastError();

    BOOL okay;
    SIZE_T numberOfBytesWritten = 0;
    okay = WriteProcessMemory(processInformation.hProcess, (LPVOID)memWithinOtherProcess, (LPCVOID)dllBaseAddress, dllSize, &numberOfBytesWritten);
    
    SIZE_T nameSize = (payloadDllName.length() + 1) * sizeof(wchar_t);
    
    DWORD_PTR dllNameInOtherProcess = memWithinOtherProcess + dllSize - nameSize;
    okay = WriteProcessMemory(processInformation.hProcess, (LPVOID)dllNameInOtherProcess, payloadDllName.c_str(), nameSize, &numberOfBytesWritten);

    DWORD_PTR callLoadLibraryInOtherProcess = procAddressWithinDll + memWithinOtherProcess;
    DWORD remoteThreadId = 0;

#if _DEBUG
    const bool _debug = true;
#else
    const bool _debug = false;
#endif

    if (IsDebuggerPresent() || _debug)
    {
        bool DebuggerDialog(const PROCESS_INFORMATION & processInformation);
        if (DebuggerDialog(processInformation))
        {

        }
        else
        {
            return TerminateProcess(processInformation.hProcess, 1);
        }
    }

    //if (IsDebuggerPresent() || _debug)
    //{
    //    MessageBoxA(NULL, "Attach a debugger to the other application, set a breakpoint there, press OK on this MessageBox, then click RUN in the other debugger.", "Debugging Instructions", 0);
    //}

    HANDLE hThread = CreateRemoteThread(processInformation.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)callLoadLibraryInOtherProcess, (LPVOID)dllNameInOtherProcess, 0, &remoteThreadId);
    
    DWORD result = 0;
    
    if (hThread != NULL)
    {
        WaitForSingleObject(hThread, INFINITE);
        DWORD exitCode = 0;
        okay = GetExitCodeThread(hThread, &exitCode);
        CloseHandle(hThread);
        result = exitCode;
    }
    else
    {
        result = GetLastError();
    }
    okay = VirtualFreeEx(processInformation.hProcess, (LPVOID)memWithinOtherProcess, 0, MEM_RELEASE);
    return result;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    wstring commandLine = GetProcessCommandLine();
    wstring initialDirectory = FindInitialDirectory(commandLine);
    if (commandLine.empty())
    {
        MessageBoxW(NULL, L"Please drag and drop an EXE file onto this EXE file.", L"Instructions", 0);
        return ERROR_FILE_NOT_FOUND;
    }
    
    PROCESS_INFORMATION processInformation = {};
    bool okay = StartSuspendedProcess(commandLine, initialDirectory, nShowCmd, processInformation);
    DWORD result = InjectDllIntoRemoteProcess(processInformation, L"Stretcher.dll");
    AllowSetForegroundWindow(processInformation.dwProcessId);
    ResumeThread(processInformation.hThread);
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);
	return result;
}


