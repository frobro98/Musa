
#include "Entity.hpp"
#include "Internal/EntityPool.hpp"
#include "ECS/SystemsManager.hpp"

namespace Musa
{
namespace
{
static Map<EntityID, Entity*> entityPool;
static Map<EntityID, DynamicArray<Component*>> ownedComponentMap; // This may point to the collection of components, but there isn't any notion of what component is what
static Map<EntityID, ComponentSet> entityDescriptions;
static Map<ComponentSet, DynamicArray<Entity*>> componentsOnEntities;
}

namespace Internal
{
Component* GetComponentFromImpl(const Entity& entity, ComponentType type)
{
	REF_CHECK(entity);
	DynamicArray<Component*>& compList = ownedComponentMap[entity.id];
	Component* foundComponent = nullptr;
	for (auto& component : compList)
	{
		if (component->type == type)
		{
			foundComponent = component;
			break;
		}
	}
	return foundComponent;
}
bool HasComponentImpl(const Entity& entity, ComponentType type)
{
	REF_CHECK(entity);
	ComponentSet& desc = entityDescriptions[entity.id];
	return desc.ContainsType(type);
}
void AttachComponentImpl(const Entity& entity, Component& comp)
{
	REF_CHECK(entity, comp);
	// TODO - When a component is attached, it needs to find all systems use the component type and tell them to potentially update
	ownedComponentMap[entity.id].Add(&comp);
	AddComponentWithTypeTo(comp.type, entityDescriptions[entity.id]);
}
void AttachComponentsImpl(const Entity& /*entity*/, const ComponentSet& compDesc)
{
	auto& set = compDesc.set;
	for (uint32 i = 0; i < set.size(); ++i)
	{
		if (set.test(i))
		{

		}
	}
}
void AssociateSystemsWithEntity(const Entity& entity)
{
	GetSystemsManager().AddEntityToProcess(entity);
}
}

Entity* CreateEntity()
{
	Entity* entity = new Entity(GetNewEntityID());
	entityPool.Add(entity->id, entity);
	return entity;
}

Entity* CreateEntity(const ComponentSet& components)
{
	REF_CHECK(components);

	Entity* entity = new Entity(GetNewEntityID());
	entityPool.Add(entity->id, entity);
	AttachComponentsTo(*entity, components);
	return entity;
}

Entity* CreateEntity(const tchar* name)
{
	Assert(name);

	Entity* entity = new Entity(GetNewEntityID(), name);
	entityPool.Add(entity->id, entity);
	return entity;
}

Entity* CreateEntity(const tchar* name, const ComponentSet& components)
{
	Assert(name);
	REF_CHECK(components);

	Entity* entity = new Entity(GetNewEntityID());
	entityPool.Add(entity->id, entity);
	AttachComponentsTo(*entity, components);
	return entity;
}

void Musa::DestroyEntity(Entity& entity)
{
	REF_CHECK(entity);
	bool result = entityPool.Remove(entity.id);
	Assert(result);

	// TODO - Consider having a lambda passed into the try find method to prevent this copy? Or to prevent having a pointer?
	DynamicArray<Component*>* componentList = ownedComponentMap.Find(entity.id);

	for (auto& component : *componentList)
	{
		Internal::ComponentPool* pool = Internal::GetComponentPool(component->type);
		Assert(pool != nullptr);
		pool->DestroyComponent(component);
	}

	ownedComponentMap.Remove(entity.id);
}

Entity* FindEntity(const EntityID& id)
{
	return entityPool[id];
}

Entity* FindEntity(const tchar* name)
{
	Entity* entity = nullptr;
	for (const auto& entityPair : entityPool)
	{
		if (entityPair.second != nullptr && entityPair.second->name == name)
		{
			entity = entityPair.second;
			break;
		}
	}
	return entity;
}

ComponentSet GetComponentDescriptionOf(const Entity& entity)
{
	return entityDescriptions[entity.id];
}

}
