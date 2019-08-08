#pragma once

// TODO - Rename the file to match the name of the struct

#include "Math/BoundsVolumes.hpp"
#include "Math/Matrix.h"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct MaterialRenderInfo;

// TODO - There needs to be different ones for this and for animated meshes!
struct MeshRenderInfo 
{
	~MeshRenderInfo()
	{
		delete vertexBuffer;
		delete indexBuffer;
		delete transformBuffer;
	}

	SphereBounds bounds;
	MaterialRenderInfo* meshMaterial = nullptr;
	NativeVertexBuffer* vertexBuffer = nullptr;
	NativeIndexBuffer* indexBuffer = nullptr;
	NativeUniformBuffer* transformBuffer = nullptr;
};
