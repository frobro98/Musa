#pragma once

struct View;
struct FrameRenderTargets;
class Scene;
class RenderObjectManager;
class RenderContext;
struct RenderTarget;

namespace DeferredRender
{
void Render(const FrameRenderTargets& frameTargets, Scene& scene, const RenderObjectManager& renderManager, const View& view);
void ComposeBackbuffer(RenderContext& context, const RenderTarget& sceneColor, const RenderTarget& uiColor, const View& view);
}


