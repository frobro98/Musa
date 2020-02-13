#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/FixedArray.hpp"
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
	FixedArray<RenderTargetAttachment, GBufferCount> colorAttachments;
	RenderTargetAttachment depthAttachment;
	Extents2D targetExtents;
	bool hasDepth = true;

	friend bool operator==(const RenderTargetDescription& lhs, const RenderTargetDescription& rhs)
	{
		bool result = lhs.colorAttachments.Size() == rhs.colorAttachments.Size() &&
			lhs.depthAttachment == rhs.depthAttachment;
		if (result)
		{
			for (uint32 i = 0; i < lhs.colorAttachments.Size(); ++i)
			{
				result &= lhs.colorAttachments[i] == rhs.colorAttachments[i];
			}

			return result;
		}

		return false;
	}
};

// Allocated textures associated with a render texture
struct NativeRenderTargets
{
	const NativeTexture* colorTargets[GBufferCount];
	const NativeTexture* depthTarget;
	uint32 numColorTargets;
};

inline uint32 GetHash(const RenderTargetDescription& desc)
{
	struct HashableTargetDescription
	{
		FixedArray<RenderTargetAttachment, GBufferCount> colorDescs;
		RenderTargetAttachment depthDesc;
	} hashDesc;
	hashDesc.colorDescs.Resize(desc.colorAttachments.Size());
	Memcpy(hashDesc.colorDescs.GetData(), sizeof(hashDesc.colorDescs), desc.colorAttachments.GetData(), sizeof(desc.colorAttachments));
	Memcpy(&hashDesc.depthDesc, sizeof(hashDesc.depthDesc), &desc.depthAttachment, sizeof(desc.depthAttachment));

	return fnv(&hashDesc, sizeof(HashableTargetDescription));
}