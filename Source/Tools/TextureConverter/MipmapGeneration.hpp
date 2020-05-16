// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

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