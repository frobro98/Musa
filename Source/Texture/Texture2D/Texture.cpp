#include "Texture.h"
#include "TextureUtilities.hpp"

namespace
{

ImageFormat DeserializeImageFormat(DeserializeBase& ser)
{
	uint32 formatInt;
	Deserialize(ser, formatInt);
	return static_cast<ImageFormat>(formatInt);
}

}

uint32 Texture::TotalSize() const
{
	uint32 totalSize = 0;
	for (const auto& level : mipLevels)
	{
		totalSize += level.mipData.GetSize();
	}
	return totalSize;
}

ResourceBlob ConstructBlobOfMipLevels(const DynamicArray<MipmapLevel>& levels)
{
	uint32 totalSize = 0;
	for (const auto& level : levels)
	{
		totalSize += level.mipData.GetSize();
	}
	
	uint8* data = new uint8[totalSize];
	uint8* iter = data;
	for (const auto& level : levels)
	{
		Memcpy(iter, level.mipData.GetSize(), level.mipData.GetData(), level.mipData.GetSize());
		iter += level.mipData.GetSize();
	}

	return ResourceBlob(data, totalSize);
}

void Serialize(SerializeBase& ser, const MipmapLevel& level)
{
	Serialize(ser, level.mipData);
	Serialize(ser, level.width);
	Serialize(ser, level.height);
}

void Deserialize(DeserializeBase& ser, MipmapLevel& level)
{
	Deserialize(ser, level.mipData);
	Deserialize(ser, level.width);
	Deserialize(ser, level.height);
}

void Serialize(SerializeBase& ser, const Texture& tex)
{
	Serialize(ser, tex.name);
	Serialize(ser, static_cast<uint32>(tex.format));
	Serialize(ser, tex.mipLevels);
}

void Deserialize(DeserializeBase& ser, Texture& tex)
{
	Deserialize(ser, tex.name);
	tex.format = DeserializeImageFormat(ser);
	Deserialize(ser, tex.mipLevels);
}