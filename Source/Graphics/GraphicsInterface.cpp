
#include "Vulkan/VulkanGraphicsInterface.hpp"

namespace
{
static VulkanGraphicsInterface graphicsInterface;
}

GraphicsInterface& GetGraphicsInterface()
{
	return graphicsInterface;
}
