#pragma once

#include "PlatformDefinitions.h"
#include "Collision/Primitives/Sphere.h"

#define OBJECT_NAME_SIZE   32
#define TEXTURE_NAME_SIZE  32
#define TEXTURE_COUNT	   5

#pragma pack(push, 1)
struct ModelFileHeader
{
	// object name
	char         objName[OBJECT_NAME_SIZE];

	// vertex buffer
	uint32 numVerts;
	uint32 vertBufferOffset;

	// trilist index 
	uint32 numFaces;
	uint32 facesBufferOffset;

	Sphere boundingSphere;

	ModelFileHeader()
	{
		memset(this, 0x0, sizeof(ModelFileHeader));
	}
};
#pragma pack(pop)
