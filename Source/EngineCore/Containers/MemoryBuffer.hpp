// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/Uncopyable.hpp"

// Container used to store 64 bit sized data
class MemoryBuffer final : private Uncopyable
{
public:
	MemoryBuffer() = default;
	MemoryBuffer(size_t initialSize);
	~MemoryBuffer();

	MemoryBuffer(MemoryBuffer&& other) noexcept;
	MemoryBuffer& operator=(MemoryBuffer&& other) noexcept;

	void Add(const void* data, size_t dataSize);

	uint8* Offset(size_t offset);
	void IncreaseSize(size_t sizeToIncreaseBy);

	uint8* GetData() const;
	size_t Size() const;

private:
	void AllocMem();
	void ReallocMem(size_t newSize);
	void DeallocMem();

private:
	uint8* memData = nullptr;
	size_t memSize = 0;
};
