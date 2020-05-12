// Copyright 2020, Nathan Blane

#include "RenderTarget.hpp"

RenderTargetDescription CreateRenderTargetDescription(const FixedArray<const RenderTarget*, MaxColorTargetCount>& colorTargets, const RenderTarget* depthTarget, RenderTargetAccess depthAccess)
{
	// TODO - Check can be made in a more compile time way
	AssertFunc([&] {
		uint32 width = colorTargets[0]->width;
		uint32 height = colorTargets[0]->height;
		for (uint32 i = 1; i < colorTargets.Size(); ++i)
		{
			const RenderTarget* rt = colorTargets[i];
			if (width != rt->width || 
				height != rt->height)
				return false;
		}
		return true;
	}, "Dimensions aren't the same for each target!");

	RenderTargetDescription desc = {};
	uint32 numColorTargets = colorTargets.Size();
	desc.colorAttachments.Resize(numColorTargets);
	desc.targetExtents = { (float32)colorTargets[0]->width,(float32)colorTargets[0]->height };
	for (uint32 i = 0; i < numColorTargets; ++i)
	{
		const RenderTarget* colorTarget = colorTargets[i];
		RenderTargetAttachment& attachment = desc.colorAttachments[i];
		attachment.format = colorTarget->format;
		attachment.load = colorTarget->load;
		attachment.store = colorTarget->store;
		attachment.stencilLoad = colorTarget->stencilLoad;
		attachment.stencilStore = colorTarget->stencilStore;
		attachment.sampleCount = colorTarget->sampleCount;
	}

	if (depthTarget != nullptr)
	{
		desc.hasDepth = true;
		desc.depthAttachment.format = depthTarget->format;
		desc.depthAttachment.load = depthTarget->load;
		desc.depthAttachment.store = depthTarget->store;
		desc.depthAttachment.stencilLoad = depthTarget->stencilLoad;
		desc.depthAttachment.stencilStore = depthTarget->stencilStore;
		desc.depthAttachment.sampleCount = depthTarget->sampleCount;
		desc.depthAttachment.access = depthAccess;
	}
	else
	{
		desc.hasDepth = false;
	}
	

	return desc;
}

NativeRenderTargets CreateNativeRenderTargets(const FixedArray<const RenderTarget*, MaxColorTargetCount>& colorTargets, const RenderTarget * depthTarget)
{
	// TODO - Check can be made in a more compile time way
	AssertFunc([&] {
		uint32 width = colorTargets[0]->width;
		uint32 height = colorTargets[0]->height;
		for (uint32 i = 1; i < colorTargets.Size(); ++i)
		{
			const RenderTarget* rt = colorTargets[i];
			if (width != rt->width ||
				height != rt->height)
				return false;
		}
		return true;
		}, "Dimensions aren't the same for each target!");

	NativeRenderTargets targets = {};
	uint32 numColorTargets = colorTargets.Size();
	targets.colorTargets.Resize(numColorTargets);
	for (uint32 i = 0; i < numColorTargets; ++i)
	{
		targets.colorTargets[i] = colorTargets[i]->nativeTarget.Get();
	}

	if (depthTarget != nullptr)
	{
		targets.depthTarget = depthTarget->nativeTarget.Get();
	}

	return targets;
}
