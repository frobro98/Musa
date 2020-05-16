// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/MemoryBuffer.hpp"
#include "Serialization/DeserializeBase.hpp"

class MemoryDeserializer : public DeserializeBase
{
public:
	MemoryDeserializer(MemoryBuffer& memory);

	virtual void DeserializeData(void* data, size_t dataSize) override;

	MemoryBuffer& GetSerializedData() const;

private:
	MemoryBuffer& memoryData;
	uint64 bufferReadLoc = 0;
};