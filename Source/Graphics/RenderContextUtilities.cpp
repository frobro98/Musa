
#include "RenderContextUtilities.hpp"
#include "RenderTargetDescription.hpp"
#include "RenderContext.hpp"
#include "GraphicsInterface.hpp"

void TransitionTargetsToRead(RenderContext& renderer, NativeRenderTargets& targets)
{
	uint32 totalTargetCount = targets.depthTarget ? targets.colorTargets.Size() + 1 : targets.colorTargets.Size();
	DynamicArray<const NativeTexture*> gbufferTargets(totalTargetCount);
	uint32 i;
	for (i = 0; i < targets.colorTargets.Size(); ++i)
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
	uint32 totalTargetCount = targets.depthTarget ? targets.colorTargets.Size() + 1 : targets.colorTargets.Size();
	DynamicArray<const NativeTexture*> gbufferTargets(totalTargetCount);
	uint32 i;
	for (i = 0; i < targets.colorTargets.Size(); ++i)
	{
		gbufferTargets[i] = targets.colorTargets[i];
	}
	if (targets.depthTarget)
	{
		gbufferTargets[i] = targets.depthTarget;
	}
	renderer.TransitionToWriteState(gbufferTargets.GetData(), gbufferTargets.Size());
}

UniquePtr<RenderTarget> CreateRenderTarget(ImageFormat format, uint32 width, uint32 height, LoadOperation loadOp, StoreOperation storeOp, LoadOperation stencilLoadOp, StoreOperation stencilStoreOp, TextureUsage::Type usage)
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

	rt->nativeTarget = GetGraphicsInterface().CreateEmptyTexture2D(width, height, format, 1, usage);

	return rt;
}