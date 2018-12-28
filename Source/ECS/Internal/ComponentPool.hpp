#pragma once

#include "EngineCore/Platform.h"

#include <type_traits>

#include "Containers/Array.h"

namespace Musa
{
struct ComponentType;
struct Component;

namespace Internal
{

class ComponentPool final
{
public:

	template<class Comp, typename = std::enable_if_t<std::is_base_of_v<Component, Comp>>>
	Comp* CreateComponent()
	{
		Comp* comp = new Comp();
		componentPool.Add(comp);
		return comp;
	}
	void DestroyComponent(Component* component);

	// TODO - Might be better to always have the component index with it
	Component* FindComponentAtIndex(uint32 index);
	uint32 FindIndexOf(Component& component);

private:
	Array<Component*> componentPool;
};

ComponentPool* RegisterComponentType(ComponentType type);
ComponentPool* GetComponentPool(ComponentType type);

}
}