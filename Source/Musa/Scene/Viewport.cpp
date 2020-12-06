// Copyright 2020, Nathan Blane

#include "Viewport.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/RenderContextUtilities.hpp"

Viewport::Viewport(void* windowHandle, u32 width, u32 height)
	: viewWidth(width),
	viewHeight(height)
{
	graphicsViewport = GetGraphicsInterface().CreateViewport(windowHandle, viewWidth, viewHeight);

	InitializeRenderTargets();
}

void Viewport::InitializeRenderTargets()
{
	GBuffer& gbuffer = renderTargets.gbuffer;
	gbuffer.positionTexture = CreateRenderTarget(
		ImageFormat::RGBA_16f, viewWidth, viewHeight,
		LoadOperation::Clear, StoreOperation::Store,
		LoadOperation::DontCare, StoreOperation::DontCare,
		TextureUsage::RenderTarget
	);
	gbuffer.normalTexture = CreateRenderTarget(
		ImageFormat::RGBA_16f,
		viewWidth, viewHeight,
		LoadOperation::Clear, StoreOperation::Store,
		LoadOperation::DontCare, StoreOperation::DontCare,
		TextureUsage::RenderTarget
	);
	gbuffer.diffuseTexture = CreateRenderTarget(
		ImageFormat::RGBA_8norm,
		viewWidth, viewHeight,
		LoadOperation::Clear, StoreOperation::Store,
		LoadOperation::DontCare, StoreOperation::DontCare,
		TextureUsage::RenderTarget
	);

	SceneRenderTargets& sceneTargets = renderTargets.sceneTargets;
	sceneTargets.sceneColorTexture = CreateRenderTarget(
		ImageFormat::RGBA_8norm,
		viewWidth, viewHeight,
		LoadOperation::Clear, StoreOperation::Store,
		LoadOperation::DontCare, StoreOperation::DontCare,
		TextureUsage::RenderTarget
	);
	sceneTargets.depthTexture = CreateRenderTarget(
		ImageFormat::DS_32f_8u,
		viewWidth, viewHeight,
		LoadOperation::Clear, StoreOperation::Store,
		LoadOperation::DontCare, StoreOperation::DontCare,
		TextureUsage::DepthStencilTarget
	);

	renderTargets.userInterfaceTarget = CreateRenderTarget(
		ImageFormat::RGBA_8norm,
		viewWidth, viewHeight,
		LoadOperation::Clear, StoreOperation::Store,
		LoadOperation::DontCare, StoreOperation::DontCare,
		TextureUsage::RenderTarget
	);
}
