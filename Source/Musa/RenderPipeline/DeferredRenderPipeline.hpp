#pragma once

struct View;
struct FrameRenderTargets;
class Scene;

namespace DeferredRender
{
void Render(const FrameRenderTargets& frameTargets, Scene& scene, const View& view);
}


