#pragma once

#include "Vulkan/VulkanAbstractions.h"

struct VulkanTexture : public NativeTexture
{
	VkImageView imageView = VK_NULL_HANDLE;
	VulkanImage* image = nullptr;
	const VulkanDevice& logicalDevice;

	VulkanTexture(VulkanDevice& device, VulkanImage& allocedImage);

	~VulkanTexture();

	// TODO - Might consider implementing copying for these instead of leaving them default
	VulkanTexture(const VulkanTexture&) = default;
	VulkanTexture& operator=(const VulkanTexture&) = default;

	VulkanTexture(VulkanTexture&& other) noexcept
		: imageView(other.imageView),
		image(other.image),
		logicalDevice(other.logicalDevice)
	{
		other.image = nullptr;
		other.imageView = VK_NULL_HANDLE;
	}

	VulkanTexture& operator=(VulkanTexture&& other) noexcept
	{
		if (&other != this && other.image->Initialized())
		{
			image = other.image;
			imageView = other.imageView;

			other.image = nullptr;
			other.imageView = VK_NULL_HANDLE;
		}

		return *this;
	}
};