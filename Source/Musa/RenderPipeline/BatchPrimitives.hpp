#pragma once

#include "Math/Vector3.hpp"
#include "Color.hpp"

struct View;

// Define structs or data that gets batched for specific primitives

struct BatchedLineDescription
{
	Vector3 start;
	Vector3 end;
	Color32 color;
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
void RenderBatchedPrimitives(class Renderer& renderer, const View& view);

