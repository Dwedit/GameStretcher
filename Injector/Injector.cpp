struct IUnknown;
#include <Windows.h>
#include <stddef.h>
#include <string.h>
#include "win32_crt_memcpy.cpp"

#define ALLOW_ORDINALS 0
#define ALLOW_FORWARDED_EXPORTS 0
#define INCLUDE_HELLO_WORLD 0
#define SKIP_CHECKS 0

struct MyListEntry
{
    MyListEntry* Flink;
    MyListEntry* Bbink;
    void* word1;
    void* word2;
    HMODULE moduleHandle;
    void* word4;
};

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    //remaining data removed
} PEB, * PPEB;

typedef struct _TEB {
    PVOID Reserved1[12];
    PPEB ProcessEnvironmentBlock;
	ULONG LastErrorValue;
	//remaining data removed
} TEB, * PTEB;

HMODULE GetKernel32ModuleHandleFromTeb()
{
    _TEB* teb = NtCurrentTeb();
    MyListEntry* entry = (MyListEntry*)&teb->ProcessEnvironmentBlock->Ldr->InMemoryOrderModuleList;
    return entry->Flink->Flink->Flink->moduleHandle;
}

unsigned int GetLastErrorFromTeb()
{
	_TEB* teb = NtCurrentTeb();
	return teb->LastErrorValue;
}

extern "C" {
	__declspec (dllexport) HMODULE WINAPI CallLoadLibraryA(LPCSTR dllName);
}
/*
static inline IMAGE_NT_HEADERS* WINAPI ImageNtHeader(_In_ const PVOID Base)
{
    IMAGE_DOS_HEADER* DosHeader = (IMAGE_DOS_HEADER*)Base;
    if (DosHeader
        && DosHeader->e_magic == IMAGE_DOS_SIGNATURE
        && DosHeader->e_lfanew >= 0u
        && DosHeader->e_lfanew < 0x10000000u)
    {
        IMAGE_NT_HEADERS* ImageNtHeader = (IMAGE_NT_HEADERS*)((BYTE*)DosHeader + DosHeader->e_lfanew);
        if (ImageNtHeader->Signature == IMAGE_NT_SIGNATURE)
            return ImageNtHeader;
    }
    return nullptr;
}
*/

//Contains code from https://github.com/Speedi13/Custom-GetProcAddress-and-GetModuleHandle-and-more/blob/master/CustomWinApi.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//			Equivalent to the windows api function ImageDirectoryEntryToDataEx
//////////////////////////////////////////////////////////////////////////////////////////////////
static inline PVOID WINAPI ImageDirectoryEntryToDataInternal(PVOID Base, ULONG* Size, DWORD SizeOfHeaders, IMAGE_DATA_DIRECTORY* DataDirectory, IMAGE_FILE_HEADER* ImageFileHeader, void* ImageOptionalHeader)
{
	*(ULONG*)Size = NULL;
#if !SKIP_CHECKS
	if (!DataDirectory->VirtualAddress || !DataDirectory->Size || !SizeOfHeaders)
		return nullptr;
#endif
	*(ULONG*)Size = DataDirectory->Size;
	return (char*)Base + DataDirectory->VirtualAddress;
}

