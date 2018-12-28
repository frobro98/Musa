#pragma once

#include "Containers/Array.h"
#include "Serialization/DeserializeBase.hpp"

class MemoryDeserializer : public DeserializeBase
{
public:
	MemoryDeserializer(Array<uint8>& memory);

	virtual void DeserializeData(void* data, uint32 dataSize) override;

	Array<uint8> GetSerializedData() const;

private:
	Array<uint8> memoryData;
	uint32 bufferReadLoc = 0;
};