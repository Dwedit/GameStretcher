#include "MemoryUnlocker.h"

MemoryUnlocker::MemoryUnlocker() : memInfo(), oldProtect(0)
{

}
MemoryUnlocker::MemoryUnlocker(void *mem) : memInfo(), oldProtect(0)
{
	Unlock(mem);
}
MemoryUnlocker::~MemoryUnlocker()
{
	Lock();
}
void MemoryUnlocker::Unlock(void* mem)
{
	if (memInfo.BaseAddress == NULL)
	{
		BOOL okay;
		SIZE_T returnSize;
		returnSize = VirtualQuery(mem, &memInfo, sizeof(MEMORY_BASIC_INFORMATION));
		oldProtect = memInfo.Protect & 0xFF;
		if (oldProtect == PAGE_READWRITE ||
			oldProtect == PAGE_WRITECOPY ||
			oldProtect == PAGE_EXECUTE_READWRITE ||
			oldProtect == PAGE_EXECUTE_WRITECOPY) return;
		DWORD protect = PAGE_READWRITE;
		if (memInfo.Protect & 0xF0)
		{
			protect = PAGE_EXECUTE_READWRITE;
		}
		okay = VirtualProtect(memInfo.BaseAddress, memInfo.RegionSize, protect, &oldProtect);
	}
}
void MemoryUnlocker::Lock()
{
	if (oldProtect == PAGE_READWRITE ||
		oldProtect == PAGE_WRITECOPY ||
		oldProtect == PAGE_EXECUTE_READWRITE ||
		oldProtect == PAGE_EXECUTE_WRITECOPY) return;
	if (memInfo.BaseAddress)
	{
		BOOL okay;
		okay = VirtualProtect(memInfo.BaseAddress, memInfo.RegionSize, oldProtect, &oldProtect);
		memInfo = MEMORY_BASIC_INFORMATION();
		oldProtect = 0;
	}
}
bool MemoryUnlocker::UnlockExists() const
{
	return memInfo.BaseAddress != NULL;
}
