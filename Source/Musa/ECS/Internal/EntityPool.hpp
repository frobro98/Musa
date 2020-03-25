#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "EngineCore/Containers/DynamicArray.hpp"
#include "EngineCore/Containers/Map.h"
 
#include "ECS/Entity.hpp"

// namespace Musa
// {
// struct Component;
// 
// namespace Internal
// {
// 
// class EntityPool final
// {
// public:
// 	EntityPool() = delete;
// 
// 	static Entity* CreateEntity();
// 	static Entity* CreateEntity(const tchar* name);
// 	static void DestroyEntity(Entity* entity);
// 
// 	static Entity* FindEntityByID(const EntityID& index);
// 	static Entity* FindEntityByName(const tchar* name);
// 
// 	static void AttachComponentTo(const Entity& entity, Component& componentToAttach);
// 	static Component* FindComponentOn(const Entity& entity, ComponentType type);
// 	static bool EntityHasComponent(const Entity& entity, ComponentType type);
// 
// 	static ComponentGroupDescription FindComponentTypesOn(const Entity& entity);
// 
// };
// 
// }
// }
