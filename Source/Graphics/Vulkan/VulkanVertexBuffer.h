#pragma once

#include "VulkanAbstractions.h"
#include "Containers/DynamicArray.hpp"

struct Vertex;
class VulkanDevice;
class VulkanCommandBuffer;
class ResourceBlob;

class VulkanVertexBuffer
{
public:
	VulkanVertexBuffer(VulkanDevice& device, const DynamicArray<Vertex>& vertices);
	~VulkanVertexBuffer();

	VulkanVertexBuffer(const VulkanVertexBuffer&);
	//VulkanVertexBuffer& operator=(const VulkanVertexBuffer&) = delete;

	void Bind(VulkanCommandBuffer* cmdBuffer) const;

	inline VulkanBuffer& GetBuffer() { return *vertexBuffer; }

private:
	VulkanDevice& logicalDevice;
	VulkanBuffer* vertexBuffer;
	
};

// TODO - this is the exact same class as VulkanVertexBuffer. Needs to be better...
class VulkanWeightsBuffer
{
public:
	VulkanWeightsBuffer(VulkanDevice& device, const DynamicArray<VertexBoneWeights>& weights);
	~VulkanWeightsBuffer();

	VulkanWeightsBuffer(const VulkanWeightsBuffer&) = delete;
	VulkanWeightsBuffer& operator=(const VulkanWeightsBuffer&) = delete;

	bool IsValid() const { return weightsBuffer != nullptr; }
	void Bind(VulkanCommandBuffer* cmdBuffer) const;

	inline VulkanBuffer& GetBuffer() { return *weightsBuffer; }

private:
	VulkanDevice& logicalDevice;
	VulkanBuffer* weightsBuffer;

};

using VertexBuffer = VulkanVertexBuffer;
using WeightsBuffer = VulkanWeightsBuffer;