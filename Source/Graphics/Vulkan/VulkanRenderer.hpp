#pragma once

#include "Graphics/Renderer.hpp"
#include "Vulkan/VulkanRenderState.hpp"

struct GraphicsPipelineDescription;

class VulkanRenderer : public Renderer
{
public:
	virtual void BeginRenderFrame() override;
	virtual void EndRenderFrame() override;
	virtual void PresentRender() override;

	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth) override;
	virtual void SetScissor(uint32 offsetX, uint32 offsetY, uint32 width, uint32 height) override;
	virtual void SetRenderTarget(const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors) override;
	virtual void BindVertexBuffers(const VulkanVertexBuffer** vertexBuffers, uint32 bufferCount) override;
	virtual void SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc) override;
	virtual void SetUniformBuffer(const VulkanUniformBuffer& uniformBuffer, uint32 bufferIndex) override;
	virtual void SetTexture(const VulkanTexture& texture, const TextureSampler& sampler, uint32 textureIndex) override;
//	virtual void SetStorageBuffer() override;
	virtual void Draw(uint32 vertexCount, uint32 instanceCount) override;
	virtual void DrawIndexed(const VulkanIndexBuffer& indexBuffer, uint32 instanceCount) override;

private:
	VulkanRenderState currentRenderState;
};