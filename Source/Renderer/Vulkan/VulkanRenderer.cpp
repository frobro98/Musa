// TODO - This will be revisited when I have a working Vulkan renderer

// 
// #include "VulkanRenderer.h"
// #include "Platform.h"
// #include "Renderer/Renderer.h"
// #include "VulkanCreateInfos.h"
// #include "VulkanDevice.h"
// 
// #if GRAPHICS_API_VK
// 
// VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugReportFlagsEXT flags,
// 	VkDebugReportObjectTypeEXT objType,
// 	uint64_t srcObj, size_t location, int32_t msgCode,
// 	const char* layerPrefix, const char* msg,
// 	void* userData)
// {
// 	UNUSED(objType, srcObj, location, msgCode, layerPrefix, userData);
// 
// 	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
// 	{
// 		printf("WARNING: ");
// 		printf("@[%s]: %s\n", layerPrefix, msg);
// 	}
// 	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
// 	{
// 		printf("PERFORMANCE: ");
// 		printf("@[%s]: %s\n", layerPrefix, msg);
// 	}
// 	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
// 	{
// 		printf("ERROR: ");
// 		printf("@[%s]: %s\n", layerPrefix, msg);
// 	}
// 	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
// 	{
// 		printf("DEBUG: ");
// 		printf("@[%s]: %s\n", layerPrefix, msg);
// 	}
// 
// 	return false;
// }
// 
// VulkanRenderer::~VulkanRenderer()
// {
// 	// TODO - add clean up for vulkan classes
// 	vkDestroyDebugReportCallbackEXT(instance, debugReportHandle, nullptr);
// 	vkDestroyInstance(instance, nullptr);
// }
// 
// void VulkanRenderer::Initialize()
// {
// 	const tchar* validationLayers[] = {
// 		"VK_LAYER_LUNARG_standard_validation"
// 	};
// 
// 	const tchar* instanceExtensions[] = {
// 		VK_KHR_SURFACE_EXTENSION_NAME,
// 		VK_PLATFORM_SURFACE_EXTENSION,
// 		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
// 	};
// 
// 
// 	VkDebugReportFlagsEXT debugFlags =
// 		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
// 		VK_DEBUG_REPORT_WARNING_BIT_EXT |
// 		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
// 		VK_DEBUG_REPORT_ERROR_BIT_EXT |
// 		VK_DEBUG_REPORT_DEBUG_BIT_EXT;
// 
// 	VkDebugReportCallbackCreateInfoEXT debugInfo = Vk::DebugReportCallbackInfo(VulkanDebugCallback, debugFlags, this);
// 	VkInstanceCreateInfo instanceInfo = Vk::InstanceInfo(validationLayers, ArraySize(validationLayers),
// 		instanceExtensions, ArraySize(instanceExtensions), &debugInfo);
// 	vkCreateInstance(&instanceInfo, nullptr, &instance);
// 
// 	// Trying to get around warnings
// 	void* createDebugFunc = vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
// 	void* destroyDebugFunc = vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
// 	vkCreateDebugReportCallbackEXT = reinterpret_cast<vk_create_debug_report>(createDebugFunc);
// 	vkDestroyDebugReportCallbackEXT = reinterpret_cast<vk_destroy_debug_report>(destroyDebugFunc);
// 
// 	vkCreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debugReportHandle);
// }
// 
// void VulkanRenderer::SetupRendering(const Window* window)
// {
// 	assert(window != nullptr);
// 
// 	logicalDevice = new VulkanDevice(this);
// 	logicalDevice->Initialize(instance, window);
// 
// 
// }
// 
// void VulkanRenderer::ConfigureSwapchain()
// {
// }
// 
// 
// #endif