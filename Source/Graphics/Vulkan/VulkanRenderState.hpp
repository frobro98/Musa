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
class WriteDescriptorSet;

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

	// Changes the framebuffer being rendered to, while ending and beginning a new renderpass
	void SetFramebufferTarget(VulkanCommandBuffer& cmdBuffer, const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors, bool inlinedContents = true);
	void SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDescription);
	void BindState(VulkanCommandBuffer& cmdBuffer);

	void SetUniformBuffer(const VulkanBuffer& buffer, uint32 bindIndex);
	void SetStorageBuffer(const VulkanBuffer& buffer, uint32 bindIndex);
	void SetTexture(const VulkanTexture& texture, uint32 bindIndex);

	bool IsTextureInRender(const VulkanTexture& texture);

private:
	void ResetState();

private:

	WriteDescriptorSet* writeDescriptorSet;
	VulkanFramebuffer* framebufferContext;
	VulkanPipeline* currentPipeline;
};