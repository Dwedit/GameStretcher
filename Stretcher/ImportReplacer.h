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

class ModuleBaseNameMap
{
	unordered_map<HMODULE, string> map;
	string dummy;
	bool AddToCache(HMODULE module);
public:
	const string &GetModuleBaseNameLowercase(HMODULE module);
};

class ImportMap
{
	unordered_map<string, unordered_map<string, pair<FARPROC, FARPROC*>>> map;
	mutable ModuleBaseNameMap baseNameMap;
public:
	void AddImport(LPCSTR dllName, LPCSTR functionName, FARPROC replacementFunction, FARPROC* pOldFunction);
	FARPROC GetProcAddress(LPCSTR moduleName, LPCSTR procName) const;
	FARPROC GetProcAddress(const string &moduleName, LPCSTR procName) const;
	FARPROC GetProcAddress(HMODULE module, LPCSTR procName) const;
	void ReplaceImports(HMODULE Base = NULL) const;
};
