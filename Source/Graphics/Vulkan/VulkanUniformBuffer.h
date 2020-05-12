// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanAbstractions.h"
#include "Types/Uncopyable.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class VulkanUniformBuffer : public NativeUniformBuffer , private Uncopyable
{
public:
	VulkanUniformBuffer(const VulkanDevice& device, uint32 bufferSize);
	~VulkanUniformBuffer();

	void UpdateUniforms(const void* bufferData);

	inline VulkanBuffer& GetBuffer() const { return *uniformBuffer; }

private:
	const VulkanDevice& logicalDevice;
	VulkanBuffer* uniformBuffer;
	
};

class VulkanGlobalUniformBuffer
{
public:
	VulkanGlobalUniformBuffer(const VulkanDevice& device, uint32 totalMemory);
	~VulkanGlobalUniformBuffer();

	void AddDataToBuffer(const void* bufferData, uint32 dataSize);
	inline VulkanBuffer& GetBuffer() const { return *globalBuffer; }

private:
	const VulkanDevice& logicalDevice;
	VulkanBuffer* globalBuffer;
	uint32 bufferPosition;
	uint32 alignment;
};

