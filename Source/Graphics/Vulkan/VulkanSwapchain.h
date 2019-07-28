#pragma once

#include "Graphics.h"
#include "Containers/DynamicArray.hpp"
#include "VulkanAbstractions.h"
#include "Graphics/RenderTargetDescription.hpp"

class VulkanDevice;
class VulkanSurface;
class VulkanRenderPass;
class VulkanCommandBuffer;
class VulkanFramebuffer;

class VulkanSwapchain
{
public:
	VulkanSwapchain(VulkanDevice& device, VulkanSurface* renderSurface);
	~VulkanSwapchain();

	VulkanSwapchain(const VulkanSwapchain&) = delete;
	VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

	void Initialize();
	void Terminate();

	void Recreate();

	VkResult GetNextImage();
	void SubmitFrame();
	void Present();

	RenderTargetDescription GetSwapchainImageDescription() const;
	RenderTargetTextures GetSwapchainTarget() const;

	inline VkFormat GetSwapchainFormat() const { return swapchainFormat; }
	inline VkExtent2D GetSwapchainExtent() const { return swapchainExtent; }
	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }

private:
	void CreateSwapchain();
	void CacheSwapchainImages();
	void CreateDepthImage();
	void InitializeRenderTargets();
	void InitializeSwapchainSyncronization();

private:
	VkExtent2D swapchainExtent = {};
	VkSwapchainKHR swapchainHandle = VK_NULL_HANDLE;
	VkSemaphore imageAvailable = VK_NULL_HANDLE;
	VkSemaphore renderingFinished = VK_NULL_HANDLE;
	RenderTargetDescription targetDescription = {};

	VkFormat swapchainFormat = {};
	VulkanImage* depthImage;
	DynamicArray<VkImage> swapchainImages;
	DynamicArray<VulkanTexture*> swapchainImageTargets;

	VulkanDevice& logicalDevice;
	VulkanSurface* surface;
	VulkanRenderPass* renderPass;
	uint32 imageIndex;
	uint32 pad[1] = { 0 };
};

