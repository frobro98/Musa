#pragma once

#include "Containers/DynamicArray.hpp"
#include "Lighting/Light.hpp"

struct GBuffer;
struct View;
class RenderContext;

namespace DeferredRender
{
void RenderLights(RenderContext& renderer, Scene& scene, const GBuffer& gbuffer, const View& view);
}
