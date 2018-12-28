#pragma once

#include "Graphics.h"
#include "Shader/ShaderStages.hpp"
#include "Containers/Array.h"

// TODO - determine if this RenderPipelineInfo is actually necessary
struct RenderPipelineInfo
{
	class VulkanShader* shaders[ShaderStage::Stage_Max];

	struct DepthStencilDesc
	{
		bool32 depthTestEnabled;
		bool32 depthWriteEnabled;
		bool32 stencilTestEnabled;
		uint32 depthCompareOp;
		uint32 frontCompareMask;
		uint32 frontWriteMask;
		uint32 frontReference;
		uint32 backCompareMask;
		uint32 backWriteMask;
		uint32 backReference;
		uint8 frontPassOp;
		uint8 frontFailOp;
		uint8 frontDepthFailOp;
		uint8 frontCompareOp;
		uint8 backPassOp;
		uint8 backFailOp;
		uint8 backDepthFailOp;
		uint8 backCompareOp;
		// TODO - Find out what is involved when enabling the depth bounds test
		//bool32 depthBoundsTestEnabled;
		//float32 minDepthBounds;
		//float32 maxDepthBounds;

		void WriteToDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencilInfo) const
		{
			depthStencilInfo.depthTestEnable = depthTestEnabled;
			depthStencilInfo.depthWriteEnable = depthWriteEnabled;
			depthStencilInfo.stencilTestEnable = stencilTestEnabled;
			depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			depthStencilInfo.front = depthStencilInfo.back;
			depthStencilInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;

			//depthStencilInfo.depthCompareOp = static_cast<VkCompareOp>(depthCompareOp);
			//depthStencilInfo.front.compareMask = frontCompareMask;
			//depthStencilInfo.front.writeMask = frontWriteMask;
			//depthStencilInfo.front.reference = frontReference;
			//depthStencilInfo.front.compareOp = static_cast<VkCompareOp>(frontCompareOp);
			//depthStencilInfo.front.passOp = static_cast<VkStencilOp>(frontPassOp);
			//depthStencilInfo.front.failOp = static_cast<VkStencilOp>(frontFailOp);
			//depthStencilInfo.front.depthFailOp = static_cast<VkStencilOp>(frontDepthFailOp);
			//depthStencilInfo.back.compareMask = backCompareMask;
			//depthStencilInfo.back.writeMask = backWriteMask;
			//depthStencilInfo.back.reference = backReference;
			//depthStencilInfo.back.compareOp = static_cast<VkCompareOp>(backCompareOp);
			//depthStencilInfo.back.passOp = static_cast<VkStencilOp>(backPassOp);
			//depthStencilInfo.back.failOp = static_cast<VkStencilOp>(backFailOp);
			//depthStencilInfo.back.depthFailOp = static_cast<VkStencilOp>(backDepthFailOp);

			//depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
			//depthStencilInfo.maxDepthBounds = 0.f;
			//depthStencilInfo.minDepthBounds = 0.f;
		}
		void ReadFromDepthStencil(const VkPipelineDepthStencilStateCreateInfo& depthStencilInfo)
		{
			depthTestEnabled = depthStencilInfo.depthTestEnable;
			depthWriteEnabled = depthStencilInfo.depthWriteEnable;
			stencilTestEnabled = depthStencilInfo.stencilTestEnable;
			depthCompareOp = depthStencilInfo.depthCompareOp;
			frontCompareMask = depthStencilInfo.front.compareMask;
			frontWriteMask = depthStencilInfo.front.writeMask;
			frontReference = depthStencilInfo.front.reference;
			frontCompareOp = static_cast<uint8>(depthStencilInfo.front.compareOp);
			frontPassOp = static_cast<uint8>(depthStencilInfo.front.passOp);
			frontFailOp = static_cast<uint8>(depthStencilInfo.front.failOp);
			frontDepthFailOp = static_cast<uint8>(depthStencilInfo.front.depthFailOp);
			backCompareMask = depthStencilInfo.back.compareMask;
			backWriteMask = depthStencilInfo.back.writeMask;
			backReference = depthStencilInfo.back.reference;
			backCompareOp = static_cast<uint8>(depthStencilInfo.back.compareOp);
			backPassOp = static_cast<uint8>(depthStencilInfo.back.passOp);
			backFailOp = static_cast<uint8>(depthStencilInfo.back.failOp);
			backDepthFailOp = static_cast<uint8>(depthStencilInfo.back.depthFailOp);
		}

	} depthStencilInfo;

	struct Rasterizer
	{
		bool32 depthClampEnabled; // TODO - Look this up and see what depthClampEnabled actually is
		bool32 depthBiasEnabled; // TODO - Look up depthBiasEnabled and its uses
		float32 depthBiasConstant;
		float32 depthBiasSlope;
		float32 depthBiasClamp;
		uint8 polygonMode;
		uint8 cullMode;
		uint8 pad[2];

		void WriteToRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizerInfo) const
		{
			rasterizerInfo.polygonMode = static_cast<VkPolygonMode>(polygonMode);
			rasterizerInfo.cullMode = static_cast<VkCullModeFlags>(cullMode);
			rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			//rasterizerInfo.depthBiasEnable = depthBiasEnabled;
			rasterizerInfo.depthClampEnable = VK_FALSE;
			//rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
			rasterizerInfo.lineWidth = 1.0f;
			//rasterizerInfo.depthBiasConstantFactor = depthBiasConstant;
			//rasterizerInfo.depthBiasSlopeFactor = depthBiasSlope;
			//rasterizerInfo.depthBiasClamp = depthBiasClamp;
		}
		void ReadFromRasterizer(const VkPipelineRasterizationStateCreateInfo& rasterizerInfo)
		{
			polygonMode = static_cast<uint8>(rasterizerInfo.polygonMode);
			cullMode = static_cast<uint8>(rasterizerInfo.cullMode);
			depthBiasEnabled = rasterizerInfo.depthBiasEnable;
			depthBiasConstant = rasterizerInfo.depthBiasConstantFactor;
			depthBiasSlope = rasterizerInfo.depthBiasSlopeFactor;
			depthClampEnabled = rasterizerInfo.depthClampEnable;
			depthBiasClamp = rasterizerInfo.depthBiasClamp;
		}

	} rasterizer;

	struct VertexBinding
	{
		uint32 binding;
		uint32 stride;
		uint32 inputRate;

		void WriteToBinding(VkVertexInputBindingDescription& vkVertexBinding) const
		{
			vkVertexBinding.binding = binding;
			vkVertexBinding.stride = stride;
			vkVertexBinding.inputRate = static_cast<VkVertexInputRate>(inputRate);
		}
		void ReadFromBinding(const VkVertexInputBindingDescription& vkVertexBinding)
		{
			binding = vkVertexBinding.binding;
			stride = vkVertexBinding.stride;
			inputRate = vkVertexBinding.inputRate;
		}
	};
	Array<VertexBinding> vertexBindings;

	struct VertexAttribute
	{
		uint32 binding;
		uint32 location;
		uint32 offset;
		// TODO - store format in a better way than uint16
		uint32 format;

		void WriteToAttribute(VkVertexInputAttributeDescription& vkVertexAttrib) const
		{
			vkVertexAttrib.binding = binding;
			vkVertexAttrib.location = location;
			vkVertexAttrib.offset = offset;
			vkVertexAttrib.format = static_cast<VkFormat>(format);
		}
		void ReadFromAttribute(const VkVertexInputAttributeDescription& vkVertexAttrib)
		{
			binding = vkVertexAttrib.binding;
			location = vkVertexAttrib.location;
			offset = vkVertexAttrib.offset;
			format = vkVertexAttrib.format;
		}
	};
	Array<VertexAttribute> vertexAttributes;

	// Blend information
	struct ColorBlendAttachment
	{
		bool32 enableBlending;
		uint32 colorMask;
		uint32 srcBlend;
		uint32 dstBlend;
		uint32 srcAlphaBlend;
		uint32 dstAlphaBlend;
		uint32 colorBlendOperation;
		uint32 alphaBlendOperation;

		void WriteToAttachment(VkPipelineColorBlendAttachmentState& colorBlendAttachment) const
		{
			colorBlendAttachment.blendEnable = VK_FALSE;//enableBlending;
			colorBlendAttachment.colorWriteMask = 0xf;// static_cast<VkColorComponentFlags>(colorMask);
			//colorBlendAttachment.srcColorBlendFactor = static_cast<VkBlendFactor>(srcBlend);
			//colorBlendAttachment.dstColorBlendFactor = static_cast<VkBlendFactor>(dstBlend);
			//colorBlendAttachment.colorBlendOp = static_cast<VkBlendOp>(colorBlendOperation);
			//colorBlendAttachment.srcAlphaBlendFactor = static_cast<VkBlendFactor>(srcAlphaBlend);
			//colorBlendAttachment.dstAlphaBlendFactor = static_cast<VkBlendFactor>(dstAlphaBlend);
			//colorBlendAttachment.alphaBlendOp = static_cast<VkBlendOp>(alphaBlendOperation);
		}
		void ReadFromAttachment(const VkPipelineColorBlendAttachmentState& colorBlendAttachment)
		{
			enableBlending = colorBlendAttachment.blendEnable;
			colorMask = colorBlendAttachment.colorWriteMask;
			srcBlend = colorBlendAttachment.srcColorBlendFactor;
			dstBlend = colorBlendAttachment.dstColorBlendFactor;
			colorBlendOperation = colorBlendAttachment.colorBlendOp;
			srcAlphaBlend = colorBlendAttachment.srcAlphaBlendFactor;
			dstAlphaBlend = colorBlendAttachment.dstAlphaBlendFactor;
			alphaBlendOperation = colorBlendAttachment.alphaBlendOp;
		}
	};
	Array<ColorBlendAttachment> blendAttachments;

	uint32 topology;
	bool32 enableLogicalOperation;
	VkLogicOp logicOperation;

	//uint32 viewportScissorCount;
};
