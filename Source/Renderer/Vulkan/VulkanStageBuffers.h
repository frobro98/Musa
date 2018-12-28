#pragma once

#include "Graphics.h"
#include "VulkanAbstractions.h"
#include "Containers/Array.h"

class VulkanStageBuffer
{
public:
	VulkanStageBuffer(void* stagingData, size_t dataSize);
	~VulkanStageBuffer();

	VulkanBuffer GetBuffer() const { return stageBuffer; }

private:
	VulkanBuffer stageBuffer;
};

class VulkanStageBufferManager
{
public:
	static VulkanStageBuffer* StageData(void* data, size_t dataSize);
	static void ResetBuffers();

private:
	static VulkanStageBufferManager& Instance();
	VulkanStageBufferManager() = default;
	~VulkanStageBufferManager();

private:
	Array<VulkanStageBuffer*> stageBuffers;
	
};