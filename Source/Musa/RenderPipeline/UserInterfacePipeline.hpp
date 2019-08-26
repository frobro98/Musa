#pragma once

#include "Types/Intrinsics.hpp"
#include "String/String.h"

struct View;
class Renderer;

void RenderTextOnScreen(Renderer& renderer, const View& view, const String& text);

