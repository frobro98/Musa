#include "VulkanRenderer.hpp"
#include "VulkanRenderState.hpp"
#include "EngineCore/Assertion.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanDescriptorSet.h"
#include "GraphicsInterface.hpp"
#include "VulkanUniformBuffer.h"
#include "VulkanDevice.h"
#include "VulkanUtilities.h"
#include "VulkanVertexBuffer.h"
#include "VulkanBufferAllocation.hpp"
#include "VulkanViewport.hpp"
#include "VulkanStagingBufferManager.hpp"
#include "VulkanSampler.hpp"
#include "Graphics/GraphicsInterface.hpp"

VulkanRenderer::VulkanRenderer(VulkanDevice& device)
	: logicalDevice(device),
	currentRenderState(device)
{
}

void VulkanRenderer::BeginRenderFrame(NativeViewport& renderViewport)
{
	VulkanViewport* vp = static_cast<VulkanViewport*>(&renderViewport);
	vp->AcquireBackBuffer();
	backBuffer = &vp->GetBackBuffer();
	logicalDevice.GetStagingBufferManager().ProcessDeferredReleases();
}

void VulkanRenderer::EndRenderFrame(NativeViewport& renderViewport)
{
	VulkanViewport* vp = static_cast<VulkanViewport*>(&renderViewport);
	vp->SubmitFrame();
	vp->PresentFrame();
}

void VulkanRenderer::InitializeWithRenderState(GraphicsPipelineDescription& pipelineDesc) const
{
	pipelineDesc.renderTargets = currentTarget;
}

void VulkanRenderer::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth)
{
	viewport.x = (float32)x;
	viewport.y = (float32)y;
	viewport.width = (float32)width;
	viewport.height = (float32)height;
	viewport.minDepth = minDepth;
	viewport.maxDepth = maxDepth;
	updateViewState = true;
}

void VulkanRenderer::SetScissor(uint32 offsetX, uint32 offsetY, uint32 width, uint32 height)
{
	// TODO - This doesn't necessarily mean that the scissor will be in effect. There could be a way to optimize this call with reusing something like th viewport
	scissorRect.offset = { (int32)offsetX, (int32)offsetY };
	scissorRect.extent = { width, height };
	updateViewState = true;
}

void VulkanRenderer::SetRenderTarget(const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors)
{
	VulkanCommandBuffer& cmdBuffer = *logicalDevice.GetCmdBufferManager().GetActiveGraphicsBuffer();
	currentTarget = targetDescription;
	currentRenderState.SetFramebufferTarget(cmdBuffer, targetDescription, renderTextures, clearColors);
}

void VulkanRenderer::SetVertexBuffer(const NativeVertexBuffer& vertexBuffer)
{
	const VulkanVertexBuffer* vb = static_cast<const VulkanVertexBuffer*>(&vertexBuffer);
	vertexBuffersAndOffsets.vertexBuffers.Add(vb);
	vertexBuffersAndOffsets.vertexBufferOffsets.Add(vb->GetBuffer().memory->alignedOffset);
}

void VulkanRenderer::SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc)
{
	currentRenderState.SetGraphicsPipeline(pipelineDesc);
}

void VulkanRenderer::SetUniformBuffer(const NativeUniformBuffer& uniformBuffer, uint32 bufferIndex)
{
	const VulkanUniformBuffer* ub = static_cast<const VulkanUniformBuffer*>(&uniformBuffer);
	currentRenderState.SetUniformBuffer(ub->GetBuffer(), bufferIndex);
}

void VulkanRenderer::SetTexture(const NativeTexture& texture, const NativeSampler& sampler, uint32 textureIndex)
{
	const VulkanTexture* tex = static_cast<const VulkanTexture*>(&texture);
	const VulkanSampler* samp = static_cast<const VulkanSampler*>(&sampler);
	currentRenderState.SetTexture(*tex, *samp, textureIndex);
}

