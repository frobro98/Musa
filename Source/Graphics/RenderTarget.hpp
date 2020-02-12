#pragma once

#include "Types/UniquePtr.hpp"
#include "Containers/FixedArray.hpp"
// TODO - This file needs to live in the Graphics library
#include "Texture/ImageFormats.h"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Graphics/RenderTargetDescription.hpp"

struct RenderTarget
{
	UniquePtr<NativeTexture> nativeTarget;
	uint32 width;
	uint32 height;
	uint32 sampleCount;
	ImageFormat format;
	LoadOperation load;
	LoadOperation stencilLoad;
	StoreOperation store;
	StoreOperation stencilStore;
};

RenderTargetDescription CreateRenderTargetDescription(const FixedArray<RenderTarget*, GBufferCount + 1>& colorTargets, const RenderTarget* depthTarget);
NativeRenderTargets CreateNativeRenderTargets(const FixedArray<RenderTarget*, GBufferCount + 1>& colorTargets, const RenderTarget* depthTarget);
