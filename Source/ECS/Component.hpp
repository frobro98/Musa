#pragma once

#include "EngineCore/Types.h"
#include "ECS/Internal/ComponentPool.hpp"
#include "ECS/ComponentTypeDefinition.hpp"

namespace Musa
{

// Base class of all components
struct Component
{
	ComponentType type;

	Component(ComponentType type);
	virtual ~Component() = default;

	Component(const Component&) = delete;
	Component(Component&&) = delete;
	Component& operator=(const Component&) = delete;
	Component& operator=(Component&&) = delete;
};

template <class Comp, typename = std::enable_if_t<std::is_base_of_v<Component, Comp>>>
Comp* CreateComponent()
{
	ComponentType type = Comp::GetComponentType();

	Internal::ComponentPool* pool = Internal::GetComponentPool(type);
	if (pool == nullptr)
	{
		pool = Internal::RegisterComponentType(type);
	}

	return pool->CreateComponent<Comp>();
}

void DestroyComponent(Component* component);

}
