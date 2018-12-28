#pragma once

#include "VulkanAbstractions.h"
#include "Containers/Array.h"

struct Vertex;
class VulkanCommandBuffer;

class VulkanVertexBuffer
{
public:
	VulkanVertexBuffer(const Array<Vertex>& vertices);
	~VulkanVertexBuffer();

	VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
	VulkanVertexBuffer& operator=(const VulkanVertexBuffer&) = delete;

	void Bind(VulkanCommandBuffer* cmdBuffer) const;

	inline VulkanBuffer* GetBuffer() { return &vertexBuffer; }

private:
	VulkanBuffer vertexBuffer;
	
};

// TODO - this is the exact same class as VulkanVertexBuffer. Needs to be better...
class VulkanWeightsBuffer
{
public:
	VulkanWeightsBuffer(const Array<VertexBoneWeights>& weights);
	~VulkanWeightsBuffer();

	VulkanWeightsBuffer(const VulkanWeightsBuffer&) = delete;
	VulkanWeightsBuffer& operator=(const VulkanWeightsBuffer&) = delete;

	bool IsValid() const { return weightsBuffer.size > 0; }
	void Bind(VulkanCommandBuffer* cmdBuffer) const;

	inline VulkanBuffer* GetBuffer() { return &weightsBuffer; }

private:
	VulkanBuffer weightsBuffer;

};

using VertexBuffer = VulkanVertexBuffer;
using WeightsBuffer = VulkanWeightsBuffer;