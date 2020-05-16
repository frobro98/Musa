// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/MemoryBuffer.hpp"
#include "Serialization/SerializeBase.hpp"

class MemorySerializer : public SerializeBase
{
public:
	MemorySerializer() = default;

	virtual void SerializeData(const void* data, size_t dataSize) override;

	MemoryBuffer& GetSerializedData() const;

private:
	MemoryBuffer& memoryData;
};