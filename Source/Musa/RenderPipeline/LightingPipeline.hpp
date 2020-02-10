#pragma once

#include "Containers/DynamicArray.hpp"
#include "Lighting/Light.hpp"

struct GBuffer;
struct SceneRenderTargets;
struct View;
class RenderContext;

void RenderLights(RenderContext& renderer, const DynamicArray<Light>& lights, const GBuffer& gbuffer, const SceneRenderTargets& sceneColorTexture, const View& view);
