#pragma once

#include "Types/Intrinsics.hpp"
#include "Types/Uncopyable.hpp"
#include "Types/UniquePtr.hpp"
#include "Algorithms.hpp"
#include "Containers/DynamicArray.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "ECS/DLLDef.h"
#include "ECS/Types.hpp"
#include "ECS/Entity.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/Internal/robin_hood.hpp"
#include "ECS/ArchetypeChunk.hpp"
#include "ECS/System.hpp"
#include "ECS/SystemType.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/QueryCache.hpp"

namespace Musa
{
// template<typename... Args>
// struct TypeList
// {
// };
// 
// 
// template<typename... Args, typename Func>
// void ForeachInternal(TypeList<Args...>, Func&&)
// {
// 	static_assert(always_false<Args...>::value, "Foreach doesn't support more than 6 arguments");
// }
// 
// template<typename T0, typename Func>
// void ForeachInternal(TypeList<T0> params, Func&& f)
// {
// 	printf("T0\n");
// 
// 	const ComponentType* type = GetTypeFor<T0>();
// 	uint64 archetypeID = type->hash.archetypeBit;
// 	World w;
// 	
// 	for (const auto& archetype : w.archetypes)
// 	{
// 		if (!!(archetype->archetypeHashID & archetypeID))
// 		{
// 			// We have one that might match
// 			// Check the types living in the archetype
// 			for (const auto& hash : archetype->typeHashes)
// 			{
// 				if (hash.typenameHash == type->hash.typenameHash)
// 				{
// 					// found type
// 				}
// 			}
// 		}
// 	}
// }
// 
// template<typename T0, typename T1, typename Func>
// void ForeachInternal(TypeList<T0, T1> params, Func&& f)
// {
// 	printf("T0, T1\n");
// }
// 
// template<typename T0, typename T1, typename T2, typename Func>
// void ForeachInternal(TypeList<T0, T1, T2> params, Func&& f)
// {
// 	printf("T0, T1, T2\n");
// }
// 
// template<typename T0, typename T1, typename T2, typename T3, typename Func>
// void ForeachInternal(TypeList<T0, T1, T2, T3> params, Func&& f)
// {
// 	printf("T0, T1, T2, T3\n");
// }
// 
// template<typename T0, typename T1, typename T2, typename T3, typename T4, typename Func>
// void ForeachInternal(TypeList<T0, T1, T2, T3, T4> params, Func&& f)
// {
// 	printf("T0, T1, T2, T3, T4\n");
// }
// 
// template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename Func>
// void ForeachInternal(TypeList<T0, T1, T2, T3, T4, T5> params, Func&& f)
// {
// 	printf("T0, T1, T2, T3, T4, T5\n");
// }
// 
// template<typename Func>
// void Foreach(Func&& f)
// {
// 	using params = decltype(args(&Func::operator()));
// 	ForeachInternal(params{}, std::forward<Func>(f));
// }

struct Component;
class System;

struct ECS_API World final : private Uncopyable
{
	World();

	template<typename... Comps>
	Entity CreateEntity();
	template<uint32 N>
	Entity CreateEntity(const ComponentType* (&types)[N]);
	Entity CreateEntity(const ComponentType** types, uint32 typeCount);

	Entity CreateEntity(Archetype& archetype);
	void DestroyEntity(Entity entity);

	bool IsEntityValid(Entity entity) const;

	template <class Sys, typename... Args>
	Sys& CreateSystem(Args&&... args);
	template <class Sys>
	void DestroySystem();

	void Update();

	template <typename Comp>
	void AddComponentTo(Entity entity);
	template <typename Comp>
	void RemoveComponent(Entity entity);
	template <typename Comp>
	void SetComponentDataOn(Entity entity, Comp&& component);
	template <typename Comp>
	Comp& GetComponentDataOn(Entity entity) const;
	template <typename Comp>
	bool HasComponent(Entity entity) const;

	DynamicArray<const SystemType*> systemTypesInWorld;
	DynamicArray<UniquePtr<System>> systems;

	// stores all of the different archetypes based on their similar  archetypeHashIDs
	DynamicArray<EntityBridge> entityBridges;
	DynamicArray<uint32> deadIndices;

	DynamicArray<UniquePtr<Archetype>> archetypes;
	DynamicArray<ArchetypeMask> archetypeHashIDs;
	robin_hood::unordered_flat_map<ArchetypeMask, DynamicArray<Archetype*>> archetypesByHash;
	uint32 totalLivingEntities = 0;

