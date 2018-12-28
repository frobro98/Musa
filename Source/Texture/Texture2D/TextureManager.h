#pragma once

#include "PlatformDefinitions.h"
// TODO - Move String up a directory level....or make Core required for every lib
#include "String/String.h"
#include "File/Path.hpp"
#include "TextureChunk.h"
#include "ImageFormats.h"

struct Texture;

class TextureManager
{
public:
	static constexpr const char* DefaultFileName = "default.tga";
	// TODO - Might not be the best place. Could be in Texture scope
	static constexpr const char* DefaultTexture = "_default-texture";

	static Texture* LoadTextureFromFile(const Path& textureFilePath, const String& textureName);
	static Texture* LoadTexture(Array<uint8>& textureData, const char* textureName);
	static void UnloadTexture(const char* textureName);
	static Texture* FindTexture(const char* textureName);

	static Texture* CompressTextureData(const TextureChunk& chunkData, uint8* textureData, const char* textureName, ImageFormat format);
	static Texture* CompressTextureData(Array<uint8>& textureData, const char* textureName);

private:
	Texture* Compress(const TextureChunk& chunk, uint8* textureData, ImageFormat format);

private:
	static TextureManager& Instance();

	struct TextureNode
	{
		TextureNode* next = nullptr;
		String textureName;
		Texture* texture = nullptr;
	};

	TextureNode* texHead = nullptr;
};