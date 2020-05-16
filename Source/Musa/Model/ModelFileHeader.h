// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "MemoryUtilities.h"
#include "Math/BoundsVolumes.hpp"

constexpr uint32 ObjectNameSize = 32;
constexpr uint32 TextureNameSize = 32;
constexpr uint32 TextureCount = 5;

#pragma pack(push, 1)
struct ModelFileHeader
{
	// object name
	char         objName[ObjectNameSize];

	// vertex buffer
	uint32 numVerts;
	uint32 vertBufferOffset;

	// trilist index 
	uint32 numFaces;
	uint32 facesBufferOffset;

	SphereBounds boundingSphere;

	ModelFileHeader()
	{
		Memset(this, 0x0, sizeof(ModelFileHeader));
	}
};
#pragma pack(pop)
