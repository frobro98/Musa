// Copyright 2020, Nathan Blane

#pragma once

// TODO - Not a great name for what its doing. Hopefully I'll get a better name later on?

#include "Types/UniquePtr.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/GraphicsResourceFlags.hpp"

class RenderContext;
struct NativeRenderTargets;

void TransitionTargetsToRead(RenderContext& renderer, NativeRenderTargets& targets);
void TransitionTargetsToWrite(RenderContext& renderer, NativeRenderTargets& targets);

UniquePtr<RenderTarget> CreateRenderTarget(
	ImageFormat format, 
	uint32 width, uint32 height,
	LoadOperation loadOp, StoreOperation storeOp,
	LoadOperation stencilLoadOp, StoreOperation stencilStoreOp,
	TextureUsage::Type usage
);

