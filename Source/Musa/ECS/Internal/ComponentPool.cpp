
#include "ECS/Internal/ComponentPool.hpp"
#include "ECS/ComponentTypeDefinition.hpp"
#include "ECS/Component.hpp"
#include "Containers/Map.h"

namespace Musa::Internal
{

namespace
{
//static Map<ComponentType, ComponentPool*> componentTypePools;
}

// ComponentPool* RegisterComponentType(ComponentType type)
// {
// 	// TODO - Implement Map::ContainsKey
// 	ComponentPool* pool;
// 	if (!componentTypePools.TryFind(type, pool))
// 	{
// 		pool = new ComponentPool;
// 		componentTypePools.Add(type, pool);
// 	}
// 	return pool;
// }
// 
// ComponentPool* GetComponentPool(ComponentType type)
// {
// 	ComponentPool* pool = nullptr;
// 	componentTypePools.TryFind(type, pool);
// 	return pool;
// }


void ComponentPool::DestroyComponent(Component* component)
{
	[[maybe_unused]] bool removed = componentPool.TryRemoveElement(component);
	Assert(removed);
	delete &component;
}

Component* ComponentPool::FindComponentAtIndex(uint32 index)
{
	Assert(index < componentPool.Size());
	Component* component = componentPool[index];
	Assert(component != nullptr);
	return component;
}

uint32 ComponentPool::FindIndexOf(Component& component)
{
	Assert(componentPool.Contains(&component));
	uint32 index;
	[[maybe_unused]] bool result = componentPool.TryFindFirstIndex(&component, index);
	Assert(result);
	return index;
}
}
