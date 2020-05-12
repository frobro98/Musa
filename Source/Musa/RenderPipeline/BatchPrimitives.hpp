// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/BatchCollection.hpp"

struct View;

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
void RenderBatchedPrimitives(class RenderContext& renderer, const View& view);