void VulkanRenderer::Draw(uint32 vertexCount, uint32 instanceCount)
{
	VulkanCommandBuffer& cmdBuffer = *logicalDevice.GetCmdBufferManager().GetActiveGraphicsBuffer();
	if(updateViewState)
	{
		UpdateViewState(cmdBuffer);
	}

	if (!vertexBuffersAndOffsets.vertexBuffers.IsEmpty())
	{
		cmdBuffer.BindVertexBuffers(
			vertexBuffersAndOffsets.vertexBuffers.GetData(),
			vertexBuffersAndOffsets.vertexBufferOffsets.GetData(),
			vertexBuffersAndOffsets.vertexBuffers.Size()
		);
		vertexBuffersAndOffsets.vertexBuffers.Clear();
		vertexBuffersAndOffsets.vertexBufferOffsets.Clear();
	}

	currentRenderState.BindState(cmdBuffer);

	cmdBuffer.Draw(vertexCount, instanceCount, 0, 0);
}

void VulkanRenderer::DrawIndexed(const NativeIndexBuffer& indexBuffer, uint32 instanceCount)
{
	const VulkanIndexBuffer* ib = static_cast<const VulkanIndexBuffer*>(&indexBuffer);
	VulkanCommandBuffer& cmdBuffer = *logicalDevice.GetCmdBufferManager().GetActiveGraphicsBuffer();
	if (updateViewState)
	{
		UpdateViewState(cmdBuffer);
	}

	if (!vertexBuffersAndOffsets.vertexBuffers.IsEmpty())
	{
		cmdBuffer.BindVertexBuffers(
			vertexBuffersAndOffsets.vertexBuffers.GetData(),
			vertexBuffersAndOffsets.vertexBufferOffsets.GetData(),
			vertexBuffersAndOffsets.vertexBuffers.Size()
		);
		vertexBuffersAndOffsets.vertexBuffers.Clear();
		vertexBuffersAndOffsets.vertexBufferOffsets.Clear();
	}
	uint32 indexCount = ib->GetNumberOfIndicies();
	cmdBuffer.BindIndexBuffer(*ib);

	currentRenderState.BindState(cmdBuffer);

	cmdBuffer.DrawIndexed(indexCount, instanceCount, 0, 0, 0);
}

void VulkanRenderer::DrawRaw(const ResourceArray& /*rawVerts*/)
{
	// This will use temp memory to allocate a vertex buffer for these raw vertices
}

NativeTexture* VulkanRenderer::GetBackBuffer() const
{
	return backBuffer;
}

void VulkanRenderer::UpdateViewState(VulkanCommandBuffer& cmdBuffer)
{
	updateViewState = false;
	cmdBuffer.SetViewport(0, 1, &viewport);
	cmdBuffer.SetScissor(0, 1, &scissorRect);
}

