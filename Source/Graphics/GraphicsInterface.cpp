
#include "Vulkan/VulkanGraphicsInterface.hpp"

GraphicsInterface& GetGraphicsInterface()
{
	static VulkanGraphicsInterface graphicsInterface;
	return graphicsInterface;
}
