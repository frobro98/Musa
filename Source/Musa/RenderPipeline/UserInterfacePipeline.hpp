#pragma once

#include "Types/Intrinsics.hpp"

struct Vector2;
struct View;
struct Color32;
class Renderer;

void AddTextToScreen(const tchar* text, float32 textScale, const Vector2& screenPosition, const Color32& color);
void RenderText(Renderer& renderer, const View& view);

