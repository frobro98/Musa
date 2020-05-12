// Copyright 2020, Nathan Blane

#pragma once

#include "Platform.h"

enum class Chunk
{
	VERTS_TYPE,
	WEIGHTS_TYPE,
	SKEL_TYPE,
	ANIM_TYPE,
	TEXTURE_TYPE
};

constexpr const unsigned int ChunkNameSize = 64;

struct ChunkHeader
{
	// data:
	char     chunkName[ChunkNameSize];
	uint32   chunkSize;
	uint32   hashNum;
	Chunk	 type;
};
