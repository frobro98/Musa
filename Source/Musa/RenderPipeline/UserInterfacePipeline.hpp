#pragma once

#include "Types/Intrinsics.hpp"
#include "Visualization/EngineStatView.hpp"


struct Vector2;
struct View;
struct Color32;
class Renderer;

class UserInterfacePipeline
{
public:
	void RenderScreenText(Renderer& renderer, const View& view);

private:
	EngineStatView statView;
};

