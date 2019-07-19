
#include "VulkanRenderingCloset.hpp"
#include "VulkanShader.h"
#include "VulkanDescriptorSet.h"
#include "VulkanPipeline.h"
#include "VulkanMemory.h"
#include "VulkanUtilities.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanCreateInfos.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptorLayoutManager.h"

#include "Shader/ShaderObjects/ShaderResource.hpp"
#include "Shader/ShaderStructure.hpp"


VkDescriptorType MusaConstantToDescriptorType(ShaderConstant constant)
{
	switch (constant.bindingType)
	{
		case ShaderConstantType::StorageBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case ShaderConstantType::StorageDynamicBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		case ShaderConstantType::TextureSampler:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case ShaderConstantType::UniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case ShaderConstantType::UniformDynamicBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		default:
			Assert(false);
	}

	return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

VulkanRenderingCloset::VulkanRenderingCloset(const VulkanDevice& device)
	: logicalDevice(&device)
{
}

VulkanPipeline* VulkanRenderingCloset::FindOrCreatePipeline(const GraphicsPipelineDescription& desc)
{
	VulkanPipeline* pipeline = nullptr;
	if (!pipelineStore.TryFind(desc, pipeline))
	{
		pipeline = CreatePipeline(desc);
	}
	return pipeline;
}

VulkanRenderPass* VulkanRenderingCloset::FindOrCreateRenderPass(const RenderTargetDescription& desc)
{
	VulkanRenderPass* renderPass = nullptr;
	if (!renderPassStore.TryFind(desc, renderPass))
	{
		renderPass = CreateRenderPass(desc);
	}
	return renderPass;
}

VulkanFramebuffer* VulkanRenderingCloset::FindOrCreateFramebuffer(const RenderTargetDescription& desc, const RenderTargetTextures& correspondingRTs)
{
	SimilarFramebuffers* frameBuffers = framebufferStore.Find(desc);
	VulkanFramebuffer* framebuffer = nullptr;
	if (frameBuffers)
	{
		for (uint32 i = 0; i < frameBuffers->Size(); ++i)
		{
			VulkanFramebuffer* fb = (*frameBuffers)[i];
			if (fb->ContainsRTs(correspondingRTs))
			{
				framebuffer = fb;
				break;
			}
		}
		if (!framebuffer)
		{
			framebuffer = CreateFramebuffer(desc, correspondingRTs);
			frameBuffers->Add(framebuffer);
		}
	}
	else
	{
		framebuffer = CreateFramebuffer(desc, correspondingRTs);
		SimilarFramebuffers similarFBs(1);
		similarFBs[0] = framebuffer;
		framebufferStore.Add(desc, std::move(similarFBs));
	}
	return framebuffer;
}

VulkanPipeline* VulkanRenderingCloset::CreatePipeline(const GraphicsPipelineDescription& desc)
{
	VulkanRenderPass* renderPass = FindOrCreateRenderPass(desc.renderTargets);
	VulkanPipelineLayout* layout = ConfigurePipelineLayout(desc);
	
	VulkanPipeline* pipeline = new VulkanPipeline(*logicalDevice);
	pipeline->Initialize(layout, desc, renderPass);
	pipelineStore.Add(desc, pipeline);

	return pipeline;
}

VulkanRenderPass* VulkanRenderingCloset::CreateRenderPass(const RenderTargetDescription& desc)
{
	VulkanRenderPass* renderPass = new VulkanRenderPass(*logicalDevice, desc);
	renderPassStore.Add(desc, renderPass);
	return renderPass;
}

VulkanFramebuffer* VulkanRenderingCloset::CreateFramebuffer(const RenderTargetDescription& desc, const RenderTargetTextures& correspondingRTs)
{
	VulkanRenderPass* renderPass = FindOrCreateRenderPass(desc);
	VulkanFramebuffer* framebuffer = new VulkanFramebuffer(*logicalDevice);
	framebuffer->Initialize(desc, correspondingRTs, renderPass);
	return framebuffer;
}

VulkanPipelineLayout* VulkanRenderingCloset::ConfigurePipelineLayout(const GraphicsPipelineDescription& desc)
{
	VulkanDescriptorSetLayout* dsLayout = GetDescriptorLayoutManager().CreateSetLayout();

	{
		const DynamicArray<ShaderConstant>& uniforms = desc.vertexShader->shaderUniforms;
		for (const auto& constant : uniforms)
		{
			dsLayout->AddDescriptorBinding(MusaConstantToDescriptorType(constant), VK_SHADER_STAGE_VERTEX_BIT, constant.binding);
		}
	}

	if (desc.fragmentShader)
	{
		const DynamicArray<ShaderConstant>& uniforms = desc.fragmentShader->shaderUniforms;
		for (const auto& constant : uniforms)
		{
			dsLayout->AddDescriptorBinding(MusaConstantToDescriptorType(constant), VK_SHADER_STAGE_FRAGMENT_BIT, constant.binding);
		}
	}

	dsLayout->BindLayout();

	return new VulkanPipelineLayout(*logicalDevice, { dsLayout });
}
