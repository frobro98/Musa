#pragma once

#include "Graphics.h"
#include "VulkanAbstractions.h"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class VulkanUniformBuffer : public NativeUniformBuffer
{
public:
	VulkanUniformBuffer(const VulkanDevice& device, uint32 bufferSize);
	~VulkanUniformBuffer();

	VulkanUniformBuffer(const VulkanUniformBuffer&) = delete;
	VulkanUniformBuffer& operator=(const VulkanUniformBuffer&) = delete;

	void UpdateUniforms(const void* bufferData);

	inline VulkanBuffer& GetBuffer() const { return *uniformBuffer; }

private:
	const VulkanDevice& logicalDevice;
	VulkanBuffer* uniformBuffer;
	
};

