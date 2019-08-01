#pragma once

#include "Graphics/Renderer.hpp"
#include "Vulkan/VulkanRenderState.hpp"

struct GraphicsPipelineDescription;
class VulkanVertexBuffer;

class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer(VulkanDevice& device);

	virtual void BeginRenderFrame(NativeViewport& viewport) override;
	virtual void EndRenderFrame(NativeViewport& renderViewport) override;

	virtual void InitializeWithRenderState(GraphicsPipelineDescription& pipelineDesc) const override;

	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth) override;
	virtual void SetScissor(uint32 offsetX, uint32 offsetY, uint32 width, uint32 height) override;
	virtual void SetRenderTarget(const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors) override;
	virtual void SetVertexBuffer(const NativeVertexBuffer& vertexBuffer) override;
	virtual void SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc) override;
	virtual void SetUniformBuffer(const NativeUniformBuffer& uniformBuffer, uint32 bufferIndex) override;
	virtual void SetTexture(const NativeTexture& texture, const NativeSampler& sampler, uint32 textureIndex) override;
//	virtual void SetStorageBuffer() override;
	virtual void Draw(uint32 vertexCount, uint32 instanceCount) override;
	virtual void DrawIndexed(const NativeIndexBuffer& indexBuffer, uint32 instanceCount) override;

	virtual NativeTexture* GetBackBuffer() const override;

	virtual void TransitionToWriteState(const NativeTexture** textures, uint32 textureCount) override;
	virtual void TransitionToReadState(const NativeTexture** textures, uint32 textureCount) override;


private:
	void UpdateViewState(VulkanCommandBuffer& cmdBuffer);

private:
	struct
	{
		DynamicArray<const VulkanVertexBuffer*> vertexBuffers;
		DynamicArray<uint32> vertexBufferOffsets;
	} vertexBuffersAndOffsets;

	// TODO - It sort of feels weird to have this exist in the renderer. Maybe it should, I don't know
	NativeTexture* backBuffer;

	VulkanDevice& logicalDevice;

	RenderTargetDescription currentTarget;
	VulkanRenderState currentRenderState;
	VkViewport viewport;
	// TODO - Isn't always used or enabled...
	VkRect2D  scissorRect;
	// TODO - Might be better to split this update up into each "dirty" element, like viewport but not scissor
	bool updateViewState;
};