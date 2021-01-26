// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

namespace Musa
{

// Base class of all components
struct Component
{
	// NOTE - No virtual destructor here. The ComponentType works around this by calling the concrete type's destructor
};

}
