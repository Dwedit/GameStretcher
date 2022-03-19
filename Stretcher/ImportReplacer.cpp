#include "ImportReplacer.h"
#include <Psapi.h>

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
	* (ULONG*)Size = DataDirectory->Size;
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
/// <param name="MappedAsImage">If the flag is TRUE, the file is mapped by the system as an image. If this flag is FALSE, the file is mapped as a data file by the MapViewOfFile / ReadFile function</param>
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

string ToLower(const char* inputStr)
{
	size_t len = strlen(inputStr);
	string result;
	result.reserve(len);
	for (size_t i = 0; i < len; i++)
	{
		result.push_back(tolower(inputStr[i]));
	}
	return result;
}

void ImportReplacer::GetImports(LPCSTR dllNameToMatch)
{
	string dllNameToMatchLower = ToLower(dllNameToMatch);
	ULONG ImportDirectorySize = 0;
	PVOID ImportDirectory = ImageDirectoryEntryToDataEx(this->Base, IMAGE_DIRECTORY_ENTRY_IMPORT, &ImportDirectorySize);
	if (ImportDirectory == NULL)
	{
		return;
	}
	const char* base = reinterpret_cast<const char*>(this->Base);
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = static_cast<PIMAGE_IMPORT_DESCRIPTOR>(ImportDirectory);
	while (ImportDescriptor->Name != 0)
	{
		LPCSTR dllName = base + ImportDescriptor->Name;
		unordered_map<string, FARPROC*> map2;
		string dllNameLower = ToLower(dllName);

		if (dllNameToMatchLower == dllNameLower)
		{
			this->map[dllNameLower] = map2;
			const LONG_PTR* namePtrs = reinterpret_cast<const LONG_PTR*>(base + ImportDescriptor->OriginalFirstThunk);
			const LONG_PTR* codeAddrs = reinterpret_cast<const LONG_PTR*>(base + ImportDescriptor->FirstThunk);
			if (ImportDescriptor->OriginalFirstThunk != 0)
			{
				//normal case
				int funcIndex = 0;
				while (namePtrs[funcIndex] != 0)
				{
					const short* ordinalPtr = reinterpret_cast<const short*>(base + namePtrs[funcIndex]);
					WORD ordinal = *ordinalPtr;
					ordinalPtr++;
					LPCSTR funcName = reinterpret_cast<const char*>(ordinalPtr);
					map2[funcName] = const_cast<FARPROC*>(reinterpret_cast<const FARPROC*>(&codeAddrs[funcIndex]));
					funcIndex++;
				}
			}
			else
			{
				//Not sure about this, but it seems to work for one EXE
				int funcIndex = 0;
				const char* p = dllName + strlen(dllName) + 1;
				while (true)
				{
					WORD word = *(const WORD*)p;
					if (word != 0) break;
					p += sizeof(WORD);
					const char* funcName = p;
					p += strlen(funcName) + 1;
					map2[funcName] = const_cast<FARPROC*>(reinterpret_cast<const FARPROC*>(&codeAddrs[funcIndex]));
					funcIndex++;
				}
			}
			swap(this->map[dllNameLower], map2);
			break;
		}
		ImportDescriptor++;
	}
}

ImportReplacer::ImportReplacer() : Base(0)
{
	Init(GetModuleHandleA(NULL));
}

ImportReplacer::ImportReplacer(HMODULE Base)
{
	Init(Base);
}
void ImportReplacer::Init(HMODULE Base)
{
	if (Base == NULL) Base = GetModuleHandleA(NULL);
	if (memoryUnlocker.UnlockExists())
	{
		memoryUnlocker.Lock();
	}
	this->map.clear();
	this->Base = Base;
	//GetImports();
}
bool ImportReplacer::ReplaceImport(LPCSTR dllName, LPCSTR functionName, FARPROC replacementFunction, FARPROC* pOldFunction)
{
	string dllNameLowercase = ToLower(dllName);
	const auto find1 = map.find(dllNameLowercase);
	if (find1 == map.end()) return false;
	const auto &map2 = find1->second;
	const auto find2 = map2.find(functionName);
	if (find2 == map2.end()) return false;
	FARPROC* importAddress = find2->second;
	if (importAddress == NULL) return false;

	if (pOldFunction)
	{
		if (*pOldFunction == NULL)
		{
			*pOldFunction = *importAddress;
		}
	}
	if (!memoryUnlocker.UnlockExists())
	{
		memoryUnlocker.Unlock(importAddress);
	}
	*importAddress = replacementFunction;
	return true;
}

