// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

class SerializeBase;
class DeserializeBase;

// TODO - Currently, this object represents both general data, for resources such as textures, and for a collection of data, like index and vertex data. It might be good to separate this behavior
class ResourceBlob final
{
public:
	ResourceBlob() = default;
	ResourceBlob(uint8* blobData, size_t blobSize);
	ResourceBlob(const ResourceBlob& other);
	ResourceBlob(ResourceBlob&& other) noexcept;
	~ResourceBlob();

	ResourceBlob& operator=(const ResourceBlob& other);
	ResourceBlob& operator=(ResourceBlob&& other) noexcept;

	void MergeWith(const ResourceBlob& other);

	inline uint8* GetData() { return data; }
	inline const uint8* GetData() const { return data; }
	inline size_t GetSize() const { return size; }

	friend ResourceBlob CombineBlobs(const ResourceBlob& blob0, const ResourceBlob& blob1);
	friend void Serialize(SerializeBase& ser, const ResourceBlob& blob);
	friend void Deserialize(DeserializeBase& ser, ResourceBlob& blob);
private:
	uint8* data = nullptr;
	size_t size = 0;
};
