
#include "LightingPipeline.hpp"
#include "FrameRenderTargets.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderTargetDescription.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Scene/ScreenView.hpp"
#include "Shader/ShaderObjects/DeferredLightingShading.hpp"

static void SetupLightRender(Renderer& renderer, const GBuffer& gbuffer, const SceneRenderTargets& sceneColorTexture)
{
	UNUSED(sceneColorTexture);
//	renderer.SetRenderTarget();

	GraphicsPipelineDescription pipelineDesc = {};
	renderer.InitializeWithRenderState(pipelineDesc);
	pipelineDesc.vertexInputs = {};
	pipelineDesc.rasterizerDesc = { 1.25f, 1.75f, FillMode::Full, CullingMode::Front };
	pipelineDesc.blendingDescs[0] = BlendDesc();
	pipelineDesc.depthStencilTestDesc = DepthTestDesc();
	pipelineDesc.topology = PrimitiveTopology::TriangleList;
	pipelineDesc.vertexShader = &GetShader<DeferredLightingVert>()->GetNativeShader();
	pipelineDesc.fragmentShader = &GetShader<DeferredLightingVert>()->GetNativeShader();
	renderer.SetGraphicsPipeline(pipelineDesc);

	renderer.SetTexture(*gbuffer.positionTexture, *SamplerDesc(), 0);
	renderer.SetTexture(*gbuffer.normalTexture, *SamplerDesc(), 1);
	renderer.SetTexture(*gbuffer.diffuseTexture, *SamplerDesc(), 2);
}

static void RenderLight(Renderer& renderer, const Light& light, const GBuffer& gbuffer, const View& view)
{
	UNUSED(light, gbuffer);
	// Set viewport
	uint32 viewWidth = (uint32)view.description.viewport.width;
	uint32 viewHeight = (uint32)view.description.viewport.height;
	renderer.SetViewport(0, 0, viewWidth, viewHeight, 0, 1);
	renderer.SetScissor(0, 0, viewWidth, viewHeight);

}

void RenderLights(Renderer& renderer, const DynamicArray<Light*>& lights, const GBuffer& gbuffer, const SceneRenderTargets& sceneColorTexture, const View& view)
{
	if (lights.Size() > 0)
	{
		SetupLightRender(renderer, gbuffer, sceneColorTexture);
		// Render individual lights
		for (const auto& light : lights)
		{
			RenderLight(renderer, *light, gbuffer, view);
		}
	}
}
