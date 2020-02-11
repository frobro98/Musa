
#include "RenderContextUtilities.hpp"
#include "RenderTargetDescription.hpp"
#include "RenderContext.hpp"
#include "GraphicsInterface.hpp"

void TransitionTargetsToRead(RenderContext& renderer, NativeRenderTargets& targets)
{
	uint32 targetCount = targets.depthTarget ? targets.numColorTargets + 1 : targets.numColorTargets;
	DynamicArray<const NativeTexture*> gbufferTargets(targetCount);
	uint32 i;
	for (i = 0; i < targets.numColorTargets; ++i)
	{
		gbufferTargets[i] = targets.colorTargets[i];
	}
	if (targets.depthTarget)
	{
		gbufferTargets[i] = targets.depthTarget;
	}
	renderer.TransitionToReadState(gbufferTargets.GetData(), gbufferTargets.Size());
}

void TransitionTargetsToWrite(RenderContext& renderer, NativeRenderTargets& targets)
{
	uint32 targetCount = targets.depthTarget ? targets.numColorTargets + 1 : targets.numColorTargets;
	DynamicArray<const NativeTexture*> gbufferTargets(targetCount);
	uint32 i;
	for (i = 0; i < targets.numColorTargets; ++i)
	{
		gbufferTargets[i] = targets.colorTargets[i];
	}
	if (targets.depthTarget)
	{
		gbufferTargets[i] = targets.depthTarget;
	}
	renderer.TransitionToWriteState(gbufferTargets.GetData(), gbufferTargets.Size());
}

UniquePtr<RenderTarget> CreateRenderTarget(ImageFormat format, uint32 width, uint32 height, LoadOperation loadOp, StoreOperation storeOp, LoadOperation stencilLoadOp, StoreOperation stencilStoreOp)
{
	UniquePtr<RenderTarget> rt = MakeUnique<RenderTarget>();

	rt->format = format;
	rt->width = width;
	rt->height = height;
	rt->load = loadOp;
	rt->store = storeOp;
	rt->stencilLoad = stencilLoadOp;
	rt->stencilStore = stencilStoreOp;
	rt->sampleCount = 1;

	rt->nativeTarget = GetGraphicsInterface().CreateEmptyTexture2D(width, height, format, 1, TextureUsage::RenderTarget);

	return rt;
}
