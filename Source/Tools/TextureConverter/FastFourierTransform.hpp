#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"

void FastFourier(
	uint32 numSamples,
	const DynamicArray<float>& realIn, 
	const DynamicArray<float>& imageIn,
	DynamicArray<float>& realOut,
	DynamicArray<float>& imageOut
);

void InverseFastFourier(
	uint32 numSamples,
	const DynamicArray<float>& realIn,
	const DynamicArray<float>& imageIn,
	DynamicArray<float>& realOut,
	DynamicArray<float>& imageOut
);