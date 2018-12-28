#pragma once

#include "Platform.h"

class Renderer;
class Camera;
class Model;

class ComputeEntity;
class VulkanComputePipeline;

class SceneRendering
{
public:
	SceneRendering() = default;
	~SceneRendering() = default;

	void RenderScene(Renderer* renderer);

	void SetActiveCamera(Camera* camera);
	//void SetTexture(class Texture* texture);

private:
	void CreateComputePipelines();
	void CreateFirstPassPipeline();
	void CreateBlendingPassPipeline();
	void CreateProcessingPathPipeline();
	void CreateFinalPassPipeline();
	ComputeEntity& BuildComputeCommandBuffer(Model& model);

private:
	Renderer* renderer = nullptr;
	Camera* activeCamera = nullptr;
	VulkanComputePipeline* firstPassPipeline = nullptr;
	VulkanComputePipeline* blendingPassPipeline = nullptr;
	VulkanComputePipeline* processLocalsPassPipeline = nullptr;
	VulkanComputePipeline* finalPassPipeline = nullptr;
};