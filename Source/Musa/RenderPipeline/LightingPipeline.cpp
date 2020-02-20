
#include "LightingPipeline.hpp"
#include "Scene/GBufferTargets.hpp"
#include "Scene/SceneRenderTargets.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderTargetDescription.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Scene/ScreenView.hpp"
#include "Shader/ShaderObjects/DeferredLightingShading.hpp"
#include "Scene/Scene.hpp"
#include "Lighting/Light.hpp"
#include "Math/MatrixUtilities.hpp"

constexpr uint32 ShadowMapWidth = 1024;
constexpr uint32 ShadowMapHeight = 1024;

static void SetupLightRender(RenderContext& renderer, const GBuffer& gbuffer)
{
//	renderer.SetRenderTarget();

	GraphicsPipelineDescription pipelineDesc = {};
	renderer.InitializeWithRenderState(pipelineDesc);
	pipelineDesc.vertexInputs = {};
	pipelineDesc.rasterizerDesc = { 1.25f, 1.75f, FillMode::Full, CullingMode::Front };
	pipelineDesc.blendingDescs[0] = BlendDesc<ColorMask::RGB, BlendOperation::Add, BlendFactor::One, BlendFactor::One, BlendOperation::Add, BlendFactor::One, BlendFactor::One>();
	pipelineDesc.depthStencilTestDesc = DepthTestDesc<CompareOperation::LessThanOrEqual, false>();
	pipelineDesc.topology = PrimitiveTopology::TriangleList;
	pipelineDesc.vertexShader = &GetShader<DeferredLightingVert>()->GetNativeShader();
	pipelineDesc.fragmentShader = &GetShader<DeferredLightingFrag>()->GetNativeShader();
	renderer.SetGraphicsPipeline(pipelineDesc);

	renderer.SetTexture(*gbuffer.positionTexture->nativeTarget, *SamplerDesc(), 0);
	renderer.SetTexture(*gbuffer.normalTexture->nativeTarget, *SamplerDesc(), 1);
	renderer.SetTexture(*gbuffer.diffuseTexture->nativeTarget, *SamplerDesc(), 2);
}

static void RenderLight(RenderContext& renderer, Light& light, const View& view)
{	
	LightDescription lightDesc = light.GetLightDescription();
	LightProperties lightProps = {};
	lightProps.direction = -light.GetDirection();
	lightProps.position = lightDesc.position;
	lightProps.lightViewTransform = Math::ConstructViewMatrix(light.GetPosition(), Vector4::Zero, Vector4::UpAxis);
	lightProps.lightProjection = Math::ConstructPerspectiveMatrix(90.f, (float)ShadowMapWidth / (float)ShadowMapHeight, .1f, 1000.);

	GetGraphicsInterface().PushBufferData(light.GetLightBuffer(), &lightProps);

	renderer.SetUniformBuffer(*view.viewBuffer, 3);
	renderer.SetUniformBuffer(light.GetLightBuffer(), 4);
	renderer.Draw(3, 1);
}

namespace DeferredRender
{
void RenderLights(RenderContext& renderer, const Scene& scene, const GBuffer& gbuffer, const View& view)
{
	auto& lights = scene.GetLights();
	if (lights.Size() > 0)
	{
		SetupLightRender(renderer, gbuffer);

		uint32 viewWidth = (uint32)view.description.viewport.width;
		uint32 viewHeight = (uint32)view.description.viewport.height;
		renderer.SetViewport(0, 0, viewWidth, viewHeight, 0, 1);
		renderer.SetScissor(0, 0, viewWidth, viewHeight);

		// Render individual lights
		for (const auto& light : lights)
		{
			RenderLight(renderer, *light, view);
		}
	}
}
}
