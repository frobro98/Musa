#pragma once

#include "Containers/DynamicArray.hpp"
#include "Lighting/Light.hpp"

struct GBuffer;
struct SceneRenderTargets;
struct View;
class RendererContext;

void RenderLights(RendererContext& renderer, const DynamicArray<Light>& lights, const GBuffer& gbuffer, const SceneRenderTargets& sceneColorTexture, const View& view);
