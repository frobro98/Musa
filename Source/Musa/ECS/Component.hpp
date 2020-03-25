#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
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
inline Comp* CreateComponent()
{
	ComponentType type = Comp::GetComponentType();

	Internal::ComponentPool* pool = Internal::GetComponentPool(type);
	if (pool == nullptr)
	{
		pool = Internal::RegisterComponentType(type);
	}

	return pool->CreateComponent<Comp>();
}

inline Component* CreateComponentFrom(const ComponentType& /*type*/)
{

}

void DestroyComponent(Component* component);

}
