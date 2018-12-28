#pragma once

#include "PlatformDefinitions.h"
#include "Containers/Array.h"
#include "String/String.h"
#include "ImageFormats.h"

class SerializeBase;
class DeserializeBase;

struct MipMapLevel
{
	Array<uint8> imageData;
	uint32 width = 0;
	uint32 height = 0;

	const uint8* Data() const
	{
		return imageData.GetData();
	}

	uint32 Size() const
	{
		return imageData.Size();
	}
};

void Serialize(SerializeBase& ser, const MipMapLevel& level);
void Deserialize(DeserializeBase& ser, MipMapLevel& level);

struct Texture
{
	String name;
	Array<MipMapLevel> mipLevels;
	ImageFormat format = ImageFormat::Invalid;

	// Texture size including all of the mip map levels
	uint32 TotalSize() const;

	// TODO - This belongs more on the gpu side of things. Deals with sampler info
	int32 wrapS;
	int32 wrapT;
	int32 minFilter;
	int32 magFilter;

	int32 texID = -1;
};

void Serialize(SerializeBase& ser, const Texture& tex);
void Deserialize(DeserializeBase& ser, Texture& tex);