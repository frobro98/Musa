#pragma once

#include "Types/Intrinsics.hpp"
#include "Extents.hpp"
#include "FNV-1a.h"
#include "MemoryUtilities.h"
#include "Texture/ImageFormats.h"
#include "Graphics/GraphicsAPIDefinitions.hpp"

// TODO - Remove these includes by abstracting them
#include "Vulkan/VulkanAbstractions.h"

constexpr uint32 GBufferCount = 3;

struct ColorDescription
{
	ImageFormat format;
	LoadOperation load;
	LoadOperation stencilLoad;
	StoreOperation store;
	StoreOperation stencilStore;
	uint32 sampleCount;

	friend bool operator==(const ColorDescription& lhs, const ColorDescription& rhs)
	{
		return lhs.format == rhs.format &&
			lhs.load == rhs.load &&
			lhs.stencilLoad == rhs.stencilLoad &&
			lhs.store == rhs.store &&
			lhs.stencilStore == rhs.stencilStore &&
			lhs.sampleCount == rhs.sampleCount;
	}
};

struct DepthStencilDescription
{
	ImageFormat format;
	LoadOperation load;
	LoadOperation stencilLoad;
	StoreOperation store;
	StoreOperation stencilStore;
	uint32 sampleCount;

	friend bool operator==(const DepthStencilDescription& lhs, const DepthStencilDescription& rhs)
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
	ColorDescription colorDescs[GBufferCount];
	DepthStencilDescription depthDesc;
	Extents2D targetExtents;
	// TODO - Rename this so that it is more clear how many COLOR targets there are...
	uint32 targetCount;
	bool hasDepth = true;

	friend bool operator==(const RenderTargetDescription& lhs, const RenderTargetDescription& rhs)
	{
		bool result = lhs.targetCount == rhs.targetCount &&
			lhs.depthDesc == rhs.depthDesc;
		if (result)
		{
			uint32 targetCount = lhs.targetCount;
			for (uint32 i = 0; i < targetCount; ++i)
			{
				result &= lhs.colorDescs[i] == rhs.colorDescs[i];
			}

			return result;
		}

		return false;
	}
};

// Allocated textures associated with a render texture
struct RenderTargetTextures
{
	const VulkanTexture* colorTargets[GBufferCount];
	const VulkanTexture* depthTarget;
	uint32 targetCount;
};

inline uint32 GetHash(const RenderTargetDescription& desc)
{
	struct HashableTargetDescription
	{
		ColorDescription colorDescs[GBufferCount];
		DepthStencilDescription depthDesc;
		uint32 targetCount;
	} hashDesc;

	Memcpy(hashDesc.colorDescs, sizeof(hashDesc.colorDescs), desc.colorDescs, sizeof(desc.colorDescs));
	Memcpy(&hashDesc.depthDesc, sizeof(hashDesc.depthDesc), &desc.depthDesc, sizeof(desc.depthDesc));
	hashDesc.targetCount = desc.targetCount;

	return fnv(&hashDesc, sizeof(HashableTargetDescription));
}