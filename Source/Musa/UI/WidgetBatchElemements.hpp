#pragma once

#include "Containers/DynamicArray.hpp"
#include "Mesh/GeometryPrimitives.h"

struct NativeTexture;

struct BatchElement
{
	DynamicArray<PrimitiveVertex> batchedVertices;
	const NativeTexture* batchTexture;
	//TODO - clipping must be addressed
	// Either scissor or stencil clipping...?
};

class WidgetBatchElements
{
	BatchElement& GetBatchElement(const NativeTexture* batchTexture);
	forceinline const DynamicArray<BatchElement>& GetBatchElementList() { return batchElements; }

	// TODO - Clipping

private:
	DynamicArray<BatchElement> batchElements;
};

