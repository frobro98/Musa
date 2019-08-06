#include "ResourceBlob.hpp"
#include "MemoryUtilities.h"
#include "Serialization/SerializeBase.hpp"
#include "Serialization/DeserializeBase.hpp"
#include "Assertion.h"

ResourceBlob::ResourceBlob(uint8* blobData, uint32 blobSize)
	: data(blobData),
	size(blobSize)
{
	Assert(blobData != nullptr);
	Assert(blobSize > 0);
}

ResourceBlob::ResourceBlob(const ResourceBlob& other)
	: data(other.data),
	size(other.size)
{
}

ResourceBlob::ResourceBlob(ResourceBlob&& other) noexcept
	:data(other.data),
	size(other.size)
{
	other.data = nullptr;
	other.size = 0;
}

ResourceBlob::~ResourceBlob()
{
	delete data;
}

ResourceBlob& ResourceBlob::operator=(const ResourceBlob& other)
{
	if (this != &other)
	{
		delete data;
		data = other.data;
		size = other.size;
	}

	return *this;
}

ResourceBlob& ResourceBlob::operator=(ResourceBlob&& other) noexcept
{
	if (this != &other)
	{
		data = other.data;
		size = other.size;
		other.data = nullptr;
		other.size = 0;
	}

	return *this;
}

void ResourceBlob::MergeWith(const ResourceBlob& other)
{
	uint32 newSize = size + other.size;
	uint8* tmp = new uint8[newSize];

	Memcpy(tmp, size, data, size);
	Memcpy(tmp + size, other.size, other.data, other.size);

	delete data;
	data = tmp;
	size = newSize;
}

ResourceBlob CombineBlobs(const ResourceBlob& blob0, const ResourceBlob& blob1)
{
	uint32 newSize = blob0.size + blob1.size;
	uint8* data = new uint8[newSize];

	Memcpy(data, blob0.size, blob0.data, blob0.size);
	Memcpy(data + blob0.size, blob1.size, blob1.data, blob1.size);

	return ResourceBlob(data, newSize);
}

void Serialize(SerializeBase& ser, const ResourceBlob& blob)
{
	Serialize(ser, blob.GetData(), blob.GetSize());
}

void Deserialize(DeserializeBase& ser, ResourceBlob& blob)
{
	uint8* data;
	uint32 size;
	Deserialize(ser, data, size);
	blob = ResourceBlob(data, size);
}
