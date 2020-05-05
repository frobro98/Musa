#pragma once

#include "VulkanAbstractions.h"
#include "Types/Uncopyable.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct Face;
class VulkanDevice;

class VulkanIndexBuffer : public NativeIndexBuffer, private Uncopyable
{
public:
	VulkanIndexBuffer(VulkanDevice& device, const DynamicArray<Face>& indices);
	~VulkanIndexBuffer();

	inline VulkanBuffer& GetBuffer() const { return *indexBuffer; }
	inline uint32 GetNumberOfIndicies() const { return numIndicies; }

private:
	VulkanDevice& logicalDevice;
	VulkanBuffer* indexBuffer;
	uint32 numIndicies;
};
