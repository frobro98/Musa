
#include "DeferredRenderPipeline.hpp"
#include "SceneRenderPipeline.h"
#include "UserInterfacePipeline.hpp"

#include "Engine/FrameRenderTargets.hpp"
#include "Graphics/GraphicsInterface.hpp"

void DeferredRender(const FrameRenderTargets& frameTargets, const View& view)
{
	RenderContext* renderContext = GetGraphicsInterface().GetRenderContext();
	Assert(renderContext);
	RenderSceneDeferred(*renderContext, frameTargets.gbuffer, frameTargets.sceneTargets, view);
}

