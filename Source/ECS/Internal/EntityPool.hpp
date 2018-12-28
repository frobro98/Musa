#pragma once

#include "EngineCore/Types.h"
#include "EngineCore/Containers/Array.h"
#include "EngineCore/Containers/Map.h"
 
#include "ECS/Entity.hpp"

namespace Musa
{
struct Component;

namespace Internal
{

class EntityPool final
{
public:
	EntityPool() = delete;

	static Entity* CreateEntity();
	static Entity* CreateEntity(const tchar* name);
	static void DestroyEntity(Entity* entity);

	static Entity* FindEntityByID(const EntityID& index);
	static Entity* FindEntityByName(const tchar* name);

	static void AttachComponentTo(const Entity& entity, Component& componentToAttach);
	static Component* FindComponentOn(const Entity& entity, ComponentType type);
	static bool EntityHasComponent(const Entity& entity, ComponentType type);

	static ComponentGroupDescription FindComponentTypesOn(const Entity& entity);

private:
	static Map<EntityID, Entity*> entityPool;
	static Map<EntityID, Array<Component*>> ownedComponentMap; // This may point to the collection of components, but there isn't any notion of what component is what
	static Map<EntityID, ComponentGroupDescription> entityDescriptions;
	static Map<ComponentGroupDescription, Array<Entity*>> componentsOnEntities;
};

}
}
