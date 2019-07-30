#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "Graphics/RenderTargetDescription.hpp"

#include "Graphics/Vulkan/VulkanIndexBuffer.h"
#include "Graphics/Vulkan/VulkanVertexBuffer.h"

#include "Color.hpp"

class VulkanFramebuffer;
class VulkanPipeline;
class VulkanCommandBuffer;
class VulkanDescriptorSet;

struct RenderTargetDescription;
struct RenderTargetTextures;
struct GraphicsPipelineDescription;
struct VulkanBuffer;
struct VulkanTexture;
struct TextureSampler;


// Describes what is currently processing within a render pass in Vulkan
// When a new render target is set, the current render pass ends and a new one begins
class VulkanRenderState
{
public:
	void FillWithRenderTargetDescription(GraphicsPipelineDescription& pipelineDescription) const;

	// Changes the framebuffer being rendered to, while ending and beginning a new renderpass
	void SetFramebufferTarget(VulkanCommandBuffer& cmdBuffer, const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors, bool inlinedContents = true);
	void SetGraphicsPipeline(VulkanCommandBuffer& cmdBuffer, const GraphicsPipelineDescription& pipelineDescription);
	void Bind(VulkanCommandBuffer& cmdBuffer) const;

	void SetUniformBuffer(const VulkanBuffer& buffer, uint32 bindIndex);
	void SetStorageBuffer(const VulkanBuffer& buffer, uint32 bindIndex);
	void SetTexture(const VulkanTexture& texture, uint32 bindIndex);

	inline VulkanFramebuffer& GetCurrentFramebuffer() const { return *framebufferContext; }

private:
	void ResetState();

private:
	
	RenderTargetDescription currentTarget;
	VulkanFramebuffer* framebufferContext;
	VulkanPipeline* currentPipeline;
	VulkanDescriptorSet* activeDescriptorSet;

};