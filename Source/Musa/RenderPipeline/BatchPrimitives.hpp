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

struct BatchedTriangleDescription
{
	Color32 color;
	Vector3 pos0;
	Vector3 pos1;
	Vector3 pos2;
};

struct BatchedQuadDescription
{
	Color32 color;
	Vector3 position;
	float width;
	float height;
};

struct BatchedCircleDescription
{
	Color32 color;
	Vector3 position;
	Vector3 up;
	Vector3 right;
	float radius;
	float numSegments;
};

// Define functions to batch specific primitives, like lines, quads, circles, etc

void BatchLinePrimitive(const BatchedLineDescription& lineDesc);
void BatchLinePrimitives(const BatchedLineDescription* lineDesc, uint32 lineCount);

void BatchTrianglePrimitive(const BatchedTriangleDescription& triDesc);
void BatchQuadPrimitive(const BatchedQuadDescription& quadDesc);
void BatchCirclePrimitive(const BatchedCircleDescription& circleDesc);

void BatchWireTrianglePrimitive(const BatchedTriangleDescription& triDesc);
void BatchWireQuadPrimitive(const BatchedQuadDescription& quadDesc);
void BatchWireCirclePrimitive(const BatchedCircleDescription& circleDesc);


// Render current batch of primitives
void RenderBatchedPrimitives(class Renderer& renderer, const View& view);

