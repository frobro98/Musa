#include "TextureManager.h"
#include "Texture.h"
#include "DirectoryLocations.h"
#include "Importers/TGAImporter.h"
#include "Importers/TextureCompression.h"
#include "MemoryUtilities.h"
#include "String/String.h"
#include "Graphics.h"
#include "Serialization/MemoryDeserializer.hpp"
#include "Archiver/FileDeserializer.hpp"
#include "Graphics/GraphicsInterface.hpp"

namespace
{
CompressionFormat ImageToCompressionFormat(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::BC1:
			return CompressionFormat::BC1;
		case ImageFormat::BC3:
			return CompressionFormat::BC3;
		case ImageFormat::BC7:
			return CompressionFormat::BC7;
		case ImageFormat::RGB_8u:
		case ImageFormat::RGB_8norm:
		case ImageFormat::RGB_16f:
		case ImageFormat::BGR_8u:
		case ImageFormat::RGBA_8u:
		case ImageFormat::RGBA_8norm:
		case ImageFormat::RGBA_16f:
		case ImageFormat::BGRA_8u:
		case ImageFormat::Gray_8u:
		case ImageFormat::D_32f:
		case ImageFormat::DS_32f_8u:
		case ImageFormat::DS_24f_8u:
		case ImageFormat::Invalid:
			Assert(false);
	}

	return CompressionFormat::Invalid;
}
}

Texture* TextureManager::LoadTextureFromFile(const Path& textureFilePath, const String& /*textureName*/)
{
	Texture* texture = new Texture;
	FileDeserializer deserializer(textureFilePath);
	Deserialize(deserializer, *texture);
	texture->wrapS = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->wrapT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->minFilter = VK_FILTER_LINEAR;
	texture->magFilter = VK_FILTER_LINEAR;

	ConfigureNativeTexture(*texture);

	texturesLoaded.Add(texture);

	return texture;
}

Texture* TextureManager::LoadTexture(DynamicArray<uint8>& textureData, const char* /*textureName*/)
{
	Texture* texture = new Texture;
	MemoryDeserializer deserializer(textureData);
	Deserialize(deserializer, *texture);
	texture->wrapS = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->wrapT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->minFilter = VK_FILTER_LINEAR;
	texture->magFilter = VK_FILTER_LINEAR;

	ConfigureNativeTexture(*texture);

	texturesLoaded.Add(texture);

	return texture;
}

Texture* TextureManager::CompressTextureData(const TextureChunk& chunkData, uint8* textureData, const char* /*textureName*/, ImageFormat format)
{
	Texture* texture = Compress(chunkData, textureData, format);

	texturesLoaded.Add(texture);

	return texture;
}

Texture* TextureManager::CompressTextureData(DynamicArray<uint8>& textureData, const char* /*textureName*/)
{
	Texture uncompressed;
	Texture compressed;
	MemoryDeserializer deserializer(textureData);
	Deserialize(deserializer, uncompressed);

	Compression::CompressForGPU(uncompressed, compressed, CompressionFormat::BC7);

	Texture* texture = new Texture;
	*texture = std::move(compressed);
	texture->wrapS = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->wrapT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->minFilter = VK_FILTER_LINEAR;
	texture->magFilter = VK_FILTER_LINEAR;

	texturesLoaded.Add(texture);

	return texture;
}

Texture* TextureManager::Compress(const TextureChunk& chunk, uint8* textureData, ImageFormat format)
{
	UNUSED(textureData, chunk, format);
	Texture uncompressed;
	Texture compressed;
// 	uncompressed.width = chunk.width;
// 	uncompressed.height = chunk.height;
// 	uncompressed.format = chunk.format;
// 	uncompressed.imageData = Array<uint8>(textureData, chunk.size);
// 	Compression::CompressForGPU(uncompressed, compressed, ImageToCompressionFormat(format));
// 	compressed.format = format;

	Texture* texture = new Texture;// (chunkData.width, chunkData.height, chunkData.format, std::move(data));
	*texture = std::move(compressed);
	texture->wrapS = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->wrapT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->minFilter = VK_FILTER_LINEAR;
	texture->magFilter = VK_FILTER_LINEAR;
	return texture;
}

void TextureManager::ConfigureNativeTexture(Texture& texture)
{
	ResourceBlob textureBlob = ConstructBlobOfMipLevels(texture.mipLevels);

	uint32 width = texture.mipLevels[0].width;
	uint32 height = texture.mipLevels[0].height;
	texture.gpuResource = GetGraphicsInterface().CreateInitializedTexture2D(textureBlob, width, height, texture.format, texture.mipLevels.Size(), TextureUsage::SampledResource);
}

void TextureManager::UnloadTexture(const char* textureName)
{
	for (uint32 i = 0; i < texturesLoaded.Size(); ++i)
	{
		if (textureName == texturesLoaded[i]->name)
		{
			delete texturesLoaded[i];
			texturesLoaded.Remove(i);
			break;
		}
	}
}

Texture* TextureManager::FindTexture(const char* textureName)
{
	for (uint32 i = 0; i < texturesLoaded.Size(); ++i)
	{
		if (textureName == texturesLoaded[i]->name)
		{
			return texturesLoaded[i];
		}
	}

	return nullptr;
}

TextureManager& GetTextureManager()
{
	static TextureManager tmInstance;
	return tmInstance;
}
