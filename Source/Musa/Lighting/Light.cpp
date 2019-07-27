#include "Light.hpp"
#include "Graphics/GraphicsInterface.hpp"

Light::Light()
{
	lightInformationBuffer = GetGraphicsInterface().CreateUniformBuffer(sizeof(LightProperties));
}
