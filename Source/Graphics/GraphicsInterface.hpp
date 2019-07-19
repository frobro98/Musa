#pragma once

#include "Graphics.h"
#include "GraphicsResourceFlags.hpp"
#include "Texture/ImageFormats.h"

class ResourceBlob;
class VulkanDevice;
class VulkanSwapchain;

class GraphicsInterface
{
public:

	virtual ~GraphicsInterface() = default;

	virtual void InitializeGraphics(void* windowHandle, uint32 width, uint32 height) = 0;

	virtual VulkanDevice* GetGraphicsDevice() = 0;
	virtual VulkanSwapchain* GetGraphicsSwapchain() = 0;

	virtual VulkanTexture* CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	virtual VulkanTexture* CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
// 	virtual VulkanFramebuffer* CreateRenderTarget() = 0;

};

GraphicsInterface& GetGraphicsInterface();
