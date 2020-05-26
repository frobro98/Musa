
#include "Platform/Platform.hpp"
#include "Platform/PlatformMemory.hpp"

namespace Memory
{
void* PlatformAlloc(size_t size)
{
	void* alloc = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	return alloc;
}

void PlatformFree(void* p)
{
	VirtualFree(p, 0, MEM_RELEASE);
}
bool PlatformProtect(void* p, size_t size, PlatformProtectionKind procKind)
{
	DWORD mode = 0;
	switch (procKind)
	{
		case Memory::PlatformProtectionKind::ReadWrite:
		case Memory::PlatformProtectionKind::Write:
		{
			mode = PAGE_READWRITE;
		}break;

		case Memory::PlatformProtectionKind::NoAccess:
		default:
		{
			mode = PAGE_NOACCESS;
		}break;

		case Memory::PlatformProtectionKind::Read:
		{
			mode = PAGE_READONLY;
		}break;
	}
	DWORD oldProc;
	return VirtualProtect(p, size, mode, &oldProc);
}
NODISCARD PlatformMemoryInfo GetPlatformMemoryInfo()
{
	static PlatformMemoryInfo memInfo;
	if (memInfo.pageSize == 0)
	{
		MEMORYSTATUSEX status = {};
		status.dwLength = sizeof(status);
		::GlobalMemoryStatusEx(&status);

		SYSTEM_INFO sysInfo = {};
		GetSystemInfo(&sysInfo);

		memInfo.totalPhys = status.ullTotalPhys;
		memInfo.totalVirtual = status.ullTotalVirtual;
		memInfo.pageSize = sysInfo.dwPageSize;
		memInfo.allocationGranularity = sysInfo.dwAllocationGranularity;
	}
	return memInfo;
}
}
