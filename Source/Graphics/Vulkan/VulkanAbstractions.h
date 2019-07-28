#pragma once

#include "Graphics.h"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class VulkanDevice;
class ImageMemory;
class BufferMemory;
class ImageMemory;

struct VulkanImage
{
	~VulkanImage();

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
	TextureSampler sampler;
	VulkanTexture(VulkanImage& allocedImage);

	~VulkanTexture();

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
	VulkanDevice& device;

	VulkanBuffer(VulkanDevice& dev, VkBuffer hdl, BufferMemory& mem)
		: handle(hdl),
		memory(&mem),
		device(dev)
	{
	}

	~VulkanBuffer();

	// TODO - Might consider implementing copying for these instead of leaving them default
	VulkanBuffer(const VulkanBuffer&) = default;
	VulkanBuffer& operator=(const VulkanBuffer&) = default;

	VulkanBuffer(VulkanBuffer&& other) noexcept
		: handle(other.handle),
		memory(other.memory),
		device(other.device)
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
