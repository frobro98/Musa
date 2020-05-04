#pragma once

#include "PlatformDefinitions.h"
#include "Containers/DynamicArray.hpp"
// TODO - Move String up a directory level....or make Core required for every lib
#include "String/String.h"
#include "Path/Path.hpp"
#include "Texture/TextureChunk.h"
#include "Texture/ImageFormats.h"

struct Texture;

class TextureManager
{
public:
	static constexpr const char* DefaultFileName = "default.tex";
	// TODO - Might not be the best place. Could be in Texture scope
	static constexpr const char* DefaultTexture = "_default-texture";

	// Only exists because it's a static object and it needs to destroyed before the vulkan instance
	void Deinitialize();

	Texture* LoadTextureFromFile(const Path& textureFilePath, const String& textureName);
	Texture* LoadTexture(DynamicArray<uint8>& textureData, const char* textureName);
	void UnloadTexture(const char* textureName);
	Texture* FindTexture(const char* textureName);

	Texture* CompressTextureData(const TextureChunk& chunkData, uint8* textureData, const char* textureName, ImageFormat format);
	Texture* CompressTextureData(DynamicArray<uint8>& textureData, const char* textureName);

	// TODO - This is to get around the problem of global texture not having gpu resources...need to fix this issue somehow
	void AddTexture(Texture& tex);

private:
	Texture* Compress(const TextureChunk& chunk, uint8* textureData, ImageFormat format);
	void ConfigureNativeTexture(Texture& texture);

private:
	DynamicArray<Texture*> texturesLoaded;
};

TextureManager& GetTextureManager();
