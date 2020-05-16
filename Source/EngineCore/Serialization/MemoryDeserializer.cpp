// Copyright 2020, Nathan Blane

#include "Assertion.h"
#include "MemoryDeserializer.hpp"
#include "MemoryUtilities.h"

MemoryDeserializer::MemoryDeserializer(MemoryBuffer& memory)
	: memoryData(memory)
{
}

void MemoryDeserializer::DeserializeData(void* data, size_t dataSize)
{
	Assert(dataSize <= memoryData.Size() - bufferReadLoc);
	Memcpy(data, dataSize, memoryData.Offset(bufferReadLoc), dataSize);
	bufferReadLoc += dataSize;
}

MemoryBuffer& MemoryDeserializer::GetSerializedData() const
{
	return memoryData;
}
