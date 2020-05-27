// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanDefinitions.h"
#include "GraphicsInterface.hpp"
#include "Graphics/RenderContext.hpp"
#include "VulkanDevice.h"


class VulkanGraphicsInterface final : public GraphicsInterface
{
public:
	virtual void InitializeGraphics() override;
	virtual void DeinitializeGraphics() override;

	virtual UniquePtr<NativeVertexShader> CreateVertexShader(const MemoryBuffer& vertexCode) override;
	virtual UniquePtr<NativeFragmentShader> CreateFragmentShader(const MemoryBuffer& fragmentCode) override;
	virtual UniquePtr<NativeGeometryShader> CreateGeometryShader(const MemoryBuffer& geometryCode) override;
	virtual UniquePtr<NativeTessEvaluationShader> CreateTessEvaluationShader(const MemoryBuffer& tessEvalCode) override;
	virtual UniquePtr<NativeTessControlShader> CreateTessControlShader(const MemoryBuffer& tessCtrlCode) override;
	virtual UniquePtr<NativeComputeShader> CreateComputeShader(const MemoryBuffer& computeCode) override;

	virtual UniquePtr<NativeViewport> CreateViewport(void* windowHandle, u32 viewWidth, u32 viewHeight) override;

	virtual UniquePtr<NativeVertexBuffer> CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const override;
	virtual UniquePtr<NativeIndexBuffer> CreateIndexBuffer(const DynamicArray<Face>& faces) const override;
	virtual UniquePtr<NativeUniformBuffer> CreateUniformBuffer(u32 bufferSize) const override;
	virtual void PushBufferData(NativeUniformBuffer& buffer, const void* data) const override;

	virtual UniquePtr<NativeTexture> CreateEmptyTexture2D(u32 width, u32 height, ImageFormat textureFormat, u32 mipLevels, TextureUsage::Type usage) override;
	virtual UniquePtr<NativeTexture> CreateInitializedTexture2D(const ResourceBlob& textureBlob, u32 width, u32 height, ImageFormat textureFormat, u32 mipLevels, TextureUsage::Type usage) override;
	virtual NativeSampler* CreateTextureSampler(const SamplerDescription& params) override;
	virtual void PushTextureData(NativeTexture& texture, const ResourceBlob& textureBlob) override;

	virtual void* GetGraphicsDevice() override;
	virtual RenderContext* GetRenderContext() override;

private:
	void CreateInstance();

private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;

	UniquePtr<RenderContext> renderContext;
	UniquePtr<VulkanDevice> logicalDevice;
};
