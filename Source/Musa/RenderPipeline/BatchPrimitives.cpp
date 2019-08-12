#include "BatchPrimitives.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceArray.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Shader/ShaderDefinition.hpp"
#include "Scene/ScreenView.hpp"

static DynamicArray<PrimitiveVertex> batchedLines;

static void RenderBatchedLines(Renderer& renderer, const View& /*view*/)
{
	GraphicsPipelineDescription pipelineDesc = {};
	renderer.InitializeWithRenderState(pipelineDesc);
//	pipelineDesc.vertexShader = GetShader;
	pipelineDesc.fragmentShader;
	pipelineDesc.blendingDescs[0] = BlendDesc();
	pipelineDesc.topology = PrimitiveTopology::LineList;
	pipelineDesc.depthStencilTestDesc = DepthTestDesc();
	pipelineDesc.rasterizerDesc = RasterDesc();
	renderer.SetGraphicsPipeline(pipelineDesc);
	
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
