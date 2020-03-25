#include "EntityPool.hpp"
#include "ECS/Component.hpp"
#include "ECS/Internal/ComponentPool.hpp"
#include "ECS/ComponentTypeDefinition.hpp"

namespace Musa::Internal
{

// Entity* EntityPool::CreateEntity()
// {
// 	return new Entity(EntityID());
// }
// 
// Entity* EntityPool::CreateEntity(const tchar* name)
// {
// 	return new Entity(EntityID(), name);
// }
// 
// void EntityPool::DestroyEntity(Entity* entity)
// {
// 	bool result = entityPool.Remove(entity->id);
// 	Assert(result);
// 	DynamicArray<Component*> componentList;
// 	result = ownedComponentMap.TryFind(entity->id, componentList);
// 	Assert(result);
// 
// 	for (auto& component : componentList)
// 	{
// 		ComponentPool* pool = GetComponentPool(component->type);
// 		Assert(pool);
// 		pool->DestroyComponent(component);
// 	}
// 	ownedComponentMap.Remove(entity->id);
// }
// 
// Entity* EntityPool::FindEntityByID(const EntityID& index)
// {
// 	Entity* entity = entityPool[index];
// 	Assert(entity != nullptr);
// 	return entity;
// }
// 
// Entity* EntityPool::FindEntityByName(const tchar* name)
// {
// 	Entity* entity = nullptr;
// 	for(const auto& ent : entityPool)
// 	{
// 		if (ent.second != nullptr && ent.second->name == name)
// 		{
// 			entity = ent.second;
// 			break;
// 		}
// 	}
// 	return entity;
// }
// 
// void EntityPool::AttachComponentTo(const Entity& entity, Component& componentToAttach)
// {
// 	// TODO - Component needs to be added to the system(s) that it's required in. Need to check all components on entity as well
// 	ownedComponentMap[entity.id].Add(&componentToAttach);
// 	AddComponentWithTypeTo(componentToAttach.type, entityDescriptions[entity.id]);
// }
// 
// Component* EntityPool::FindComponentOn(const Entity& entity, ComponentType type)
// {
// 	DynamicArray<Component*>& compList = ownedComponentMap[entity.id];
// 	Component* foundComponent = nullptr;
// 	for (auto& component : compList)
// 	{
// 		if (component->type == type)
// 		{
// 			foundComponent = component;
// 			break;
// 		}
// 	}
// 	return foundComponent;
// }
// 
// bool EntityPool::EntityHasComponent(const Entity& entity, ComponentType type)
// {
// 	ComponentGroupDescription& desc = entityDescriptions[entity.id];
// 	return desc.ContainsType(type);
// }
// 
// ComponentGroupDescription EntityPool::FindComponentTypesOn(const Entity& entity)
// {
// 	return entityDescriptions[entity.id];
// }

}
