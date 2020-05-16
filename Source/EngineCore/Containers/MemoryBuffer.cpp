// Copyright 2020, Nathan Blane

#include "MemoryBuffer.hpp"
#include "Assertion.h"
#include "Platform.h"
#include "MemoryUtilities.h"

MemoryBuffer::MemoryBuffer(size_t size)
	: memSize(size)
{
	AllocMem();
}

MemoryBuffer::~MemoryBuffer()
{
	DeallocMem();
}

MemoryBuffer::MemoryBuffer(MemoryBuffer&& other) noexcept
	: memData(other.memData),
	memSize(other.memSize)
{
	other.memData = nullptr;
	other.memSize = 0;
}

MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer&& other) noexcept
{
	if (this != &other)
	{
		memData = other.memData;
		memSize = other.memSize;

		other.memData = nullptr;
		other.memSize = 0;
	}

	return *this;
}

void MemoryBuffer::Add(const void* data, size_t dataSize)
{
	ReallocMem(memSize + dataSize);
	Memcpy(memData + memSize, dataSize, data, dataSize);
	memSize += dataSize;
}

uint8 * MemoryBuffer::Offset(size_t offset)
{
	Assert(offset < memSize);
	return memData + offset;
}

void MemoryBuffer::IncreaseSize(size_t sizeToIncreaseBy)
{
	ReallocMem(memSize + sizeToIncreaseBy);
	memSize += sizeToIncreaseBy;
}

uint8* MemoryBuffer::GetData() const
{
	return memData;
}

size_t MemoryBuffer::Size() const
{
	return memSize;
}

void MemoryBuffer::AllocMem()
{
	memData = (uint8*)malloc(memSize);
}

void MemoryBuffer::ReallocMem(size_t newSize)
{
	uint8* tmp = memData;
	uint8* newData = (uint8*)malloc(newSize);
	memData = newData;
	Memcpy(memData, memSize, tmp, memSize);
	free(tmp);
}

void MemoryBuffer::DeallocMem()
{
	free(memData);
	memData = nullptr;
}

