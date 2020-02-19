#pragma once

#include "Types/Intrinsics.hpp"
#include "Visualization/EngineStatView.hpp"


struct Vector2;
struct View;
struct Color32;
struct RenderTarget;
class RenderContext;

namespace UI
{
class Context;
}

namespace DeferredRender
{
void RenderUI(RenderContext& renderContext, UI::Context& ui, const RenderTarget& sceneColorTarget, const RenderTarget& uiTarget, const View& view);
}




