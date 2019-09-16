#pragma once

#include "Types/Intrinsics.hpp"
#include "InputDefinitions.hpp"

namespace Internal
{
void KeyMessageDownReceived(Inputs::Type key, bool isPressed, bool isRepeated);
void KeyMessageUpReceived(Inputs::Type key);
void MouseMovementChange(uint32 mouseX, uint32 mouseY);
}
