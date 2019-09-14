#pragma once

#include "String/String.h"
#include "Color.hpp"
#include "Math/Vector2.hpp"

struct ScreenTextItem
{
	String text;
	Color32 color;
	Vector2 screenPosition;
	float32 scale;
};