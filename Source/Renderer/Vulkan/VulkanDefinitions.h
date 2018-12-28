#pragma once

#if !GRAPHICS_API_VK
//#error "Vulkan must be installed to run this graphics engine"
#endif

#include "Platform.h"

WALL_WRN_PUSH
#include "vulkan/vulkan.h"
WALL_WRN_POP

#define CHECK_VK(expression) (assert(expression == VK_SUCCESS))

#ifdef _DEBUG
// vkCreateDebugReportCallbackEXT call replication
#define VK_CREATE_DEBUG_REPORT(funcName) VkResult (VKAPI_PTR *funcName)(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
typedef VK_CREATE_DEBUG_REPORT(vk_create_debug_report);
static vk_create_debug_report vkCreateDebugReportCallbackEXT_ = nullptr;
#define vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT_

// vkDestroyDebugReportCallbackEXT call replication
#define VK_DESTROY_DEBUG_REPORT(funcName) VkResult (VKAPI_PTR *funcName)(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
typedef VK_DESTROY_DEBUG_REPORT(vk_destroy_debug_report);
static vk_destroy_debug_report vkDestroyDebugReportCallbackEXT_ = nullptr;
#define vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT_
#endif //DEBUG

using GPUHandle = VkPhysicalDevice;

// TODO - Create vulkan allocator for hook into my memory system