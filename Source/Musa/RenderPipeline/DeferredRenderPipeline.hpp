#pragma once

struct View;
struct FrameRenderTargets;
class Scene;

void DeferredRender(const FrameRenderTargets& frameTargets, Scene& scene, const View& view);


