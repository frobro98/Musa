#pragma once

#include "GraphicsInterface.hpp"

class VulkanGraphicsInterface final : public GraphicsInterface
{
public:
	virtual ~VulkanGraphicsInterface();

	virtual void InitializeGraphics(void* windowHandle, uint32 width, uint32 height) override;

	virtual VulkanDevice* GetGraphicsDevice() override;
	virtual VulkanSwapchain* GetGraphicsSwapchain() override;

	virtual VulkanTexture* CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;
	virtual VulkanTexture* CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) override;

private:
	void CreateInstance();

private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;

	class VulkanDevice* logicalDevice = nullptr;
	class VulkanSwapchain* swapchain = nullptr;
	class VulkanRenderingCloset* renderingStorage = nullptr;
};
