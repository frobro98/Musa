#include "BatchPrimitives.hpp"
#include "Graphics/RenderContext.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/SimplePrimitiveRendering.hpp"
#include "Scene/ScreenView.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"

static BatchCollection collection;


void BatchLinePrimitive(const BatchedLineDescription& lineDesc)
{
	collection.BatchLine(lineDesc);
}

void BatchLinePrimitives(const BatchedLineDescription* lineDesc, uint32 lineCount)
{
	collection.BatchLine(lineDesc, lineCount);
}

void BatchTrianglePrimitive(const BatchedTriangleDescription& triDesc)
{
	collection.BatchTriangle(triDesc);
}

void BatchQuadPrimitive(const BatchedQuadDescription& quadDesc)
{
	collection.BatchQuad(quadDesc);
}

void BatchCirclePrimitive(const BatchedCircleDescription& circleDesc)
{
	collection.BatchCircle(circleDesc);
}

void BatchWireTrianglePrimitive(const BatchedTriangleDescription& triDesc)
{	
	collection.BatchWireTriangle(triDesc);
}

void BatchWireQuadPrimitive(const BatchedQuadDescription& quadDesc)
{
	collection.BatchWireQuad(quadDesc);
}

void BatchWireCirclePrimitive(const BatchedCircleDescription & circleDesc)
{
	collection.BatchWireCircle(circleDesc);
}

void RenderBatchedPrimitives(RenderContext& renderer, const View& view)
{
	collection.RenderBatches(renderer, 
		GetShader<SimplePrimitiveVert>()->GetNativeShader(), GetShader<SimplePrimitiveFrag>()->GetNativeShader(), *view.viewBuffer, *(WhiteTexture()->gpuResource));
}
