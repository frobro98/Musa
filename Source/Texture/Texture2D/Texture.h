#pragma once

#include "PlatformDefinitions.h"
#include "Containers/DynamicArray.hpp"
#include "String/String.h"
#include "ImageFormats.h"
#include "Texture2D/MipmapLevel.hpp"

struct VulkanTexture;
class SerializeBase;
class DeserializeBase;

struct Texture
{
	String name;
	DynamicArray<MipmapLevel> mipLevels;
	VulkanTexture* gpuResource = nullptr;
	ImageFormat format = ImageFormat::Invalid;

	// Texture size including all of the mip map levels
	uint32 TotalSize() const;

	// TODO - This belongs more on the gpu side of things. Deals with sampler info
	int32 wrapS;
	int32 wrapT;
	int32 minFilter;
	int32 magFilter;
	uint32 pad[1] = { 0 };
};

ResourceBlob ConstructBlobOfMipLevels(const DynamicArray<MipmapLevel>& levels);

void Serialize(SerializeBase& ser, const MipmapLevel& level);
void Deserialize(DeserializeBase& ser, MipmapLevel& level);

void Serialize(SerializeBase& ser, const Texture& tex);
void Deserialize(DeserializeBase& ser, Texture& tex);