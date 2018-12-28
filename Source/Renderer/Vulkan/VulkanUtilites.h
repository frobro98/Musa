#pragma once

#include "VulkanAbstractions.h"

#if GRAPHICS_API_VK

VulkanImage ImageLayoutTransition(const VulkanImage& image, const VkImageSubresourceRange& resourceRange, VkImageLayout newLayout);
void CopyToDeviceBuffer(const VulkanBuffer& stagingBuffer, const VulkanBuffer& dstBuffer);
void CopyBufferToImage(const VulkanBuffer& buffer, const VulkanImage& image);

#endif