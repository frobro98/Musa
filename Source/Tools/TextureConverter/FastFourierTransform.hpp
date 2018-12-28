#pragma once

#include "EngineCore/Types.h"
#include "Containers/Array.h"

void FastFourier(
	uint32 numSamples,
	const Array<float>& realIn, 
	const Array<float>& imageIn,
	Array<float>& realOut,
	Array<float>& imageOut
);

void InverseFastFourier(
	uint32 numSamples,
	const Array<float>& realIn,
	const Array<float>& imageIn,
	Array<float>& realOut,
	Array<float>& imageOut
);