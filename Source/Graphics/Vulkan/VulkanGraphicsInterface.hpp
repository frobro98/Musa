#pragma once

#include "GraphicsInterface.hpp"

class VulkanDevice;

class VulkanGraphicsInterface final : public GraphicsInterface
{
public:
	virtual ~VulkanGraphicsInterface();

	virtual void InitializeGraphics() override;

	virtual NativeViewport* CreateViewport(void* windowHandle, uint32 viewWidth, uint32 viewHeight) override;

	virtual NativeVertexBuffer* CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const override;
	virtual NativeIndexBuffer* CreateIndexBuffer(const DynamicArray<Face>& faces) const override;
	virtual NativeUniformBuffer* CreateUniformBuffer(uint32 bufferSize) const override;
	virtual void PushBufferData(NativeUniformBuffer& buffer, const void* data) const override;

	virtual NativeTexture* CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual NativeTexture* CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual void PushTextureData(NativeTexture& texture, const ResourceBlob& textureBlob) override;

	virtual NativeSampler* CreateTextureSampler(const SamplerDescription& params) override;

	virtual void* GetGraphicsDevice() override;
	virtual Renderer* GetRenderContext() override;

private:
	void CreateInstance();

private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;

	Renderer* renderContext = nullptr;
	VulkanDevice* logicalDevice = nullptr;
};