static inline PVOID WINAPI ImageDirectoryEntryToData32(PVOID Base, USHORT DirectoryEntry, ULONG* Size, IMAGE_FILE_HEADER* ImageFileHeader, IMAGE_OPTIONAL_HEADER32* ImageOptionalHeader)
{
	*(ULONG*)Size = NULL;
#if !SKIP_CHECKS
	if (DirectoryEntry >= ImageOptionalHeader->NumberOfRvaAndSizes)
		return nullptr;
#endif
	IMAGE_DATA_DIRECTORY* DataDirectory = &ImageOptionalHeader->DataDirectory[DirectoryEntry];
#if !SKIP_CHECKS
	if (!DataDirectory->VirtualAddress || !DataDirectory->Size)
		return nullptr;
#endif
	return ImageDirectoryEntryToDataInternal(Base,
		Size,
		ImageOptionalHeader->SizeOfHeaders,
		DataDirectory,
		ImageFileHeader,
		ImageOptionalHeader);
}
static inline PVOID WINAPI ImageDirectoryEntryToData64(PVOID Base, USHORT DirectoryEntry, ULONG* Size, IMAGE_FILE_HEADER* ImageFileHeader, IMAGE_OPTIONAL_HEADER64* ImageOptionalHeader)
{
	*(ULONG*)Size = NULL;
#if !SKIP_CHECKS
	if (DirectoryEntry >= ImageOptionalHeader->NumberOfRvaAndSizes)
		return nullptr;
#endif
	IMAGE_DATA_DIRECTORY* DataDirectory = &ImageOptionalHeader->DataDirectory[DirectoryEntry];
#if !SKIP_CHECKS
	if (!DataDirectory->VirtualAddress || !DataDirectory->Size)
		return nullptr;
#endif
	return ImageDirectoryEntryToDataInternal(Base,
		Size,
		ImageOptionalHeader->SizeOfHeaders,
		DataDirectory,
		ImageFileHeader,
		ImageOptionalHeader);
}

/// <summary>
/// Locates a directory entry within the image header and returns the address of the data for the directory entry
/// </summary>
/// <param name="Base">The base address of the image or data file</param>
/// <param name="DirectoryEntry">The directory entry to be located</param>
/// <param name="Size">A pointer to a variable that receives the size of the data for the directory entry that is located</param>
/// <returns>If the function succeeds, the return value is a pointer to the data for the directory entry</returns>
static inline PVOID WINAPI ImageDirectoryEntryToDataEx(PVOID Base, USHORT DirectoryEntry, ULONG* Size)
{
	*(ULONG*)Size = NULL;

	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)Base;
#if !SKIP_CHECKS
	if (!pDosHeader)
		return nullptr;
