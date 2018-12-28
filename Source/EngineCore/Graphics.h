#pragma once

#ifndef GRAPHICS_API_VK 
#define GRAPHICS_API_VK 1
#endif

#if GRAPHICS_API_VK
#include "Renderer/Vulkan/VulkanDefinitions.h"
#include "Renderer/Vulkan/VulkanAbstractions.h"
#else
#error "Vulkan must be activated"
#endif

#include "Model/GeometryPrimitives.h"
