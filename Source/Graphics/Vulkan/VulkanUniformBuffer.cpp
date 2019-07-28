#include "VulkanUniformBuffer.h"
#include "VulkanDevice.h"
#include "VulkanMemoryManager.hpp"
#include "VulkanBufferAllocation.hpp"


// VulkanUniformBuffer::~VulkanUniformBuffer()
// {
// 	if (mappedData != nullptr)
// 	{
// 		VulkanMemory::UnmapMemory(uniformBuffer->allocatedMemory);
// 		mappedData = nullptr;
// 	}
// 	VulkanMemory::DeallocateBuffer(uniformBuffer);
// }

VulkanUniformBuffer::VulkanUniformBuffer(const VulkanDevice & device, uint32 bufferSize)
	: logicalDevice(device)
{
	uniformBuffer = logicalDevice.GetMemoryManager().AllocateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
	logicalDevice.GetMemoryManager().DeallocateBuffer(*uniformBuffer);
}

void VulkanUniformBuffer::UpdateUniforms(const void* bufferData)
{
	uniformBuffer->memory->LockForWrite();
	Memcpy(uniformBuffer->memory->GetMappedPtr(), static_cast<size_t>(uniformBuffer->memory->size), bufferData, static_cast<size_t>(uniformBuffer->memory->size));
	uniformBuffer->memory->Unlock();
}
