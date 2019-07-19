#pragma once

#include "Containers/DynamicArray.hpp"
#include "Serialization/DeserializeBase.hpp"

class MemoryDeserializer : public DeserializeBase
{
public:
	MemoryDeserializer(DynamicArray<uint8>& memory);

	virtual void DeserializeData(void* data, uint64 dataSize) override;

	DynamicArray<uint8> GetSerializedData() const;

private:
	DynamicArray<uint8> memoryData;
	uint64 bufferReadLoc = 0;
};