// Copyright 2020, Nathan Blane

#pragma once

#include "PlatformDefinitions.h"
#include "BasicTypes/UniquePtr.hpp"
#include "Containers/DynamicArray.hpp"
#include "String/String.h"
#include "Texture/ImageFormats.h"
#include "Texture/Texture2D/MipmapLevel.hpp"
#include "Texture/TextureSampleSettings.hpp"
#include "Texture/TextureDll.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class SerializeBase;
class DeserializeBase;

struct TEX_API Texture
{
	Texture() = default;
	Texture(uint8 r, uint8 g, uint8 b, uint8 a);

	uint32 GetWidth() const;
	uint32 GetHeight() const;

	String name;
	DynamicArray<MipmapLevel> mipLevels;
	UniquePtr<NativeTexture> gpuResource;
	ImageFormat format = ImageFormat::Invalid;
	TextureFilter filter = TextureFilter::Linear;
	TextureAddress addrMode = TextureAddress::Repeat;
	TextureMipMode mipMode = TextureMipMode::Linear;

	// Texture size including all of the mip map levels
	uint32 TotalSize() const;
};

ResourceBlob ConstructBlobOfMipLevels(const DynamicArray<MipmapLevel>& levels);

TEX_API void Serialize(SerializeBase& ser, const MipmapLevel& level);
TEX_API void Deserialize(DeserializeBase& ser, MipmapLevel& level);

TEX_API void Serialize(SerializeBase& ser, const Texture& tex);
TEX_API void Deserialize(DeserializeBase& ser, Texture& tex);

TEX_API Texture* WhiteTexture();
TEX_API Texture* BlackTexture();
