#include "MemoryDeserializer.hpp"

MemoryDeserializer::MemoryDeserializer(DynamicArray<uint8>& memory)
	: memoryData(memory)
{
}

void MemoryDeserializer::DeserializeData(void* data, uint64 dataSize)
{
	Assert(dataSize <= memoryData.Size() - bufferReadLoc);
	Memcpy(data, dataSize, &memoryData[(uint32)bufferReadLoc], dataSize);
	bufferReadLoc += dataSize;
}

DynamicArray<uint8> MemoryDeserializer::GetSerializedData() const
{
	return memoryData;
}
