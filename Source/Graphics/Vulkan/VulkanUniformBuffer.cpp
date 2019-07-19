#include "VulkanUniformBuffer.h"
#include "VulkanMemory.h"
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

void VulkanUniformBuffer::Initialize(const VulkanDevice& device, uint32 bufferSize)
{
	uniformBuffer = device.GetMemoryManager().AllocateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void VulkanUniformBuffer::UpdateUniforms(const void* bufferData)
{
	uniformBuffer->memory->LockForWrite();
	Memcpy(uniformBuffer->memory->GetMappedPtr(), static_cast<size_t>(uniformBuffer->memory->size), bufferData, static_cast<size_t>(uniformBuffer->memory->size));
	uniformBuffer->memory->Unlock();
}
