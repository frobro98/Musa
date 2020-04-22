#pragma once

#include "Types/Intrinsics.hpp"
#include "Types/UniquePtr.hpp"
#include "Containers/DynamicArray.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "ECS/DLLDef.h"
#include "ECS/Types.hpp"
#include "ECS/Entity.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/Internal/robin_hood.hpp"

namespace Musa
{

struct ArchetypeChunk;
struct Archetype;
struct Component;
struct World;

struct ECS_API World final
{
	World() = default;
	World(const World&) = delete;
	World& operator=(const World&) = delete;

	template<typename... Comps, typename = std::enable_if_t<can_attach_to_entity_v<Comps...>>>
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	bool IsEntityValid(Entity entity) const;

	template <typename Comp>
	void AddComponentTo(Entity entity);
	template <typename Comp>
	void RemoveComponent(Entity entity);
	template <typename Comp>
	bool HasComponent(Entity entity) const;

	// stores all of the different archetypes based on their similar  archetypeHashIDs
	DynamicArray<EntityBridge> entityBridges;
	DynamicArray<uint32> deadIndices;

	DynamicArray<UniquePtr<Archetype>> archetypes;
	DynamicArray<ArchetypeHashID> archetypeHashIDs;
	robin_hood::unordered_flat_map<ArchetypeHashID, DynamicArray<Archetype*>> archetypesByHash;
	uint32 totalLivingEntities = 0;

private:
	Entity ConstructEntityInternals(World& world, const ComponentType** types, uint32 typeCount);
	void HookUpComponentType(World& world, Entity entity, const ComponentType* type);
	void UnhookComponentType(World& world, Entity entity, const ComponentType* type);
};

template<typename ...Comps, typename>
inline Entity World::CreateEntity()
{
	static_assert(sizeof...(Comps) > 0, "Can't have an empty entity at this point!");
	static const ComponentType* types[] = { GetTypeFor<Comps>()... };
	constexpr uint32 typeCount = (uint32)ArraySize(types);

	InsertionSort(types, typeCount);

	return ConstructEntityInternals(*this, types, typeCount);
}

template<typename Comp>
inline void World::AddComponentTo(Entity entity)
{
	const ComponentType* type = GetTypeFor<Comp>();
	HookUpComponentType(*this, entity, type);
}

template<typename Comp>
inline void World::RemoveComponent(Entity entity)
{
	const ComponentType* type = GetTypeFor<Comp>();
	HookUpComponentType(*this, type);
}

template<typename Comp>
inline bool World::HasComponent(Entity entity) const
{
	return false;
}

}

