#pragma once

#include "vulkan/vulkan.h"
#include "GraphicsInterface.hpp"
#include "Graphics/Renderer.hpp"
#include "VulkanDevice.h"


class VulkanGraphicsInterface final : public GraphicsInterface
{
public:
	virtual void InitializeGraphics() override;
	virtual void DeinitializeGraphics() override;

	virtual UniquePtr<NativeViewport> CreateViewport(void* windowHandle, uint32 viewWidth, uint32 viewHeight) override;

	virtual UniquePtr<NativeVertexBuffer> CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const override;
	virtual UniquePtr<NativeIndexBuffer> CreateIndexBuffer(const DynamicArray<Face>& faces) const override;
	virtual UniquePtr<NativeUniformBuffer> CreateUniformBuffer(uint32 bufferSize) const override;
	virtual void PushBufferData(NativeUniformBuffer& buffer, const void* data) const override;

	virtual UniquePtr<NativeTexture> CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual UniquePtr<NativeTexture> CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual NativeSampler* CreateTextureSampler(const SamplerDescription& params) override;
	virtual void PushTextureData(NativeTexture& texture, const ResourceBlob& textureBlob) override;

	virtual void* GetGraphicsDevice() override;
	virtual Renderer* GetRenderContext() override;

private:
	void CreateInstance();

private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;

	UniquePtr<Renderer> renderContext;
	UniquePtr<VulkanDevice> logicalDevice;
};
