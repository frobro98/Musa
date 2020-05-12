// Copyright 2020, Nathan Blane

#include "VulkanRenderingCloset.hpp"
#include "VulkanShader.h"
#include "VulkanDescriptorSet.h"
#include "VulkanPipeline.h"
#include "VulkanUtilities.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanCreateInfos.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptorLayoutManager.h"

// TODO - The shader resource that lives in the Shader.lib doesn't belong here..
#include "Shader/ShaderResource.hpp"


VulkanRenderingCloset::VulkanRenderingCloset(const VulkanDevice& device)
	: logicalDevice(device)
{
}

VulkanRenderingCloset::~VulkanRenderingCloset()
{
	for (auto pipelinePair : pipelineStore)
	{
		delete pipelinePair.second;
	}
	for (auto renderPassPair : renderPassStore)
	{
		delete renderPassPair.second;
	}
	for (auto similarFBPair : framebufferStore)
	{
		for (auto framebuffer : similarFBPair.second)
		{
			delete framebuffer;
		}
	}
	for (auto samplerPair : samplerStore)
	{
		vkDestroySampler(logicalDevice.GetNativeHandle(), samplerPair.second, nullptr);
	}
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

VulkanFramebuffer* VulkanRenderingCloset::FindOrCreateFramebuffer(const RenderTargetDescription& desc, const NativeRenderTargets& correspondingRTs)
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

VkSampler VulkanRenderingCloset::FindOrCreateSampler(const SamplerDescription& params)
{
	VkSampler* foundSampler = samplerStore.Find(params);
	if (!foundSampler)
	{
		VkSamplerCreateInfo samplerInfo = Vk::SamplerInfo(params);
		VkSampler sampler;
		vkCreateSampler(logicalDevice.GetNativeHandle(), &samplerInfo, nullptr, &sampler);
		samplerStore.Add(params, sampler);
		return sampler;
	}

	return *foundSampler;
}

VulkanPipeline* VulkanRenderingCloset::CreatePipeline(const GraphicsPipelineDescription& desc)
{
	VulkanRenderPass* renderPass = FindOrCreateRenderPass(desc.renderTargets);
	VulkanPipelineLayout* layout = ConfigurePipelineLayout(desc);
	
	VulkanPipeline* pipeline = new VulkanPipeline(logicalDevice);
	pipeline->Initialize(layout, desc, renderPass);
	pipelineStore.Add(desc, pipeline);

	return pipeline;
}

VulkanRenderPass* VulkanRenderingCloset::CreateRenderPass(const RenderTargetDescription& desc)
{
	VulkanRenderPass* renderPass = new VulkanRenderPass(logicalDevice, desc);
	renderPassStore.Add(desc, renderPass);
	return renderPass;
}

VulkanFramebuffer* VulkanRenderingCloset::CreateFramebuffer(const RenderTargetDescription& desc, const NativeRenderTargets& correspondingRTs)
{
	VulkanRenderPass* renderPass = FindOrCreateRenderPass(desc);
	VulkanFramebuffer* framebuffer = new VulkanFramebuffer(logicalDevice);
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

	return new VulkanPipelineLayout(logicalDevice, { dsLayout });
}
