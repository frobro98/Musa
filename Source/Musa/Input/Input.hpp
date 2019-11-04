#pragma once

#include "String/String.h"
#include "String/StringView.hpp"
#include "Containers/Map.h"
#include "InputDefinitions.hpp"
#include "Math/IntVector2.hpp"
#include "Input/PlayerInputContext.hpp"

class MusaApp;

namespace Input
{
void InitializeInput(MusaApp& app);

bool IsPressed(Inputs::Type key);
bool IsDown(Inputs::Type key);
IntVector2 GetMousePosition();
}
