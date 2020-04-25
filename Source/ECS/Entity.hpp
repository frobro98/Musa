#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "ECS/DLLDef.h"

namespace Musa
{
struct Archetype;
struct ArchetypeChunk;
struct World;

// External reference to group of components
struct Entity final
{
	uint32 id;
	uint32 version;
};

inline bool operator==(const Entity& lhs, const Entity& rhs)
{
	return lhs.id == rhs.id && lhs.version == rhs.version;
}

struct EntityBridge
{
	ArchetypeChunk* chunk;
	uint32 version;
	uint32 chunkIndex;
};

ECS_API Entity CreateEntityWith(Archetype& archetype);
}

