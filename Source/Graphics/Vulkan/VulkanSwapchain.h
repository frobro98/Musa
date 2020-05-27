// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanDefinitions.h"

#include "BasicTypes/Uncopyable.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/RenderTargetDescription.hpp"

class VulkanDevice;
class VulkanSurface;
class VulkanRenderPass;
class VulkanCommandBuffer;
class VulkanFramebuffer;
struct VulkanTexture;

class VulkanSwapchain : private Uncopyable
{
public:
	VulkanSwapchain(VulkanDevice& device, VulkanSurface* renderSurface);
	~VulkanSwapchain();

	void Initialize();
	void Terminate();

	void Recreate();

	VkResult GetNextImage();
	void SubmitFrame();
	void Present();

	RenderTargetDescription GetSwapchainImageDescription() const;
	VulkanTexture& GetSwapchainTarget() const;

	inline VkFormat GetSwapchainFormat() const { return swapchainFormat; }
	inline VkExtent2D GetSwapchainExtent() const { return swapchainExtent; }
	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }

private:
	void CreateSwapchain();
	void CacheSwapchainImages();
	void InitializeRenderTargets();
	void InitializeSwapchainSyncronization();

private:
	VkExtent2D swapchainExtent = {};
	VkSwapchainKHR swapchainHandle = VK_NULL_HANDLE;
	VkSemaphore imageAvailable = VK_NULL_HANDLE;
	VkSemaphore renderingFinished = VK_NULL_HANDLE;
	RenderTargetDescription targetDescription = {};

	VkFormat swapchainFormat = {};
	DynamicArray<VkImage> swapchainImages;
	DynamicArray<VulkanTexture*> swapchainImageTargets;

	VulkanDevice& logicalDevice;
	VulkanSurface* surface;
	VulkanRenderPass* renderPass;
	u32 imageIndex;
	u32 pad[1] = { 0 };
};

