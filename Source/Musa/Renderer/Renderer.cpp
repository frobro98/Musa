
#include "Platform.h"
#include "Renderer.h"
#include "SceneRendering.h"
#include "Model/ModelFactory.h"

#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/Vulkan/VulkanCreateInfos.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanCommandBuffer.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanMemory.h"
#include "Graphics/Vulkan/VulkanShaderManager.h"
#include "Graphics/Vulkan/VulkanRenderingCloset.hpp"
#include "Graphics/Vulkan/VulkanStagingBufferManager.hpp"

Renderer::~Renderer()
{
}

void Renderer::SetupRendering()
{	
	sceneRendering = new SceneRendering;
}

void Renderer::SetCurrentScene(Scene& currentScene)
{
	scene = &currentScene;
}

void Renderer::RenderFrame(const View& view)
{
	// TODO - Need to uncomment these when I have something to actually submit
	if (PrepareFrame())
	{
		GetGraphicsInterface().GetGraphicsDevice()->GetStagingBufferManager().ProcessDeferredReleases();

		sceneRendering->RenderScene(this, *scene, view);

		SubmitFrame();
	}
}

bool Renderer::PrepareFrame()
{
	VkResult result = GetGraphicsInterface().GetGraphicsSwapchain()->GetNextImage();
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		GetGraphicsInterface().GetGraphicsSwapchain()->Recreate();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		// TODO - log
		Assert(false);
		return false;
	}

	return true;
}

void Renderer::SubmitFrame()
{
	GetGraphicsInterface().GetGraphicsSwapchain()->SubmitFrame();
	GetGraphicsInterface().GetGraphicsSwapchain()->Present();
}
