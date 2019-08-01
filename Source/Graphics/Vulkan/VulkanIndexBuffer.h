#pragma once

#include "VulkanAbstractions.h"
#include "Containers/DynamicArray.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct Face;
class VulkanDevice;

class VulkanIndexBuffer : public NativeIndexBuffer
{
public:
	VulkanIndexBuffer(VulkanDevice& device, const DynamicArray<Face>& indices);
	~VulkanIndexBuffer();
	VulkanIndexBuffer(const VulkanIndexBuffer&) = delete;
	VulkanIndexBuffer& operator=(const VulkanIndexBuffer&) = delete;

	inline VulkanBuffer& GetBuffer() const { return *indexBuffer; }
	inline uint32 GetNumberOfIndicies() const { return numIndicies; }

private:
	VulkanDevice& logicalDevice;
	VulkanBuffer* indexBuffer;
	uint32 numIndicies;
};
