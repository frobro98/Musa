#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorPool.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "VulkanCommandBuffer.h"


static VkShaderStageFlagBits ConvertToVkStage(ShaderStage shaderStage)
{
	switch (shaderStage)
	{
	case Stage_Vert:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case Stage_TessControl:
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case Stage_TessEval:
		return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	case Stage_Geom:
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	case Stage_Frag:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case Stage_Comp:
		return VK_SHADER_STAGE_COMPUTE_BIT;
	case Stage_Max:
	default:
		assert(false);
		return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	}
}

VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice * device, Array<VulkanDescriptorSetLayout*>&& layouts)
	: descriptorSetLayouts(std::move(layouts)),
	logicalDevice(device)
{
	Array<VkDescriptorSetLayout> layoutHandles;
	layoutHandles.Reserve(descriptorSetLayouts.Size());
	// TODO - Make arrays for each loop able
	for (uint32 i = 0; i < descriptorSetLayouts.Size(); ++i)
	{
		VkDescriptorSetLayout handle = descriptorSetLayouts[i]->GetNativeHandle();
		assert(handle != VK_NULL_HANDLE);
		layoutHandles.Add(handle);
	}

	VkPipelineLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = layoutHandles.Size();
	layoutInfo.pSetLayouts = layoutHandles.GetData();

	CHECK_VK(vkCreatePipelineLayout(logicalDevice->GetNativeHandle(), &layoutInfo, nullptr, &pipelineLayout));
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
	vkDestroyPipelineLayout(logicalDevice->GetNativeHandle(), pipelineLayout, nullptr);
	for (uint32 i = 0; i < descriptorSetLayouts.Size(); ++i)
	{
		delete descriptorSetLayouts[i];
	}
}

VulkanPipeline::VulkanPipeline(VulkanDevice * device)
	: logicalDevice(device)
{
}

VulkanPipeline::~VulkanPipeline()
{
}

