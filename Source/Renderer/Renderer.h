#pragma once

#include "Graphics.h"

#if GRAPHICS_API_VK

class Window;
class DrawList;
class SceneRendering;
class VulkanRenderPass;
class VulkanSwapchain;

// This will be the entry point for vulkan and  OpenGL
// TODO - Make this an actual cross-rendering platform renderer
class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer();
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	virtual void Initialize();
	virtual void SetupRendering(const Window* window);
	void ConfigureSwapchain(const Window* window);

	void RenderFrame();

	inline DrawList* GetDrawList() const { return drawList; }

	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }
	inline VulkanSwapchain* GetSwapchain() const { return swapchain; }
	// TODO - consider renaming or rearchitecting to remove parts of the engine from accessing the Instance
	inline VkInstance GetNativeHandle() const { return instance; }

	// TODO - Remove getting logical device as soon as humanly possible!
	inline class VulkanDevice* GetDevice() const { return logicalDevice; }

private:
	bool PrepareFrame();
	void SubmitFrame();

	// Vulkan specific stuff
private:
	VkDebugReportCallbackEXT debugReportHandle = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;

	class VulkanDevice* logicalDevice = nullptr;
	class VulkanCommandBufferManager* cbManager = nullptr;
	class PipelineManager* pipelineManager = nullptr;
	VulkanSwapchain* swapchain = nullptr;
	VulkanRenderPass* renderPass = nullptr;
	DrawList* drawList = nullptr;

private:
	SceneRendering* sceneRendering = nullptr;
	uint32 pad[2];
};

#endif