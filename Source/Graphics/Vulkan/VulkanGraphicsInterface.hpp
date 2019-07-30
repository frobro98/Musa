#pragma once

#include "GraphicsInterface.hpp"

class VulkanGraphicsInterface final : public GraphicsInterface
{
public:
	virtual ~VulkanGraphicsInterface();

	virtual void InitializeGraphics() override;

	virtual VulkanDevice* GetGraphicsDevice() override;

	virtual VulkanViewport* CreateViewport(void* windowHandle, uint32 viewWidth, uint32 viewHeight) override;

	virtual VulkanVertexBuffer* CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const override;
	virtual VulkanIndexBuffer* CreateIndexBuffer(const DynamicArray<Face>& faces) const override;
	virtual VulkanUniformBuffer* CreateUniformBuffer(uint32 bufferSize) const override;

	virtual VulkanTexture* CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual VulkanTexture* CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual void PushTextureData(VulkanTexture& texture, const ResourceBlob& textureBlob) override;

	virtual VulkanFramebuffer* CreateOrFindFramebuffer(const RenderTargetDescription& targetDesc, const RenderTargetTextures& renderTextures) override;
	virtual TextureSampler CreateTextureSampler(const TextureSamplerCreateParams& params) override;

private:
	void CreateInstance();

private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;

	class VulkanDevice* logicalDevice = nullptr;
	class VulkanRenderingCloset* renderingStorage = nullptr;
};
