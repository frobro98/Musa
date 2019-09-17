#include "BatchPrimitives.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceArray.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/SimplePrimitiveRendering.hpp"
#include "Scene/ScreenView.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Math/MathConstants.h"
#include "Math/MathUtilities.h"

struct PrimitiveTriangle
{
	PrimitiveVertex v0;
	PrimitiveVertex v1;
	PrimitiveVertex v2;
};

using LineList = DynamicArray<PrimitiveVertex>;
using TriList = DynamicArray<PrimitiveTriangle>;


static LineList batchedLines;
static TriList batchedTris;

static DynamicArray<NativeUniformBuffer*> frameTransformBuffers;

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

static void RenderBatchedTriangles(Renderer& renderer, const View& view)
{
	if (batchedTris.Size() > 0)
	{
		uint32 vertCount = batchedTris.Size() * 3;
		DynamicArray<PrimitiveVertex> totalTriVerts;
		totalTriVerts.Reserve(vertCount);
		for (uint32 i = 0; i < batchedTris.Size(); ++i)
		{
			const PrimitiveTriangle& tri = batchedTris[i];
			totalTriVerts.Add(tri.v0);
			totalTriVerts.Add(tri.v1);
			totalTriVerts.Add(tri.v2);
		}

		GraphicsPipelineDescription pipelineDesc = {};
		renderer.InitializeWithRenderState(pipelineDesc);
		pipelineDesc.vertexShader = &GetShader<SimplePrimitiveVert>()->GetNativeShader();
		pipelineDesc.fragmentShader = &GetShader<SimplePrimitiveFrag>()->GetNativeShader();
		pipelineDesc.blendingDescs[0] = BlendDesc();
		pipelineDesc.blendingDescs[1] = BlendDesc();
		pipelineDesc.blendingDescs[2] = BlendDesc();
		pipelineDesc.topology = PrimitiveTopology::TriangleList;
		pipelineDesc.depthStencilTestDesc = DepthTestDesc();
		pipelineDesc.rasterizerDesc = RasterDesc<FillMode::Full, CullingMode::None>();
		pipelineDesc.vertexInputs = GetVertexInput<PrimitiveVertex>();
		renderer.SetGraphicsPipeline(pipelineDesc);

		renderer.SetUniformBuffer(*view.viewBuffer, 0);
		renderer.SetTexture(*(WhiteTexture()->gpuResource), *SamplerDesc(), 1);

		renderer.DrawRaw(totalTriVerts, 1);
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

void BatchTrianglePrimitive(const BatchedTriangleDescription& triDesc)
{
	PrimitiveVertex v0 = {
		triDesc.pos0,
		Vector2(0, 0), 
		triDesc.color
	};
	PrimitiveVertex v1 = {
	triDesc.pos1,
	Vector2(1, 1),
	triDesc.color
	};
	PrimitiveVertex v2 = {
		triDesc.pos2,
		Vector2(0, 1),
		triDesc.color
	};
	batchedTris.Add(PrimitiveTriangle{ v0, v1, v2 });
}

void BatchQuadPrimitive(const BatchedQuadDescription& quadDesc)
{
	const float halfWidth = quadDesc.width * .5f;
	const float halfHeight = quadDesc.height * .5f;
	{
		PrimitiveVertex v0 = {
			Vector3(
				quadDesc.position.x - halfWidth,
				quadDesc.position.y - halfHeight,
				quadDesc.position.z),
			Vector2(0, 0), quadDesc.color
		};
		PrimitiveVertex v1 = {
			Vector3(
				quadDesc.position.x + halfWidth,
				quadDesc.position.y + halfHeight,
				quadDesc.position.z),
			Vector2(1, 1), quadDesc.color
		};
		PrimitiveVertex v2 = {
			Vector3(
				quadDesc.position.x - halfWidth,
				quadDesc.position.y + halfHeight,
				quadDesc.position.z),
			Vector2(0, 1), quadDesc.color
		};

		batchedTris.Add(PrimitiveTriangle{ v0, v1, v2 });
	}

	{
		PrimitiveVertex v0 = {
		Vector3(
			quadDesc.position.x - halfWidth,
			quadDesc.position.y - halfHeight,
			quadDesc.position.z),
		Vector2(0, 0), quadDesc.color
		};
		PrimitiveVertex v1 = {
			Vector3(
				quadDesc.position.x + halfWidth,
				quadDesc.position.y - halfHeight,
				quadDesc.position.z),
			Vector2(1, 0), quadDesc.color
		};
		PrimitiveVertex v2 = {
			Vector3(
				quadDesc.position.x + halfWidth,
				quadDesc.position.y + halfHeight,
				quadDesc.position.z),
			Vector2(1, 1), quadDesc.color
		};
		batchedTris.Add(PrimitiveTriangle{ v0, v1, v2 });
	}
}

void BatchCirclePrimitive(const BatchedCircleDescription& circleDesc)
{
	const Vector3 center = circleDesc.position;
	const Vector3 up = circleDesc.up;
	const Vector3 right = circleDesc.right;
	const float32 angleIncrement = Math::TwoPi / circleDesc.numSegments;
	const float32 radius = circleDesc.radius;
	const float32 diameter = radius * 2.f;
	float32 delta = 0;

	PrimitiveVertex v0 = {
		center,
		Vector2(.5f, .5f), circleDesc.color
	};

	auto positionCalculation = [&](float delt) {
		return center + (Math::Cos(delt) * right + Math::Sin(delt) * up) * radius;
	};

	for (uint32 i = 0; i < circleDesc.numSegments; ++i)
	{
		Vector3 pos = positionCalculation(delta);
		PrimitiveVertex v1 = {
			pos,
			Vector2(
				.5f + (pos.x - center.x) / diameter, 
				.5f + (pos.y - center.y) / diameter
			),
			circleDesc.color
		};
		delta += angleIncrement;

		pos = positionCalculation(delta);
		PrimitiveVertex v2 = {
			pos,
			Vector2(
				.5f + (pos.x - center.x) / diameter,
				.5f + (pos.y - center.y) / diameter
			),
			circleDesc.color
		};

		batchedTris.Add(PrimitiveTriangle{ v0, v1, v2 });
	}
}

void BatchWireTrianglePrimitive(const BatchedTriangleDescription& triDesc)
{	
	BatchedLineDescription line = {};
	line.color = triDesc.color;
	line.start = triDesc.pos0;
	line.end = triDesc.pos1;
	BatchLinePrimitive(line);

	line.start = triDesc.pos1;
	line.end = triDesc.pos2;
	BatchLinePrimitive(line);

	line.start = triDesc.pos2;
	line.end = triDesc.pos0;
	BatchLinePrimitive(line);
	
}

void BatchWireQuadPrimitive(const BatchedQuadDescription& quadDesc)
{
	float halfWidth = quadDesc.width * .5f;
	float halfHeight = quadDesc.height * .5f;

	Vector3 v0 = Vector3(
		quadDesc.position.x - halfWidth,
		quadDesc.position.y - halfHeight,
		quadDesc.position.z);
	Vector3 v1 = Vector3(
		quadDesc.position.x - halfWidth,
		quadDesc.position.y + halfHeight,
		quadDesc.position.z);
	Vector3 v2 = Vector3(
		quadDesc.position.x + halfWidth,
		quadDesc.position.y + halfHeight,
		quadDesc.position.z);
	Vector3 v3 = Vector3(
		quadDesc.position.x + halfWidth,
		quadDesc.position.y - halfHeight,
		quadDesc.position.z);

	BatchedLineDescription line = {};
	line.color = quadDesc.color;
	line.start = v0;
	line.end = v1;
	BatchLinePrimitive(line);

	line.start = v1;
	line.end = v2;
	BatchLinePrimitive(line);

	line.start = v2;
	line.end = v3;
	BatchLinePrimitive(line);

	line.start = v3;
	line.end = v0;
	BatchLinePrimitive(line);
}

void BatchWireCirclePrimitive(const BatchedCircleDescription & circleDesc)
{
	const Vector3 center = circleDesc.position;
	const Vector3 up = circleDesc.up;
	const Vector3 right = circleDesc.right;
	const float32 angleIncrement = Math::TwoPi / circleDesc.numSegments;
	const float32 radius = circleDesc.radius;
	float32 delta = 0;

	auto positionCalculation = [&](float delt) {
		return center + (Math::Cos(delt) * right + Math::Sin(delt) * up) * radius;
	};
	Vector3 prevVertPos = positionCalculation(delta);
	delta += angleIncrement;

	BatchedLineDescription line = {};
	line.color = circleDesc.color;

	for (uint32 i = 0; i < circleDesc.numSegments; ++i)
	{
		Vector3 currVertPos = positionCalculation(delta);
		line.start = prevVertPos;
		line.end = currVertPos;
		BatchLinePrimitive(line);

		delta += angleIncrement;
		prevVertPos = currVertPos;
	}
}

void RenderBatchedPrimitives(Renderer& renderer, const View& view)
{
	RenderBatchedLines(renderer, view);
	RenderBatchedTriangles(renderer, view);

	batchedLines.Clear();
	batchedTris.Clear();
}