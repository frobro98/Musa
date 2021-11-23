// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/Color.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Graphics/GraphicsAPI.hpp"

struct RenderTargetDescription;
struct NativeRenderTargets;
struct GraphicsPipelineDescription;
struct ResourceArray;

class GRAPHICS_API RenderContext
{
public:
	virtual ~RenderContext() = default;

	virtual void BeginRenderFrame(NativeViewport& viewport) = 0;
	virtual void EndRenderFrame(NativeViewport& renderViewport) = 0;

	virtual void InitializeWithRenderState(GraphicsPipelineDescription& pipelineDesc) const = 0;

	virtual void SetViewport(u32 x, u32 y, u32 width, u32 height, float minDepth, float maxDepth) = 0;
	virtual void SetScissor(u32 offsetX, u32 offsetY, u32 width, u32 height) = 0;
	virtual void SetRenderTarget(const RenderTargetDescription& targetDescription, const NativeRenderTargets& renderTextures, const DynamicArray<Color32>& clearColors) = 0;
	virtual void SetVertexBuffer(const NativeVertexBuffer& vertexBuffer) = 0;
	virtual void SetGraphicsPipeline(const GraphicsPipelineDescription& pipelineDesc) = 0;
	virtual void SetGlobalUniform(const void* uniformData, u32 dataSize) = 0;
	virtual void SetUniformBuffer(const NativeUniformBuffer& uniformBuffer, u32 bufferIndex) = 0;
	virtual void SetTexture(const NativeTexture& texture, const NativeSampler& sampler, u32 textureIndex) = 0;
	
	virtual void Draw(u32 vertexCount, u32 instanceCount) = 0;
	virtual void DrawIndexed(const NativeIndexBuffer& indexBuffer, u32 instanceCount) = 0;
	virtual void DrawRaw(const ResourceArray& rawVerts, u32 instanceCount) = 0;
	virtual void DrawRawIndexed(const ResourceArray& rawVerts, const ResourceArray& rawIndices, u32 indexStride, u32 instanceCount) = 0;

	virtual NativeTexture* GetBackBuffer() const = 0;

	virtual void TransitionToWriteState(const NativeTexture** textures, u32 textureCount) = 0;
	virtual void TransitionToReadState(const NativeTexture** textures, u32 textureCount) = 0;
};
