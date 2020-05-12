#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <unordered_map>
#include <string>
#include <algorithm>

using std::unordered_map;
using std::pair;
using std::string;
using std::swap;

#include "MemoryUnlocker.h"

//Replaces DLL Imports in a module with custom functions
class ImportReplacer
{
	unordered_map<string, unordered_map<string, FARPROC*>> map;
	MemoryUnlocker memoryUnlocker;
	HMODULE Base;

public:
	ImportReplacer();
	ImportReplacer(HMODULE Base);
	void Init(HMODULE Base = NULL);
	bool ReplaceImport(LPCSTR dllName, LPCSTR functionName, FARPROC replacementFunction, FARPROC* pOldFunction);
	void GetImports(LPCSTR dllNameToMatch);
};
