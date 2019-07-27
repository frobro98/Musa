
#include "VulkanMemory.h"
#include "VulkanDevice.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"

void VulkanMemory::Initialize(VulkanDevice * device)
{
	// TODO - Initialize memory tracking as well as device
	Instance().logicalDevice = device;
}

uint32 VulkanMemory::QueryMemoryType(uint32 typeFilter, VkMemoryPropertyFlags propertyFlags)
{
	uint32 typeIndex = 0;
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(Instance().logicalDevice->GetPhysicalDevice(), &memProperties);
	for (uint32 i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
		{
			typeIndex = i;
		}
	}

	return typeIndex;
}

VulkanMemory& VulkanMemory::Instance()
{
	static VulkanMemory vmInstance;
	return vmInstance;
}
