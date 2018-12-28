#pragma once

#include "Graphics.h"

struct Texture;

struct VulkanImage
{
	VkImage handle = VK_NULL_HANDLE;
	VkDeviceMemory allocatedMemory = VK_NULL_HANDLE;
	VkImageLayout layout;
	VkFormat format;
	uint32 width = 0;
	uint32 height = 0;
	uint32 levels = 1;
	uint32 pad[1] = { 0 };

	bool Initialized() const { return handle != VK_NULL_HANDLE && allocatedMemory != VK_NULL_HANDLE; }
};

struct VulkanTexture
{
	VulkanImage image;
	// TODO - Consider making VkImageView its own structure - probably not but think about it
	VkImageView imageView = VK_NULL_HANDLE;
	VkSampler sampler = VK_NULL_HANDLE;
	Texture* cpuTexture = nullptr;
	uint32 pad[1] = { 0 };

	VulkanTexture() = default;

	// TODO - Might consider implementing copying for these instead of leaving them default
	VulkanTexture(const VulkanTexture&) = default;
	VulkanTexture& operator=(const VulkanTexture&) = default;

	VulkanTexture(VulkanTexture&& other) noexcept
		: image(std::move(other.image)),
		imageView(other.imageView),
		sampler(other.sampler)
	{
		other.imageView = VK_NULL_HANDLE;
		other.sampler = VK_NULL_HANDLE;
	}

	VulkanTexture& operator=(VulkanTexture&& other) noexcept
	{
		if (&other != this && other.image.Initialized())
		{
			image = std::move(other.image);
			imageView = other.imageView;
			sampler = other.sampler;

			other.imageView = VK_NULL_HANDLE;
			other.sampler = VK_NULL_HANDLE;
		}

		return *this;
	}
};

// TODO - This is getting big enough to make it a class
struct VulkanBuffer
{
	VkBuffer handle = VK_NULL_HANDLE;
	VkDeviceSize size = 0;
	VkDeviceMemory allocatedMemory = VK_NULL_HANDLE;

	VulkanBuffer() = default;

	// TODO - Might consider implementing copying for these instead of leaving them default
	VulkanBuffer(const VulkanBuffer&) = default;
	VulkanBuffer& operator=(const VulkanBuffer&) = default;

	VulkanBuffer(VulkanBuffer&& other) noexcept
		: handle(other.handle),
		size(other.size),
		allocatedMemory(other.allocatedMemory)
	{
		other.handle = VK_NULL_HANDLE;
		other.size = 0;
		other.allocatedMemory = VK_NULL_HANDLE;
	}

	VulkanBuffer& operator=(VulkanBuffer&& other) noexcept
	{
		if (&other != this && other.Initialized())
		{
			handle = other.handle;
			size = other.size;
			allocatedMemory = other.allocatedMemory;

			other.handle = VK_NULL_HANDLE;
			other.size = 0;
			other.allocatedMemory = VK_NULL_HANDLE;
		}

		return *this;
	}

	bool Initialized() const { return handle != VK_NULL_HANDLE && allocatedMemory != VK_NULL_HANDLE; }
};
