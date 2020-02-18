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
void RenderUI(RenderContext& renderer, UI::Context& ui, EngineStatView& statView, const RenderTarget& sceneColorTarget, const RenderTarget& uiRenderTarget);
}




