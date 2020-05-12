#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>

//RAII memory read-write unlocker for Win32 (unlocks only one block)
class MemoryUnlocker
{
	MEMORY_BASIC_INFORMATION memInfo;
	DWORD oldProtect;
public:
	MemoryUnlocker();
	MemoryUnlocker(void *mem);
	~MemoryUnlocker();
	void Unlock(void* mem);
	void Lock();
	bool UnlockExists() const;
};
