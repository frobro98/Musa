
#include "Component.hpp"
#include "Internal/ComponentPool.hpp"

namespace Musa
{

Component::Component(ComponentType type_)
	: type(type_)
{
}


void DestroyComponent(Component* component)
{
	Internal::ComponentPool* pool = Internal::GetComponentPool(component->type);
	Assert(pool);
	pool->DestroyComponent(component);
}

}