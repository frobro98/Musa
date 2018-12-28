#pragma once

#include "EngineCore/Types.h"

struct Texture;

enum class MipMapFilter
{
	Box,
	Kaiser,
	FFT
};

uint32 GetMaxMipMapLevels(
	uint32 width, 
	uint32 height
);

bool GenerateMipMapLevels(
	Texture& texture,
	uint32 levelsToGen,
	MipMapFilter generationFilter
);