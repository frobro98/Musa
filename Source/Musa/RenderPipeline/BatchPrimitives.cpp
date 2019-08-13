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
static DynamicArray<BatchedQuadDescription> batchedQuads;

static void RenderBatchedLines(Renderer& renderer, const View& view)
{
	if (batchedLines.Size() > 0)
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
		renderer.SetTexture(*(WhiteTexture()->gpuResource), *SamplerDesc(), 1);

		renderer.DrawRaw(batchedLines, 1);
	}
}

static void RenderBatchedQuads(Renderer& renderer, const View& view)
{
	UNUSED(view, renderer);
	if (batchedQuads.Size() > 0)
	{
		batchedQuads.Sort(
			[](const BatchedQuadDescription& desc0, const BatchedQuadDescription& desc1)
			{
				return desc0.texture != desc1.texture;
			}
		);
	}
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

void BatchLinePrimitives(const BatchedLineDescription* lineDesc, uint32 lineCount)
{
	for (uint32 i = 0; i < lineCount; ++i)
	{
		BatchLinePrimitive(lineDesc[i]);
	}
}

void BatchQuadPrimitive(const BatchedQuadDescription& quadDesc)
{
	batchedQuads.Add(quadDesc);
}

void RenderBatchedPrimitives(Renderer& renderer, const View& view)
{
	RenderBatchedLines(renderer, view);
	RenderBatchedQuads(renderer, view);

	batchedLines.Clear();
	batchedQuads.Clear();
}