bool ModuleBaseNameMap::AddToCache(HMODULE module)
{
	char buffer[MAX_PATH];
	DWORD result = GetModuleBaseNameA(GetCurrentProcess(), module, buffer, MAX_PATH);
	if (result == 0) return false;
	map.emplace(module, ToLower(buffer));
	return true;
}

const string& ModuleBaseNameMap::GetModuleBaseNameLowercase(HMODULE module)
{
	if (module == NULL) return dummy;
again:
	auto found = map.find(module);
	if (found != map.end())
	{
		return found->second;
	}
	else
	{
		if (AddToCache(module))
		{
			goto again;
		}
		return dummy;
	}
	
}

void ImportMap::AddImport(LPCSTR dllName, LPCSTR functionName, FARPROC replacementFunction, FARPROC* pOldFunction)
{
	string moduleNameStr = ToLower(dllName);
back:
	auto found = map.find(moduleNameStr);
	if (found == map.end())
	{
		typedef decltype(found->second) map2Type;
		map.emplace(moduleNameStr, map2Type());
		goto back;
	}
	auto& map2 = found->second;
	string procNameStr = functionName;
	map2.emplace(procNameStr, std::make_pair(replacementFunction, pOldFunction));
}

FARPROC ImportMap::GetProcAddress(LPCSTR moduleName, LPCSTR procName) const
{
	string moduleNameStr = ToLower(moduleName);
	return GetProcAddress(moduleNameStr, procName);
}
FARPROC ImportMap::GetProcAddress(const string& moduleNameStr, LPCSTR procName) const
{
	auto found = map.find(moduleNameStr);
	if (found != map.end())
	{
		string procNameStr = procName;
		auto map2 = found->second;
		auto found2 = map2.find(procNameStr);
		if (found2 != map2.end())
		{
			FARPROC* pOldFunc = found2->second.second;
			if (pOldFunc != NULL && *pOldFunc == NULL)
			{
				*pOldFunc = ::GetProcAddress(GetModuleHandleA(moduleNameStr.c_str()), procName);
			}
			return found2->second.first;
		}
	}
	return NULL;
}
FARPROC ImportMap::GetProcAddress(HMODULE module, LPCSTR procName) const
{
	if (module == NULL) return NULL;
	const string &baseNameLowercase = baseNameMap.GetModuleBaseNameLowercase(module);
	return GetProcAddress(baseNameLowercase, procName);
}
void ImportMap::ReplaceImports(HMODULE Base) const
{
	if (Base == NULL) Base = GetModuleHandleA(NULL);
	ImportReplacer replacer(Base);
	for (auto iterator1 = map.cbegin(); iterator1 != map.cend(); iterator1++)
	{
		const string& moduleName = iterator1->first;
		replacer.GetImports(moduleName.c_str());
	}
	for (auto iterator1 = map.cbegin(); iterator1 != map.cend(); iterator1++)
	{
		const string& moduleName = iterator1->first;
		const auto& map2 = iterator1->second;
		for (auto iterator2 = map2.cbegin(); iterator2 != map2.cend(); iterator2++)
		{
			const string& procName = iterator2->first;
			FARPROC procAddress = iterator2->second.first;
			FARPROC* oldAddress = iterator2->second.second;
			replacer.ReplaceImport(moduleName.c_str(), procName.c_str(), procAddress, oldAddress);
		}
	}
}
