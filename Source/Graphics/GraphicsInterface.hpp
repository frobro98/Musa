#pragma once

#include "Graphics.h"
#include "GraphicsResourceFlags.hpp"
#include "Texture/ImageFormats.h"
#include "GraphicsResourceDefinitions.hpp"
#include "RenderTargetDescription.hpp"
#include "Containers/DynamicArray.hpp"

struct Vertex;
struct Face;
class ResourceBlob;
class VulkanDevice;
class VulkanSwapchain;
class VulkanFramebuffer;
class VulkanViewport;
class VulkanVertexBuffer;
class VulkanIndexBuffer;
class VulkanUniformBuffer;

class GraphicsInterface
{
public:

	// TODO - Decide on whether or not to put the Begin/EndFrame calls in this interface or a different interface more focused on the actual render pipeline

	virtual ~GraphicsInterface() = default;

	virtual void InitializeGraphics() = 0;
	
	virtual VulkanViewport* CreateViewport(void* windowHandle, uint32 viewWidth, uint32 viewHeight) = 0;

	virtual VulkanVertexBuffer* CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const = 0;
	virtual VulkanIndexBuffer* CreateIndexBuffer(const DynamicArray<Face>& faces) const = 0;
	virtual VulkanUniformBuffer* CreateUniformBuffer(uint32 bufferSize) const = 0;

	virtual VulkanTexture* CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	virtual VulkanTexture* CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	virtual void PushTextureData(VulkanTexture& texture, const ResourceBlob& textureBlob) = 0;
	virtual VulkanFramebuffer* CreateOrFindFramebuffer(const RenderTargetDescription& targetDesc, const RenderTargetTextures& renderTextures) = 0;
	virtual TextureSampler CreateTextureSampler(const TextureSamplerCreateParams& params) = 0;

	virtual VulkanDevice* GetGraphicsDevice() = 0;

	// TODO - These functions might access more command buffer related behaviors, so they might not belong here
	virtual void TransitionToWriteState(const VulkanTexture** textures, uint32 textureCount) = 0;
	virtual void TransitionToReadState(const VulkanTexture** textures, uint32 textureCount) = 0;
};

GraphicsInterface& GetGraphicsInterface();
