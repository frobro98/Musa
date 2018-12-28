#pragma once

#include "Renderer/Renderer.h"

// TODO - Will return to this implementation when things are actually functional in Vulkan
#if GRAPHICS_API_VK

class Window;
class VulkanRenderPass;

class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer() = default;
	~VulkanRenderer();
	VulkanRenderer(const VulkanRenderer&) = delete;
	VulkanRenderer& operator=(const VulkanRenderer&) = delete;

	virtual void Initialize() override;
	virtual void SetupRendering(const Window* window) override;

	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }
	// TODO - consider renaming or rearchitecting to remove parts of the engine from accessing the Instance
	inline VkInstance GetNativeHandle() const { return instance; }

private:
	void ConfigureSwapchain();

private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;
	class VulkanDevice* logicalDevice = nullptr;
	class VulkanSwapchain* swapchain = nullptr;

	VulkanRenderPass* renderPass = nullptr;
	int32 pad1[2];
};

typedef VulkanRenderer RenderingEngine;

#endif