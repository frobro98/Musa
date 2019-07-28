#pragma once

#include "Graphics.h"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "FNV-1a.h"
#include "Math/MathUtilities.h"

class VulkanViewport;

struct TextureSamplerCreateParams
{
	TextureSamplerCreateParams(
		SamplerFilter filter_ = SamplerFilter::Linear,
		SamplerAddressMode uAddrMode = SamplerAddressMode::Clamp,
		SamplerAddressMode vAddrMode = SamplerAddressMode::Clamp,
		SamplerMipmapMode mipMode_ = SamplerMipmapMode::Linear,
		float32 anisotropy = 0,
		float32 minLOD = 0,
		float32 maxLOD = 1,
		float32 mipLODBias = 0
	)
		: maxAnisotropy(anisotropy),
		minLod(minLOD),
		maxLod(maxLOD),
		mipLodBias(mipLODBias),
		filter(filter_),
		addrModeU(uAddrMode),
		addrModeV(vAddrMode),
		mipMode(mipMode_),
		enabledAnisotropy(!Math::IsZero(anisotropy))
	{
	}

	float32 maxAnisotropy;
	float32 minLod;
	float32 maxLod;
	float32 mipLodBias;
	SamplerFilter filter;
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
			lhs.filter == rhs.filter &&
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

