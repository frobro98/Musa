#pragma once

#include "Color.hpp"
#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"

struct RenderTargetDescription;
struct RenderTargetTextures;
class VulkanVertexBuffer;
class VulkanIndexBuffer;
class VulkanUniformBuffer;
struct GraphicsPipelineDescription;
struct VulkanTexture;
struct TextureSampler;

class Renderer
{
public:
	virtual ~Renderer() = default;

	virtual void BeginRenderFrame() = 0;
	virtual void EndRenderFrame() = 0;
	virtual void PresentRender() = 0;

	virtual void InitializeWithRenderState(GraphicsPipelineDescription& pipelineDesc) const = 0;

	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth) = 0;
	virtual void SetScissor(uint32 offsetX, uint32 offsetY, uint32 width, uint32 height) = 0;
	virtual void SetRenderTarget(const RenderTargetDescription& targetDescription, const RenderTargetTextures& renderTextures, const DynamicArray<Color32>& clearColors) = 0;
	virtual void SetVertexBuffer(const VulkanVertexBuffer& vertexBuffer) = 0;
	virtual void SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc) = 0;
	virtual void SetUniformBuffer(const VulkanUniformBuffer& uniformBuffer, uint32 bufferIndex) = 0;
	virtual void SetTexture(const VulkanTexture& texture, uint32 textureIndex) = 0;
	//virtual void SetStorageBuffer(cons) = 0;
	virtual void Draw(uint32 vertexCount, uint32 instanceCount) = 0;
	virtual void DrawIndexed(const VulkanIndexBuffer& indexBuffer, uint32 instanceCount) = 0;

	virtual void TransitionToWriteState(const VulkanTexture** textures, uint32 textureCount) = 0;
	virtual void TransitionToReadState(const VulkanTexture** textures, uint32 textureCount) = 0;
};

Renderer& GetRenderContext();