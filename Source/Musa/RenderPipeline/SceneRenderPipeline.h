// Copyright 2020, Nathan Blane

#pragma once

class RenderContext;
class Scene;
class RenderObjectManager;
struct View;
struct GBuffer;
struct SceneRenderTargets;

namespace DeferredRender
{
void RenderSceneDeferred(RenderContext& renderContext, Scene& scene, const GBuffer& gbuffer, const SceneRenderTargets& sceneColorTexture, const RenderObjectManager& renderManager, const View& view);
}
