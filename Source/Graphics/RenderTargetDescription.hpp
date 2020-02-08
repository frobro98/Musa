#pragma once

#include "Types/Intrinsics.hpp"
#include "Extents.hpp"
#include "FNV-1a.h"
#include "MemoryUtilities.h"
#include "Texture/ImageFormats.h"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

constexpr uint32 GBufferCount = 3;

struct RenderTargetAttachment
{
	ImageFormat format;
	LoadOperation load;
	LoadOperation stencilLoad;
	StoreOperation store;
	StoreOperation stencilStore;
	uint32 sampleCount;

	friend bool operator==(const RenderTargetAttachment& lhs, const RenderTargetAttachment& rhs)
	{
		return lhs.format == rhs.format &&
			lhs.load == rhs.load &&
			lhs.stencilLoad == rhs.stencilLoad &&
			lhs.store == rhs.store &&
			lhs.stencilStore == rhs.stencilStore &&
			lhs.sampleCount == rhs.sampleCount;
	}
};

struct RenderTargetDescription
{
	RenderTargetAttachment colorAttachments[GBufferCount];
	RenderTargetAttachment depthAttachment;
	Extents2D targetExtents;
	uint32 numColorAttachments = 0;
	bool hasDepth = true;

	friend bool operator==(const RenderTargetDescription& lhs, const RenderTargetDescription& rhs)
	{
		bool result = lhs.numColorAttachments == rhs.numColorAttachments &&
			lhs.depthAttachment == rhs.depthAttachment;
		if (result)
		{
			uint32 numColorAttachments = lhs.numColorAttachments;
			for (uint32 i = 0; i < numColorAttachments; ++i)
			{
				result &= lhs.colorAttachments[i] == rhs.colorAttachments[i];
			}

			return result;
		}

		return false;
	}
};

// Allocated textures associated with a render texture
struct RenderTargetTextures
{
	const NativeTexture* colorTargets[GBufferCount];
	const NativeTexture* depthTarget;
	uint32 numColorTargets;
};

inline uint32 GetHash(const RenderTargetDescription& desc)
{
	struct HashableTargetDescription
	{
		RenderTargetAttachment colorDescs[GBufferCount];
		RenderTargetAttachment depthDesc;
		uint32 targetCount;
	} hashDesc;

	Memcpy(hashDesc.colorDescs, sizeof(hashDesc.colorDescs), desc.colorAttachments, sizeof(desc.colorAttachments));
	Memcpy(&hashDesc.depthDesc, sizeof(hashDesc.depthDesc), &desc.depthAttachment, sizeof(desc.depthAttachment));
	hashDesc.targetCount = desc.numColorAttachments;

	return fnv(&hashDesc, sizeof(HashableTargetDescription));
}