#pragma once

#include "Graphics.h"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class VulkanSwapchain;
class VulkanSurface;
class VulkanDevice;
struct VulkanTexture;

struct VulkanBackBuffer
{
	VulkanTexture* colorTexture;
};

// TODO - Figure out how to represent native resources. 
// Currently, there can should either be a table that has handles to resources or derive from set resources essentially for type safety

// Represents the viewport of the entire window
class VulkanViewport final : public NativeViewport
{
public:
	VulkanViewport(VulkanDevice& device, VkInstance instance, void* windowHandle, uint32 viewWidth, uint32 viewHeight);
	~VulkanViewport();

	void AcquireBackBuffer();
	void SubmitFrame();
	void PresentFrame();

	inline VulkanTexture& GetBackBuffer() const { return *backBufferTexture; }
	inline uint32 GetWidth() const { return viewportWidth; }
	inline uint32 GetHeight() const { return viewportHeight; }

private:
	VulkanTexture* backBufferTexture = nullptr;
	VulkanDevice& logicalDevice;
	VulkanSurface* viewSurface;
	VulkanSwapchain* swapchain;
	uint32 viewportWidth, viewportHeight;
};