void VulkanPipeline::Initialize(const VulkanPipelineLayout* layout, const RenderPipelineInfo& renderingInfo, VulkanRenderPass* renderPass)
{
	pipelineLayout = layout;
	renderInfo = std::move(renderingInfo);

	Array<VkPipelineShaderStageCreateInfo> shaderStages;
	for (uint32 i = 0; i < static_cast<uint32>(ShaderStage::Stage_Max); ++i)
	{
		if (renderingInfo.shaders[i])
		{
			VkPipelineShaderStageCreateInfo vkShaderStage = {};
			vkShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vkShaderStage.stage = ConvertToVkStage(renderingInfo.shaders[i]->GetShaderStage());
			vkShaderStage.module = renderingInfo.shaders[i]->GetModule();
			vkShaderStage.pName = "main";
			vkShaderStage.pSpecializationInfo;

			shaderStages.Add(vkShaderStage);
		}
	}

	Array<VkVertexInputBindingDescription> inputBindings(renderingInfo.vertexBindings.Size());
	for (uint32 i = 0; i < inputBindings.Size(); ++i)
	{
		renderingInfo.vertexBindings[i].WriteToBinding(inputBindings[i]);
	}
	Array<VkVertexInputAttributeDescription> inputAttribs(renderingInfo.vertexAttributes.Size());
	for (uint32 i = 0; i < inputAttribs.Size(); ++i)
	{
		renderingInfo.vertexAttributes[i].WriteToAttribute(inputAttribs[i]);
	}

	// Input information
	VkPipelineVertexInputStateCreateInfo vertInputInfo = {};
	vertInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertInputInfo.vertexBindingDescriptionCount = inputBindings.Size();
	vertInputInfo.pVertexBindingDescriptions = inputBindings.GetData();
	vertInputInfo.vertexAttributeDescriptionCount = inputAttribs.Size();
	vertInputInfo.pVertexAttributeDescriptions = inputAttribs.GetData();

	// Input Assembly info
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = static_cast<VkPrimitiveTopology>(renderingInfo.topology);
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// Rasterizer info
	VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
	rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	renderingInfo.rasterizer.WriteToRasterizer(rasterizerInfo);

	// Multisampling info
	VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	// TODO - Look up what these sampling settings are meant to do in the render pipeline
	//multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	//multisampleInfo.alphaToOneEnable = VK_FALSE;
	//multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	//multisampleInfo.minSampleShading = 0.f;
	//multisampleInfo.pSampleMask = nullptr;

	// Depth-stencil info
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	renderingInfo.depthStencilInfo.WriteToDepthStencil(depthStencilInfo);

	// Blending states
	Array<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
	for (uint32 i = 0; i < renderingInfo.blendAttachments.Size(); ++i)
	{
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		renderingInfo.blendAttachments[i].WriteToAttachment(colorBlendAttachment);

		colorBlendAttachments.Add(colorBlendAttachment);
	}

	// Color blend state info
	VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.attachmentCount = (uint32)colorBlendAttachments.Size();
	colorBlendInfo.pAttachments = colorBlendAttachments.GetData();
	colorBlendInfo.blendConstants[0] = 1.0f;
	colorBlendInfo.blendConstants[1] = 1.0f;
	colorBlendInfo.blendConstants[2] = 1.0f;
	colorBlendInfo.blendConstants[3] = 1.0f;

	// Viewport info
	VkPipelineViewportStateCreateInfo viewportInfo = {};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.scissorCount = 1;

	// Dynamic states
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	// Dynamic state info
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = ArraySize(dynamicStates);
	dynamicStateInfo.pDynamicStates = dynamicStates;

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.Size();
	pipelineInfo.pStages = shaderStages.GetData();
	pipelineInfo.pVertexInputState = &vertInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisampleInfo;
	pipelineInfo.pDepthStencilState = &depthStencilInfo;
	pipelineInfo.pColorBlendState = &colorBlendInfo;
	pipelineInfo.pDynamicState = &dynamicStateInfo;
	pipelineInfo.layout = pipelineLayout->GetNativeHandle();
	pipelineInfo.renderPass = renderPass->GetNativeHandle();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	CHECK_VK(vkCreateGraphicsPipelines(logicalDevice->GetNativeHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
}

void VulkanPipeline::Bind(VulkanCommandBuffer * cmdBuffer) const
{
	assert(cmdBuffer->GetBufferState() == VulkanCommandBuffer::State::InRenderPass);
	vkCmdBindPipeline(cmdBuffer->GetNativeHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void VulkanPipeline::BindDescriptorSet(VulkanCommandBuffer* cmdBuffer, VulkanDescriptorSet* descriptorSet) const
{
	assert(cmdBuffer->GetBufferState() == VulkanCommandBuffer::State::InRenderPass);

	VkDescriptorSet ds = descriptorSet->GetNativeHandle();
	vkCmdBindDescriptorSets(cmdBuffer->GetNativeHandle(), 
		VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->GetNativeHandle(),
		0, 1, &ds, 0, nullptr);
}

VulkanDescriptorSet * VulkanPipeline::GetUnusedDescriptorSet()
{
	static uint32 numDescriptorsCreated = 0;
	VulkanDescriptorSet* descriptorSet = nullptr;
	if (unusedSetIndex < descriptorSets.Size())
	{
		descriptorSet = descriptorSets[unusedSetIndex];
		++unusedSetIndex;
	}
	else
	{
		++numDescriptorsCreated;
		VulkanDescriptorPool* pool = logicalDevice->GetActivePool();
		descriptorSet = pool->AllocateDescriptorSet(pipelineLayout->GetDescriptorSetLayouts()[0]);

		assert(descriptorSet != nullptr);
		descriptorSets.Add(descriptorSet);
		++unusedSetIndex;
	}

	return descriptorSet;
}

void VulkanPipeline::ResetPipeline()
{
	unusedSetIndex = 0;
}