void VulkanRenderer::TransitionToWriteState(const NativeTexture** textures, uint32 textureCount)
{
	Assert(textureCount > 0);
	Assert(textures != nullptr);

	// TODO - Need to bring the logical device into the vulkan renderer as well
	VulkanCommandBuffer& cmdBuffer = *logicalDevice.GetCmdBufferManager().GetActiveGraphicsBuffer();
	if (cmdBuffer.IsInRenderPass())
	{
		cmdBuffer.EndRenderPass();
	}

	DynamicArray<VkImageMemoryBarrier> imageBarriers;
	imageBarriers.Reserve(textureCount);
	VkPipelineStageFlags srcStageFlags = 0;
	VkPipelineStageFlags dstStageFlags = 0;
	for (uint32 i = 0; i < textureCount; ++i)
	{
		const VulkanTexture* texture = static_cast<const VulkanTexture*>(textures[i]);
		VulkanImage& image = *texture->image;
		bool isDepthStencilTexture = (image.aspectFlags &
			(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)) != 0;
		VkImageLayout srcLayout = image.layout;
		VkImageLayout dstLayout = isDepthStencilTexture ?
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		if (srcLayout != dstLayout)
		{
			VkImageMemoryBarrier imageBarrier = {};
			imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageBarrier.image = image.handle;
			imageBarrier.oldLayout = srcLayout;
			imageBarrier.newLayout = dstLayout;
			imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.subresourceRange.aspectMask = image.aspectFlags;
			imageBarrier.subresourceRange.baseArrayLayer = 0;
			imageBarrier.subresourceRange.baseMipLevel = 0;
			imageBarrier.subresourceRange.layerCount = 1;
			imageBarrier.subresourceRange.levelCount = image.mipmapLevels;

			imageBarrier.srcAccessMask = GetAccessFlagsFor(srcLayout);
			imageBarrier.dstAccessMask = GetAccessFlagsFor(dstLayout);

			srcStageFlags |= GetStageFor(srcLayout);
			dstStageFlags |= GetStageFor(dstLayout);

			imageBarriers.Add(imageBarrier);

			image.layout = dstLayout;
		}
	}

	if (!imageBarriers.IsEmpty())
	{
		Assert(srcStageFlags != 0);
		Assert(dstStageFlags != 0);

		cmdBuffer.ImageMemoryBarrier(
			srcStageFlags, dstStageFlags,
			0,
			imageBarriers.Size(), imageBarriers.GetData()
		);
	}
}

void VulkanRenderer::TransitionToReadState(const NativeTexture** textures, uint32 textureCount)
{
	Assert(textureCount > 0);
	Assert(textures != nullptr);

	// TODO - Need to bring the logical device into the vulkan renderer as well
	VulkanCommandBuffer& cmdBuffer = *logicalDevice.GetCmdBufferManager().GetActiveGraphicsBuffer();
	if (cmdBuffer.IsInRenderPass())
	{
		cmdBuffer.EndRenderPass();
	}

	DynamicArray<VkImageMemoryBarrier> imageBarriers;
	imageBarriers.Reserve(textureCount);
	VkPipelineStageFlags srcStageFlags = 0;
	VkPipelineStageFlags dstStageFlags = 0;
	for (uint32 i = 0; i < textureCount; ++i)
	{
		const VulkanTexture* texture = static_cast<const VulkanTexture*>(textures[i]);
		VulkanImage& image = *texture->image;
		bool isDepthStencilTexture = (image.aspectFlags &
			(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)) != 0;
		VkImageLayout srcLayout = image.layout;
		VkImageLayout dstLayout = isDepthStencilTexture ?
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		if (srcLayout != dstLayout)
		{
			VkImageMemoryBarrier imageBarrier = {};
			imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageBarrier.image = image.handle;
			imageBarrier.oldLayout = srcLayout;
			imageBarrier.newLayout = dstLayout;
			imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.subresourceRange.aspectMask = image.aspectFlags;
			imageBarrier.subresourceRange.baseArrayLayer = 0;
			imageBarrier.subresourceRange.baseMipLevel = 0;
			imageBarrier.subresourceRange.layerCount = 1;
			imageBarrier.subresourceRange.levelCount = image.mipmapLevels;

			imageBarrier.srcAccessMask = GetAccessFlagsFor(srcLayout);
			imageBarrier.dstAccessMask = GetAccessFlagsFor(dstLayout);

			srcStageFlags |= GetStageFor(srcLayout);
			dstStageFlags |= GetStageFor(dstLayout);

			imageBarriers.Add(imageBarrier);

			image.layout = dstLayout;
		}
	}

	if (!imageBarriers.IsEmpty())
	{
		Assert(srcStageFlags != 0);
		Assert(dstStageFlags != 0);

		cmdBuffer.ImageMemoryBarrier(
			srcStageFlags, dstStageFlags,
			0,
			imageBarriers.Size(), imageBarriers.GetData()
		);
	}
}


