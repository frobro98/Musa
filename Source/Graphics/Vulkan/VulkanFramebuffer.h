#pragma once

#include "Graphics.h"
#include "Containers/DynamicArray.hpp"
#include "EngineCore/Extents.hpp"
#include "Graphics/RenderTargetDescription.hpp"

class VulkanDevice;
class VulkanRenderPass;
struct VulkanTexture;
struct RenderTargetDescription;
struct RenderTargetTextures;
struct VulkanTexture;

class VulkanFramebuffer
{
public:
	VulkanFramebuffer(const VulkanDevice& device);
	~VulkanFramebuffer();
	VulkanFramebuffer(const VulkanFramebuffer&) = delete;
	VulkanFramebuffer& operator=(const VulkanFramebuffer&) = delete;

	void Initialize(const RenderTargetDescription& targetDesc, const RenderTargetTextures& renderTextures, VulkanRenderPass* renderPass_);

	bool ContainsRT(const VulkanTexture& texture);
	bool ContainsRTs(const RenderTargetTextures& renderTextures);

	uint32 GetAttachmentCount() const { return nativeTargets.targetCount + 1; }

	inline VkFramebuffer GetNativeHandle() const { return frameBuffer; }
	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }
	inline uint32 GetWidth() const { return extents.width; }
	inline uint32 GetHeight() const { return extents.height; }

private:
	RenderTargetTextures nativeTargets;
	DynamicArray<VkImageView> viewAttachments;
	VkFramebuffer frameBuffer = VK_NULL_HANDLE;
	VkExtent2D extents;
	const VulkanDevice* logicalDevice;
	VulkanRenderPass* renderPass;
};