	QueryCache* queryCache;

private:
	Entity ConstructEntityInternals(World& world, const ComponentType** types, uint32 typeCount);
	void HookUpComponentType(World& world, Entity entity, const ComponentType* type);
	void UnhookComponentType(World& world, Entity entity, const ComponentType* type);
};

forceinline Archetype& GetEntityArchetype(World& world, Entity entity)
{
	Assert(world.IsEntityValid(entity));
	return *world.entityBridges[entity.id].chunk->footer.owner;
}

template<typename ...Comps>
inline Entity World::CreateEntity()
{
	static_assert(all_can_attach_to_entity_v<Comps...>, "Invalid type trying to attach to Entity");
	if constexpr (sizeof...(Comps) > 0)
	{
		static const ComponentType* types[] = { GetComponentTypeFor<Comps>()... };
		constexpr uint32 typeCount = (uint32)ArraySize(types);
		static_assert(typeCount < MaxComponentsPerArchetype, "Trying to attach too many components to this Entity!");

		InsertionSort(types, typeCount);

		return ConstructEntityInternals(*this, types, typeCount);
	}
	else
	{
		return ConstructEntityInternals(*this, nullptr, 0);
	}
}

template<uint32 N>
inline Entity World::CreateEntity(const ComponentType* (&types)[N])
{
	static_assert(N < MaxComponentsPerArchetype, "Trying to attach too many components to this Entity!");
	return CreateEntity(types, N);
}

template<class Sys, typename ...Args>
inline Sys& World::CreateSystem(Args&&... args)
{
	static_assert(std::is_base_of_v<System, Sys>, "Type passed in as a template parameter must be derived from Musa::System");
	const SystemType* type = GetSystemTypeFor<Sys>();
	Sys* system = new Sys(std::forward<Args>(args)...);
	system->InitializeInternals(*this);
	system->Initialize();

	systemTypesInWorld.Add(type);
	systems.Add(UniquePtr<Sys>(system));
	return *system;
}

template<class Sys>
inline void World::DestroySystem()
{
	static_assert(std::is_base_of_v<System, Sys>, "Type passed in as a template parameter must be derived from Musa::System");
	const SystemType* type = GetSystemTypeFor<Sys>();
	int32 index = systemTypesInWorld.FindFirstIndex(type);

	Assert(index >= 0);
	systems[index]->Deinitialize();

	systems.Remove(index);
	systemTypesInWorld.Remove(index);
}

template<typename Comp>
inline void World::AddComponentTo(Entity entity)
{
	static_assert(can_attach_to_entity_v<Comp>, "Invalid type trying to attach to Entity");
	const ComponentType* type = GetComponentTypeFor<Comp>();
	HookUpComponentType(*this, entity, type);
}

template<typename Comp>
inline void World::RemoveComponent(Entity entity)
{
	static_assert(can_attach_to_entity_v<Comp>, "Invalid type trying to remove from Entity");
	const ComponentType* type = GetComponentTypeFor<Comp>();
	UnhookComponentType(*this, entity, type);
}

template<typename Comp>
inline void World::SetComponentDataOn(Entity entity, Comp&& component)
{
	static_assert(can_attach_to_entity_v<Comp>, "Invalid type trying to check on Entity");
	Assert(IsEntityValid(entity));

	const EntityBridge& bridge = entityBridges[entity.id];
	ChunkArray<Comp> chunkArr = GetChunkArray<Comp>(*bridge.chunk);
	chunkArr[bridge.chunkIndex] = std::forward<Comp>(component);
}

template<typename Comp>
inline Comp& World::GetComponentDataOn(Entity entity) const
{
	static_assert(can_attach_to_entity_v<Comp>, "Invalid type trying to check on Entity");
	Assert(IsEntityValid(entity));

	const EntityBridge& bridge = entityBridges[entity.id];
	ChunkArray<Comp> chunkArr = GetChunkArray<Comp>(*bridge.chunk);
	return chunkArr[bridge.chunkIndex];
}

template<typename Comp>
inline bool World::HasComponent(Entity entity) const
{
	static_assert(can_attach_to_entity_v<Comp>, "Invalid type trying to check on Entity");
	Assert(IsEntityValid(entity));

	const EntityBridge& bridge = entityBridges[entity.id];
	ChunkArray<Comp> chunkArr = GetChunkArray<Comp>(*bridge.chunk);
	return chunkArr.IsValid();
}

}

