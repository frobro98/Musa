#pragma once

#include "Containers/Array.h"
#include "Serialization/SerializeBase.hpp"

class MemorySerializer : public SerializeBase
{
public:
	MemorySerializer() = default;

	virtual void SerializeData(const void* data, uint32 dataSize) override;

	Array<uint8> GetSerializedData() const;

private:
	Array<uint8> memoryData;
};