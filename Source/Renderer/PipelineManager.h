#pragma once

#include "Containers/Array.h"
#include "Graphics.h"
#include "String/String.h"
#include "Vulkan/VulkanRenderingInfo.h"

class VulkanDevice;
class VulkanShader;
class VulkanPipeline;
class VulkanComputePipeline;
class VulkanPipelineLayout;

class PipelineManager
{
public:
	// TODO - Create some struct that contains initialization data
	static VulkanPipeline* CreatePipeline(const tchar* pipelineName, const VulkanPipelineLayout* layout, const RenderPipelineInfo& renderingInfo);
	static VulkanPipeline* CreatePipeline(const String& pipelineName, const VulkanPipelineLayout* layout, const RenderPipelineInfo& renderingInfo);
	static VulkanComputePipeline* CreatePipeline(const VulkanPipelineLayout* layout, VulkanShader* shader);
	static VulkanPipeline* FindPipeline(const tchar* pipelineName);

private:
	friend class Renderer;

	struct PipelineNode
	{
		VulkanPipeline* pipeline = nullptr;
		PipelineNode* next = nullptr;
		String pipelineName;
	};
private:
	PipelineManager(VulkanDevice* device);
	~PipelineManager();

	static PipelineManager* pmInstance;
	static PipelineManager& Instance();

private:
	VulkanDevice* logicalDevice;
	PipelineNode* head = nullptr;
};