#endif
	IMAGE_FILE_HEADER* ImageFileHeader = nullptr;
	IMAGE_OPTIONAL_HEADER* ImageOptionalHeader = nullptr;

	LONG NtHeaderFileOffset = pDosHeader->e_lfanew;
	IMAGE_NT_HEADERS* ImageNtHeader = (PIMAGE_NT_HEADERS)((LPBYTE)pDosHeader + NtHeaderFileOffset);

	if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE
		&& NtHeaderFileOffset > 0
		&& NtHeaderFileOffset < 0x10000000u
		&& ImageNtHeader->Signature == IMAGE_NT_SIGNATURE)
	{
		ImageFileHeader = &ImageNtHeader->FileHeader;
		ImageOptionalHeader = &ImageNtHeader->OptionalHeader;
	}
	else
	{
		ImageFileHeader = (IMAGE_FILE_HEADER*)Base;
		ImageOptionalHeader = (IMAGE_OPTIONAL_HEADER*)((BYTE*)Base + 0x14);
	}
	switch (ImageOptionalHeader->Magic)
	{
#ifndef _WIN64
	case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
		return ImageDirectoryEntryToData32(
			Base,
			DirectoryEntry,
			Size,
			ImageFileHeader,
			(IMAGE_OPTIONAL_HEADER32*)ImageOptionalHeader);
#else
	case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
		return ImageDirectoryEntryToData64(
			Base,
			DirectoryEntry,
			Size,
			ImageFileHeader,
			(IMAGE_OPTIONAL_HEADER64*)ImageOptionalHeader);
#endif
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//					Equivalent to the windows api function GetProcAddress
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of an exported function inside the specified module
/// </summary>
/// <param name="hModule">Address of the module</param>
/// <param name="lpProcName">Name of the exported procedure</param>
/// <returns>returns the exported procedure address inside the specified module</returns>
static FARPROC WINAPI GetExportAddress(_In_ HMODULE hModule, _In_ LPCSTR lpProcName)
{
#if !SKIP_CHECKS
	if (lpProcName == NULL)
		return nullptr;
#endif
#if ALLOW_ORDINALS
	unsigned short ProcOrdinal = 0xFFFF;
	if ((ULONG_PTR)lpProcName < 0xFFFF)
		ProcOrdinal = (ULONG_PTR)lpProcName & 0xFFFF;
	else
	{
		//in case of "#123" resolve the ordinal to 123
		if (lpProcName[0] == '#')
		{
			DWORD OrdinalFromString = atoi(lpProcName + 1);
			if (OrdinalFromString < 0xFFFF &&
				OrdinalFromString != 0)
			{
				ProcOrdinal = OrdinalFromString & 0xFFFF;
				lpProcName = (LPCSTR)(ProcOrdinal);
			}
		}
	}
#endif
	IMAGE_DOS_HEADER* DosHeader = (IMAGE_DOS_HEADER*)hModule;
#if !SKIP_CHECKS
	if (!DosHeader || DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return nullptr;
#endif
	//only OptionalHeader is different between 64bit and 32bit so try not to touch it!
	IMAGE_NT_HEADERS* NtHeader = (IMAGE_NT_HEADERS*)((DWORD_PTR)DosHeader + DosHeader->e_lfanew);
#if !SKIP_CHECKS
	if (NtHeader->Signature != IMAGE_NT_SIGNATURE)
		return nullptr;
#endif
	ULONG ExportDirectorySize = NULL;
	IMAGE_EXPORT_DIRECTORY* ExportDirectory = (IMAGE_EXPORT_DIRECTORY*)ImageDirectoryEntryToDataEx(DosHeader, IMAGE_DIRECTORY_ENTRY_EXPORT, &ExportDirectorySize);
#if !SKIP_CHECKS
	if (!ExportDirectory || !ExportDirectorySize)
		return nullptr;
#endif
	//check if any export functions are present
#if !SKIP_CHECKS
	if (!ExportDirectory->NumberOfFunctions)
		return nullptr;
#endif
	//from BlackBone
	//https://github.com/DarthTon/Blackbone/blob/3dc33d815011b83855af607013d34c836b9d0877/src/BlackBone/Process/ProcessModules.cpp#L266
#if !SKIP_CHECKS
	// Fix invalid directory size
	if (ExportDirectorySize <= sizeof(IMAGE_EXPORT_DIRECTORY))
	{
		// New size should take care of max number of present names (max name length is assumed to be 255 chars)
		ExportDirectorySize = static_cast<DWORD>(ExportDirectory->AddressOfNameOrdinals - (DWORD)((BYTE*)(ExportDirectory)-(BYTE*)(DosHeader))
			+ max(ExportDirectory->NumberOfFunctions, ExportDirectory->NumberOfNames) * 255);
	}
#endif
	DWORD AddressOfNamesRVA = ExportDirectory->AddressOfNames;
	DWORD AddressOfFunctionsRVA = ExportDirectory->AddressOfFunctions;
	DWORD AddressOfNameOrdinalsRVA = ExportDirectory->AddressOfNameOrdinals;

	DWORD* ExportNames = (DWORD*)((BYTE*)DosHeader + AddressOfNamesRVA);
	DWORD* Functions = (DWORD*)((BYTE*)DosHeader + AddressOfFunctionsRVA);
	WORD* Ordinals = (WORD*)((BYTE*)DosHeader + AddressOfNameOrdinalsRVA);

	for (DWORD i = 0; i < ExportDirectory->NumberOfFunctions; i++)
	{
		unsigned short OrdinalIndex = Ordinals[i];

		DWORD ExportFncOffset = Functions[OrdinalIndex];
#if !SKIP_CHECKS
		if (!ExportFncOffset)
			continue;
#endif
		char* ProcNamePtr = (char*)((char*)DosHeader + ExportNames[i]);
		BYTE* ExportFnc = (BYTE*)((BYTE*)DosHeader + ExportFncOffset);
#if ALLOW_FORWARDED_EXPORTS
		//Forwarded exports:
		if (//Not supported with ordinals for forwarded export by name
			//Check for forwarded export:
			ExportFnc > ((BYTE*)ExportDirectory) &&
			ExportFnc < ((BYTE*)ExportDirectory + ExportDirectorySize))
		{
			//for example inside the Kernelbase.dll's export table
			//NTDLL.RtlDecodePointer
			//It could also forward an ordinal
			//NTDLL.#123
			char* ForwardedString = (char*)ExportFnc;
			DWORD ForwardedStringLen = (DWORD)strlen(ForwardedString) + 1;
			if (ForwardedStringLen >= 256)
				continue;
			char szForwardedLibraryName[256];
			memcpy(szForwardedLibraryName, ForwardedString, ForwardedStringLen);
			char* ForwardedFunctionName = NULL;
			char* ForwardedFunctionOrdinal = NULL;
			for (DWORD s = 0; s < ForwardedStringLen; s++)
			{
				if (szForwardedLibraryName[s] == '.')
				{
					szForwardedLibraryName[s] = NULL;
					ForwardedFunctionName = &ForwardedString[s + 1];
					break;
				}
			}

			//forwarded by ordinal
			if (ForwardedFunctionName != nullptr && ForwardedFunctionName[0] == '#')
			{
				ForwardedFunctionOrdinal = ForwardedFunctionName + 1;
				ForwardedFunctionName = NULL;
			}
			if (ForwardedFunctionName)
			{
				if (strcmp(lpProcName, ForwardedFunctionName) != NULL)
					continue;

				HMODULE hForwardedDll = CallLoadLibraryA(szForwardedLibraryName);
				FARPROC ForwardedFunction = (FARPROC)GetExportAddress(hForwardedDll, ForwardedFunctionName);
				return (FARPROC)ForwardedFunction;
			}
			else
				if (ForwardedFunctionOrdinal && ProcOrdinal < 0xFFFF)
				{
					DWORD ForwardedOrdinal = atoi(ForwardedFunctionOrdinal);
					if (ForwardedOrdinal > 0xFFFF ||
						ForwardedOrdinal == 0 ||
						ForwardedOrdinal != ProcOrdinal)
						continue;

					HMODULE hForwardedDll = CallLoadLibraryA(szForwardedLibraryName);
					FARPROC ForwardedFunction = (FARPROC)GetExportAddress(hForwardedDll, (char*)(ForwardedOrdinal & 0xFFFF));
					return (FARPROC)ForwardedFunction;
				}
				else
					continue;
		}
#endif
#if ALLOW_ORDINALS
		if ((ULONG_PTR)lpProcName > 0xFFFF && strcmp(lpProcName, ProcNamePtr) == NULL)
			return (FARPROC)ExportFnc;
		else
		{
			if ((OrdinalIndex + 1) == ProcOrdinal)
				return (FARPROC)ExportFnc;
		}
#else
		if (0 == strcmp(lpProcName, ProcNamePtr)) return (FARPROC)ExportFnc;
#endif
	}
	return nullptr;
}

//#pragma data_seg(push,".text")
//extern "C"
//{
//	const char stringloadLibraryA[] = "LoadLibraryA";
//}
//
//const char* GetLoadLibraryAString()
//{
//	return stringloadLibraryA;
//}

//#pragma data_seg(pop)

typedef HMODULE(WINAPI* FP_LoadLibraryA)(LPCSTR);
typedef HMODULE(WINAPI* FP_LoadLibraryW)(LPCWSTR);
//typedef void (WINAPI* FP_ExitThread)(DWORD);
typedef int(WINAPI* FP_MessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
typedef int(WINAPI* FP_MessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);


extern "C"
{
	__declspec (dllexport) HMODULE WINAPI CallLoadLibraryA(LPCSTR dllName)
	{
		const char stringloadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A',0 };
		HMODULE kernel32Module = GetKernel32ModuleHandleFromTeb();
		FARPROC proc = GetExportAddress(kernel32Module, stringloadLibraryA);
		FP_LoadLibraryA _LoadLibraryA = (FP_LoadLibraryA)proc;
		HMODULE module = _LoadLibraryA(dllName);
		return module;
	}

	__declspec (dllexport) HMODULE WINAPI CallLoadLibraryW(LPCWSTR dllName)
	{
		const char stringloadLibraryW[] = { 'L','o','a','d','L','i','b','r','a','r','y','W',0 };
		HMODULE kernel32Module = GetKernel32ModuleHandleFromTeb();
		FARPROC proc = GetExportAddress(kernel32Module, stringloadLibraryW);
		FP_LoadLibraryW _LoadLibraryW = (FP_LoadLibraryW)proc;
		HMODULE module = _LoadLibraryW(dllName);
		return module;
	}

	__declspec (dllexport) DWORD WINAPI CallLoadLibraryThreadEntryA(LPCSTR dllName)
	{
		HMODULE module = CallLoadLibraryA(dllName);
		if (module != NULL) return 0;
		return GetLastErrorFromTeb();
	}

	__declspec (dllexport) DWORD WINAPI CallLoadLibraryThreadEntryW(LPCWSTR dllName)
	{
		HMODULE module = CallLoadLibraryW(dllName);
		if (module != NULL) return 0;
		return GetLastErrorFromTeb();
	}

#if INCLUDE_HELLO_WORLD
	__declspec (dllexport) DWORD WINAPI HelloWorldThreadEntry(LPVOID parameter)
	{
		{
			const char stringMessageBoxA[] = { 'M','e','s','s','a','g','e','B','o','x','A',0 };
			const char stringUser32Dll[] = { 'U','s','e','r','3','2','.','d','l','l',0 };
			const char stringHelloWorld[] = { 'H','e','l','l','o',' ','W','o','r','l','d','!',0 };

			HMODULE user32 = CallLoadLibraryA(stringUser32Dll);
			FARPROC proc = GetExportAddress(user32, stringMessageBoxA);
			FP_MessageBoxA _MessageBoxA = (FP_MessageBoxA)proc;
			_MessageBoxA(NULL, stringHelloWorld, stringHelloWorld, 0);
		}

		{
			const char stringMessageBoxW[] = { 'M','e','s','s','a','g','e','B','o','x','W',0 };
			const wchar_t stringUser32Dll[] = { 'U','s','e','r','3','2','.','d','l','l',0 };
			const wchar_t stringHelloWorld[] = { 'H','e','l','l','o',' ','W','o','r','l','d','!',0 };

			HMODULE user32 = CallLoadLibraryW(stringUser32Dll);
			FARPROC proc = GetExportAddress(user32, stringMessageBoxW);
			FP_MessageBoxW _MessageBoxW = (FP_MessageBoxW)proc;
			_MessageBoxW(NULL, stringHelloWorld, stringHelloWorld, 0);
		}
		return 0;
	}
#endif
	//__declspec (dllexport) DWORD WINAPI ThreadStartRoutine(LPVOID lpThreadParameter)
	//{
		//const char exitThreadString[] = { 'E','x','i','t','T','h','r','e','a','d',0 };
		//HMODULE kernel32Module = GetKernel32ModuleHandleFromTeb();
		//FARPROC proc = GetExportAddress(kernel32Module, exitThreadString, true);
		//FP_ExitThread _ExitThread = (FP_ExitThread)proc;

		//_ExitThread(0);
		//return 0;
	//}


}

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#if INCLUDE_HELLO_WORLD
extern "C"
{
	//entry point for RunDLL32
	_declspec(dllexport) void CALLBACK entry(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
	{
		HelloWorldThreadEntry(NULL);
	}
}
#endif
