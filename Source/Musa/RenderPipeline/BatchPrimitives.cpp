#include "BatchPrimitives.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceArray.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/SimplePrimitiveRendering.hpp"
#include "Scene/ScreenView.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"

static DynamicArray<PrimitiveVertex> batchedLines;

static void RenderBatchedLines(Renderer& renderer, const View& view)
{
	GraphicsPipelineDescription pipelineDesc = {};
	renderer.InitializeWithRenderState(pipelineDesc);
	pipelineDesc.vertexShader = &GetShader<SimplePrimitiveVert>()->GetNativeShader();
	pipelineDesc.fragmentShader = &GetShader<SimplePrimitiveFrag>()->GetNativeShader();
	pipelineDesc.blendingDescs[0] = BlendDesc();
	pipelineDesc.blendingDescs[1] = BlendDesc();
	pipelineDesc.blendingDescs[2] = BlendDesc();
	pipelineDesc.topology = PrimitiveTopology::LineList;
	pipelineDesc.depthStencilTestDesc = DepthTestDesc();
	pipelineDesc.rasterizerDesc = RasterDesc();
	pipelineDesc.vertexInputs = GetVertexInput<PrimitiveVertex>();
	renderer.SetGraphicsPipeline(pipelineDesc);
	
	renderer.SetUniformBuffer(*view.viewBuffer, 0);
	renderer.SetTexture(*(::Internal::WhiteTexture()->gpuResource), *SamplerDesc(), 1);

	renderer.DrawRaw(batchedLines, 1);
}

void BatchLinePrimitive(const BatchedLineDescription& lineDesc)
{
	PrimitiveVertex vert = {};
	vert.position = lineDesc.start;
	vert.color = lineDesc.color;
	batchedLines.Add(vert);

	vert = {};
	vert.position = lineDesc.end;
	vert.color = lineDesc.color;
	batchedLines.Add(vert);
}

void RenderBatchedPrimitives(Renderer& renderer, const View& view)
{
	RenderBatchedLines(renderer, view);

	batchedLines.Clear();
}
