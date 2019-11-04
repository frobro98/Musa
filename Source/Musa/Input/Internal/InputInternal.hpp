#pragma once

#include "Types/Intrinsics.hpp"
#include "Input/InputDefinitions.hpp"

namespace Internal
{
void KeyMessageDownReceived(Inputs::Type key, bool isPressed, bool isRepeated);
void KeyMessageUpReceived(Inputs::Type key);

void UpdateInputMap();
}
