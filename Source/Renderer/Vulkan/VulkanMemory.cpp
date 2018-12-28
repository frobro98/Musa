
#include "VulkanMemory.h"
#include "VulkanDevice.h"

#if GRAPHICS_API_VK

void VulkanMemory::Initialize(VulkanDevice * device)
{
	// TODO - Initialize memory tracking as well as device
	Instance().logicalDevice = device;
}

// TODO - creating an image neeeeeds to be more flexible
VulkanImage VulkanMemory::AllocateImage(
	uint32 imgWidth, uint32 imgHeight,
	VkFormat imgFormat, uint32 mipMapLevels,
	VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags memoryProperties)
{
	VulkanImage image = {};
	image.layout = VK_IMAGE_LAYOUT_PREINITIALIZED;
	image.format = imgFormat;
	image.width = imgWidth;
	image.height = imgHeight;
	image.levels = mipMapLevels;

	// TODO - Figure out if I should move this create info where all the others live
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.initialLayout = image.layout;
	imageInfo.format = imgFormat;
	imageInfo.extent = { imgWidth, imgHeight, 1};
	imageInfo.arrayLayers = 1;
	imageInfo.mipLevels = mipMapLevels;
	imageInfo.tiling = tiling;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VulkanDevice* device = Instance().logicalDevice;
	VkImage img;
	CHECK_VK(vkCreateImage(device->GetNativeHandle(), &imageInfo, nullptr, &img));

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(device->GetNativeHandle(), img, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = QueryMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

	VkDeviceMemory mem;
	CHECK_VK(vkAllocateMemory(device->GetNativeHandle(), &allocInfo, nullptr, &mem));
	CHECK_VK(vkBindImageMemory(device->GetNativeHandle(), img, mem, 0));

	image.handle = img;
	image.allocatedMemory = mem;

	return image;
}

void VulkanMemory::DeallocateImage(VulkanImage& image)
{
	vkDestroyImage(Instance().logicalDevice->GetNativeHandle(), image.handle, nullptr);
	vkFreeMemory(Instance().logicalDevice->GetNativeHandle(), image.allocatedMemory, nullptr);
	image.handle = VK_NULL_HANDLE;
}

VulkanBuffer VulkanMemory::AllocateBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
{
	VulkanBuffer buffer;
	buffer.size = bufferSize;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;
	bufferInfo.usage = usageFlags;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VulkanDevice* device = Instance().logicalDevice;
	CHECK_VK(vkCreateBuffer(device->GetNativeHandle(), &bufferInfo, nullptr, &buffer.handle));

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device->GetNativeHandle(), buffer.handle, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = QueryMemoryType(memoryRequirements.memoryTypeBits, memoryFlags);

	CHECK_VK(vkAllocateMemory(device->GetNativeHandle(), &allocInfo, nullptr, &buffer.allocatedMemory));
	CHECK_VK(vkBindBufferMemory(device->GetNativeHandle(), buffer.handle, buffer.allocatedMemory, 0));

	return buffer;
}

void VulkanMemory::DeallocateBuffer(VulkanBuffer& buffer)
{
	vkDestroyBuffer(Instance().logicalDevice->GetNativeHandle(), buffer.handle, nullptr);
	vkFreeMemory(Instance().logicalDevice->GetNativeHandle(), buffer.allocatedMemory, nullptr);
	buffer.handle = VK_NULL_HANDLE;
}

void* VulkanMemory::MapMemory(VkDeviceMemory memory, VkDeviceSize memorySize)
{
	void* data;
	CHECK_VK(vkMapMemory(Instance().logicalDevice->GetNativeHandle(), memory, 0, memorySize, 0, &data));
	return data;
}

void VulkanMemory::UnmapMemory(VkDeviceMemory memory)
{
	vkUnmapMemory(Instance().logicalDevice->GetNativeHandle(), memory);
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

VulkanMemory & VulkanMemory::Instance()
{
	static VulkanMemory vmInstance;
	return vmInstance;
}

#endif