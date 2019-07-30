#pragma once

#include "Platform.h"

class Renderer;
class Camera;
class Model;
class Scene;
class Viewport;
struct View;
class Renderer;

// TODO - These need to be refactored out of here!
#include "Graphics/Vulkan/VulkanRenderState.hpp"
class VulkanCommandBuffer;

class SceneRendering
{
public:
	SceneRendering();

	void RenderScene(Scene& scene, const Viewport& viewport, const View& view);

private:
// 	uint32 DetermineJobCount() const;
// 	void SecondaryGBufferPass_Job(VulkanCommandBuffer& secondaryCmdBuffer, const View& view, VkFramebuffer framebufferHandle, VkRenderPass renderpassHandle, uint32 startRenderIndex, uint32 endRenderIndex) const;

	//void ForwardRender(Scene& scene, const View& view);
	void DeferredRender(Scene& scene, const Viewport& viewport, const View& view);

	void RenderGBufferPass(Scene& scene, const View& view);
	void RenderShadowPass(Scene& scene);
	void RenderUnlitToScreen();
	void RenderGBUffersToScreen(Scene& scene, const View& view);

	void TransitionTargetsToRead(RenderTargetTextures& targets);
	void TransitionTargetsToWrite(RenderTargetTextures& targets);

	void SetViewportAndScissor(const View& view) const;

	// These functions are waaaaayyyy too low level for rendering in the SceneRendering class
	// They need to be moved outta here
// 	void CreateComputePipelines();
// 	void CreateFirstPassPipeline();
// 	void CreateBlendingPassPipeline();
// 	void CreateProcessingPathPipeline();
// 	void CreateFinalPassPipeline();
// 	ComputeEntity& BuildComputeCommandBuffer(Model& model);

private:
	Renderer* renderer;
	//VulkanRenderState renderingState;
// 	VulkanComputePipeline* firstPassPipeline = nullptr;
// 	VulkanComputePipeline* blendingPassPipeline = nullptr;
// 	VulkanComputePipeline* processLocalsPassPipeline = nullptr;
// 	VulkanComputePipeline* finalPassPipeline = nullptr;
};