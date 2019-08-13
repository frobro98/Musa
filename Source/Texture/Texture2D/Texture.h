#pragma once

#include "PlatformDefinitions.h"
#include "Containers/DynamicArray.hpp"
#include "String/String.h"
#include "ImageFormats.h"
#include "Texture2D/MipmapLevel.hpp"
#include "TextureSampleSettings.hpp"

struct NativeTexture;
class SerializeBase;
class DeserializeBase;

struct Texture
{
	Texture() = default;
	Texture(uint8 r, uint8 g, uint8 b, uint8 a);
	~Texture();

	String name;
	DynamicArray<MipmapLevel> mipLevels;
	NativeTexture* gpuResource = nullptr;
	ImageFormat format = ImageFormat::Invalid;
	TextureFilter filter = TextureFilter::Linear;
	TextureAddress addrMode = TextureAddress::Repeat;
	TextureMipMode mipMode = TextureMipMode::Linear;

	// Texture size including all of the mip map levels
	uint32 TotalSize() const;
};

ResourceBlob ConstructBlobOfMipLevels(const DynamicArray<MipmapLevel>& levels);

void Serialize(SerializeBase& ser, const MipmapLevel& level);
void Deserialize(DeserializeBase& ser, MipmapLevel& level);

void Serialize(SerializeBase& ser, const Texture& tex);
void Deserialize(DeserializeBase& ser, Texture& tex);

Texture* WhiteTexture();
Texture* BlackTexture();
