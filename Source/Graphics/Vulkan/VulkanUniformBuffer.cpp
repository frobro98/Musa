#include "VulkanUniformBuffer.h"
#include "VulkanDevice.h"
#include "VulkanMemoryManager.hpp"
#include "VulkanBufferAllocation.hpp"

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

VulkanGlobalUniformBuffer::VulkanGlobalUniformBuffer(const VulkanDevice& device, uint32 totalMemory)
	: logicalDevice(device)
{
	globalBuffer = logicalDevice.GetMemoryManager().AllocateBuffer(totalMemory, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

VulkanGlobalUniformBuffer::~VulkanGlobalUniformBuffer()
{
	logicalDevice.GetMemoryManager().DeallocateBuffer(*globalBuffer);
}

void VulkanGlobalUniformBuffer::AddDataToBuffer(const void* bufferData, uint32 dataSize)
{
	uint32 alignedBufferPos = Align(bufferPosition, alignment);
	if (alignedBufferPos + dataSize >= globalBuffer->memory->blockSize)
	{
		alignedBufferPos = 0;
	}
	globalBuffer->memory->LockForWrite();
	Memcpy((uint8*)globalBuffer->memory->GetMappedPtr() + alignedBufferPos, (size_t)dataSize, bufferData, (size_t)dataSize);
	globalBuffer->memory->Unlock();
}
