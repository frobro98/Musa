#pragma once

#include "Graphics.h"
#include "VulkanAbstractions.h"

class VulkanUniformBuffer
{
public:
	//VulkanUniformBuffer(VulkanDevice* device);
	VulkanUniformBuffer();
	~VulkanUniformBuffer();

	VulkanUniformBuffer(const VulkanUniformBuffer&) = delete;
	VulkanUniformBuffer& operator=(const VulkanUniformBuffer&) = delete;

	void Initialize(uint32 bufferSize);
	void UpdateUniforms(const void* bufferData);

	inline VulkanBuffer& GetBuffer() { return uniformBuffer; }

	void* GetMappedData();

private:
	VulkanBuffer uniformBuffer;
	void* mappedData = nullptr;
	uint32 pad[1] = { 0 };
};


typedef VulkanUniformBuffer UniformBuffer;
