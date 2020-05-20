// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "VertexInputDescription.hpp"
#include "Utilities/Hash.hpp"
#include "RenderTargetDescription.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Math/MathFunctions.hpp"
#include "Graphics/GraphicsInterface.hpp"

struct DepthStencilTestingDescription
{
	CompareOperation depthCompareOp;
	bool depthWriteEnabled;
	bool frontStencilTestEnabled;
	StencilOperation frontPassOp;
	StencilOperation frontFailOp;
	StencilOperation frontDepthFailOp;
	CompareOperation frontCompareOp;
	bool backStencilTestEnabled;
	StencilOperation backPassOp;
	StencilOperation backFailOp;
	StencilOperation backDepthFailOp;
	CompareOperation backCompareOp;
	uint8 stencilReadMask;
	uint8 stencilWriteMask;
	bool pad[1] = { false };

	friend bool operator==(const DepthStencilTestingDescription& lhs, const DepthStencilTestingDescription& rhs)
	{
		return 	lhs.depthCompareOp == rhs.depthCompareOp &&
			lhs.depthWriteEnabled == rhs.depthWriteEnabled &&
			lhs.frontStencilTestEnabled == rhs.frontStencilTestEnabled &&
			lhs.frontPassOp == rhs.frontPassOp &&
			lhs.frontFailOp == rhs.frontFailOp &&
			lhs.frontDepthFailOp == rhs.frontDepthFailOp &&
			lhs.frontCompareOp == rhs.frontCompareOp &&
			lhs.backStencilTestEnabled == rhs.backStencilTestEnabled &&
			lhs.backPassOp == rhs.backPassOp &&
			lhs.backFailOp == rhs.backFailOp &&
			lhs.backDepthFailOp == rhs.backDepthFailOp&&
			lhs.backCompareOp == rhs.backCompareOp&&
			lhs.stencilReadMask == rhs.stencilReadMask &&
			lhs.stencilWriteMask == rhs.stencilWriteMask;
	}
};

template<
	CompareOperation depthOp = CompareOperation::LessThanOrEqual,
	bool enableDepthWrite = true,
	bool frontStencilTest = false,
	StencilOperation frontPass = StencilOperation::Keep,
	StencilOperation frontFail = StencilOperation::Keep,
	StencilOperation frontDepthFail = StencilOperation::Keep,
	CompareOperation frontCompareOp = CompareOperation::Always,
	bool backStencilTest = false,
	StencilOperation backPass = StencilOperation::Keep,
	StencilOperation backFail = StencilOperation::Keep,
	StencilOperation backDepthFail = StencilOperation::Keep,
	CompareOperation backCompareOp = CompareOperation::Always,
	uint8 stencilTestReadMask = 0xf,
	uint8 stencilTestWriteMask = 0xf
>
constexpr DepthStencilTestingDescription DepthTestDesc()
{
	return DepthStencilTestingDescription
	{
		depthOp,
		enableDepthWrite,
		frontStencilTest,
		frontPass,
		frontFail,
		frontDepthFail,
		frontCompareOp,
		backStencilTest,
		backPass,
		backFail,
		backDepthFail,
		backCompareOp,
		stencilTestReadMask,
		stencilTestWriteMask
	};
}

struct RasterizerDescription
{
	float32 depthBiasConstant;
	float32 depthBiasSlope;
	FillMode fillMode;
	CullingMode cullMode;
	

	friend bool operator==(const RasterizerDescription& lhs, const RasterizerDescription& rhs)
	{
		return lhs.depthBiasConstant == rhs.depthBiasConstant &&
			lhs.depthBiasSlope == rhs.depthBiasSlope &&
			lhs.fillMode == rhs.fillMode &&
			lhs.cullMode == rhs.cullMode;
	}
};

template<
	FillMode fillMode = FillMode::Full,
	CullingMode cullMode = CullingMode::Back
>
constexpr RasterizerDescription RasterDesc()
{
	return RasterizerDescription
	{
		1.f,
		1.f,
		fillMode,
		cullMode
	};
}

