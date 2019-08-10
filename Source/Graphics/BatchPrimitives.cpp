#include "BatchPrimitives.hpp"
#include "Renderer.hpp"
#include "Mesh/GeometryPrimitives.h"

static DynamicArray<Vertex> batchedLines;

static void RenderBatchedLines()
{

}

void BatchLinePrimitive(const BatchedLineDescription& /*lineDesc*/)
{

}

void RenderBatchedPrimitives(Renderer& /*renderer*/)
{
	RenderBatchedLines();


	batchedLines.Clear();
}
