// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Math/BoundsVolumes.hpp"
#include "Math/Matrix4.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct MaterialRenderInfo;

// TODO - There needs to be different ones for this and for animated meshes!
struct MeshRenderInfo 
{
	SphereBounds bounds;
	MaterialRenderInfo* meshMaterial = nullptr;
	NativeVertexBuffer* vertexBuffer = nullptr;
	NativeIndexBuffer* indexBuffer = nullptr;
	UniquePtr<NativeUniformBuffer> transformBuffer;
};
