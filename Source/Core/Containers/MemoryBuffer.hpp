// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

// TODO - It makes sense for this to be copyable, but just make sure it makes sense later...
// Container used to store 64 bit sized data
class CORE_API MemoryBuffer final
{
public:
	MemoryBuffer() = default;
	MemoryBuffer(size_t initialSize);
	~MemoryBuffer();

	MemoryBuffer(const MemoryBuffer& other);
	MemoryBuffer& operator=(const MemoryBuffer& other);

	void Add(const void* data, size_t dataSize);

	uint8* Offset(size_t offset) const;
	void IncreaseSize(size_t sizeToIncreaseBy);

	uint8* GetData() const;
	size_t Size() const;

	void Copy(uint8* data, size_t size);

private:
	void AllocMem();
	void ReallocMem(size_t newSize);
	void DeallocMem();

private:
	uint8* memData = nullptr;
	size_t memSize = 0;
};
