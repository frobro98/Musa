// Copyright 2020, Nathan Blane

#include "VulkanRenderState.hpp"
#include "Debugging/Assertion.hpp"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderingCloset.hpp"
#include "VulkanDevice.h"
#include "VulkanDescriptorSet.h"
#include "GraphicsInterface.hpp"
#include "VulkanAbstractions.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanBufferAllocation.hpp"
#include "VulkanFramebuffer.h"

VulkanRenderState::VulkanRenderState(VulkanDevice& device)
	: logicalDevice(device)
{
}

void VulkanRenderState::SetFramebufferTarget(VulkanCommandBuffer& cmdBuffer, const RenderTargetDescription& targetDescription, const NativeRenderTargets& renderTextures, const DynamicArray<Color32>& clearColors, bool inlinedContents)
{
	if (cmdBuffer.IsInRenderPass())
	{
		cmdBuffer.EndRenderPass();
		framebufferContext = nullptr;
	}

	VulkanFramebuffer* newTargetFB = logicalDevice.GetRenderingStorage()->FindOrCreateFramebuffer(targetDescription, renderTextures);

	cmdBuffer.BeginRenderpass(newTargetFB, clearColors, inlinedContents);

	framebufferContext = newTargetFB;
}

void VulkanRenderState::SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDescription)
{
// 	if (cmdBuffer.GetLevel() == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
// 	{
// 		Assert(cmdBuffer.IsInRenderPass());
// 	}

	VulkanPipeline* newPipeline = logicalDevice.GetRenderingStorage()->FindOrCreatePipeline(pipelineDescription);

	currentPipeline = newPipeline;
	writeDescriptorSet = &newPipeline->GetWriteDescriptorSet();
}

void VulkanRenderState::BindState(VulkanCommandBuffer& cmdBuffer)
{
	VulkanDescriptorSet* ds = currentPipeline->GetUnusedDescriptorSet(cmdBuffer);
	ds->UpdateDescriptorSet(*writeDescriptorSet);

 	currentPipeline->Bind(&cmdBuffer);
 	currentPipeline->BindDescriptorSet(&cmdBuffer, ds);
}

void VulkanRenderState::SetUniformBuffer(const VulkanBuffer& buffer, u32 bindIndex)
{
	Assert(writeDescriptorSet);
	writeDescriptorSet->SetBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, buffer, bindIndex);
}

void VulkanRenderState::SetStorageBuffer(const VulkanBuffer& buffer, u32 bindIndex)
{
	Assert(writeDescriptorSet);
	writeDescriptorSet->SetBuffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, buffer, bindIndex);
}

void VulkanRenderState::SetTexture(const VulkanTexture& texture, const VulkanSampler& sampler, u32 bindIndex)
{
	Assert(writeDescriptorSet);
	writeDescriptorSet->SetTexture(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, texture, sampler, bindIndex);
}

bool VulkanRenderState::IsTextureInRender(const VulkanTexture& texture)
{
	return framebufferContext && framebufferContext->ContainsRT(texture);
}

