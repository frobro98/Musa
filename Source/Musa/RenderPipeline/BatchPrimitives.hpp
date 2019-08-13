#pragma once

#include "Math/Vector3.hpp"
#include "Math/Vector2.hpp"
#include "Math/Matrix.h"
#include "Color.hpp"

struct View;
struct NativeTexture;

// Define structs or data that gets batched for specific primitives

struct BatchedLineDescription
{
	Vector3 start;
	Vector3 end;
	Color32 color;
};

struct BatchedQuadDescription
{
	Matrix transform;
	Color32 color;
	NativeTexture* texture;
	float uStart, vStart;
	float uEnd, vEnd;
	Vector3 position;
	float width;
	float height;
};

// Define functions to batch specific primitives, like lines, quads, circles, etc

void BatchLinePrimitive(const BatchedLineDescription& lineDesc);
void BatchLinePrimitives(const BatchedLineDescription* lineDesc, uint32 lineCount);

void BatchQuadPrimitive(const BatchedQuadDescription& quadDesc);
void BatchCirclePrimitive();


// Render current batch of primitives
void RenderBatchedPrimitives(class Renderer& renderer, const View& view);

