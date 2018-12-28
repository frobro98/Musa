#include "MemoryDeserializer.hpp"

MemoryDeserializer::MemoryDeserializer(Array<uint8>& memory)
	: memoryData(memory)
{
}

void MemoryDeserializer::DeserializeData(void* data, uint32 dataSize)
{
	Assert(dataSize <= memoryData.Size() - bufferReadLoc);
	Memcpy(data, dataSize, &memoryData[bufferReadLoc], dataSize);
	bufferReadLoc += dataSize;
}

Array<uint8> MemoryDeserializer::GetSerializedData() const
{
	return memoryData;
}
