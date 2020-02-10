#include "Light.hpp"
#include "Graphics/GraphicsInterface.hpp"

Light::Light(GameWorld& world)
	: GameObject(world)
{
	lightInformationBuffer = GetGraphicsInterface().CreateUniformBuffer(sizeof(LightProperties));
}
