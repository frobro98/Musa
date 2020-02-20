#pragma once

#include "VulkanDefinitions.h"
#include "Containers/DynamicArray.hpp"
#include "EngineCore/Extents.hpp"
#include "Graphics/RenderTargetDescription.hpp"

class VulkanDevice;
class VulkanRenderPass;
struct VulkanTexture;
struct RenderTargetDescription;
struct NativeRenderTargets;
struct VulkanTexture;

class VulkanFramebuffer
{
public:
	VulkanFramebuffer(const VulkanDevice& device);
	~VulkanFramebuffer();
	VulkanFramebuffer(const VulkanFramebuffer&) = delete;
	VulkanFramebuffer& operator=(const VulkanFramebuffer&) = delete;

	void Initialize(const RenderTargetDescription& targetDesc, const NativeRenderTargets& renderTextures, VulkanRenderPass* renderPass_);

	bool ContainsRT(const VulkanTexture& texture);
	bool ContainsRTs(const NativeRenderTargets& renderTextures);

	uint32 GetAttachmentCount() const { return viewAttachments.Size(); }
	bool HasDepthAttachment() const { return nativeTargets.depthTarget != nullptr; }

	inline VkFramebuffer GetNativeHandle() const { return frameBuffer; }
	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }
	inline uint32 GetWidth() const { return extents.width; }
	inline uint32 GetHeight() const { return extents.height; }

private:
	NativeRenderTargets nativeTargets{};
	DynamicArray<VkImageView> viewAttachments;
	VkFramebuffer frameBuffer = VK_NULL_HANDLE;
	VkExtent2D extents;
	const VulkanDevice* logicalDevice;
	VulkanRenderPass* renderPass;
};