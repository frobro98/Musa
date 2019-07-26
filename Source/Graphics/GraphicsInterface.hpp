#pragma once

#include "Graphics.h"
#include "GraphicsResourceFlags.hpp"
#include "Texture/ImageFormats.h"
#include "GraphicsResourceDefinitions.hpp"
#include "RenderTargetDescription.hpp"

class ResourceBlob;
class VulkanDevice;
class VulkanSwapchain;
class VulkanFramebuffer;
class VulkanViewport;

class GraphicsInterface
{
public:

	// TODO - Decide on whether or not to put the Begin/EndFrame calls in this interface or a different interface more focused on the actual render pipeline

	virtual ~GraphicsInterface() = default;

	virtual void InitializeGraphics() = 0;
	
	virtual VulkanViewport* CreateViewport(void* windowHandle, uint32 viewWidth, uint32 viewHeight) = 0;

	virtual VulkanTexture* CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	virtual VulkanTexture* CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	virtual void PushTextureData(VulkanTexture& texture, const ResourceBlob& textureBlob) = 0;
	virtual VulkanFramebuffer* CreateOrFindFramebuffer(const RenderTargetDescription& targetDesc, const RenderTargetTextures& renderTextures) = 0;
	virtual TextureSampler CreateTextureSampler(const TextureSamplerCreateParams& params) = 0;

	virtual VulkanDevice* GetGraphicsDevice() = 0;

	// TODO - These functions might access more command buffer related behaviors, so they might not belong here
	virtual void TransitionToWriteState(const VulkanTexture* texture, uint32 textureCount) = 0;
	virtual void TransitionToReadState(const VulkanTexture* texture, uint32 textureCount) = 0;
};

GraphicsInterface& GetGraphicsInterface();
