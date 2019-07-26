#pragma once

#include "Graphics.h"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "FNV-1a.h"

class VulkanViewport;

struct TextureSamplerCreateParams
{
	float32 maxAnisotropy;
	float32 minLod;
	float32 maxLod;
	float32 mipLodBias;
	SamplerFilter minFilter;
	SamplerFilter magFilter;
	SamplerAddressMode addrModeU;
	SamplerAddressMode addrModeV;
	SamplerMipmapMode mipMode;
	bool enabledAnisotropy;

	friend bool operator==(const TextureSamplerCreateParams& lhs, const TextureSamplerCreateParams& rhs)
	{
		return lhs.maxAnisotropy == rhs.maxAnisotropy &&
			lhs.mipLodBias == rhs.mipLodBias &&
			lhs.minLod == rhs.minLod &&
			lhs.maxLod == rhs.maxLod &&
			lhs.minFilter == rhs.minFilter &&
			lhs.magFilter == rhs.magFilter &&
			lhs.addrModeU == rhs.addrModeU &&
			lhs.addrModeV == rhs.addrModeV &&
			lhs.mipMode == rhs.mipMode &&
			lhs.enabledAnisotropy == rhs.enabledAnisotropy;
	}

	friend uint32 GetHash(const TextureSamplerCreateParams& params)
	{
		return fnv(&params, sizeof(TextureSamplerCreateParams));
	}
};

// TODO - Get a more generic way of representing a sampler, as well as all other resources...
// Sampler texture state created via lookup of 
struct TextureSampler
{
	VkSampler nativeSampler;
};

