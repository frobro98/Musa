#pragma once

#include "Graphics.h"
#include "Containers/Array.h"
#include "VulkanAbstractions.h"

#if GRAPHICS_API_VK

class VulkanDevice;
class VulkanSurface;
class VulkanRenderPass;
class VulkanCommandBuffer;
class VulkanFramebuffer;

class VulkanSwapchain
{
public:
	VulkanSwapchain(VulkanDevice* device, VulkanSurface* renderSurface);
	~VulkanSwapchain();

	VulkanSwapchain(const VulkanSwapchain&) = delete;
	VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

	void Initialize();
	void Terminate();

	void Recreate();

	VkResult GetNextImage();
	void SubmitFrame();
	void Present();

	VulkanFramebuffer* GetActiveFramebuffer() const { return swapchainFramebuffers[imageIndex]; }

	inline VkFormat GetSwapchainFormat() const { return swapchainFormat; }
	inline VkExtent2D GetSwapchainExtent() const { return swapchainExtent; }
	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }

private:
	void CreateSwapchain();
	void CreateSwapchainImageViews();
	// TODO - Renderpass creation should probably move. Need to understand its purpose more
	void CreateRenderPass();
	void CreateDepthView();
	void CreateSwapchainFramebuffers();
	void InitializeSwapchainSyncronization();

private:
	VkExtent2D swapchainExtent = {};
	VulkanImage depthImage = {};
	VkImageView depthImageView = VK_NULL_HANDLE;
	VkSwapchainKHR swapchainHandle = VK_NULL_HANDLE;
	VkSemaphore imageAvailable = VK_NULL_HANDLE;
	VkSemaphore copyFinished = VK_NULL_HANDLE;
	VkSemaphore computeFinished = VK_NULL_HANDLE;
	VkSemaphore renderingFinished = VK_NULL_HANDLE;
	VkFence waitCompute = VK_NULL_HANDLE;

	VkFormat swapchainFormat = {};
	//VkFormat depthFormat;

	Array<VkImageView> swapchainImageViews;
	Array<VulkanFramebuffer*> swapchainFramebuffers;

	VulkanDevice* logicalDevice;
	VulkanSurface* surface;
	VulkanRenderPass* renderPass;
	uint32 imageIndex;
	uint32 pad[1];
};

#endif