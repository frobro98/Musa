
#include "VulkanStagingBufferManager.hpp"
#include "VulkanMemory.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFence.hpp"
#include "VulkanDevice.h"

static uint32 QueryMemoryType(const VulkanDevice& device, uint32 typeFilter, VkMemoryPropertyFlags propertyFlags)
{
	uint32 typeIndex = 0;
	const VkPhysicalDeviceMemoryProperties& memoryProperties = device.GetMemoryProperties();
	for (uint32 i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) &&
			(memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
		{
			typeIndex = i;
		}
	}

	return typeIndex;
}


VulkanStagingBuffer::VulkanStagingBuffer(VkBuffer handle, VkDeviceSize size)
	: stagingHandle(handle),
	stagingSize(size)
{
}

void VulkanStagingBuffer::ReleaseMemory(const VulkanDevice& device)
{
	vkDestroyBuffer(device.GetNativeHandle(), stagingHandle, nullptr);
	vkFreeMemory(device.GetNativeHandle(), backingAllocation, nullptr);
}


VulkanStagingBufferManager::VulkanStagingBufferManager(const VulkanDevice& device)
	: logicalDevice(device)
{
}

VulkanStagingBufferManager::~VulkanStagingBufferManager()
{
}

VulkanStagingBuffer* VulkanStagingBufferManager::AllocateStagingBuffer(VkDeviceSize bufferSize)
{
	VulkanStagingBuffer* stagingBuffer = nullptr;
	for (auto buffer : resonentStagingBuffers)
	{
		if (buffer->stagingSize == bufferSize)
		{
			stagingBuffer = buffer;
			break;
		}
	}

	if (stagingBuffer == nullptr)
	{
		stagingBuffer = CreateFreshBuffer(bufferSize);
		resonentStagingBuffers.Add(stagingBuffer);
	}

	return stagingBuffer;
}

void VulkanStagingBufferManager::ReleaseStagingBuffer(VulkanCommandBuffer& cmdBuffer, VulkanStagingBuffer& buffer)
{
	DeferredStageReleaseElement elem = {};
	elem.stagingBuffer = &buffer;
	elem.associatedFence = cmdBuffer.GetFence();
	deferredReleaseBuffers.Add(elem);
	resonentStagingBuffers.RemoveAll(&buffer);
}

void VulkanStagingBufferManager::ProcessDeferredReleases()
{
	for (auto& elem : deferredReleaseBuffers)
	{
		FenceState status = elem.associatedFence->CheckFenceStatus();
		if (status == FenceState::Signaled)
		{
			//VulkanMemory::DeallocateBuffer(elem.stagingBuffer);
			elem.stagingBuffer->ReleaseMemory(logicalDevice);
			delete elem.stagingBuffer;
			elem.stagingBuffer = nullptr;
		}
	}

	for (int32 i = (int32)deferredReleaseBuffers.Size() - 1; i >= 0; --i)
	{
		uint32 index = (uint32)i;
		if (deferredReleaseBuffers[index].stagingBuffer == nullptr)
		{
			deferredReleaseBuffers.Remove(index);
		}
	}
}

VulkanStagingBuffer* VulkanStagingBufferManager::CreateFreshBuffer(VkDeviceSize size)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer bufferHandle = VK_NULL_HANDLE;
	CHECK_VK(vkCreateBuffer(logicalDevice.GetNativeHandle(), &bufferInfo, nullptr, &bufferHandle));

	VulkanStagingBuffer* stagingBuffer = new VulkanStagingBuffer(bufferHandle, size);

	AllocateMemoryFor(*stagingBuffer);

	return stagingBuffer;
}

void VulkanStagingBufferManager::AllocateMemoryFor(VulkanStagingBuffer& buffer)
{
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(logicalDevice.GetNativeHandle(), buffer.stagingHandle, &memoryRequirements);

	VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = QueryMemoryType(logicalDevice, memoryRequirements.memoryTypeBits, memoryFlags);
	
	CHECK_VK(vkAllocateMemory(logicalDevice.GetNativeHandle(), &allocInfo, nullptr, &buffer.backingAllocation));
	CHECK_VK(vkBindBufferMemory(logicalDevice.GetNativeHandle(), buffer.stagingHandle, buffer.backingAllocation, 0));

	CHECK_VK(vkMapMemory(logicalDevice.GetNativeHandle(), buffer.backingAllocation, 0, buffer.stagingSize, 0, &buffer.mappedData));
}
