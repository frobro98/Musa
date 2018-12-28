#include "VulkanStorageBuffer.h"
#include "VulkanMemory.h"
#include "MemoryUtilities.h"

VulkanStorageBuffer::~VulkanStorageBuffer()
{
	if (mappedData != nullptr)
	{
		VulkanMemory::UnmapMemory(storageBuffer.allocatedMemory);
	}
}

void VulkanStorageBuffer::Initialize(uint32 bufferSize)
{
	storageBuffer = VulkanMemory::AllocateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	mappedData = VulkanMemory::MapMemory(storageBuffer.allocatedMemory, storageBuffer.size);
}

void VulkanStorageBuffer::UpdateStorage(const void * bufferData)
{
	if (mappedData == nullptr)
	{
		mappedData = VulkanMemory::MapMemory(storageBuffer.allocatedMemory, storageBuffer.size);
	}
	Memcpy(mappedData, static_cast<size_t>(storageBuffer.size), bufferData, static_cast<size_t>(storageBuffer.size));
	VulkanMemory::UnmapMemory(storageBuffer.allocatedMemory);
	mappedData = nullptr;
}

void* VulkanStorageBuffer::GetMappedData()
{
	if (mappedData == nullptr)
	{
		mappedData = VulkanMemory::MapMemory(storageBuffer.allocatedMemory, storageBuffer.size);
	}
	return mappedData;
}
