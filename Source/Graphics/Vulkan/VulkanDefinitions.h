#pragma once



#include "Platform.h"
#include "EngineCore/Assertion.h"

WALL_WRN_PUSH
#include "vulkan/vulkan.h"
WALL_WRN_POP

#define CHECK_VK(expression) Assert(expression == VK_SUCCESS)

// vkCreateDebugReportCallbackEXT call replication
#define VK_CREATE_DEBUG_REPORT(funcName) VkResult (VKAPI_PTR *funcName)(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
typedef VK_CREATE_DEBUG_REPORT(vk_create_debug_report);
static vk_create_debug_report vkCreateDebugReportCallbackEXT_ = nullptr;
#ifdef _DEBUG
#define vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT_
#else
#define vkCreateDebugReportCallbackEXT 
#endif

// vkDestroyDebugReportCallbackEXT call replication
#define VK_DESTROY_DEBUG_REPORT(funcName) VkResult (VKAPI_PTR *funcName)(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
typedef VK_DESTROY_DEBUG_REPORT(vk_destroy_debug_report);
static vk_destroy_debug_report vkDestroyDebugReportCallbackEXT_ = nullptr;
#ifdef _DEBUG
#define vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT_
#else
#define vkDestroyDebugReportCallbackEXT 
#endif

using GPUHandle = VkPhysicalDevice;

// TODO - Create vulkan allocator for hook into my memory system