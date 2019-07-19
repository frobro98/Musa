#include "Light.hpp"
// TODO - Remove vulkan renferences
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanMemory.h"

Light::Light()
{
	lightInformationBuffer = VulkanMemory::CreateUniformBuffer(sizeof(LightProperties));
}
