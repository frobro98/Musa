#pragma once

#include "Graphics.h"
#include "VulkanAbstractions.h"

class VulkanUniformBuffer
{
public:
	VulkanUniformBuffer() = default;
	//~VulkanUniformBuffer();

	VulkanUniformBuffer(const VulkanUniformBuffer&) = delete;
	VulkanUniformBuffer& operator=(const VulkanUniformBuffer&) = delete;

	void Initialize(const VulkanDevice& device, uint32 bufferSize);
	void UpdateUniforms(const void* bufferData);

	inline VulkanBuffer& GetBuffer() { return *uniformBuffer; }

private:
	VulkanBuffer* uniformBuffer;
	
};


typedef VulkanUniformBuffer UniformBuffer;
