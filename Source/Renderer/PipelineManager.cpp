#include "PipelineManager.h"
#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanComputePipeline.h"
#include "Vulkan/VulkanDevice.h"
#include "Renderer/Renderer.h"

PipelineManager* PipelineManager::pmInstance = nullptr;

VulkanPipeline* PipelineManager::CreatePipeline(const tchar* pipelineName, const VulkanPipelineLayout* layout, const RenderPipelineInfo& renderingInfo)
{
	VulkanPipeline* pipeline = new VulkanPipeline(Instance().logicalDevice);
	pipeline->Initialize(layout, renderingInfo, Instance().logicalDevice->GetRenderContext()->GetRenderPass());
	
	PipelineNode* node = new PipelineNode;
	node->pipeline = pipeline;
	node->pipelineName = pipelineName;
	node->next = Instance().head;
	Instance().head = node; 
	
	return pipeline;
}

VulkanPipeline* PipelineManager::CreatePipeline(const String& pipelineName, const VulkanPipelineLayout* layout, const RenderPipelineInfo& renderingInfo)
{
	VulkanPipeline* pipeline = new VulkanPipeline(Instance().logicalDevice);
	pipeline->Initialize(layout, renderingInfo, Instance().logicalDevice->GetRenderContext()->GetRenderPass());

	PipelineNode* node = new PipelineNode;
	node->pipeline = pipeline;
	node->pipelineName = pipelineName;
	node->next = Instance().head;
	Instance().head = node;

	return pipeline;
}

VulkanComputePipeline* PipelineManager::CreatePipeline(const VulkanPipelineLayout* layout, VulkanShader* shader)
{
	VulkanComputePipeline* pipeline = new VulkanComputePipeline(Instance().logicalDevice);
	pipeline->Initialize(layout, shader);
	return pipeline;
}

VulkanPipeline* PipelineManager::FindPipeline(const tchar* pipelineName)
{
	// TODO - This find will be modified once I have a Map implementation...
	PipelineNode* current = Instance().head;
	while (current != nullptr)
	{
		if (pipelineName == current->pipelineName)
		{
			return current->pipeline;
		}

		current = current->next;
	}

	return nullptr;
}

PipelineManager::PipelineManager(VulkanDevice* device)
	: logicalDevice(device)
{
}

PipelineManager::~PipelineManager()
{
}

PipelineManager& PipelineManager::Instance()
{
	assert(pmInstance != nullptr);
	return *pmInstance;
}
