#include "VulkanStageBuffers.h"
#include "VulkanMemory.h"
#include "VulkanUtilites.h"

#include "MemoryUtilities.h"

VulkanStageBuffer::VulkanStageBuffer(void * stagingData, size_t dataSize)
{
	stageBuffer = VulkanMemory::AllocateBuffer(
		dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	void* data = VulkanMemory::MapMemory(stageBuffer.allocatedMemory, stageBuffer.size);
	Memcpy(data, dataSize, stagingData, dataSize);
	VulkanMemory::UnmapMemory(stageBuffer.allocatedMemory);
}

VulkanStageBuffer::~VulkanStageBuffer()
{
	VulkanMemory::DeallocateBuffer(stageBuffer);
}

//////////////////////////////////////////////////////////////////////////

VulkanStageBuffer * VulkanStageBufferManager::StageData(void* data, size_t dataSize)
{
	VulkanStageBuffer* buffer = new VulkanStageBuffer(data, dataSize);
	Instance().stageBuffers.Add(buffer);
	return buffer;
}

void VulkanStageBufferManager::ResetBuffers()
{
	// TODO - cache the buffers, instead of deleting them
	for (uint32 i = 0; i < Instance().stageBuffers.Size(); ++i)
	{
		delete Instance().stageBuffers[i];
		Instance().stageBuffers.Remove(i);
	}
}

VulkanStageBufferManager & VulkanStageBufferManager::Instance()
{
	static VulkanStageBufferManager smInstance;
	return smInstance;
}

VulkanStageBufferManager::~VulkanStageBufferManager()
{
	// TODO - this will cache buffers. Need to actually delete them here
	ResetBuffers();
}
