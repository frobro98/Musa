
#include "Entity.hpp"
#include "Internal/EntityPool.hpp"
#include "ECS/SystemsManager.hpp"

namespace Musa
{

namespace Internal
{
Component* GetComponentFromImpl(const Entity& entity, ComponentType type)
{
	return EntityPool::FindComponentOn(entity, type);
}
bool HasComponentImpl(const Entity& entity, ComponentType type)
{
	return EntityPool::EntityHasComponent(entity, type);
}
void AttachComponentImpl(const Entity& entity, Component& comp)
{
	// TODO - When a component is attached, it needs to find all systems use the component type and tell them to potentially update
	EntityPool::AttachComponentTo(entity, comp);
}
void AssociateSystemsWithEntity(const Entity& entity)
{
	GetSystemsManager().AddEntityToProcess(entity);
}
}

Entity* CreateEntity()
{
	return Internal::EntityPool::CreateEntity();
}

Entity* CreateEntity(const tchar* name)
{
	return Internal::EntityPool::CreateEntity(name);
}

void Musa::DestroyEntity(Entity& entity)
{
	Internal::EntityPool::DestroyEntity(&entity);
}

Entity* FindEntity(const EntityID& id)
{
	return Internal::EntityPool::FindEntityByID(id);
}

Entity* FindEntity(const tchar* name)
{
	return Internal::EntityPool::FindEntityByName(name);
}

ComponentGroupDescription GetComponentDescriptionOf(const Entity & entity)
{
	return Internal::EntityPool::FindComponentTypesOn(entity);
}

}
