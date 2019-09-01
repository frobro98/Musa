#pragma once

#include "Types/Intrinsics.hpp"
#include "String/String.h"

struct Vector2;
struct View;
struct Color32;
class Renderer;

void AddTextToScreen(const String& text, float32 textScale, const Vector2& screenPosition, const Color32& color);
void RenderText(Renderer& renderer, const View& view);

