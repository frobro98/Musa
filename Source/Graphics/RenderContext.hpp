// Copyright 2020, Nathan Blane

#pragma once

#include "Color.hpp"
#include "BasicTypes/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct RenderTargetDescription;
struct NativeRenderTargets;
struct GraphicsPipelineDescription;
struct ResourceArray;

class RenderContext
{
public:
	virtual ~RenderContext() = default;

	virtual void BeginRenderFrame(NativeViewport& viewport) = 0;
	virtual void EndRenderFrame(NativeViewport& renderViewport) = 0;

	virtual void InitializeWithRenderState(GraphicsPipelineDescription& pipelineDesc) const = 0;

	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth) = 0;
	virtual void SetScissor(uint32 offsetX, uint32 offsetY, uint32 width, uint32 height) = 0;
	virtual void SetRenderTarget(const RenderTargetDescription& targetDescription, const NativeRenderTargets& renderTextures, const DynamicArray<Color32>& clearColors) = 0;
	virtual void SetVertexBuffer(const NativeVertexBuffer& vertexBuffer) = 0;
	virtual void SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc) = 0;
	virtual void SetGlobalUniform(const void* uniformData, uint32 dataSize) = 0;
	virtual void SetUniformBuffer(const NativeUniformBuffer& uniformBuffer, uint32 bufferIndex) = 0;
	virtual void SetTexture(const NativeTexture& texture, const NativeSampler& sampler, uint32 textureIndex) = 0;
	
	virtual void Draw(uint32 vertexCount, uint32 instanceCount) = 0;
	virtual void DrawIndexed(const NativeIndexBuffer& indexBuffer, uint32 instanceCount) = 0;
	virtual void DrawRaw(const ResourceArray& rawVerts, uint32 instanceCount) = 0;
	virtual void DrawRawIndexed(const ResourceArray& rawVerts, const ResourceArray& rawIndices, uint32 instanceCount) = 0;

	virtual NativeTexture* GetBackBuffer() const = 0;

	virtual void TransitionToWriteState(const NativeTexture** textures, uint32 textureCount) = 0;
	virtual void TransitionToReadState(const NativeTexture** textures, uint32 textureCount) = 0;
};
