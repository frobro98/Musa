// Copyright 2020, Nathan Blane

#pragma once

// TODO - Not a great name for what its doing. Hopefully I'll get a better name later on?

#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/GraphicsResourceFlags.hpp"
#include "Graphics/GraphicsAPI.hpp"

class RenderContext;
struct NativeRenderTargets;

GRAPHICS_API void TransitionTargetsToRead(RenderContext& renderer, NativeRenderTargets& targets);
GRAPHICS_API void TransitionTargetsToWrite(RenderContext& renderer, NativeRenderTargets& targets);

GRAPHICS_API UniquePtr<RenderTarget> CreateRenderTarget(
	ImageFormat format, 
	uint32 width, uint32 height,
	LoadOperation loadOp, StoreOperation storeOp,
	LoadOperation stencilLoadOp, StoreOperation stencilStoreOp,
	TextureUsage::Type usage
);

