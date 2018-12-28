#include "VulkanUniformBuffer.h"
#include "VulkanMemory.h"
#include "VulkanDevice.h"

VulkanUniformBuffer::VulkanUniformBuffer()
{
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
	if (mappedData != nullptr)
	{
		VulkanMemory::UnmapMemory(uniformBuffer.allocatedMemory);
		mappedData = nullptr;
	}
	VulkanMemory::DeallocateBuffer(uniformBuffer);
}

void VulkanUniformBuffer::Initialize(uint32 bufferSize)
{
	uniformBuffer = VulkanMemory::AllocateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void VulkanUniformBuffer::UpdateUniforms(const void* bufferData)
{
	if (mappedData == nullptr)
	{
		mappedData = VulkanMemory::MapMemory(uniformBuffer.allocatedMemory, uniformBuffer.size);
	}
	Memcpy(mappedData, (size_t)uniformBuffer.size, bufferData, (size_t)uniformBuffer.size);
	VulkanMemory::UnmapMemory(uniformBuffer.allocatedMemory);
	mappedData = nullptr;
}

void * VulkanUniformBuffer::GetMappedData()
{
	if (mappedData == nullptr)
	{
		mappedData = VulkanMemory::MapMemory(uniformBuffer.allocatedMemory, uniformBuffer.size);
	}
	return mappedData;
}
