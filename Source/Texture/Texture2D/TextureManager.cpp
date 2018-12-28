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
		case ImageFormat::RGB_8:
		case ImageFormat::BGR_8:
		case ImageFormat::RGBA_8:
		case ImageFormat::BGRA_8:
		case ImageFormat::Gray_8:
		case ImageFormat::Invalid:
			Assert(false);
	}

	return CompressionFormat::Invalid;
}
}

Texture* TextureManager::LoadTextureFromFile(const Path& textureFilePath, const String& textureName)
{
	Texture* texture = new Texture;
	FileDeserializer deserializer(textureFilePath);
	Deserialize(deserializer, *texture);
	texture->wrapS = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->wrapT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->minFilter = VK_FILTER_LINEAR;
	texture->magFilter = VK_FILTER_LINEAR;

	TextureNode* node = new TextureNode;
	node->texture = texture;
	node->textureName = textureName;
	node->next = Instance().texHead;
	Instance().texHead = node;

	return texture;
}

Texture* TextureManager::LoadTexture(Array<uint8>& textureData, const char* textureName)
{
	Texture* texture = new Texture;
	MemoryDeserializer deserializer(textureData);
	Deserialize(deserializer, *texture);
	texture->wrapS = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->wrapT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	texture->minFilter = VK_FILTER_LINEAR;
	texture->magFilter = VK_FILTER_LINEAR;

	TextureNode* node = new TextureNode;
	node->texture = texture;
	node->textureName = textureName;
	node->next = Instance().texHead;
	Instance().texHead = node;

	return texture;
}

Texture* TextureManager::CompressTextureData(const TextureChunk& chunkData, uint8* textureData, const char* textureName, ImageFormat format)
{
	Texture* texture = Instance().Compress(chunkData, textureData, format);

	TextureNode* node = new TextureNode;
	node->texture = texture;
	node->textureName = textureName;
	node->next = Instance().texHead;
	Instance().texHead = node;

	return texture;
}

Texture * TextureManager::CompressTextureData(Array<uint8>& textureData, const char * textureName)
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

	TextureNode* node = new TextureNode;
	node->texture = texture;
	node->textureName = textureName;
	node->next = Instance().texHead;
	Instance().texHead = node;

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

void TextureManager::UnloadTexture(const char * textureName)
{
	TextureNode* current = Instance().texHead;
	// TODO - need to look at this again.... didn't get it right the first time
	while (current != nullptr)
	{
		if (textureName == current->textureName)
		{
			delete current;

			break;
		}

		current = current->next;
	}
}

Texture * TextureManager::FindTexture(const char * textureName)
{
	TextureNode* current = Instance().texHead;
	while (current != nullptr)
	{
		if (textureName == current->textureName)
		{
			return current->texture;
		}

		current = current->next;
	}

	return nullptr;
}

TextureManager & TextureManager::Instance()
{
	static TextureManager tmInstance;
	return tmInstance;
}
