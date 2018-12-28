#pragma once

#include "EngineCore/Types.h"
#include "EngineCore/String/String.h"
#include "EngineCore/GUID/Guid.hpp"
#include "ECS/Component.hpp"

namespace Musa
{

struct Entity;

namespace Internal
{
Component* GetComponentFromImpl(const Entity& entity, ComponentType type);
bool HasComponentImpl(const Entity& entity, ComponentType type);
void AttachComponentImpl(const Entity& entity, Component& comp);
void AssociateSystemsWithEntity(const Entity& entity);
}

using EntityID = Guid;

// External reference to group of components
struct Entity final
{
	const EntityID id;
	String name; // TODO - This needs to be something that is a lot simpler than just a String object...

	Entity(EntityID id_)
		: id(id_)
	{}

	Entity(EntityID id_, const tchar* entityName)
		: id(id_), name(entityName)
	{}

	Entity(const Entity& other)
		: id(other.id), name(other.name)
	{}

	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = delete;

	friend bool operator==(const Entity& lhs, const Entity& rhs)
	{
		return lhs.id == rhs.id && lhs.name == rhs.name;
	}

};

Entity* CreateEntity();
Entity* CreateEntity(const ComponentGroupDescription& components);
Entity* CreateEntity(const tchar* name);
Entity* CreateEntity(const tchar* name, const ComponentGroupDescription& components);
void DestroyEntity(Entity& entity);
Entity* FindEntity(const EntityID& id);
Entity* FindEntity(const tchar* name);

template <typename Comp, typename = std::enable_if_t<std::is_base_of_v<Component, Comp>>>
void AttachComponentTo(const Entity& entity)
{
	Comp* component = CreateComponent<Comp>();
	Internal::AttachComponentImpl(entity, *component);
}

template <typename Comp, typename = std::enable_if_t<std::is_base_of_v<Component, Comp>>>
void AttachComponentTo(const Entity& entity, Comp& component)
{
	Internal::AttachComponentImpl(entity, component);
	Internal::AssociateSystemsWithEntity(entity);
}


template <typename... Components>//, typename = std::enable_if_t<all_base_of_v<Component, Components...>>>
void AttachComponentsTo(const Entity& entity)
{
	[[maybe_unused]] int dummy[] = { 0, (Internal::AttachComponentImpl(entity, *CreateComponent<Components>()), 0)... };
	Internal::AssociateSystemsWithEntity(entity);
}

template <class Comp, typename = std::enable_if_t<std::is_base_of_v<Component, Comp>>>
Comp* GetComponentFrom(const Entity& entity)
{
	ComponentType type = Comp::GetComponentType();
	// TODO - determine if there's a way around the dynamic cast
	return dynamic_cast<Comp*>(Internal::GetComponentFromImpl(entity, type));
}

template <typename Comp, typename = std::enable_if_t<std::is_base_of_v<Component, Comp>>>
bool HasComponent(const Entity& entity)
{
	return Internal::HasComponentImpl(entity, Comp::GetComponentType<Comp>());
}

ComponentGroupDescription GetComponentDescriptionOf(const Entity& entity);

}

