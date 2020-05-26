#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

namespace Memory
{
enum class PlatformProtectionKind
{
	NoAccess,
	Read,
	Write,
	ReadWrite
};

struct PlatformMemoryInfo
{
	uint64 totalPhys;
	uint64 totalVirtual;
	uint32 pageSize; // (4K)
	uint32 allocationGranularity; // (64K) VirtualAlloc rounds up to this, which essentially means that addresses are essentially aligned
};

// Allocate Memory
NODISCARD CORE_API void* PlatformAlloc(size_t size);
// Free Memory
CORE_API void PlatformFree(void* p);
// Protect Page Memory
NODISCARD CORE_API bool PlatformProtect(void* p, size_t size, PlatformProtectionKind procKind);
// Get Platform Memory Constants
NODISCARD CORE_API PlatformMemoryInfo GetPlatformMemoryInfo();
}
