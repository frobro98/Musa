#pragma once

#include "Types/UniquePtr.hpp"
#include "Containers/DynamicArray.hpp"
// TODO - This file needs to live in the Graphics library
#include "Texture/ImageFormats.h"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Graphics/RenderTargetDescription.hpp"

struct RenderTarget
{
	UniquePtr<NativeTexture> nativeTarget;
	Extents2D dimensions;
	uint32 sampleCount;
	ImageFormat format;
	LoadOperation load;
	LoadOperation stencilLoad;
	StoreOperation store;
	StoreOperation stencilStore;
};

RenderTargetDescription CreateRenderTargetDescription(const DynamicArray<RenderTarget*>& colorTargets, const RenderTarget* depthTarget);
NativeRenderTargets CreateNativeRenderTargets(const DynamicArray<RenderTarget*>& colorTargets, const RenderTarget* depthTarget);
