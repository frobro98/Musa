#pragma once

#include "Platform.h"
// TODO - The UI part of the render pipeline shouldn't really live in the scene rendering part...
#include "RenderPipeline/UserInterfacePipeline.hpp"

class RenderContext;
class Camera;
class Model;
class Scene;
class Viewport;
class RenderContext;
class RenderObjectManager;
struct View;
struct NativeRenderTargets;
struct GBuffer;
struct SceneRenderTargets;
struct RenderTargetDescription;
struct RenderTarget;

void RenderSceneDeferred(RenderContext& renderContext, Scene& scene, const GBuffer& gbuffer, const SceneRenderTargets& sceneColorTexture, const View& view);

class SceneRenderPipeline
{
public:

	void RenderScene(RenderContext& renderer, Scene& scene, const GBuffer& gbuffer, const SceneRenderTargets& sceneTargets, RenderTarget& uiTarget, RenderObjectManager& renderManager, const Viewport& viewport, const View& view);

private:
// 	uint32 DetermineJobCount() const;
// 	void SecondaryGBufferPass_Job(VulkanCommandBuffer& secondaryCmdBuffer, const View& view, VkFramebuffer framebufferHandle, VkRenderPass renderpassHandle, uint32 startRenderIndex, uint32 endRenderIndex) const;

	//void ForwardRender(Scene& scene, const View& view);
	void DeferredRender(RenderContext& renderer, Scene& scene, const GBuffer& gbuffer, const SceneRenderTargets& sceneTargets, RenderTarget& uiTarget, RenderObjectManager& renderManager, const Viewport& viewport, const View& view);

	void RenderGBufferPass(RenderContext& renderer, const RenderTargetDescription& gbufferDesc, RenderObjectManager& renderManager, const View& view);
	//void RenderShadowPass(Renderer& renderer, Scene& scene);
	void RenderGBUffersToScreen(RenderContext& renderer, const SceneRenderTargets& sceneTargets, RenderTarget& userInterfaceTarget, const View& view);

	// TODO - This should become a helper function...
	void SetViewportAndScissor(RenderContext& renderer, const View& view) const;

	// These functions are waaaaayyyy too low level for rendering in the SceneRendering class
	// They need to be moved outta here
// 	void CreateComputePipelines();
// 	void CreateFirstPassPipeline();
// 	void CreateBlendingPassPipeline();
// 	void CreateProcessingPathPipeline();
// 	void CreateFinalPassPipeline();
// 	ComputeEntity& BuildComputeCommandBuffer(Model& model);

private:
	// TODO - The UI part of the render pipeline shouldn't really live in the scene rendering part...
	UserInterfacePipeline uiPipeline;
	//VulkanRenderState renderingState;
// 	VulkanComputePipeline* firstPassPipeline = nullptr;
// 	VulkanComputePipeline* blendingPassPipeline = nullptr;
// 	VulkanComputePipeline* processLocalsPassPipeline = nullptr;
// 	VulkanComputePipeline* finalPassPipeline = nullptr;
};