#include "VulkanUtilites.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"

VulkanImage ImageLayoutTransition(const VulkanImage& image, const VkImageSubresourceRange& resourceRange, VkImageLayout newLayout)
{
	VkImageMemoryBarrier imageBarrier = {};
	imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageBarrier.image = image.handle;
	imageBarrier.oldLayout = image.layout;
	imageBarrier.newLayout = newLayout;
	imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageBarrier.subresourceRange = resourceRange;

// 	imageBarrier.subresourceRange.baseArrayLayer = 0;
// 	imageBarrier.subresourceRange.layerCount = 1;
// 	imageBarrier.subresourceRange.baseMipLevel = 0;
// 	imageBarrier.subresourceRange.levelCount = 1;


	VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	// Layout specific definitions
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		// Has Stencil Component?
		if (image.format == VK_FORMAT_D32_SFLOAT_S8_UINT || image.format == VK_FORMAT_D24_UNORM_S8_UINT)
		{
			imageBarrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	if (image.layout == VK_IMAGE_LAYOUT_PREINITIALIZED &&
		newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
	{
		imageBarrier.srcAccessMask = VK_ACCESS_HOST_READ_BIT;
		imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		srcStageFlags = VK_PIPELINE_STAGE_HOST_BIT;
		dstStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (image.layout == VK_IMAGE_LAYOUT_PREINITIALIZED &&
		newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		imageBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		srcStageFlags = VK_PIPELINE_STAGE_HOST_BIT;
		dstStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (image.layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
		newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		srcStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
	}
	else if ((image.layout == VK_IMAGE_LAYOUT_UNDEFINED ||
		image.layout == VK_IMAGE_LAYOUT_PREINITIALIZED) &&
		newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		imageBarrier.srcAccessMask = 0;
		imageBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
			VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dstStageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}

	VulkanCommandBuffer* commandBuffer;
	// Queue family that I set up doesn't support this for transfer...
	if (!!(dstStageFlags & (VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)) )
	{
		commandBuffer = VulkanCommandBufferManager::GetActiveGraphicsBuffer();
	}
	else
	{
		commandBuffer = VulkanCommandBufferManager::GetActiveTransferBuffer();
	}

	commandBuffer->ImageMemoryBarrier(
		srcStageFlags, dstStageFlags,
		0,
		1, &imageBarrier);

	VulkanImage ret = image;
	ret.layout = newLayout;
	return ret;
}

void CopyToDeviceBuffer(const VulkanBuffer & stagingBuffer, const VulkanBuffer & dstBuffer)
{
	VulkanCommandBuffer* commandBuffer = VulkanCommandBufferManager::GetActiveTransferBuffer();

	assert(stagingBuffer.size <= dstBuffer.size);
	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = dstBuffer.size;

	commandBuffer->CopyBuffer(stagingBuffer.handle, dstBuffer.handle, 1, &copyRegion);

}

void CopyBufferToImage(const VulkanBuffer& buffer, const VulkanImage& image)
{
	VulkanCommandBuffer* commandBuffer = VulkanCommandBufferManager::GetActiveTransferBuffer();

	constexpr uint32 numColorComponents = 4;
	Array<VkBufferImageCopy> regionsToCopy(image.levels);
	uint32 offset = 0;
	for (uint32 i = 0; i < image.levels; ++i)
	{
		uint32 width = image.width >> i;
		uint32 height = image.height >> i;

		VkBufferImageCopy region = {};
		region.bufferOffset = offset;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = i;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageExtent = { width, height, 1 };

		regionsToCopy[i] = region;

		offset += width * height * numColorComponents;
	}

	commandBuffer->CopyBufferToImage(buffer.handle, image.handle, 
		(VkImageLayout)(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL | VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		regionsToCopy.Size(), regionsToCopy.GetData());
}
