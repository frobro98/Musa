#pragma once

#include "Math/Vector4.h"
#include "Color.hpp"

// Define structs or data that gets batched for specific primitives

struct BatchedLineDescription
{
	Vector4 start;
	Vector4 end;
	Color color;
};

struct BatchedQuadPrimitive
{

};

// Define functions to batch specific primitives, like lines, quads, circles, etc

void BatchLinePrimitive(const BatchedLineDescription& lineDesc);
void BatchLinePrimitives();

void BatchCirclePrimitive();
void BatchBoxPrimitive();

void BatchQuadPrimitive();


// Render current batch of primitives
void RenderBatchedPrimitives(class Renderer& renderer);

