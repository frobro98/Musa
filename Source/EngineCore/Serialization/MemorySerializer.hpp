// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/DynamicArray.hpp"
#include "Serialization/SerializeBase.hpp"

class MemorySerializer : public SerializeBase
{
public:
	MemorySerializer() = default;

	virtual void SerializeData(const void* data, uint32 dataSize) override;

	DynamicArray<uint8> GetSerializedData() const;

private:
	DynamicArray<uint8> memoryData;
};