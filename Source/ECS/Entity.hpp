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
	const uint32 id;
	const uint32 version;
};

inline bool operator==(const Entity& lhs, const Entity& rhs)
{
	return lhs.id == rhs.id && lhs.version == rhs.version;
}

struct EntityBridge
{
	ArchetypeChunk* owningChunk;
	uint32 version;
	uint32 chunkIndex;
};

ECS_API Entity CreateEntityWith(Archetype& archetype);
}

