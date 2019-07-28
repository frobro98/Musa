#include "VulkanRenderer.hpp"
#include "VulkanRenderState.hpp"
#include "EngineCore/Assertion.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanDescriptorSet.h"
#include "GraphicsInterface.hpp"
#include "VulkanUniformBuffer.h"

void VulkanRenderer::BeginRenderFrame()
{
}

void VulkanRenderer::EndRenderFrame()
{
}

void VulkanRenderer::PresentRender()
{
}

void VulkanRenderer::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth)
{
	UNUSED(x, y, width, height, maxDepth, minDepth);
}

void VulkanRenderer::SetScissor(uint32 offsetX, uint32 offsetY, uint32 width, uint32 height)
{
	// TODO - This doesn't necessarily mean that the scissor will be in effect. There could be a way to optimize this call with reusing something like th viewport
	UNUSED(offsetY, offsetX, width, height);
}

void VulkanRenderer::SetRenderTarget(const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors)
{
	UNUSED(targetDescription, renderTextures, clearColors);
}

void VulkanRenderer::BindVertexBuffers(const VulkanVertexBuffer** vertexBuffers, uint32 bufferCount)
{
	UNUSED(vertexBuffers, bufferCount);
}

void VulkanRenderer::SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc)
{
	UNUSED(pipelineDesc);
}

void VulkanRenderer::SetUniformBuffer(const VulkanUniformBuffer& uniformBuffer, uint32 bufferIndex)
{
	currentRenderState.SetUniformBuffer(uniformBuffer.GetBuffer(), bufferIndex);
}

void VulkanRenderer::SetTexture(const VulkanTexture& texture, const TextureSampler& sampler, uint32 textureIndex)
{
	UNUSED(textureIndex, texture, sampler);
}

void VulkanRenderer::Draw(uint32 vertexCount, uint32 instanceCount)
{
	UNUSED(vertexCount, instanceCount);
}

void VulkanRenderer::DrawIndexed(const VulkanIndexBuffer& indexBuffer, uint32 instanceCount)
{
	UNUSED(indexBuffer, instanceCount);
}


