#pragma once

#include "Types/Intrinsics.hpp"
#include "Visualization/EngineStatView.hpp"


struct Vector2;
struct View;
struct Color32;
class RendererContext;

namespace UI
{
class Context;
}

void RenderUI(UI::Context& ui);

class UserInterfacePipeline
{
public:
	void RenderScreenText(RendererContext& renderer, const View& view);

private:
	EngineStatView statView;
};



