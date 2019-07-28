
#include "Vulkan/VulkanRenderer.hpp"

Renderer& GetRenderContext()
{
	static VulkanRenderer renderer;
	return renderer;
}
