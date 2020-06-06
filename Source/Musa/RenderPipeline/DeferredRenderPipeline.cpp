// Copyright 2020, Nathan Blane

#include "DeferredRenderPipeline.hpp"
#include "SceneRenderPipeline.h"
#include "UserInterfacePipeline.hpp"

#include "Engine/FrameRenderTargets.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderContextUtilities.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Scene/ScreenView.hpp"
#include "Debugging/MetricInterface.hpp"
#include "Shader/ShaderObjects/ScreenRendering.hpp"

#include "BasicTypes/Color.hpp"

DECLARE_METRIC_GROUP(DeferredRender);
METRIC_STAT(ComposeTargets, DeferredRender);


namespace DeferredRender
{
void Render(const FrameRenderTargets& frameTargets, Scene& scene, const RenderObjectManager& renderManager, const View& view)
{
	RenderContext* renderContext = GetGraphicsInterface().GetRenderContext();
	Assert(renderContext);
	RenderSceneDeferred(*renderContext, scene, frameTargets.gbuffer, frameTargets.sceneTargets, renderManager, view);
}

void ComposeBackbuffer(RenderContext& context, const RenderTarget& sceneColor, const RenderTarget& uiColor, const View& view)
{
	SCOPED_TIMED_BLOCK(ComposeTargets);

	DynamicArray<Color32> clearColors = { Color32(0, 0, 0) };
	NativeTexture* backBuffer = context.GetBackBuffer();
	NativeRenderTargets backBufferTarget = {};
	backBufferTarget.colorTargets.Add(backBuffer);

	RenderTargetDescription targetDescription = {};
	targetDescription.colorAttachments.Resize(1);
	targetDescription.targetExtents = { (f32)view.description.viewport.width, (f32)view.description.viewport.height };
	targetDescription.hasDepth = false;

	RenderTargetAttachment& colorDesc = targetDescription.colorAttachments[0];
	colorDesc.format = ImageFormat::BGRA_8norm;
	colorDesc.load = LoadOperation::Clear;
	colorDesc.store = StoreOperation::Store;
	colorDesc.stencilLoad = LoadOperation::DontCare;
	colorDesc.stencilStore = StoreOperation::DontCare;
	colorDesc.sampleCount = 1;

	TransitionTargetsToWrite(context, backBufferTarget);

	// Set target to back buffer
	context.SetRenderTarget(targetDescription, backBufferTarget, clearColors);

	// Render sceneColor and UI to back buffer

	u32 viewWidth = (u32)view.description.viewport.width;
	u32 viewHeight = (u32)view.description.viewport.height;
	context.SetViewport(0, 0, viewWidth, viewHeight, 0, 1);
	context.SetScissor(0, 0, viewWidth, viewHeight);

	GraphicsPipelineDescription desc = {};
	context.InitializeWithRenderState(desc);
	desc.vertexInputs = {};
	desc.rasterizerDesc = { 1.25f, 1.75f, FillMode::Full, CullingMode::Front };
	desc.blendingDescs[0] = BlendDesc<ColorMask::RGBA, BlendOperation::Add, BlendFactor::One, BlendFactor::One, BlendOperation::Add, BlendFactor::One, BlendFactor::One>();
	desc.depthStencilTestDesc = DepthTestDesc();
	desc.topology = PrimitiveTopology::TriangleList;
	desc.vertexShader = &GetShader<ScreenRenderVert>()->GetNativeShader();
	desc.fragmentShader = &GetShader<ScreenRenderFrag>()->GetNativeShader();
	context.SetGraphicsPipeline(desc);

	context.SetTexture(*sceneColor.nativeTarget, *SamplerDesc(), 0);
	context.SetTexture(*uiColor.nativeTarget, *SamplerDesc(), 1);

	context.Draw(3, 1);

	//TransitionTargetsToRead(context, backBufferTarget);
}
}