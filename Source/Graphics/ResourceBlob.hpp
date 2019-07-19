#pragma once

#include "Types/Intrinsics.hpp"

class SerializeBase;
class DeserializeBase;

class ResourceBlob final
{
public:
	ResourceBlob() = default;
	ResourceBlob(uint8* blobData, uint32 blobSize);
	ResourceBlob(const ResourceBlob& other);
	ResourceBlob(ResourceBlob&& other);
	~ResourceBlob();

	ResourceBlob& operator=(const ResourceBlob& other);
	ResourceBlob& operator=(ResourceBlob&& other);

	void MergeWith(const ResourceBlob& other);

	inline uint8* GetData() { return data; }
	inline const uint8* GetData() const { return data; }
	inline uint32 GetSize() const { return size; }

	friend ResourceBlob CombineBlobs(const ResourceBlob& blob0, const ResourceBlob& blob1);
	friend void Serialize(SerializeBase& ser, const ResourceBlob& blob);
	friend void Deserialize(DeserializeBase& ser, ResourceBlob& blob);
private:
	uint8* data = nullptr;
	uint32 size = 0;
	uint32 pad[1] = { 0 };
};
