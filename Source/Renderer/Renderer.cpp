
#include "Platform.h"
#include "Renderer.h"
#include "SceneRendering.h"
#include "PipelineManager.h"
#include "DrawList.h"
#include "Model/ModelFactory.h"

#include "Vulkan/VulkanCreateInfos.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanCommandBuffer.h"
#include "Vulkan/VulkanSwapchain.h"
#include "Vulkan/VulkanSurface.h"
#include "Vulkan/VulkanMemory.h"
#include "Vulkan/VulkanShaderManager.h"

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObj, size_t location, int32_t msgCode,
	const char* layerPrefix, const char* msg,
	void* userData)
{
	UNUSED(objType, srcObj, location, msgCode, layerPrefix, userData);

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		printf("WARNING: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		printf("PERFORMANCE: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		printf("ERROR: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		printf("DEBUG: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}

	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		printf("INFO: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}

	return false;
}

Renderer::~Renderer()
{
	// TODO - add clean up for vulkan classes
	vkDestroyDebugReportCallbackEXT(instance, debugReportHandle, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void Renderer::Initialize()
{
	const tchar* validationLayers[] = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const tchar* instanceExtensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_PLATFORM_SURFACE_EXTENSION,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};


	VkDebugReportFlagsEXT debugFlags =
		//VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	VkDebugReportCallbackCreateInfoEXT debugInfo = Vk::DebugReportCallbackInfo(VulkanDebugCallback, debugFlags, this);
	VkInstanceCreateInfo instanceInfo = Vk::InstanceInfo(validationLayers, ArraySize(validationLayers),
		instanceExtensions, ArraySize(instanceExtensions), &debugInfo);
	vkCreateInstance(&instanceInfo, nullptr, &instance);

	// Trying to get around warnings
	void* createDebugFunc = vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	void* destroyDebugFunc = vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	vkCreateDebugReportCallbackEXT = reinterpret_cast<vk_create_debug_report>(createDebugFunc);
	vkDestroyDebugReportCallbackEXT = reinterpret_cast<vk_destroy_debug_report>(destroyDebugFunc);

	vkCreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debugReportHandle);
}

void Renderer::SetupRendering(const Window* window)
{
	drawList = new DrawList;

	logicalDevice = new VulkanDevice(this);
	logicalDevice->Initialize(instance, window);

	cbManager = new VulkanCommandBufferManager(logicalDevice);
	cbManager->InitializeInternal();
	VulkanCommandBufferManager::cbManager = cbManager;

	pipelineManager = new PipelineManager(logicalDevice);
	PipelineManager::pmInstance = pipelineManager;
	VulkanShaderManager::Instance().logicalDevice = logicalDevice;
	VulkanMemory::Initialize(logicalDevice);

	ConfigureSwapchain(window);

	renderPass = swapchain->GetRenderPass();

	sceneRendering = new SceneRendering;
	ModelFactory::Initialize(this);
}

void Renderer::ConfigureSwapchain(const Window* window)
{
	VulkanSurface* surface = new VulkanSurface(logicalDevice, window);
	swapchain = new VulkanSwapchain(logicalDevice, surface);
	swapchain->Initialize();
}

void Renderer::RenderFrame()
{
	// TODO - Need to uncomment these when I have something to actually submit
	if (PrepareFrame())
	{
		sceneRendering->RenderScene(this);

		SubmitFrame();
	}
}

bool Renderer::PrepareFrame()
{
	VkResult result = swapchain->GetNextImage();
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		swapchain->Recreate();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		// TODO - log
		assert(false);
		return false;
	}

	return true;
}

void Renderer::SubmitFrame()
{
	swapchain->SubmitFrame();
	swapchain->Present();

	VulkanCommandBufferManager::GetActiveTransferBuffer()->Reset();
	VulkanCommandBufferManager::GetActiveGraphicsBuffer()->Reset();
	VulkanCommandBufferManager::GetActiveComputeBuffer()->Reset();
}
