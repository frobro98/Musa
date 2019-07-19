#pragma once

// TODO - Rename the file to match the name of the struct

#include "Math/BoundsVolumes.hpp"
#include "Math/Matrix.h"

#include "Graphics/Vulkan/VulkanVertexBuffer.h"
#include "Graphics/Vulkan/VulkanIndexBuffer.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"

struct MaterialRenderInfo;

// TODO - There needs to be different ones for this and for animated meshes!
struct MeshRenderInfo 
{
	SphereBounds bounds;
	MaterialRenderInfo* meshMaterial;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	UniformBuffer* transformBuffer;
};
