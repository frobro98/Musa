#pragma once

#include "PlatformDefinitions.h"
#include "Containers/DynamicArray.hpp"
// TODO - Move String up a directory level....or make Core required for every lib
#include "String/String.h"
#include "File/Path.hpp"
#include "TextureChunk.h"
#include "ImageFormats.h"

struct Texture;

class TextureManager
{
public:
	static constexpr const char* DefaultFileName = "default.tex";
	// TODO - Might not be the best place. Could be in Texture scope
	static constexpr const char* DefaultTexture = "_default-texture";

	Texture* LoadTextureFromFile(const Path& textureFilePath, const String& textureName);
	Texture* LoadTexture(DynamicArray<uint8>& textureData, const char* textureName);
	void UnloadTexture(const char* textureName);
	Texture* FindTexture(const char* textureName);

	Texture* CompressTextureData(const TextureChunk& chunkData, uint8* textureData, const char* textureName, ImageFormat format);
	Texture* CompressTextureData(DynamicArray<uint8>& textureData, const char* textureName);

private:
	Texture* Compress(const TextureChunk& chunk, uint8* textureData, ImageFormat format);
	void ConfigureNativeTexture(Texture& texture);

private:
	DynamicArray<Texture*> texturesLoaded;
};

TextureManager& GetTextureManager();
