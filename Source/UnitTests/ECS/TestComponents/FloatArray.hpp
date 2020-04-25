#pragma once

#include "Types/Intrinsics.hpp"
#include "ECS/Component.hpp"

// NOTE: This definitely isn't a good kind of component! It doesn't describe what the data actually is and 
// there might be at least 2 different systems that need a float array with the same N size. What happens then???

template <size_t N>
struct FloatArray : Musa::Component
{
	float32 array[N];
};

