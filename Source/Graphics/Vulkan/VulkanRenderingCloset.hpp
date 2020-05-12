// Copyright 2020, Nathan Blane

#pragma once

// TODO - This isn't the greatest name for a class...should change at a later date

// The purpose of this file will be to hold all of the framebuffers and render passes that 
// Vulkan will be asked to produce. The framebuffers for the swapchain images will reside here,
// as well as the other render targets that might be needed. Render passes, being tied to 
// Framebuffers, will also reside here. 
//
// This may also be the place that stores the created pipeline objects as well. However, because
// those objects are so specific to only one part of the render api, this might not be the place.
// I will have to this about this a lot more...


#include "Containers/Map.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "VulkanDefinitions.h"

class VulkanDevice;
class VulkanPipeline;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanPipelineLayout;

class VulkanRenderingCloset
{
public:
	VulkanRenderingCloset(const VulkanDevice& device);
	~VulkanRenderingCloset();


	NODISCARD VulkanPipeline* FindOrCreatePipeline(const GraphicsPipelineDescription& desc);
	NODISCARD VulkanRenderPass* FindOrCreateRenderPass(const RenderTargetDescription& desc);
	// TODO - This isn't entirely correct, because a framebuffer could have the same attachment information, but not have the same render textures
	NODISCARD VulkanFramebuffer* FindOrCreateFramebuffer(const RenderTargetDescription& desc, const NativeRenderTargets& correspondingRTs);

	NODISCARD VkSampler FindOrCreateSampler(const SamplerDescription& params);
private:
	VulkanPipeline* CreatePipeline(const GraphicsPipelineDescription& desc);
	VulkanRenderPass* CreateRenderPass(const RenderTargetDescription& desc);
	VulkanFramebuffer* CreateFramebuffer(const RenderTargetDescription& desc, const NativeRenderTargets& correspondingRTs);
	VulkanPipelineLayout* ConfigurePipelineLayout(const GraphicsPipelineDescription& desc);
private:
	using SimilarFramebuffers = DynamicArray<VulkanFramebuffer*>;

	Map<GraphicsPipelineDescription, VulkanPipeline*> pipelineStore;
	Map<RenderTargetDescription, VulkanRenderPass*> renderPassStore;
	Map<RenderTargetDescription, SimilarFramebuffers> framebufferStore;
	Map<SamplerDescription, VkSampler> samplerStore;

	const VulkanDevice& logicalDevice;

};