struct BlendingDescription
{
	ColorMask colorMask;
	BlendOperation colorBlendOperation;
	BlendFactor srcBlend;
	BlendFactor dstBlend;
	BlendOperation alphaBlendOperation;
	BlendFactor srcAlphaBlend;
	BlendFactor dstAlphaBlend;

	friend bool operator==(const BlendingDescription& lhs, const BlendingDescription& rhs)
	{
		return lhs.colorMask == rhs.colorMask &&
			lhs.colorBlendOperation == rhs.colorBlendOperation &&
			lhs.srcBlend == rhs.srcBlend &&
			lhs.dstBlend == rhs.dstBlend &&
			lhs.alphaBlendOperation == rhs.alphaBlendOperation &&
			lhs.srcAlphaBlend == rhs.srcAlphaBlend &&
			lhs.dstAlphaBlend == rhs.dstAlphaBlend;
	}
};

template<
	ColorMask mask = ColorMask::RGBA, 
	BlendOperation blendOp = BlendOperation::Add, 
	BlendFactor srcFactor = BlendFactor::One, 
	BlendFactor dstFactor = BlendFactor::Zero,
	BlendOperation alphaOp = BlendOperation::Add,
	BlendFactor srcAlphaFactor = BlendFactor::One,
	BlendFactor dstAlphaFactor = BlendFactor::Zero
>
constexpr BlendingDescription BlendDesc()
{
	return BlendingDescription
		{
			mask,
			blendOp,
			srcFactor,
			dstFactor,
			alphaOp,
			srcAlphaFactor,
			dstAlphaFactor
		};
}

struct SamplerDescription
{
	SamplerDescription(
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

	friend bool operator==(const SamplerDescription& lhs, const SamplerDescription& rhs)
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

	friend uint32 GetHash(const SamplerDescription& params)
	{
		return fnv32(&params, sizeof(SamplerDescription));
	}
};

template<
	SamplerFilter filter = SamplerFilter::Linear,
	SamplerAddressMode uAddrMode = SamplerAddressMode::Clamp,
	SamplerAddressMode vAddrMode = SamplerAddressMode::Clamp,
	SamplerMipmapMode mipMode = SamplerMipmapMode::Linear,
	int32 anisotropy = 0,
	int32 minLOD = 0,
	int32 maxLOD = 1,
	int32 mipLODBias = 0
>
inline NativeSampler* SamplerDesc()
{
	SamplerDescription desc
	{
		filter,
		uAddrMode,
		vAddrMode,
		mipMode,
		anisotropy,
		minLOD,
		maxLOD,
		mipLODBias
	};
	return GetGraphicsInterface().CreateTextureSampler(desc);
}

struct GraphicsPipelineDescription
{
	RenderTargetDescription renderTargets;
	// Pipeline Information --> API implementation dependent apparently
	RasterizerDescription rasterizerDesc;
	BlendingDescription blendingDescs[MaxColorTargetCount];
	DepthStencilTestingDescription depthStencilTestDesc;
	VertexInputDescriptionList vertexInputs{};
	const NativeVertexShader* vertexShader = nullptr;
	const NativeFragmentShader* fragmentShader = nullptr;
	PrimitiveTopology topology;

	friend bool operator==(const GraphicsPipelineDescription& lhs, const GraphicsPipelineDescription& rhs)
	{
		if (lhs.renderTargets == rhs.renderTargets)
		{
			uint32 targetCount = lhs.renderTargets.colorAttachments.Size();
			bool result = true;
			for (uint32 i = 0; i < targetCount; ++i)
			{
				result &= lhs.blendingDescs[i] == rhs.blendingDescs[i];
			}

			if (result)
			{
				return lhs.rasterizerDesc == rhs.rasterizerDesc &&
					lhs.depthStencilTestDesc == rhs.depthStencilTestDesc &&
					lhs.vertexInputs == rhs.vertexInputs &&
					lhs.fragmentShader == rhs.fragmentShader &&
					lhs.topology == rhs.topology;
			}
		}
		return false;
	}

};

struct ComputePipelineInitDescription
{
	NativeComputeShader* computeShader;
};

inline uint32 GetHash(const GraphicsPipelineDescription& desc)
{
	return fnv32(&desc, sizeof(GraphicsPipelineDescription));
}
