#pragma once

#include "Platform.h"

#if GRAPHICS_API_VK
#include "Vulkan/VulkanUniformBuffer.h"
//#include "Vulkan/VulkanRenderer.h"
#else
//#include "OpenGL/OpenGLRenderer.h"
#endif

#include "Renderer.h"