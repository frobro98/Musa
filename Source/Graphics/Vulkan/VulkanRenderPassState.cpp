#include "VulkanRenderPassState.hpp"
#include "EngineCore/Assertion.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderingCloset.hpp"
#include "VulkanDevice.h"
#include "VulkanDescriptorSet.h"
#include "GraphicsInterface.hpp"

void VulkanRenderPassState::FillWithRenderTargetDescription(GraphicsPipelineDescription& pipelineDescription) const
{
	pipelineDescription.renderTargets = currentTarget;
}

void VulkanRenderPassState::SetFramebufferTarget(VulkanCommandBuffer& cmdBuffer, const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors, bool inlinedContents)
{
	if (cmdBuffer.IsInRenderPass())
	{
		cmdBuffer.EndRenderPass();
		framebufferContext = nullptr;
	}

	VulkanFramebuffer* newTargetFB = GetGraphicsInterface().GetRenderTarget(targetDescription, renderTextures);

	cmdBuffer.BeginRenderpass(newTargetFB, clearColors, inlinedContents);

	framebufferContext = newTargetFB;
	currentTarget = targetDescription;
}

void VulkanRenderPassState::SetGraphicsPipeline(VulkanCommandBuffer& cmdBuffer, const GraphicsPipelineDescription& pipelineDescription)
{
	if (cmdBuffer.GetLevel() == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
	{
		Assert(cmdBuffer.IsInRenderPass());
	}

	VulkanPipeline* newPipeline = cmdBuffer.GetDevice().GetRenderingStorage()->FindOrCreatePipeline(pipelineDescription);

	currentPipeline = newPipeline;
	// TODO - This doesn't work if the pipeline is being reused, so the active is still happening...
	activeDescriptorSet = currentPipeline->GetUnusedDescriptorSet(cmdBuffer);
}

void VulkanRenderPassState::Bind(VulkanCommandBuffer& cmdBuffer) const
{
	if (activeDescriptorSet)
	{
		activeDescriptorSet->UpdateDescriptorSet();
	}

	currentPipeline->Bind(&cmdBuffer);
	currentPipeline->BindDescriptorSet(&cmdBuffer, activeDescriptorSet);
}

void VulkanRenderPassState::SetUniformBuffer(const VulkanBuffer& buffer, uint32 bindIndex)
{
	Assert(activeDescriptorSet);
	activeDescriptorSet->SetUniformBufferInfo(buffer, bindIndex);
}

void VulkanRenderPassState::SetStorageBuffer(const VulkanBuffer& buffer, uint32 bindIndex)
{
	Assert(activeDescriptorSet);
	activeDescriptorSet->SetStorageBufferInfo(buffer, bindIndex);
}

void VulkanRenderPassState::SetTexture(const VulkanTexture& texture, const TextureSampler& sampler, uint32 bindIndex)
{
	Assert(activeDescriptorSet);
	activeDescriptorSet->SetSampledTextureInfo(texture, sampler, bindIndex);
}

void VulkanRenderPassState::ResetState()
{

}
