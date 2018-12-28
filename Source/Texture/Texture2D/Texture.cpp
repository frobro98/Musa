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
		totalSize += level.Size();
	}
	return totalSize;
}

void Serialize(SerializeBase& ser, const MipMapLevel& level)
{
	Serialize(ser, level.imageData);
	Serialize(ser, level.width);
	Serialize(ser, level.height);
}

void Deserialize(DeserializeBase& ser, MipMapLevel& level)
{
	Deserialize(ser, level.imageData);
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