#pragma once

#include "Graphics.h"

class VulkanDevice;
class ImageMemory;
class BufferMemory;
class ImageMemory;

struct VulkanImage
{
	VkImage handle = VK_NULL_HANDLE;
	VkImageLayout layout;
	VkFormat format;
	VkImageAspectFlags aspectFlags;
	uint32 width = 0;
	uint32 height = 0;
	uint32 mipmapLevels = 1;
	ImageMemory* memory;
	VulkanDevice* device = nullptr;

	VkImageView CreateView() const;

	bool Initialized() const { return handle != VK_NULL_HANDLE && memory != nullptr; }
};

struct VulkanTexture
{
	VkImageView imageView = VK_NULL_HANDLE;
	VulkanImage* image = nullptr;

	VulkanTexture(VulkanImage& allocedImage);

	~VulkanTexture()
	{
		// TODO - This needs to be cleaned up somehow!
		//vkDestroyImageView(image->device->GetNativeHandle(), imageView, nullptr);
	}

	// TODO - Might consider implementing copying for these instead of leaving them default
	VulkanTexture(const VulkanTexture&) = default;
	VulkanTexture& operator=(const VulkanTexture&) = default;

	VulkanTexture(VulkanTexture&& other) noexcept
		: imageView(other.imageView),
		image(other.image)
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

// TODO - This is getting big enough to make it a class
struct VulkanBuffer
{
	VkBuffer handle = VK_NULL_HANDLE;
	BufferMemory* memory;

	VulkanBuffer(VkBuffer hdl, BufferMemory& mem)
		: handle(hdl),
		memory(&mem)
	{
	}

	// TODO - Might consider implementing copying for these instead of leaving them default
	VulkanBuffer(const VulkanBuffer&) = default;
	VulkanBuffer& operator=(const VulkanBuffer&) = default;

	VulkanBuffer(VulkanBuffer&& other) noexcept
		: handle(other.handle),
		memory(other.memory)
	{
		other.handle = VK_NULL_HANDLE;
		other.memory = nullptr;
	}

	VulkanBuffer& operator=(VulkanBuffer&& other) noexcept
	{
		if (&other != this && other.Initialized())
		{
			handle = other.handle;
			memory = other.memory;

			other.handle = VK_NULL_HANDLE;
			other.memory = nullptr;
		}

		return *this;
	}

	bool Initialized() const { return handle != VK_NULL_HANDLE && memory != nullptr; }
};
