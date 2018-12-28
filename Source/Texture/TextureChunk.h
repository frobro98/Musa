#pragma once

#include "Platform.h"
#include "Texture2D/Texture.h"
#include "ImageFormats.h"

struct TextureChunk
{
	uint32 width;
	uint32 height;
	uint32 size;
	ImageFormat format;
};