#include "MemorySerializer.hpp"

void MemorySerializer::SerializeData(const void* data, uint32 dataSize)
{
	const uint8* byteData = reinterpret_cast<const uint8*>(data);
	for (uint32 i = 0; i < dataSize; ++i)
	{
		memoryData.Add(byteData[i]);
	}
}

Array<uint8> MemorySerializer::GetSerializedData() const
{
	return memoryData;
}
